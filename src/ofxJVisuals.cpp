//  Created by Jildert Viet on 24-01-16.

#include "ofxJVisuals.hpp"

ofxJVisuals::ofxJVisuals(glm::vec2 size){
    initMesh();
    this->size = size;
    for(int i=0; i<MAX_EVENTS_PTRS; i++)
        events[i] = nullptr;
    for(int i=0; i<NUMLAYERS; i++)
        layers[i] = new JLayer(); // Dummy events
    
    negative.load("shaders/negative");
    
#if USE_PP
    post.init(ofGetWidth(), ofGetHeight());
    noisePP = post.createPass<NoiseWarpPass>();
    noisePP->setEnabled(false);
    fxaaPP = post.createPass<FxaaPass>();
    fxaaPP->setEnabled(false);
    rgbPP = post.createPass<RGBShiftPass>();
    rgbPP->setEnabled(false);
    
    zoomPP = post.createPass<ZoomBlurPass>();
    zoomPP->setEnabled(false);
#endif

    alphaScreen = new AlphaBlackScreen(true);
    alphaScreen->size = size;
    addEvent((Event*)alphaScreen, 0);
    setAlpha(255);
    
    for(int i=0; i<9; i++)
        busses.push_back(new Bus());
    
    loadLastMaskFile();

    verdana30.load("verdana.ttf", 50, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    cout << "ofxJVisuals made" << endl;
#ifndef TARGET_RASPBERRY_PI
    ofFbo::Settings fs; // For export quality
    fs.numSamples = 8;
    fs.width = size.x;
    fs.height = size.y;
    fs.internalformat = GL_RGBA16F;
    fs.useStencil = true;
    fbo.allocate(fs);
    renderFbo.allocate(fs);
#else
    fbo.allocate(size.x, size.y, GL_RGBA);
    renderFbo.allocate(size.x, size.y, GL_RGBA);
#endif
    negativeMask.allocate(size.x, size.y, GL_RGB);
    
    fbo.begin();
        ofClear(0, 0);
    fbo.end();
    negativeMask.begin();
        ofClear(0);
        ofSetColor(255);
        ofDrawRectangle(negativeMask.getWidth()*0.25, 0, negativeMask.getWidth()*0.5, negativeMask.getHeight());
    negativeMask.end();
    
    initCam();
//    sharedFbo.allocate(2560, 800, GL_RGBA);
//    sharedFbo2.allocate(2560, 800, GL_RGBA);
}

void ofxJVisuals::initCam(){
    cam.reset();
//    cam.rotate(-180, ofVec3f(0,1,0));
//    cam.setPosition(0, 0, 0);
//    cam.rotate(-180, ofVec3f(1,0,0));
//    cam.setDistance(fbo.getWidth() * 0.25);
//    cam.lookAt(ofVec3f(0,0,0));
    
//    cam.move(0, ofGetHeight() * -0.5, 0);
//    cam.move(fbo.getWidth() * 0.5, fbo.getHeight() * 0.5, 0); // Temp for ADEtje
    
    if(!camController){
        camController = new cameraController(&cam);
        addEvent((Event*)camController, 0);
    }
}

ofxJVisuals::~ofxJVisuals(){
    cout << "ofxJVisuals deleted" << endl;
    // Delete all events?
    for(int i=0; i<NUMLAYERS; i++)
        layers[i][0].deleteNext();
    for(int i=0; i<mappers.size(); i++)
        delete mappers[i];
}

void ofxJVisuals::deconstructor(){
    delete this;
}

void ofxJVisuals::update(){
    if(bRotate)
        rotationAngle += rotationAngleIcrement;
    
    for(int i=0; i<NUMLAYERS; i++)
        layers[i]->updateMain();
    negativeLayer.updateMain();
    
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // Found this on the OF forum
    // Goal: two grey area's should be brighter when they overlap, currently they merge in the same greyness
    
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    ofEnableDepthTest();
    negativeMask.begin();
        ofClear(0);
        negativeLayer.displayMain();
    negativeMask.end();
    
    
    fbo.begin();
#if USE_PP
        glPushAttrib(GL_ENABLE_BIT);
        // setup gl state
//        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        post.begin();
#endif
        ofClear(0,0);
//        ofEnableSmoothing();
        alphaScreen->displayMain();
//        layers[0]->displayMain(); // alphaScreen and more. Call explicitly, as above
        layers[1]->displayMain(); // Non-cam layer back

//        if(bCam){
//            ofPushMatrix();
//            cam.begin();
//            ofScale(1, -1, 1);
//            ofTranslate(-ofGetWidth()/2., -ofGetHeight()/2.);
//        }
        for(uint8 i=2; i<NUMLAYERS; i++) // Don't draw layer 0: nonCamFront
            layers[i]->displayMain();
//        if(bCam){
//            cam.end();
//            ofPopMatrix();
//        }

        layers[NUMLAYERS-1]->displayMain(); // Non-cam layer front

//        ofClearAlpha();
#if USE_PP
        post.end();
        // set gl state back to original
        glPopAttrib();
#endif
    fbo.end();
    
    
    renderFbo.begin();
    if(bNegativeShader){
        negative.begin();
    }
        fbo.draw(0,0);
    if(bNegativeShader){
        negative.setUniformTexture("mask", negativeMask.getTexture(), 1);
        negative.end();
    }
    renderFbo.end();
}

void ofxJVisuals::display(){
    ofSetColor(255, brightness); // Brightness = alpha of fbo w/ black background?
    switch(fboDisplayMode){
        case 0:
            renderFbo.getTexture().bind();
            mesh.draw();
            renderFbo.getTexture().unbind();
            if(bEditMode){
                for(char i=0; i<4; i++)
                    ofDrawCircle(meshVertices[i], 10);
            }
            break;
        case 1: // Stretch
            renderFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
            break;
        case 2: //
            cout << "Display mode not supported" << endl;
//            fbo.draw(0, ofGetHeight() * 0.5 - (0.5 * (ofGetHeight() / (f.getWidth() / ofGetWidth()))), ofGetWidth(), ofGetHeight() / (f.getWidth() / ofGetWidth()));
            break;
    }
    drawMask();
}

bool ofxJVisuals::checkIfNull(Event* e){
    return (bool)e;
}

bool ofxJVisuals::checkIfNullM(Mapper* m){
    return (bool)m;
}

Event* ofxJVisuals::addEvent(Event* e, int layerIndex, unsigned short index){ // Index 0 means: don't save
    if(index){
        if(index < MAX_EVENTS_PTRS){
            if(events[index]){
                cout << "Position in array is occupied, delete" << endl;
                delete events[index]; // Sets events[index] to nullptr? Doesn't matter...
                events[index] = nullptr;
            }
            events[index] = e; // For accessing events later, with id
            cout << "Added to ptrs[] w/ index: " << index << endl;
        } else{
            cout << "Can't save new event, need a reset? SC should notice this and call killAll() ?" << endl;
            return nullptr;
        }
    }
    
    if(layerIndex>=0)
        layers[layerIndex]->addEvent(e);
    last = e;
    e->lastPtr = &last; // For setting to nullptr if event is deleted
    
    numEvents++;
//    cout << "event ID: " << e->id << endl; Used?
    
//    e->SCsender = SCsender;
    e->numEventsPtr = &numEvents;
    
    e->events = events;
    e->id = index;
    e->mappersParent = &mappers;

//    e->receivingPointers = &receivingPointers;
    return e;
}

Event* ofxJVisuals::addEvent(Event* e, VisualizerLayer l, unsigned short index){
    switch(l){
        case FUNCTIONAL:
            return addEvent(e, 0, index);
            break;
        case NON_CAM_BACK: // Is draw first
            return addEvent(e, 1, index);
            break;
        case NON_CAM_FRONT: // Drawn as last
            return addEvent(e,NUMLAYERS-1, index);
            break;
        case DEFAULT:
            return addEvent(e, 2, index);
            break;
        case NEGATIVE:
            negativeLayer.addEvent(e);
            return addEvent(e, -1, index); // Don't store it in a layer again
            break;
        default:
            break;
    }
return nullptr;
}

void ofxJVisuals::setAlpha(int alpha, bool bDo){
    alphaScreen->setAlpha(alpha);
    alphaScreen->setActiveness(bDo);
}

void ofxJVisuals::setBrightness(unsigned char b){
    brightness = b;
}

void ofxJVisuals::addMapper(Mapper* m){ // Not using this for >3 years I guess
    mappers.push_back(m);
}

void ofxJVisuals::killAll(){
    alphaScreen->bGradient = false;
#if USE_PP
    bPostProcessing = false;
#endif
    bAddMirror = false;
    
    for(uint8 i=0; i<NUMLAYERS-1; i++){
        if(layers[i]->next)
            layers[i]->next->deleteNext(); // Don't delete self, only next events
    }
    if(negativeLayer.next)
        negativeLayer.next->deleteNext();
        
    
    for(unsigned short i=0; i<MAX_EVENTS_PTRS; i++)
        events[i] = nullptr;
}

void ofxJVisuals::makeFit(glm::vec2 size){
    if(this->size == size)
        return;
    alphaScreen->size = size;
    if(fbo.getWidth() != size.x || fbo.getHeight() != size.y){
        fbo.allocate(size.x, size.y, fbo.getTexture().getTextureData().glInternalFormat);
    }
    this->size = size;
}

vector<float> ofxJVisuals::vec(float a){
    vector<float> v;
    v.push_back(a);
    return v;
}

vector<float> ofxJVisuals::vec(float a, float b){
    return vector<float>({a, b});
}
vector<float> ofxJVisuals::vec(float a, float b, float c){
    return vector<float>({a, b, c});
}
vector<float> ofxJVisuals::vec(float a, float b, float c, float d){
    return vector<float>({a, b, c, d});
}
vector<float> ofxJVisuals::vec(float a, float b, float c, float d, float e){
    return vector<float>({a, b, c, d, e});
}

void ofxJVisuals::loadLastMaskFile(){
    ofFile f;
    f.open("./maskFile.txt", ofFile::Mode::ReadOnly);
    ofBuffer b;
    b = f.readToBuffer();
    string path = b.getText();
    if(path.length() > 5){
        mask.load(path);
        maskBrightness = 0;
        bMask = true;
    }
}

ofxOscMessage ofxJVisuals::getAllEvents(){
    ofxOscMessage m;
    m.setAddress("/allEvents");
    for(unsigned short i=1; i<NUMLAYERS-1; i++){
        // A, B, C
        if(layers[i]->next){
            Event* lastRead = layers[i]->next; // First: A (skip dummy Event)
            m.addIntArg(lastRead->id);
            m.addStringArg(lastRead->type);
            
            while(lastRead->next){ // Is there a B? Yes.
                lastRead = lastRead->next; // So this is B.
                m.addIntArg(lastRead->id);
                m.addStringArg(lastRead->type);
                // has B a next? Yes. Has C a next? No
            }
        } else{
             // Only the dummy Event
        }
    }
    return m;
}

Event* ofxJVisuals::getEventById(int idToFind){
    if(idToFind<MAX_EVENTS_PTRS){
        if(events[idToFind]){
            return events[idToFind];
        } else{
            return nullptr;
        }
    } else{
        return nullptr;
    }
    
//    if(last){
//        if(last->id == idToFind){
//            return last;
//        }
//    }
//
//    for(int i=0; i<NUMLAYERS; i++){
//        Event* toCheck = layers[i]; // Dummy
//        while(toCheck->next){
//            cout << "test: layer: " << i << " " << toCheck->next->id << endl;
//            if(toCheck->next->id == idToFind){
//                return toCheck->next;
//            }
//            toCheck = toCheck->next;
//        }
//    }
//    return nullptr;
}

void ofxJVisuals::getFreePointers(){
    unsigned short freeEvents[MAX_EVENTS_PTRS];
    unsigned short numFreeEvents = 0;
    for(unsigned short i=1; i<MAX_EVENTS_PTRS; i++){ // Keep 0 free... 
        if(events[i] == nullptr){
            freeEvents[numFreeEvents] = i;
            numFreeEvents++;
        }
    }
    ofxOscMessage m;
    m.setAddress("/freePointers");
    for(unsigned short i=0; i<numFreeEvents; i++){
        m.addInt32Arg(freeEvents[i]);
    }
    SCsender->sendMessage(m);
    
    // [3, 4, 5 .. 512], when only 0, 1, 2 are still occupied / alive. numFreeEvents should 509
    
    // So should have a 'blank list', of all ptrs[x] that are free...
    // @ Reset receive a bunch of numbers that are free, use that for setting new addresses @ SC...
    
}

void ofxJVisuals::initCircularMaskFbo(glm::vec2 size, int num){
    cout << "create circle mask w/ size: " << size << endl;
    // create one circle first
    circularMask.allocate(size.x, size.y, GL_RGBA);
    ofPixels p;
    p.allocate(size.x, size.y, OF_PIXELS_RGBA);
    float yRatio, xRatio;
    for(int i=0; i<p.getWidth(); i++){
        xRatio = (float)i / p.getWidth();
        xRatio = pow((1-abs(sin(xRatio*(num*PI)))), 0.5);
        for(int j=0; j<p.getHeight(); j++){
            yRatio = float(j) / p.getHeight();
            yRatio = pow((1-sin(yRatio*(PI))), 0.5);
            float toWrite = (yRatio + xRatio) * 0.5;
            ofColor c = ofColor(0, 255 * toWrite); // Half phase of sine
            p.setColor(i, j, c);
        }
    }
    circularMask.getTexture().loadData(p);
}

void ofxJVisuals::drawMask(){
    if(bMask){
        ofSetColor(255, maskBrightness);
        mask.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    }
    if(bDrawCirclularMask){
        ofSetColor(255);
        circularMask.draw(0, 0);
    }
}

void ofxJVisuals::keyPressed(int key){
    switch(key){
        case 'q':
            fboDisplayMode = 0;
            break;
        case 'w':
            fboDisplayMode = 1;
            break;
        case 'e':
            fboDisplayMode = 2;
            break;
        case 'a':
            bEditMode = !bEditMode;
            break;
    }
}

void ofxJVisuals::initMesh(){
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    texCoords = {glm::vec2(0,0), glm::vec2(size.x, 0), glm::vec2(size.x, size.y), glm::vec2(0, size.y)};
    meshVertices = {glm::vec3(0, 0, 0), glm::vec3(size.x, 0, 0), glm::vec3(size.x, size.y, 0), glm::vec3(0, size.y, 0)};
    for(char i=0; i<4; i++){
//        mesh.addTexCoord(texCoords[i] + glm::vec2(size.x, 0)); // Center piece
        mesh.addTexCoord(texCoords[i]); // Center piece
        mesh.addVertex(meshVertices[i]);
    }
}

void ofxJVisuals::mousePressed(int x, int y, int button){
    if(bEditMode){
        unsigned char indexOfClosest = 0;
        float minDistance = 99999999;
        for(char i=0; i<4; i++){
            if(ofVec2f(x, y).distance(meshVertices[i]) < minDistance){
                minDistance = ofVec2f(x, y).distance(meshVertices[i]);
                indexOfClosest = i;
            }
        }
        cout << "Selected: " << (int)indexOfClosest << endl;
        meshVertices[indexOfClosest] = glm::vec3(x, y, 0);
        mesh.clear();
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        
        for(char i=0; i<4; i++){
            mesh.addTexCoord(texCoords[i]);
            mesh.addVertex(meshVertices[i]);
        }
    }
}

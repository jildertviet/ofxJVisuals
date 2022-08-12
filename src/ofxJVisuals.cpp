//  Created by Jildert Viet on 24-01-16.

#include "ofxJVisuals.hpp"

#define RECEIVER_PORT   6061

ofxJVisuals::ofxJVisuals(glm::vec2 size){
    initMesh();
    this->size = size;
    for(int i=0; i<MAX_EVENTS_PTRS; i++)
        events[i] = nullptr;
    for(int i=0; i<NUMLAYERS; i++)
        layers[i] = new JLayer(); // Dummy events

    negative.load("../../../../../addons/ofxJVisuals/libs/shaders/negative");

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
    addEvent((JEvent*)alphaScreen, 0);
    setAlpha(255);

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

    receiver.setup(RECEIVER_PORT);

    msgParser = new MsgParser(this);

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
        addEvent((JEvent*)camController, 0);
    }
}

ofxJVisuals::~ofxJVisuals(){
    cout << "ofxJVisuals deleted" << endl;
    // Delete all events?
    for(int i=0; i<NUMLAYERS; i++)
        layers[i][0].deleteNext();
    delete msgParser;
}

void ofxJVisuals::deconstructor(){
    delete this;
}

void ofxJVisuals::update(){
    msgParser->update();
    
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);    
        msgParser->parseMsg(m);
    }

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
            renderFbo.draw(0, 0);
            break;
        case 2: //
            cout << "Display mode not supported" << endl;
//            fbo.draw(0, ofGetHeight() * 0.5 - (0.5 * (ofGetHeight() / (f.getWidth() / ofGetWidth()))), ofGetWidth(), ofGetHeight() / (f.getWidth() / ofGetWidth()));
            break;
    }
    drawMask();
}

bool ofxJVisuals::checkIfNull(JEvent* e){
    return (bool)e;
}

JEvent* ofxJVisuals::addEvent(JEvent* e, int layerIndex, unsigned short index){ // Index 0 means: don't save
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

//    e->receivingPointers = &receivingPointers;
    return e;
}

JEvent* ofxJVisuals::addEvent(JEvent* e, VisualizerLayer l, unsigned short index){
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

void ofxJVisuals::killAll(){
    alphaScreen->bGradient = false;
#if USE_PP
    bPostProcessing = false;
#endif
    bAddMirror = false;

    for(uint8 i=1; i<NUMLAYERS-1; i++){ // Skip layer 0 'Functional'
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
//
//vector<float> ofxJVisuals::vec(float a){
//    vector<float> v;
//    v.push_back(a);
//    return v;
//}

//vector<float> ofxJVisuals::vec(float a, float b){
//    return vector<float>({a, b});
//}
//vector<float> ofxJVisuals::vec(float a, float b, float c){
//    return vector<float>({a, b, c});
//}
//vector<float> ofxJVisuals::vec(float a, float b, float c, float d){
//    return vector<float>({a, b, c, d});
//}
//vector<float> ofxJVisuals::vec(float a, float b, float c, float d, float e){
//    return vector<float>({a, b, c, d, e});
//}

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
            JEvent* lastRead = layers[i]->next; // First: A (skip dummy Event)
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

JEvent* ofxJVisuals::getEventById(int idToFind){
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
//        JEvent* toCheck = layers[i]; // Dummy
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

bool MsgParser::parseMsg(ofxOscMessage& m){
//    cout << m.getAddress() << endl;
    switch(commands[m.getAddress()]){
        case 1: // Make
            make(m);
        break;
        case 2: // setVal
            setVal(m);
        break;
        case 3: // killAll
            v->killAll();
            break;
        case 4: // addEnv
            addEnv(m);
            break;
        case 5: // addTo
            addTo(m);
            break;
        case 6:{ // doFunc
            JEvent* e = v->getEventById(m.getArgAsInt(0));
            if(!e)
                return false;
            switch(m.getArgAsInt(1)){
                case 0:
                    e->customOne();
                    break;
                case 1:
                    e->customTwo();
                    break;
                case 2:
                    e->customThree();
                    break;
                case 3:
                    e->customFour();
                    break;
                case 4:
                    e->customFive();
                    break;
            }
        }
            break;
        case 7:{ // addConnection
            JEvent* e = v->getEventById(m.getArgAsInt(0));
            if(e && e->type == "Vorm")
                ((Vorm*)e)->addConnection((Vorm*)v->getEventById(m.getArgAsInt(1)));
        }
            break;
        case 8:{ // switchRadius
            JEvent* e = v->getEventById(m.getArgAsInt(0));
            JEvent* b = v->getEventById(m.getArgAsInt(1));
            if(e && e->type == "Vorm")
                ((Vorm*)e)->switchRadiusses((Vorm*)b);
            if(m.getArgAsBool(2)){
                if(e)
                    ((Vorm*)e)->instantFormVorm();
                if(b)
                    ((Vorm*)b)->instantFormVorm();
            }
        }
            break;
        case 9:{ // changeAngleOffset
            JEvent* e = v->getEventById(m.getArgAsInt(0));
            if(e){
                if(e->type == "Vorm")
                    ((Vorm*)e)->changeAngleOffset(m.getArgAsFloat(1));
            }
        }
            break;
        case 10:{ // getInfo
            switch(m.getArgAsInt(0)){
                case 0: // Get all events
                    SCsender->sendMessage(v->getAllEvents());
                    break;
            }
        }
            break;
        case 11:{ // linkVecField
            JEvent* e = v->getEventById(m.getArgAsInt(0));
#if USE_PARTICLES
            if(e){
                if(e->type == "particleSystem")
                    ((particleSystem*)e)->setVecField((JVecField*)v->getEventById(m.getArgAsInt(1)));
            }
#endif
        }
            break;
        case 12: // getFreePointers
            v->getFreePointers(); // Sends to 6063, w/ SCsender
            break;
        case 13: // setMasterBrightness
            v->setBrightness(m.getArgAsInt(0));
            break;
        case 14: // resetCam
            v->initCam();
            break;
        case 15: // setAlpha
            v->setAlpha(m.getArgAsInt(0));
            break;
        case 16: // setCircularMaskAlpha
            v->bDrawCirclularMask = m.getArgAsBool(0);
            break;
        case 17:
            v->bCam = m.getArgAsBool(0);
            break;
        case 18: // Tilt
            v->cam.tiltDeg(m.getArgAsFloat(0));
            break;
        case 19: // Pan
            v->cam.panDeg(m.getArgAsFloat(0));
            break;
        case 20: // Roll
            v->cam.rollDeg(m.getArgAsFloat(0));
            break;
        case 21: // camRotateAround
            v->cam.rotateAroundDeg(m.getArgAsFloat(0), glm::vec3(m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3)), glm::vec3(0, 0, 0));
            break;
        case 22:
            ofColor c = ofColor(m.getArgAsInt(0), m.getArgAsInt(1), m.getArgAsInt(2));
            v->alphaScreen->setColor(c);
            v->alphaScreen->setActiveness(true);
            break;

    }
    return false;
}

bool MsgParser::make(ofxOscMessage& m){
    cout << "Make " << m.getArgAsString(0) << " with ID: " << m.getArgAsInt(1) << endl;
    JEvent* e = nullptr;
    switch(types[m.getArgAsString(0)]){
        case 1: // JRectangle
            e = new JRectangle();
            break;
        case 2: // vecField
            e = new JVecField();
            ((JVecField*)e)->underlayer = &(v->fbo);
            break;
        case 3: // JMirror
            e = new JMirror();
            break;
        case 4: // JEllipse
            e = new JEllipse();
            break;
        case 5: // JVorm
            if(m.getNumArgs() > 3){
                // Specific constructor ?
                e = new Vorm();
            } else{
                e = new Vorm();
            }
            break;
        case 6: // JSpaceCube
            e = new SpaceCube();
            break;
        case 7: // JMultiMesh
            e = new MultiMesh();
            break;
        case 8:{ // JParticles
//            e = (JEvent*)new JParticles();
#ifdef USE_PARTICLES
            e = (JEvent*)new particleSystem(m.getArgAsInt(4), ofVec2f(m.getArgAsInt(5),m.getArgAsInt(6)), ofColor::white, m.getArgAsInt(7));
#endif
//            int idToFind = m.getArgAsInt(5);
//            ((particleSystem*)e)->setVecField((JVecField*)v->getEventById(m.getArgAsInt(5)));
        }
            break;
        case 9:
            e = new JText(&(v->verdana30));
            ((JText*)e)->bCamEnabled = &(v->bCam);
            break;
        case 10:
            e = new JMesh();
            break;
        case 11:
            cout << m.getArgAsString(2) << endl;
            e = new JImage(m.getArgAsString(3));
            break;
        case 12:{
            glm::vec2 size = glm::vec2(m.getArgAsFloat(3), m.getArgAsFloat(4));
            e = new JDivisionGrid(size);
        }
            break;
        case 13:
            e = new JVideoPlayer();
            break;
        case 14:{
            glm::vec2 size = glm::vec2(m.getArgAsFloat(3), m.getArgAsFloat(4));
            cout << "Size: " << size << endl;
            e = new JShaderTest(size);
//            ((JShaderTest*)e)->mask = &(v->sharedFbo);
//            ((JShaderTest*)e)->src = &(v->sharedFbo2);
        }
            break;
        case 15:
            e = new JNoise();
            break;
        default:
        case 16:
            e = new MultiMeshMaybeTomorrow();
            break;
        case 18:
#ifdef JPhysarum_hpp
            glm::vec2 size = glm::vec2(m.getArgAsFloat(3), m.getArgAsFloat(4));
            cout << "Create JPhysarum w/ size: " << size << endl;
            e = new JPhysarum(glm::vec2(0, 0), size);

            JPhysarum* p = (JPhysarum*)e;


//            ofFloatPixels pixels;
//            pixels.allocate(1024, 1024, 3);
//            for(int i=0; i<1024; i++){
//                for(int j=0; j<1024; j++){
//                    if(i < 512){
//                        pixels.setColor(i, j, ofFloatColor(0.5, 0.5, 0.5));
//                    } else{
//                        pixels.setColor(i, j, ofFloatColor(0.0, 0.0, 0.0));
//                    }
//                }
//            }
//            ofTexture* t;
//            t = new ofTexture;
//            t->allocate(1024, 1024, GL_RGB32F);
//            t->loadData(pixels);

            // JVideoPlayer* vP = new JVideoPlayer();
            // vP->load("oil.mov");
            // v->addEvent(vP, FUNCTIONAL);
//            vp->pl
            JVecField* vf = new JVecField();
            vf->setSize(glm::vec2(4096,4096));
            vf->setMode(VECFIELD_MODE::PERLIN);
            vf->setDensity(glm::vec2(4096 / 10, 4096 / 10));
            // vf->video = &(vP->player);
//            vf->vecTex.allocate(1024, 1024, GL_RGBA32F);
//            vf->setPixelsToTest();
            vf->speed = 0.001;


            vf->drawMode = VECFIELD_MODE::HIDE;
            vf->setColor(ofColor(0,0));
            vf->complexity = 20;

            v->addEvent((JEvent*)vf);

//            p->externalVelocity = t;
            p->externalVelocity = &(vf->vecTex);
#endif
            break;
    }

    e->id = m.getArgAsInt(1);
    if(m.getNumArgs() > 2){
        if(m.getArgType(2) == ofxOscArgType::OFXOSC_TYPE_STRING){
            if(m.getArgAsString(2) == "nonCamFront"){
                cout << "Add to nonCamFront layer" << endl;
                v->addEvent(e, VisualizerLayer::NON_CAM_FRONT, e->id);
            } else if(m.getArgAsString(2) == "nonCamBack"){
                cout << "Add to nonCamBack layer" << endl;
                v->addEvent(e, VisualizerLayer::NON_CAM_BACK, e->id);
            } else if(m.getArgAsString(2) == "negative"){
                v->addEvent(e, VisualizerLayer::NEGATIVE, e->id);
            }
        } else{
            cout << "Add to layer: " << m.getArgAsInt(2) << endl;
            v->addEvent(e, m.getArgAsInt(2), e->id); // Specific layer
        }
    } else{
        v->addEvent(e, 2, e->id); // Default layer
    }
    // Send back pointer (as long) to SC. Issue: this takes time. When you call event.create(); event.setLoc([0,0]); from SC that second call won't have this pointer yet...
//    ofxOscMessage n;
//    n.setAddress("/makeConfirm");
//    n.addIntArg(m.getArgAsInt(1)); // ID from SC
//    long pointerToInt = reinterpret_cast<long>(e); // This int can later be casted to an JEvent*, but check if it's allocated/in use before using!
//    int a = (int)(pointerToInt >> 32);
//    int b = (int)pointerToInt;
//    n.addIntArg(a);
//    n.addIntArg(b);
//    SCsender->sendMessage(n);
return false;
}

void MsgParser::setVal(ofxOscMessage& m){ // Default: /setVal, 0, "size", 100, 200
    JEvent* e = v->getEventById(m.getArgAsInt(0));
    // Default: /setVal, a, b, "size", 100, 200: a and b are two uint_32 forming a long together, that can be casted to a JEvent*
//    long pointerAsInt = (long)m.getArgAsInt(1) << 32 | m.getArgAsInt(2) & 0xFFFFFFFFL;
//    JEvent* f = reinterpret_cast<JEvent*>(pointerAsInt);
    if(e){
        cout << "Event found, id: " << m.getArgAsInt(0) << ", addr: " << e << endl;
        switch (values[m.getArgAsString(1)]) {
            case 1: // color
                cout << "setColor" << endl;
                if(m.getNumArgs() == 6){ // Set first, global, color
                    e->setColor(ofColor(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4), m.getArgAsInt(5)));
                } else if (m.getNumArgs() == 7){ // Set color @ index of event
                    e->setColor(ofColor(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4), m.getArgAsInt(5)), m.getArgAsInt(6));
                }
            break;
            case 2: // loc
                if(m.getNumArgs() == 4){ // 2D
                    e->setLoc(ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), 0));
                } else if(m.getNumArgs() == 5){ // 3D
                    e->setLoc(ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4)));
                }
                break;
            case 3: // size
                if(m.getNumArgs() == 4){ // 2D
                    e->setSize(ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), 0));
                } else if(m.getNumArgs() == 5){ // 3D
                    e->setSize(ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4)));
                }
                break;
            case 4: // bMove
                e->bMove = m.getArgAsBool(2);
                break;
            case 5: // Speed
//                cout << "Set speed: " << m.getArgAsFloat(2) << endl;
//                e->speed = m.getArgAsFloat(2);
                e->setSpeed(m.getArgAsFloat(2));
                break;
            case 6: // Direction
                e->direction = ofVec2f(m.getArgAsFloat(2), m.getArgAsFloat(3));
                break;
            case 7:{ // lineLength
                // JVecField?
                if(m.getArgAsString(2) == "JVecField"){
                    ((JVecField*)e)->lineLength = m.getArgAsFloat(3);
                }
            }
                break;
            case 8:{ // complexity
                if(m.getArgAsString(2) == "JVecField")
                    ((JVecField*)e)->complexity = m.getArgAsFloat(3);
            }
                break;
            case 9:{ // density
                if(m.getArgAsString(2) == "JVecField")
                    ((JVecField*)e)->setDensity(glm::vec2(m.getArgAsInt(3), m.getArgAsInt(4)));
            }
                break;
            case 10:{ // angle
                if(m.getArgAsString(2) == "JMirror")
                    ((JMirror*)e)->angle = m.getArgAsInt(3);
            }
                break;
            case 11:{ // customArg
                e->customOneArguments[m.getArgAsInt(2)] = m.getArgAsFloat(3);
            }
                break;
            case 12:{ // bForm
                if(m.getArgAsString(2) == "JVorm")
                    ((Vorm*)e)->setState(m.getArgAsBool(3));
            }
                break;
            case 13:{ // maxSpeed
                if(m.getArgAsString(2) == "JVorm")
                    ((Vorm*)e)->change_maxspeed(m.getArgAsFloat(3), m.getArgAsFloat(4));
            }

            case 14:{ // lijnMax
                if(e->type == "Vorm")
                    ((Vorm*)e)->lijnmax = m.getArgAsFloat(2);
            }
                break;
            case 15:{ // lineWidth
                if(e->type == "Vorm"){
                    ((Vorm*)e)->lineWidth = m.getArgAsFloat(2);
                } else if(e->type == "JVecField"){
                    ((JVecField*)e)->lineWidth = m.getArgAsFloat(2);
                }
            }
                break;
            case 16: // mode
                e->setMode(m.getArgAsInt(2));
                break;
            case 17: // text
                ((JText*)e)->setText(m.getArgAsString(2));
                break;
            case 18: // path
                if(e->type == "JImage"){
                    ((JImage*)e)->loadImage(m.getArgAsString(2));
                } else{
                    ((JVideoPlayer*)e)->path = m.getArgAsString(2); // Also load!?
                }
                break;
            case 19: // rotation
                e->rotation = ofVec3f(m.getArgAsFloat(2), m.getArgAsFloat(3), m.getArgAsFloat(4));
                break;
            case 20:{ // drawMode
                if(e->type == "JVecField")
                    ((JVecField*)e)->drawMode = m.getArgAsInt(2);
            }
                break;
            case 21:
                e->setAlpha(m.getArgAsInt(2));
                break;
            case 22:
                if(e->type == "JRectangle"){
                    cout << "setQuadColor" << endl;
                    ofColor a = ofColor(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4));
                    cout << "a: " << a << endl;
                    ((JRectangle*)e)->setQuadColor(
                                    a,
                                    ofColor(m.getArgAsInt(5), m.getArgAsInt(6), m.getArgAsInt(7)),
                                    ofColor(m.getArgAsInt(8), m.getArgAsInt(9), m.getArgAsInt(10)),
                                    ofColor(m.getArgAsInt(11), m.getArgAsInt(12), m.getArgAsInt(13))
                                                   );
                    ((JRectangle*)e)->setAlpha(m.getArgAsInt(14));
                }
#ifdef JPhysarum_hpp
            case 23:
                if(e->type == "JPhysarum")
                    ((JPhysarum*)e)->sensorAngle = m.getArgAsFloat(2);
                break;
            case 24:
                if(e->type == "JPhysarum")
                    ((JPhysarum*)e)->sensorDistance = m.getArgAsFloat(2);
                break;
            case 25:
                if(e->type == "JPhysarum")
                    ((JPhysarum*)e)->turnAngle = m.getArgAsFloat(2);
                break;
            case 26:
                if(e->type == "JPhysarum")
                    ((JPhysarum*)e)->decay = m.getArgAsFloat(2);
                break;
            case 27:
                if(e->type == "JPhysarum")
                    ((JPhysarum*)e)->depositAmount = m.getArgAsFloat(2);
                break;
            case 28:
                if(e->type == "JPhysarum")
                    ((JPhysarum*)e)->balance = m.getArgAsFloat(2);
                break;
#endif
            case 29:
                e->bFill = m.getArgAsBool(2);
                break;
            case 30: // Radius
                if(e->type == "SpaceCube")
                    ((SpaceCube*)e)->radius = m.getArgAsFloat(2);
                break;
            case 31: // evolve
                e->bEvolve = m.getArgAsBool(2);
                break;
            case 32: // Zoom
                e->zoom = m.getArgAsFloat(2);
                break;
        }
    } else{
        cout << "Event not found, id: " << m.getArgAsInt(0) << endl;
    }
}

void MsgParser::addTo(ofxOscMessage& m){
    JEvent* e = v->getEventById(m.getArgAsInt(0));
    if(e){
        switch (values[m.getArgAsString(1)]) {
            case 1:{ // color
                ofColor c = e->colors[0];
                c += ofColor(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4), m.getArgAsInt(5));
                e->setColor(c);
            }
                break;
            case 2:{ // loc
                if(m.getNumArgs() == 4){ // 2d
                    e->setLoc(e->loc + ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), 0));
                } else if(m.getNumArgs() == 5){ // 3d
                    e->setLoc(e->loc + ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4)));
                }
            }
                break;
            case 3: // size
                if(m.getNumArgs() == 4){ // 2d
                    e->size = e->size + ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), 0);
                } else if(m.getNumArgs() == 5){ // 3d
                    e->size = e->size + ofVec3f(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4));
                }
                break;
            case 30: // radius
                if(e->type == "MultiMesh"){
                    ((MultiMesh*)e)->addRadius(m.getArgAsFloat(2));
                }
                break;
        }
    }
}

void MsgParser::addEnv(ofxOscMessage& m){
    JEvent* e = v->getEventById(m.getArgAsInt(0));
    if(!e)
        return;
    vector<float> times = {m.getArgAsFloat(2), m.getArgAsFloat(3), m.getArgAsFloat(4)};
    vector<float> values = {m.getArgAsFloat(5), m.getArgAsFloat(6), m.getArgAsFloat(7), m.getArgAsFloat(8)};

    bool bSave = m.getArgAsBool(10); // 9 is killArg
    switch(envValues[m.getArgAsString(1)]){
        case 1: // Width
            e->addEnv(values, times, &(e->size.x));
            break;
        case 2: // Height
            e->addEnv(values, times, &e->size.y);
            break;
        case 3:{ // Brightness
            for(int i=0; i<values.size(); i++){ // Replace -1 with current alpha value
                if(values[i] == -1)
                    values[i] = e->colors[0].a;
            }
            e->addEnvAlpha(values, times);
        }
            break;
        case 4:{ // lijnMax
            if(e->type == "Vorm")
                ((Vorm*)e)->addEnv(values, times, &((Vorm*)e)->lijnmax);
        }
            break;
        case 5: // x
            e->addEnv(values, times, &(e->loc.x));
            break;
        case 6: // y
            e->addEnv(values, times, &(e->loc.y));
            break;
        case 7: // speed
            e->addEnv(values, times, &(e->speed));
            break;
        case 8: // z
            e->addEnv(values, times, &(e->loc.z));
            break;
        case 9: // depth
            e->addEnv(values, times, &(e->size.z));
            break;
        case 10:{ // randomPoly for JDivisonGrid
            Env* envTemp = ((JDivisionGrid*)e)->addEnvRandomPoly(values, times);
            if(bSave) // Saves the envelope as a txt file when done.
                envTemp->setSave();
        }
            break;
        case 11: {// selectedPoly
            Env* envTemp = ((JDivisionGrid*)e)->addEnvSelectedPoly(values, times);
            if(bSave) // Saves the envelope as a txt file when done.
                envTemp->setSave();
        }
            break;
    }
    if(m.getArgAsBool(9)){ // bKill
        e->setEndTime(times[0] + times[1] + times[2] + 1);
        e->active = true;
    }
    if(bSave){ // Saves the envelope as a txt file when done.
        if(e->getLastEnv())
            e->getLastEnv()->setSave();
    }
}

void MsgParser::connectToSuperCollider(){
    synth.start();
    scClient.setup(6548,"127.0.0.1",SC_PORT);
    ofAddListener(ofxOscEvent::packetIn, this, &MsgParser::onSuperColliderMessageReceived);
}

void MsgParser::onSuperColliderMessageReceived(ofxOscMessage &m){ // 2: event id, 3: param id, 4: value, 5: (optional) type (r,g,b,a)
//    std::cout << "RECVd " <<  m << std::endl;
    if(m.getAddress() == "/mapVal"){
        JEvent* e = v->getEventById(m.getArgAsInt(2));
        if(!e)
            return;
//        if(m.getNumArgs() <= 5){
        int numValues = m.getArgAsInt(3);
        for(int i=0; i<numValues; i++){
            ofxOscArgType type = m.getArgType(4+numValues+i);
            if(type == ofxOscArgType::OFXOSC_TYPE_INT32){
                e->mapValues[m.getArgAsInt(4+i)]->setVal(m.getArgAsInt32(4+numValues+i));
            } else if(type == ofxOscArgType::OFXOSC_TYPE_FLOAT){
                e->mapValues[m.getArgAsInt(4+i)]->setVal(m.getArgAsFloat(4+numValues+i));
            }
        }
    } else if(m.getAddress() == "/done" && m.getArgAsString(0) == "/notify"){
        bIsNotified = true;
    }
}

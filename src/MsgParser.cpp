//
//  MsgParser.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 05-03-19.
//

#include "MsgParser.hpp"

char encodedIntToChar(int i, char index){
  return (*(((char*)&i)+index));
}

MsgParser::MsgParser(ofxJVisuals* v, bool bUseSC) : v(v), bUseSC(bUseSC){
    if(bUseSC){
      synth = new scSynth();
      connectToSuperCollider();
    }
    vector<string> commandKeys = {
        "make",
        "setVal",
        "killAll",
        "addEnv",
        "addTo",
        "doFunc",
        "addConnection",
        "switchRadius",
        "changeAngleOffset",
        "getInfo",
        "linkVecField",
        "getFreePointers",
        "setMasterBrightness",
        "resetCam",
        "setAlpha",
        "setCircularMaskAlpha",
        "setCamEnabled",
        "camTilt",
        "camPan",
        "camRoll",
        "camRotateAround",
        "setBackground",
        "fillBuffer",
        "setBDrawNegative",
    };
    for(short i=0; i<commandKeys.size(); i++){
        string key = "/";
        key += commandKeys[i];
        commands[key] = i + 1;
    }

    vector<string> typeKeys = {
        "JRectangle",
        "JVecField",
        "JMirror",
        "JEllipse",
        "JVorm",
        "JSpaceCube",
        "JMultiMesh",
        "JParticles",
        "JText",
        "JMesh",
        "JImage",
        "JDivisionGrid",
        "JVideoPlayer",
        "JShaderTest",
        "JNoise",
        "JLinesFalling",
        "JIFLogo",
        "JPhysarum",
        "JModifier",
        "JCircle",
        "JLine",
        "JShaderLines"
    };
    for(short i=0; i<typeKeys.size(); i++)
        types[typeKeys[i]] = i + 1;

    vector<string> valueKeys = {
        "color",
        "loc",
        "size",
        "bMove",
        "speed",
        "direction",
        "lineLength",
        "complexity",
        "density",
        "angle",
        "customArg",
        "bForm",
        "maxSpeed",
        "lijnMax",
        "lineWidth",
        "mode",
        "text",
        "path",
        "rotation",
        "drawMode",
        "alpha",
        "quadColor",
        "sensorAngle",
        "sensorDistance",
        "turnAngle",
        "decay",
        "deposit",
        "balance",
        "fill",
        "radius",
        "evolve",
        "zoom",
        "bus"
    };
    for(short i=0; i<valueKeys.size(); i++)
        values[valueKeys[i]] = i + 1;

    vector<string> envValueKeys = {
        "width",
        "height",
        "brightness",
        "lijnMax",
        "x",
        "y",
        "speed",
        "z",
        "depth",
        "randomPoly",
        "selectedPoly"
    };
    for(short i=0; i<envValueKeys.size(); i++)
        envValues[envValueKeys[i]] = i + 1;
}

MsgParser::~MsgParser(){
    ofxOscMessage msg;
    msg.setAddress("/quit");
    scClient.sendMessage(msg);
}

// The rest of the functions is in ofxJVisuals.hpp...
void MsgParser::update(){
    if(bIsNotified == false && bUseSC){
        if(ofGetFrameNum() % 60 == 59){
            ofxOscMessage msg;
            msg.setAddress("/notify");
            msg.addIntArg(1);
            scClient.sendMessage(msg);
        }
    }
}

ofxOscSender* MsgParser::initSCsender(string ip, int port){
    SCsender = new ofxOscSender();
    if(SCsender->setup(ip, port) == false){
        SCsender = nullptr;
    }
    return SCsender;
}

void MsgParser::connectToSuperCollider(){
    if(bUseSC)
      synth->start();
    scClient.setup(6548,"127.0.0.1",SC_PORT);
    ofAddListener(ofxOscEvent::packetIn, this, &MsgParser::onSuperColliderMessageReceived);
}

bool MsgParser::updateValues(ofxOscMessage& m, JEvent* e){
  if(m.getNumArgs() < NUM_BUSSES)
    return false; // No values[] added to msg, ignore
  float valuesToUpdate[NUM_BUSSES];
  for(int i=0; i<NUM_BUSSES; i++){
      valuesToUpdate[i] = m.getArgAsFloat(i+2); // Offset nodeID and replyID
  }
  if(!e)
    e = getEventById(m.getArgAsInt(0), m.getArgAsInt(1)); // Use subID
  if(!e)
    return false;
  e->setValuesFromFloatArray(valuesToUpdate);
  return true;
}

bool MsgParser::parseMsg(ofxOscMessage& m){
//    cout << m.getAddress() << endl;
    // cout << m << endl;
    switch(commands[m.getAddress()]){
        case 1: make(m); break; // Make
        case 2: setVal(m); break; // setVal
        case 3: killAll(); break; // killAll
        case 4: addEnv(m); break; // addEnv
        case 5: addTo(m); break; // addTo
        case 6:{ // doFunc
            JEvent* e = getEventById(m.getArgAsInt(0));
            if(!e)
                return false;
            switch(m.getArgAsInt(1)){
                case 0: e->customOne(); break;
                case 1: e->customTwo(); break;
                case 2: e->customThree(); break;
                case 3: e->customFour(); break;
                case 4: e->customFive(); break;
            }
        }
            break;
        case 7:{ // addConnection
            JEvent* e = getEventById(m.getArgAsInt(0));
            if(e && e->type == "JVorm")
                ((JVorm*)e)->addConnection((JVorm*)getEventById(m.getArgAsInt(1)));
        }
            break;
        case 8:{ // switchRadius
            JEvent* e = getEventById(m.getArgAsInt(0));
            JEvent* b = getEventById(m.getArgAsInt(1));
            if(e && e->type == "JVorm")
                ((JVorm*)e)->switchRadiusses((JVorm*)b);
            if(m.getArgAsBool(2)){
                if(e)
                    ((JVorm*)e)->instantFormJVorm();
                if(b)
                    ((JVorm*)b)->instantFormJVorm();
            }
        }
            break;
        case 9:{ // changeAngleOffset
            JEvent* e = getEventById(m.getArgAsInt(0));
            if(e){
                if(e->type == "JVorm")
                    ((JVorm*)e)->changeAngleOffset(m.getArgAsFloat(1));
            }
        }
            break;
        case 10:{ // getInfo
            // switch(m.getArgAsInt(0)){
            //     case 0: // Get all events
            //         SCsender->sendMessage(v->getAllEvents());
            //         break;
            // }
            cout << "getInfo disabled" << endl;
        }
            break;
        case 11:{ // linkVecField
            JEvent* e = getEventById(m.getArgAsInt(0));
#if USE_OPENCL
            if(e){
                if(e->type == "particleSystem")
                    ((particleSystem*)e)->setVecField((JVecField*)getEventById(m.getArgAsInt(1)));
            }
#endif
        }
            break;
        case 12: // getFreePointers
            getFreePointers(m.getRemoteHost(), m.getArgAsInt(1)); // Sends to 6063, w/ SCsender
            break;
        case 13: // setMasterBrightness
            setBrightness(m.getArgAsInt(0));
            break;
        case 14: // resetCam
            initCam();
            break;
        case 15: // setAlpha
            setAlpha(m.getArgAsInt(0));
            break;
        case 16: // setCircularMaskAlpha
            setCirclularMaskState(m.getArgAsBool(0));
            break;
        case 17:
            setCameraState(m.getArgAsBool(0));
            break;
        case 18: // Tilt
            getCam()->tiltDeg(m.getArgAsFloat(0));
            break;
        case 19: // Pan
            getCam()->panDeg(m.getArgAsFloat(0));
            break;
        case 20: // Roll
            getCam()->rollDeg(m.getArgAsFloat(0));
            break;
        case 21: // camRotateAround
            getCam()->rotateAroundDeg(m.getArgAsFloat(0), glm::vec3(m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3)), glm::vec3(0, 0, 0));
            break;
        case 22:{ // setBackground
            ofColor c = ofColor(m.getArgAsInt(0), m.getArgAsInt(1), m.getArgAsInt(2));
            getAlphaScreen()->setColor(c);
            getAlphaScreen()->setActiveness(true);
          }
            break;
        case 23:{ // fillBuffer
            JEvent* e = getEventById(m.getArgAsInt(0));
            if(e)
              e->fillBuffer(m);
          }
          break;
        case 24:{ // setBDrawNegative
            // v->bDrawNegativeLayer = m.getArgAsInt(0);
         }
         break;
    }
    return false;
}

bool MsgParser::make(ofxOscMessage& m){
    cout << m << endl;
    cout << "Make " << m.getArgAsString(0) << " with ID: " << m.getArgAsInt(1) << endl;
    JEvent* e = nullptr;
    int layer = 2; // Default
    if(m.getNumArgs() > 2){
        if(m.getArgType(2) == ofxOscArgType::OFXOSC_TYPE_STRING){
            if(m.getArgAsString(2) == "nonCamFront"){
                cout << "Add to nonCamFront layer" << endl;
                layer = VisualizerLayer::NON_CAM_FRONT;
            } else if(m.getArgAsString(2) == "nonCamBack"){
                cout << "Add to nonCamBack layer" << endl;
                layer = VisualizerLayer::NON_CAM_BACK;
            } else if(m.getArgAsString(2) == "negative"){
                layer = VisualizerLayer::NEGATIVE;
            }
        } else{
          layer = m.getArgAsInt(2);
        }
    }
    switch(types[m.getArgAsString(0)]){
        case 1: e = new JRectangle(); break;
        case 2: // vecField
            e = new JVecField();
            ((JVecField*)e)->underlayer = getFbo();
            break;
        case 3: e = new JMirror(); break;

        case 5: // JVorm
            if(m.getNumArgs() > 3){
                // Specific constructor ?
                e = new JVorm();
            } else{
                e = new JVorm();
            }
            break;
        case 6: e = new SpaceCube(); break;
        case 7: e = new MultiMesh(); break;
        case 8:{ // JParticles
//            e = (JEvent*)new JParticles();
#if USE_OPENCL
            e = (JEvent*)new particleSystem(m.getArgAsInt(4), ofVec2f(m.getArgAsInt(5),m.getArgAsInt(6)), ofColor::white, m.getArgAsInt(7));
#endif
//            int idToFind = m.getArgAsInt(5);
//            ((particleSystem*)e)->setVecField((JVecField*)v->getEventById(m.getArgAsInt(5)));
        }
            break;
        case 9:{
            e = new JText(getSelectedFont());
            if(layer == VisualizerLayer::NON_CAM_BACK || layer == VisualizerLayer::NON_CAM_FRONT){
                ((JText*)e)->bCamEnabled == false;
            }
               }
            break;
        case 10: e = new JMesh(); break;
        case 11:
            cout << m.getArgAsString(2) << endl;
            e = new JImage(m.getArgAsString(3));
            break;
        case 12:{
            glm::vec2 size = glm::vec2(m.getArgAsFloat(3), m.getArgAsFloat(4));
            e = new JDivisionGrid(size);
        }
            break;
        case 13: e = new JVideoPlayer(); break;
        case 14:{
            glm::vec2 size = glm::vec2(m.getArgAsFloat(3), m.getArgAsFloat(4));
            cout << "Size: " << size << endl;
            e = new JShaderTest(size);
//            ((JShaderTest*)e)->mask = &(v->sharedFbo);
//            ((JShaderTest*)e)->src = &(v->sharedFbo2);
        }
            break;
        case 15: e = new JNoise(); break;
        default:
        case 16: e = new MultiMeshMaybeTomorrow(); break;
        case 18:{
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
            vf->setSize(glm::vec3(size.x,size.y,0));
            vf->setMode(VECFIELD_MODE::PERLIN);
            vf->setDensity(glm::vec2(size.x / 10, size.y / 10));
            // vf->video = &(vP->player);
//            vf->vecTex.allocate(1024, 1024, GL_RGBA32F);
//            vf->setPixelsToTest();
            vf->speed = 0.001;


            vf->drawMode = VECFIELD_MODE::HIDE;
            vf->setColor(ofColor(0,0));
            vf->complexity = 20;

            addEvent((JEvent*)vf);

//            p->externalVelocity = t;
            p->externalVelocity = &(vf->vecTex);
#endif
        }
            break;
        case 19:{ // JModifier
            auto mod = JModifierFact::create(m.getArgAsInt(4));
            vector<float> busses;
            // [id, type, layer, parentID, val, val, val, val, etc]
            for(int i=5; i<m.getNumArgs(); i++){
                busses.push_back(m.getArgAsFloat(i));
            }
            mod->setViaBusses(busses);
            mod->id = m.getArgAsInt(1);
            // HIER
            getEventById(m.getArgAsInt(3))->modifiers.push_back(mod); // Add to parent
            e = (JEvent*)mod;
        }
        break;
        case 20: e = (JEvent*)new JCircle(); break;
        case 21: e = (JEvent*)new JLine(); break;
        case 22:{
          JShaderLines* s = new JShaderLines();
          getShaders()->push_back(s);
          e = (JEvent*)s;
          ((JShaderLines*)e)->load(m.getArgAsString(3));
        }
        break;
    }

    e->id = m.getArgAsInt(1);
    addEvent(e, layer, e->id);

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
    JEvent* e = getEventById(m.getArgAsInt(0));
    // Default: /setVal, a, b, "size", 100, 200: a and b are two uint_32 forming a long together, that can be casted to a JEvent*
//    long pointerAsInt = (long)m.getArgAsInt(1) << 32 | m.getArgAsInt(2) & 0xFFFFFFFFL;
//    JEvent* f = reinterpret_cast<JEvent*>(pointerAsInt);
    if(e){
        cout << "Event found, id: " << m.getArgAsInt(0) << ", addr: " << e << endl;
        switch (values[m.getArgAsString(1)]) {
            case 1: // color
                // if(m.getNumArgs() == 6){ // Set first, global, color
                    e->setColor(ofColor(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4), m.getArgAsInt(5)));
                // } else if (m.getNumArgs() == 7){ // Set color @ index of event
                    // e->setColor(ofColor(m.getArgAsInt(2), m.getArgAsInt(3), m.getArgAsInt(4), m.getArgAsInt(5)), m.getArgAsInt(6));
                // }
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
            case 6: {// Direction
                if(m.getNumArgs() >= 5){
                  e->direction = glm::vec3(m.getArgAsFloat(2), m.getArgAsFloat(3), m.getArgAsFloat(4));
                } else{
                  e->direction = glm::vec3(m.getArgAsFloat(2), m.getArgAsFloat(3), 0.0);
                }
              }
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
                e->busses[m.getArgAsInt(2)] = m.getArgAsFloat(3);
            }
                break;
            case 12:{ // bForm
                if(m.getArgAsString(2) == "JVorm")
                    ((JVorm*)e)->setState(m.getArgAsBool(3));
            }
                break;
            case 13:{ // maxSpeed
                if(m.getArgAsString(2) == "JVorm")
                    ((JVorm*)e)->changeMaxspeed(m.getArgAsFloat(3), m.getArgAsFloat(4));
            }

            case 14:{ // lijnMax
                if(e->type == "JVorm")
                    *(((JVorm*)e)->maxDistance) = m.getArgAsFloat(2);
            }
                break;
            case 15:{ // lineWidth
                if(e->type == "JVorm"){
                    ((JVorm*)e)->lineWidth = m.getArgAsFloat(2);
                } else if(e->type == "JVecField"){
                    ((JVecField*)e)->lineWidth = m.getArgAsFloat(2);
                } else{
                  e->lineWidth = m.getArgAsFloat(2);
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
            case 33:{ // bus
                vector<float> busses;
                for(int i=2; i<m.getNumArgs(); i++){
                    busses.push_back(m.getArgAsFloat(i));
                    e->setViaBusses(busses); // First used for JModifier 08-2022
                }
            }
                break;
        }
    } else{
        cout << "Event not found, id: " << m.getArgAsInt(0) << endl;
    }
}

void MsgParser::addTo(ofxOscMessage& m){
    JEvent* e = getEventById(m.getArgAsInt(0));
    if(e){
        switch (values[m.getArgAsString(1)]) {
            case 1:{ // color
                ofColor c = e->color;
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
    JEvent* e = getEventById(m.getArgAsInt(0));
    if(!e)
        return;
    vector<float> times = {m.getArgAsFloat(2), m.getArgAsFloat(3), m.getArgAsFloat(4)};
    vector<float> values = {m.getArgAsFloat(5), m.getArgAsFloat(6), m.getArgAsFloat(7), m.getArgAsFloat(8)};

    bool bSave = m.getArgAsBool(10); // 9 is killArg
    switch(envValues[m.getArgAsString(1)]){
        case 1: e->addEnv(values, times, &(e->size.x)); break; // Width
        case 2: e->addEnv(values, times, &e->size.y); break; // Height
        case 3:{ // Brightness
            for(int i=0; i<values.size(); i++){ // Replace -1 with current alpha value
                if(values[i] == -1)
                    values[i] = e->color.a;
            }
            e->addEnvAlpha(values, times);
        }
            break;
        case 4:{ // lijnMax
            if(e->type == "JVorm")
                ((JVorm*)e)->addEnv(values, times, ((JVorm*)e)->maxDistance);
        }
            break;
        case 5: e->addEnv(values, times, &(e->loc.x)); break; // x
        case 6: e->addEnv(values, times, &(e->loc.y)); break; // y
        case 7: e->addEnv(values, times, &(e->speed)); break; // speed
        case 8: e->addEnv(values, times, &(e->loc.z)); break; // z
        case 9: e->addEnv(values, times, &(e->size.z)); break; // depth
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

void MsgParser::onSuperColliderMessageReceived(ofxOscMessage &m){ // 2: event id, 3: param id, 4: value, 5: (optional) type (r,g,b,a)
   string a = m.getAddress();
    if(a == "/mapVal"){
        JEvent* e = getEventById(m.getArgAsInt(2));
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
    } else if(a == "/update"){
      updateValues(m);
    } else if(a == "/done" && m.getArgAsString(0) == "/notify"){
        bIsNotified = true;
    } else if(a == "/create"){
      create(m);
    } else if(a == "/kill"){
      kill(m);
    } else if(a == "/connect"){
      connect(m);
    } else if(a == "/trigger"){
      trigger(m);
    } else if(a == "/buffer"){
      setBuffer(m);
    }
}

bool MsgParser::kill(ofxOscMessage& m){
  cout << "kill()" << endl;
  cout << m << endl;
  JEvent* e = getEventById(m.getArgAsInt(0), m.getArgAsInt(1));
  cout << e << endl;
  if(e){
    delete e;
    cout << "Done" << endl;
    return true;
  } else{
    return false;
  }
}

bool MsgParser::create(ofxOscMessage& m){
    cout << m << endl;
    cout << "ID: " << m.getArgAsInt(0) << ", type: " << (int)encodedIntToChar(m.getArgAsInt(1), 0) << ", subID: " << (int)encodedIntToChar(m.getArgAsInt(1), 1) << endl;
    JEvent* e = nullptr;

    bool bInit = false;
    switch(encodedIntToChar(m.getArgAsInt(1), 0)){
        case jevent::JRectangle: e = new JRectangle(); break;
        case jevent::JModifierArray: e = new JModifierArray(); break;
        case jevent::JVorm: e = new JVorm(); bInit = true; break;
        case jevent::JLine: e = new JLine(); break;
        default:
            return false;
    }

    updateValues(m, e);

    e->id = m.getArgAsInt(0);
    e->subID = encodedIntToChar(m.getArgAsInt(1), 1);
    addEvent(e, e->layerID, e->id, false);

    if(bInit)
      e->init();
    return true;
}

bool MsgParser::connect(ofxOscMessage& m){
  JEvent* e = getEventById(m.getArgAsInt(0), m.getArgAsInt(1)); // Use subID
  if(!e)
    return false;

  switch((int)m.getArgAsFloat(2)){
    case jevent::ConnectionType::Modifier:{
    JEvent* target = getEventById(m.getArgAsInt(0), m.getArgAsInt(3));
    if(target){
      target->modifiers.push_back((JModifier*)e);
      // Modifier (e) is already set in /create (by using busses[5]);
    }
  }
  break;
  }
  return true;
}

bool MsgParser::trigger(ofxOscMessage& m){
  // cout << m << endl;
  // for(int i=0; i<m.getNumArgs(); i++)
    // cout << m.getArgAsInt(i) << endl;
    int targetID;
    float targetIDf = m.getArgAsFloat(2);
    memcpy(&targetID, &targetIDf, sizeof(float));
  // cout << "Target: " << targetID << ", " << m.getArgAsInt(3) << endl; // ID, subID
  JEvent* target = getEventById(targetID, m.getArgAsInt(3));
  if(target){
    // cout << target << endl;
    int numArguments = m.getNumArgs()-5;
    float* arguments = new float[numArguments]; // 10 - 5 = 5
    for(int i=0; i<numArguments; i++){
      arguments[i] = m.getArgAsFloat(i+5);
    }
    target->doFunc(m.getArgAsInt(4), arguments);
    delete arguments;
    // target->doFunc(m.getArgAsInt(4), arguments);
  } else{
    cout << "Target not found, " << targetID << ", " << m.getArgAsInt(3) << endl;
    return false;
  }
  return false;
}

bool MsgParser::setBuffer(ofxOscMessage& m){
  int targetID;
  float targetIDf = m.getArgAsFloat(0);
  memcpy(&targetID, &targetIDf, sizeof(float));
  JEvent* target = getEventById(targetID, m.getArgAsInt(1));
  if(target){
    target->buffer.clear();
    ofBuffer b = m.getArgAsBlob(2); // Parse as floats
    for(int i=0; i<b.size()/sizeof(float); i++){
      float v;
      memcpy(&v, b.getData()+(i*sizeof(float)), sizeof(float));
      target->buffer.push_back(v);
    }
  } else{
    cout << "Target not found, " << targetID << ", " << m.getArgAsInt(1) << endl;
    return false;
  }
  return false;
}

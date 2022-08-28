//
//  MsgParser.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 05-03-19.
//

#include "MsgParser.hpp"

MsgParser::MsgParser(ofxJVisuals* v){
    this->v = v;
#ifdef USE_SC
    connectToSuperCollider(); // Test
#endif
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
        "setValMod"
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
        "JModifier"
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
        "zoom"
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

//void MsgParser::addEvent(JEvent* e){
//    v->addEvent(e);
//}

MsgParser::~MsgParser(){
    ofxOscMessage msg;
    msg.setAddress("/quit");
    scClient.sendMessage(msg);
}

// The rest of the functions is in ofxJVisuals.hpp...
void MsgParser::update(){
    if(bIsNotified == false){
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

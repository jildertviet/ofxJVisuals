//
//  MsgParser.hpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 05-03-19.
//

#ifndef MsgParser_hpp
#define MsgParser_hpp

#include <stdio.h>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include "config.h"

#ifdef USE_SC
#include "ofxOscBidirectional.h"
#else
#include "ofxOsc.h"
#endif

//#include "ofxJVisuals.hpp"
#include "JRectangle.hpp"
#include "JEvent.hpp"
#include "JVecField.hpp"
#include "JMirror.h"
#include "JEllipse.h"
#include "SpaceCube.h"
#include "MultiMesh.hpp"
#include "JText.hpp"
#include "JMesh.hpp"
#include "JImage.hpp"
#include "JDivisionGrid.hpp"
#include "JVideoPlayer.hpp"
#include "JShaderTest.hpp"
#include "MultiMeshMaybeTomorrow.hpp"
#include "JPhysarum.hpp"
#include "JLine.hpp"
#include "JShader.hpp"
#include "JShaderLines.hpp"
#include "JVorm.h"
#include "JNoise.hpp"
#include "AlphaBlackScreen.hpp"

#include "scSynth.hpp"

using namespace std;

enum VisualizerLayer {
    FUNCTIONAL, // Should be deleted?
    NON_CAM_BACK,
    NON_CAM_FRONT,
    DUMMY0,
    DUMMY1,
    DUMMY2,
    DUMMY3,
    DUMMY4,
    DUMMY5,
    NEGATIVE, // Dummies, otherwise this would be 3, and override layer 3...
    DEFAULT
};

class ofxJVisuals;

class MsgParser{
public:
    MsgParser(ofxJVisuals* v, bool bUseSC);
    ~MsgParser();
    bool parseMsg(ofxOscMessage& m);
    void update();
    bool make(ofxOscMessage& m);
    bool create(ofxOscMessage& m);
    bool kill(ofxOscMessage& m);
    bool connect(ofxOscMessage& m);
    bool trigger(ofxOscMessage& m);
    bool setBuffer(ofxOscMessage& m);
    bool updateValues(ofxOscMessage& m, JEvent* e = nullptr);
    map<string, short> commands;
    map<string, short> types;
    map<string, short> values;
    map<string, short> envValues;
//    void addEvent(Event* e);

    void setVal(ofxOscMessage& m);
//    void setValType(ofxOscMessage& m);
    void addEnv(ofxOscMessage& m);
    void addTo(ofxOscMessage& m);
    ofxOscSender* SCsender = nullptr;
    ofxOscSender* initSCsender(string ip, int port);

    // Defined in ofxJVisuals
    JEvent* getEventById(unsigned int id, unsigned int subID=0);
    JEvent* addEvent(JEvent* e, int layerIndex=1, int index = 0, bool bOldVersion = true);
    ofTrueTypeFont* getSelectedFont();
    vector<JShader*>* getShaders();
    ofFbo* getFbo();
    void killAll();

    void getFreePointers(string host, int port); // Sends to 6063, w/ SCsender
    void setBrightness(float b);
    void setCam();
    void initCam();
    void setAlpha(float a);
    void setCirclularMaskState(bool bState);
    void setCameraState(bool bState);
    ofEasyCam* getCam();
    AlphaBlackScreen* getAlphaScreen();
private:
    ofxJVisuals* v = nullptr;
    bool bUseSC = false;
    ofxOscSenderReceiver scClient;
    void connectToSuperCollider();
    void onSuperColliderMessageReceived(ofxOscMessage &m);
    scSynth* synth;
    bool bIsNotified = false;
};

#endif /* MsgParser_hpp */

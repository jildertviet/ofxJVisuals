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

#include "scSynth.hpp"

using namespace std;

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

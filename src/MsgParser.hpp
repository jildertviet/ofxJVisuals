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
#include "Event.hpp"
#include "JVecField.hpp"
#include "JMirror.h"
#include "JEllipse.h"
#include "SpaceCube.h"
#include "MultiMesh.hpp"
#include "jTxt.hpp"
#include "JMesh.hpp"
#include "JImage.hpp"
#include "JDivisionGrid.hpp"
#include "JVideoPlayer.hpp"
#include "JShaderTest.hpp"
#include "MultiMeshMaybeTomorrow.hpp"
#include "JPhysarum.hpp"

#include "scSynth.hpp"

using namespace std;

class ofxJVisuals;

class MsgParser{
public:
    MsgParser(ofxJVisuals* v);
    ~MsgParser();
    bool parseMsg(ofxOscMessage& m);
    bool make(ofxOscMessage& m);
    map<string, short> commands;
    map<string, short> types;
    map<string, short> values;
    map<string, short> envValues;
//    void addEvent(Event* e);
    
    void setVal(ofxOscMessage& m);
//    void setValType(ofxOscMessage& m);
    void addEnv(ofxOscMessage& m);
    void addTo(ofxOscMessage& m);
    ofxOscSender* SCsender;
private:
    ofxJVisuals* v = nullptr;
    ofxOscSenderReceiver scClient;
    void connectToSuperCollider();
    void onSuperColliderMessageReceived(ofxOscMessage &m);
    scSynth synth;
};

#endif /* MsgParser_hpp */

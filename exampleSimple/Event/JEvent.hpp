//
//  JEvent.hpp
//  AudioAnalyzer
//
//  Created by Jildert Viet on 24-01-16.
//
//

#ifndef JEvent_hpp
#define JEvent_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxOscBidirectional.h"
#include "Env.hpp" // USE_FRAMES_AS_CLOCK is set here
#include "mapValue.h"
#include "JModifier.hpp"

//#define USE_FRAMES_AS_CLOCK false

class JEvent{
public:
    JEvent();
    ~JEvent();
    // ----------------------------------------------------------------------------------------------------
    bool active = false;
    virtual void update();
        void updateMain();

    virtual void display(){};
        void displayMain();
    
    virtual void test();
    virtual void ownDtor(){};
    unsigned int startTime = 0;
    unsigned int endTime = 1000;
    
    void deleteNext();
    
    void deleteWithFade(short int releaseTime);
    // ----------------------------------------------------------------------------------------------------
    int* numEventsPtr = nullptr;
    void setType(string type);
    string type = "JEvent";
    unsigned int id = 0;
    
    JEvent* getEvent(int index);
    
    virtual void setLoc(ofVec3f loc);
    ofVec3f size, loc, direction;
    vector<ofColor> colors = {ofColor(255,255)}; // Make stuff visible by default

    float speed = 1.;
    virtual void setSpeed(float speed);
    bool bMove = false;
    bool bEvolve = false;

    JEvent* next = nullptr;
    JEvent* previous = nullptr;
  
    virtual void specificFunction(){ return; };
    
    void addEvent(JEvent* toAdd);
    void addEventAsFirst(JEvent* toAdd);
    
    JEvent* getLast();
    
    // Envelope-stuff
    short getNumEnv();
    Env* getLastEnv();
    static bool checkIfEnvIsDone(Env* e);
    bool checkIfEnvExists(char id);
    void loopLastEnv();
    void clearEnv();
    vector<vector<Env*>> envelopes;
    
    void setEndTime(float duration);

    void setEnvelope(int attack, int sustain, int release, ofVec2f range = ofVec2f(0,255));
    void setEnvelope(int attack, int sustain, int release, int* value, ofVec2f range);
    void setEnvelope(int attack, int sustain, int release, float* value, ofVec2f range, int id=0, bool SetEndTime=true);
    
    void addEnvAlpha(float a, float s, float r, float alpha=255);
    void addEnvAlpha(vector<float> levels, vector<float> times, char curve=0);
    Env* addEnv(Env* e);
    Env* addEnv(vector<float> levels, vector<float> times, float* f, char curve=0);
    Env* addEnv(vector<float> levels, vector<float> times, int* i, char curve=0);
    Env* addEnv(vector<float> levels, vector<float> times, ofColor* c, char curve=0);
    Env* lastEnv = nullptr;
    
    bool bCheckBordersH = true;
    bool bCheckBordersV = true;
    virtual void checkBorders();
    
    virtual void setAlpha(unsigned char alpha);
    virtual void setColor(ofColor color, int index=0);

    // To clear references made to this object
    vector<JEvent**> toClear;
    JEvent** lastPtr = nullptr;
    virtual void addPtr(JEvent** p);

    vector<JEvent*>* parentContainer = nullptr;
    
    bool bFill = true;
    
    virtual void setSize(ofVec3f s);
    virtual void customOne(){};
    virtual void customTwo(){};
    virtual void customThree(){};
    virtual void customFour(){};
    virtual void customFive(){};
    float customOneArguments[5]; // Only use ptr?
    float customTwoArguments[5];
    JEvent** events = nullptr;
    
    virtual void setMode(char m);
    char mode = 0;
    ofVec3f rotation = ofVec3f(0, 0, 0);
    unsigned long getTimeMillis();
    void setStartTime();
    float zoom = 1.f;
    
    struct {
            bool operator()(Env* a, Env* b) const { return a->value < b->value; }
        } customLess;
    
    mapValue* mapValues[24];
    void setDefaultMappers();
    vector<JModifier*> modifiers;
private:

};

#endif /* JEvent_hpp */
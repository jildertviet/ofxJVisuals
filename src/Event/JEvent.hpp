//
//  JEvent.hpp
//  AudioAnalyzer
//
//  Created by Jildert Viet on 24-01-16.
//
//

#ifndef JEvent_hpp
#define JEvent_hpp

#include "Env.hpp" // USE_FRAMES_AS_CLOCK is set here
#include "JEventBase.hpp"
#include "config.h"
#include "mapValue.h"
#include "ofMain.h"
#include "ofxOscBidirectional.h"
#include <stdio.h>

//#define USE_FRAMES_AS_CLOCK false
class JModifier;
class ofxJVisuals;

class JEvent : public JEventBase {
public:
  JEvent();
  ~JEvent();
  // ----------------------------------------------------------------------------------------------------
  bool active = false;
  virtual void update();
  void updateMain();

  virtual void display(){};
  void displayMain();

  virtual void ownDtor(){};
  virtual void init(){};

  unsigned int startTime = 0;
  unsigned int endTime = 1000;

  void deleteNext();

  void deleteWithFade(short int releaseTime);
  // ----------------------------------------------------------------------------------------------------
  int *numEventsPtr = nullptr;
  void setType(string type);
  string type = "JEvent";
  unsigned int id = 0;

  JEvent *getEvent(int index);

  virtual void setLoc(glm::vec3 loc);
  // color = ofColor::white; // Make stuff visible by default

  virtual void setSpeed(float speed);
  // bool bMove = false;
  bool bEvolve = false;

  JEvent *next = nullptr;
  JEvent *previous = nullptr;

  virtual void specificFunction() { return; };

  void addEvent(JEvent *toAdd);
  void addEventAsFirst(JEvent *toAdd);

  JEvent *getLast();

  // Envelope-stuff
  short getNumEnv();
  Env *getLastEnv();
  static bool checkIfEnvIsDone(Env *e);
  bool checkIfEnvExists(char id);
  void loopLastEnv();
  void clearEnv();
  vector<vector<Env *>> envelopes;

  void setEndTime(float duration);

  void setEnvelope(int attack, int sustain, int release,
                   ofVec2f range = ofVec2f(0, 255));
  void setEnvelope(int attack, int sustain, int release, int *value,
                   ofVec2f range);
  void setEnvelope(int attack, int sustain, int release, float *value,
                   ofVec2f range, int id = 0, bool SetEndTime = true);

  void addEnvAlpha(float a, float s, float r, float alpha = 255);
  void addEnvAlpha(vector<float> levels, vector<float> times, char curve = 0);
  Env *addEnv(Env *e);
  Env *addEnv(vector<float> levels, vector<float> times, float *f,
              char curve = 0);
  Env *addEnv(vector<float> levels, vector<float> times, int *i,
              char curve = 0);
  Env *addEnv(vector<float> levels, vector<float> times, ofColor *c,
              char curve = 0);
  Env *lastEnv = nullptr;

  bool bCheckBordersH = true;
  bool bCheckBordersV = true;
  virtual void checkBorders();

  virtual void setAlpha(unsigned char alpha);
  virtual void setColor(ofColor color);

  // To clear references made to this object
  vector<JEvent **> toClear;
  JEvent **lastPtr = nullptr;
  JEvent **lastFound = nullptr;
  virtual void addPtr(JEvent **p);

  vector<JEvent *> *parentContainer = nullptr;

  virtual void setSize(glm::vec3 s);
  virtual void setSize(glm::vec2 s) { setSize(glm::vec3(s, 0)); }
  virtual void customOne(float *v = nullptr){};
  virtual void customTwo(float *v = nullptr){};
  virtual void customThree(float *v = nullptr){};
  virtual void customFour(float *v = nullptr){};
  virtual void customFive(float *v = nullptr){};
  void doFunc(int id, float *arguments = nullptr);
  // float busses[5]; // Only use ptr?
  float customTwoArguments[5];
  JEvent **events = nullptr;

  virtual void setMode(char m);
  // char mode = 0;
  // ofVec3f rotation = ofVec3f(0, 0, 0);
  unsigned long getTimeMillis();
  void setStartTime();
  float zoom = 1.f;

  struct {
    bool operator()(Env *a, Env *b) const { return a->value < b->value; }
  } customLess;

  mapValue *mapValues[24];
  void setDefaultMappers();
  vector<JModifier *> modifiers;
  void displayModifiers();
  void updateModifiers();
  void deleteModifiers();
  virtual void setViaBusses(vector<float> busses){};
  void fillBuffer(ofxOscMessage &m);
  vector<float> buffer; // For JLine
  virtual void parseBuffer(){};

  ofxJVisuals *v;
  JEvent *getEventById(unsigned int id, unsigned int subID = 0);
  JEvent &getAddr() { return *this; };
  JEvent &getAddrNext() { return next->getAddr(); };
  JEvent &at(int num) {
    if (num > 0 && next) {
      return next->at(num - 1);
    } else {
      return *this;
    }
    return *this;
  }

private:
};

#endif /* JEvent_hpp */

//
//  Env.hpp
//  Visualizer_2_4
//
//  Created by Jildert Viet on 25-09-17.
//
//

#ifndef Env_hpp
#define Env_hpp

#include "ofMain.h"
#include <stdio.h>
//#include "Wavetables.hpp"
#ifdef ENV_EASING
#include "ofxEasing.h"
#endif

#define USE_FRAMES_AS_CLOCK false

class Pointer {
public:
  Pointer(float *valF_) : valF(valF_) {}
  Pointer(int *valI_) : valI(valI_) {}
  Pointer(ofColor *valC_) : valC(valC_) {}

  void writeValue(float value);

  float *valF = nullptr;
  int *valI = nullptr;
  ofColor *valC = nullptr;
  int id;

  void *getPtr();

private:
};

class Env {
public:
  using uint8 = unsigned char;
  Env();
  ~Env();
  Env(vector<float> levels, vector<float> times);
  Env(vector<float> levels, vector<float> times, float *f, char curve);
  Env(vector<float> levels, vector<float> times, int *i, char curve);
  Env(vector<float> levels, vector<float> times, ofColor *c, char curve);
#ifdef ENV_EASING
  Env(vector<float> levels, vector<float> times, float *f,
      ofxeasing::function func);
#endif
  void trigger(vector<float> levels, vector<float> times);

  uint8_t id = 0;
  float value = 0;

  bool bEasingSet = false;
#ifdef ENV_EASING
  ofxeasing::function curveType;
#endif

  vector<float> levels;
  vector<float> times;
  int totalRunTime = 0;
  uint8 timesIndex = 0;
  bool process(bool bWrite = true);

  Pointer *ptr = nullptr;

  int startTime;
  void setStartTime();
  unsigned long getTime();
  bool active = true;
  char curve = 0;
  bool direction = 1;

  void getDirection(uint8 ti = 0);
  void setLoop(bool b = true);

  // For saving the env to a file... (To use in Blender w/ Python)
  bool bSave = false;
  void setSave();
  float *saveBuffer = nullptr;
  string fileName;
  int saveBufferWritePos = 0;
  int saveBufferSize = 0;
  void saveValueToBuffer(float v);
  unsigned int *parentID = nullptr;

  bool bDone = false;

  bool operator<(const Env &env) const { return (value < env.value); }

private:
  bool loop = false;
};
#endif /* Env_hpp */

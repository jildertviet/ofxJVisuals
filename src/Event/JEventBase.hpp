#ifndef JEventBase_hpp
#define JEventBase_hpp

// Should only be included in SC / cmake, right?
#ifndef  OF_VERSION_MAJOR
  #include "glm.hpp"
#endif

#include "ofColor.h"

#define NUM_BUSSES  32

namespace jevent{
  enum JEventType{
    JRectangle,
    JModifierArray,
    JVorm,
    JFunctionTrigger,
    JBufferSender,
    JLine
  };
  enum ConnectionType{
    Modifier,
  };
};


class JEventBase{
public:
  JEventBase(){
    for(int i=0; i<NUM_BUSSES; i++)
      values[i] = nullptr;
    linkValues();
  };

  void linkValues(){
    values[0] = &loc.x;
    values[1] = &loc.y;
    values[2] = &loc.z;

    values[3] = &size.x;
    values[4] = &size.y;
    values[5] = &size.z;

    values[6] = &direction.x;
    values[7] = &direction.y;
    values[8] = &direction.z;

    values[9] = &rotation.x;
    values[10] = &rotation.y;
    values[11] = &rotation.z;

    values[12] = &speed;
    values[13] = &lineWidth;

    values[14] = &bMove;
    values[15] = &layerID;

    values[16] = &rgba.r;
    values[17] = &rgba.g;
    values[18] = &rgba.b;
    values[19] = &rgba.a;

    values[20] = &bFill;
    values[21] = &mode;

    values[22] = &busses[0];
    values[23] = &busses[1];
    values[24] = &busses[2];
    values[25] = &busses[3];
    values[26] = &busses[4];
    values[27] = &busses[5];
    values[28] = &busses[6];
    values[29] = &busses[7];
    values[30] = &busses[8];
    values[31] = &busses[9];
  }

  float* values[NUM_BUSSES];

  float* valuesToFloatArray(){
    float* valuesToSend = new float[NUM_BUSSES];
    for(int i=0; i<NUM_BUSSES; i++){
      if(values[i]){
        valuesToSend[i] = *values[i];
      } else{
        valuesToSend[i] = 0.0;
      }
    }
    return valuesToSend;
  };

  void setValuesFromFloatArray(float* a){
    for(int i=0; i<NUM_BUSSES; i++){
      if(values[i])
        *values[i] = a[i];
    }
    color = ofColor(rgba[0], rgba[1], rgba[2], rgba[3] * 255);
  }

  glm::vec3 loc = {0,0,0};
  glm::vec3 size = {100, 100, 100};
  glm::vec3 direction = {1, 0, 0};
  glm::vec3 rotation = {0, 0, 0};

  float speed = 1;
  float lineWidth = 1;

  float bMove = true;
  float layerID = 2;
  float bFill = true;
  float mode = 0;

  ofColor color; // There's a SC-version!!!
  glm::vec4 rgba = {255, 255, 255, 1.0};
  unsigned int subID = 0;

  jevent::JEventType type;

  float busses[10];
};
#endif // JEventBase

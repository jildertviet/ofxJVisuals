#ifndef JEventBase_hpp
#define JEventBase_hpp

#include "glm.hpp"
#include "ofColor.h"

#define NUM_BUSSES  32

namespace jevent{
  enum JEventType{
    JRectangle
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
  }

  glm::vec3 loc = {0,0,0};
  glm::vec3 size = {100, 100, 100};
  glm::vec3 direction = {1, 0, 0};
  glm::vec3 rotation = {0, 0, 0};

  float speed = 1;
  float lineWidth = 1;

  float bMove = true;
  float layerID = 2;

  ofColor color; // SC-version!!!
};
#endif // JEventBase

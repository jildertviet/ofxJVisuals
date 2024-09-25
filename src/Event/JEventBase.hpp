#ifndef JEventBase_hpp
#define JEventBase_hpp

// Should only be included in SC / cmake, right?
#ifndef OF_VERSION_MAJOR
#include "glm.hpp"
#endif

#include "ofColor.h"
#include <iostream>
#define NUM_VALUES 23
#define MAX_VALUES 64
#define MAX_BUSSES MAX_VALUES - NUM_VALUES

namespace jevent {
enum JEventType {
  JRectangle,
  JModifierArray,
  JVorm,
  JFunctionTrigger,
  JBufferSender,
  JLine,
  JWaveform,
  JModifierRedraw,
  JShader,        // 8
  JEventDynamicS, // This is only used for compilation of code. Object should be
                  // not used in OF
  JEventCircle
};

enum ConnectionType {
  Modifier,
};
}; // namespace jevent

class JEventBase {
public:
  JEventBase(){};

  void linkValues(float numBusses) {
    this->numBusses = numBusses;
    totalNumValues = numBusses + NUM_VALUES;
    // busses = new float[(int)numBusses];
    // values = new float *[totalNumValues];
    for (int i = 0; i < totalNumValues; i++)
      values[i] = nullptr;

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
    values[22] = &numBusses;

    for (int i = 0; i < numBusses; i++) {
      values[23 + i] = &busses[i];
    }
  }

  // float **values = nullptr;

  float *valuesToFloatArray() {
    // std::cout << "valuesToFloatArray" << std::endl;
    // float *valuesToSend = new float[totalNumValues];
    for (int i = 0; i < totalNumValues; i++) {
      if (!values) {
        std::cout << "Values is nullptr" << std::endl;
        return nullptr;
      }
      if (values[i]) {
        valuesToSend[i] = *values[i];
      } else {
        valuesToSend[i] = 0.0;
      }
    }
    // std::cout << "valuesToFloatArrayEnd" << std::endl;
    return valuesToSend;
    // return values;
  };

  void setValuesFromFloatArray(float *a) {
    for (int i = 0; i < totalNumValues; i++) {
      if (values[i])
        *values[i] = a[i];
    }
    color = ofColor(rgba[0], rgba[1], rgba[2], rgba[3] * 255);
  }

  glm::vec3 loc = {0, 0, 0};
  glm::vec3 size = {100, 100, 100};
  glm::vec3 direction = {1, 0, 0};
  glm::vec3 rotation = {0, 0, 0};

  float speed = 1;
  float lineWidth = 1;

  float bMove = false;
  float layerID = 2;
  float bFill = true;
  float mode = 0;

  ofColor color; // There's a SC-version!!!
  glm::vec4 rgba = {255, 255, 255, 1.0};
  unsigned int subID = 0;

  jevent::JEventType type;

  // float *busses;
  int totalNumValues = 0;
  float valuesToSend[MAX_VALUES];
  float *values[MAX_VALUES];
  float busses[MAX_VALUES - NUM_VALUES];
  float numBusses = 10;
};
#endif // JEventBase

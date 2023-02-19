#ifndef JEventBase_hpp
#define JEventBase_hpp

#include "glm.hpp"

#define NUM_BUSSES  32

class JEventBase{
public:
  JEventBase(){};

  float* values[NUM_BUSSES];

  glm::vec3 loc = {0,0,0};
  glm::vec3 size = {100, 100, 100};
  glm::vec3 color = {255, 255, 255};
  float brightness = 1;
  float layerID = 2; // 11

  float speed = 1;
  glm::vec3 direction = {1, 0, 0};
  float bMove = true;
  float lineWidth = 1;
  float bFill = true;

  glm::vec3 rotation = {0, 0, 0};
};
#endif // JEventBase

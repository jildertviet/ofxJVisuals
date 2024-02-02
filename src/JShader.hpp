#ifndef JShader_hpp
#define JShader_hpp

#include "JEvent.hpp"
#include "ofMain.h"
#include <stdio.h>

class JShader : public JEvent {
public:
  JShader();
  ofShader s;
  void load(string name);
  void begin();
  void end();

  void parseBuffer();
};
#endif /* JShader_hpp */

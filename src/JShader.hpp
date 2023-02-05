#ifndef JShader_hpp
#define JShader_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"

class JShader: public JEvent{
  public:
    JShader();
    ofShader s;
    void load(string name);
    void begin();
    void end();
};
#endif /* JShader_hpp */

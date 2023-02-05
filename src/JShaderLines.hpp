#ifndef JShaderLines_hpp
#define JShaderLines_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JShader.hpp"

class JShaderLines: public JShader{
  public:
    JShaderLines();
    void update();
    float numLines = 108;
    float lineWidth = 1.65;
    float diagonalScale = 18.0;
    float diagonalPow = 1.8;
};
#endif /* JShaderLines_hpp */

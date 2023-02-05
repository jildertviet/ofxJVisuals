#include "JShaderLines.hpp"

JShaderLines::JShaderLines(){

}

void JShaderLines::update(){
  s.setUniform2f("size", size.x, size.y);
  s.setUniform1f("numLines", numLines);
  s.setUniform1f("lineWidth", lineWidth);
  s.setUniform1f("diagonalScale", diagonalScale);
  s.setUniform1f("diagonalPow", diagonalPow);
}

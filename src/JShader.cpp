#include "JShader.hpp"

JShader::JShader(){

}

void JShader::load(string name){
  s.load("../../../../../addons/ofxJVisuals/libs/shaders/" + name);
}

void JShader::begin(){
  s.begin();
}

void JShader::end(){
  s.end();
}

#include "JShader.hpp"

JShader::JShader() { setType("JShader"); }

void JShader::ownDtor() {
  cout << "Deconstruct shader" << endl;
  if (parent) {
    for (int i = 0; i < parent->size(); i++) {
      if (parent->at(i) == this) {
        parent->at(i) = nullptr;
        parent->erase(parent->begin() + i);
        cout << "Shader found in parent at index: " << i << endl;
        break;
      }
    }
  }
}

void JShader::load(string name) {
  s.load("../../../../../addons/ofxJVisuals/libs/shaders/" + name);
}

void JShader::begin() {
  s.begin();
  s.setUniform2f("iResolution", ofGetWindowSize());
  s.setUniform1f("iTime", ofGetElapsedTimeMillis());
  for (int i = 1; i < 10; i++) { // Bus 1 is for type...
    s.setUniform1f("bus_" + ofToString(i - 1), busses[i]);
  }
}

void JShader::end() { s.end(); }

void JShader::parseBuffer() {
  char *p = new char[buffer.size() + 1];
  memset(p, 0x00, buffer.size() + 1); // Null terminated
  for (int i = 0; i < buffer.size(); i++) {
    p[i] = (char)buffer[i];
  }
  string path(p);
  cout << "Load shader: " << path << endl;
  if (s.load(p))
    cout << "Shader loaded" << endl;
}

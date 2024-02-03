#ifndef JShader_hpp
#define JShader_hpp

#include "JEvent.hpp"
#include "ofMain.h"
#include <stdio.h>

class JShader : public JEvent {
public:
  JShader();
  void ownDtor() override;
  ofShader s;
  void load(string name);
  void begin();
  void end();

  void parseBuffer();
  vector<JShader *> *parent = nullptr;
};
#endif /* JShader_hpp */

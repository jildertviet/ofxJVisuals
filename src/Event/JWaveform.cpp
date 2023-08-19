#include "JWaveform.hpp"

JWaveform::JWaveform(){
  setType("JWaveform");
}

void JWaveform::display(){
  path.setFillColor(color);
  path.draw(0, 0);
}

void JWaveform::specificFunction(){

}

void JWaveform::parseBuffer(){
  path.clear();
  path.moveTo(loc.x, loc.y);
  for(int i=0; i<buffer.size(); i++){
    float x = size.x / buffer.size();
    x *= i;
    x += loc.x;
    path.lineTo(x, buffer[i] + loc.y);
  }
  path.lineTo(loc.x + size.x, 0 + loc.y);
  path.lineTo(loc.x, loc.y);
  path.close();
}

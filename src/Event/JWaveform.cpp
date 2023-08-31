#include "JWaveform.hpp"

JWaveform::JWaveform(){
  setType("JWaveform");
}

void JWaveform::display(){
  switch((char)mode){
    case 0:
      path.setFillColor(color);
      path.draw(0, 0);
    break;
    case 1:
      ofSetColor(color);
      m.draw();
    break;
  }

}

void JWaveform::specificFunction(){

}

void JWaveform::parseBuffer(){
  switch((char)mode){
    case 0:{
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
    break;
    case 1:{
      int lineSpacing = busses[0]; // Bad name?
      m.clear();
      m.setMode(OF_PRIMITIVE_TRIANGLES);
      for(int i=0; i<buffer.size(); i+=lineSpacing){
        float x = size.x / buffer.size();
        x *= (i);
        x += loc.x;
        m.addVertex(glm::vec3(x - lineWidth, loc.y, loc.z));
        m.addVertex(glm::vec3(x + lineWidth, loc.y, loc.z));
        m.addVertex(glm::vec3(x + lineWidth, buffer[i] + loc.y, loc.z));

        m.addVertex(glm::vec3(x + lineWidth, buffer[i] + loc.y, loc.z));
        m.addVertex(glm::vec3(x - lineWidth, buffer[i] + loc.y, loc.z));
        m.addVertex(glm::vec3(x - lineWidth, loc.y, loc.z));
      }
    }
    break;
  }
}

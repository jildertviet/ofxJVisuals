//
//  JLine.cpp
//  Visualizer_2_3
//
//  Created by Jildert Viet on 18-09-17.
//
//

#include "JLine.hpp"

JLine::JLine(){
    setType("JLine");

    color = ofColor(255);
    speed = 2;
    seed = ofRandom(1000);
    loc = glm::vec3(0, ofGetHeight()/2, 0); // Middle of screen
//    balance = ofRandom(1.0);
//    heightOffset = 100;
    size = glm::vec3(ofGetWindowSize(), 0);
}

void JLine::specificFunction(){
    // line.clear();
    switch((int)mode){
        case 0:
          line.clear();
        //    line.addVertex(ofVec2f(0, ofGetHeight()/2));
            line.curveTo(loc);
            for(int i=0; i<(numPoints+2); i++){
                float noiseVal = ofNoise((ofGetElapsedTimef() * speed) + (i*2) + seed) * 2 - 1; // -1 to 1
                noiseVal = noiseVal * sin((i/(float)numPoints)*PI);
                float height = amplitude * noiseVal;
                height += loc.y;
                height += ofMap(i, 0, numPoints+2, -heightOffset * balance, heightOffset * (1-balance));
        //        cout << noiseVal << endl;
                line.curveTo(
                             ofVec3f(loc.x + (size.x+100)*(i/(float)numPoints) - 100,
                                     height,
                                     loc.z
                                     ),
                             50);
            }
            break;
        case 1:

          break;
    }
}

void JLine::display(){
    ofSetColor(color);
    ofSetLineWidth(3);

    if(rotation != 0){
        ofPushMatrix();
        ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
        ofRotateDeg(rotation);
        ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
        line.draw();
        ofPopMatrix();
    } else{
      line.draw();
    }
}

void JLine::toRect(ofVec2f loc, ofVec2f size){
    mode = 1;
    // Make env to move loc and size
}

void JLine::fromBuffer(){
  mode = 2;
  line.clear();
  line.addVertex(buffer[0], buffer[1]);
  for(int i=1; i<buffer.size()/2; i++){
    glm::vec2 v = glm::vec2(buffer[i*2], buffer[i*2+1]);
    line.lineTo(ofPoint(v));
  }
  // line.addVertex(buffer[buffer.size()-2], buffer[buffer.size()-1]);
  // for(int i=0; i<line.getVertices().size(); i++){
    // cout << line.getVertices()[i][0] << ", " << line.getVertices()[i][1] << endl;
  // }
}

void JLine::setLoc(glm::vec3 loc){
  this->loc = loc;
  calcLine();
}

void JLine::setSize(glm::vec3 size){
  this->size = size;
  calcLine();
}

void JLine::calcLine(){
  switch((int)mode){
    case 0:{

    }
    case 1:{
      line.clear();
      line.addVertex(loc.x, loc.y);
      line.addVertex(loc.x + size.x, loc.y);
      line.addVertex(loc.x + size.x, loc.y+size.y);
      line.addVertex(loc.x, loc.y+size.y);
      line.addVertex(loc.x, loc.y);
    }
    case 2:{

    }
  }
}

void JLine::customOne(){
  fromBuffer();
}

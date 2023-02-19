//
//  Image.hpp
//  Bas
//
//  Created by Jildert Viet on 18-03-16.
//
//

#ifndef Image_hpp
#define Image_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#include "ofxSvg.h"

class JSvg: public JEvent{
public:
  JSvg(string path){
    svg.load(path);
  };
  void display(){
    ofScale(scaleFact.x, scaleFact.y);
    svg.draw();
  }
  void setSize(glm::vec3 size){
    scaleFact = size / glm::vec2(svg.getWidth(), svg.getHeight());
  }
  glm::vec2 scaleFact = glm::vec2(1.0);
  ofxSVG svg;
};

class JImage: public JEvent{
    enum DrawMode{
        DEFAULT
    };
public:
    JImage(string filename, glm::vec3 loc = glm::vec3(0.0));
    JImage(){};
    ofImage image;

    void display();
    void specificFunction();

    DrawMode drawMode = JImage::DrawMode::DEFAULT;

    bool loadImage(string path);
    bool bLoadSucces = false;

    JSvg* svg = nullptr;
    void setSize(glm::vec3 size) override;
};
#endif /* JImage_hpp */

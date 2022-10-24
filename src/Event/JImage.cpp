//
//  Image.cpp
//  Bas
//
//  Created by Jildert Viet on 18-03-16.
//
//

#include "JImage.hpp"

JImage::JImage(string filename, ofVec2f loc){
    this->loc = loc;
    setType("JImage");

    bLoadSucces = loadImage(filename);
    colors[0] = ofColor(255,255);
}

void JImage::display(){
    if(!bLoadSucces)
        return;
    ofSetColor(colors[0]);
    ofPushMatrix();
    ofTranslate(loc);
    if(zoom != 1.0){
        ofTranslate(size.x * 0.5, size.y * 0.5);
        ofScale(zoom);
        ofTranslate(size.x * -0.5, size.y * -0.5);
    }

    switch(drawMode){
        case DEFAULT:{
            if(svg){
              svg->display();
            } else{
              image.draw(0, 0, size.x, size.y);
            }
          }
            break;
    }

    ofPopMatrix();
}

void JImage::specificFunction(){

}

bool JImage::loadImage(string path){
  if(ofStringTimesInString(path, ".svg") != 0){
    svg = new JSvg(path);
    size = glm::vec2(svg->svg.getWidth(),svg->svg.getHeight());
    cout << "Img is SVG" << endl;
    return true;
  } else{
    if(svg){
      delete svg;
      svg = nullptr;
    }
    image.clear();
    if(image.load(path)){
        cout << "Image " << path << " loaded" << endl;
        size = glm::vec2(image.getWidth(), image.getHeight());
        return true;
    } else{
        return false;
    }
  }
}

void JImage::setSize(glm::vec3 size){
  if(svg){
    svg->setSize(size);
  } else{
    size = size;
  }
}

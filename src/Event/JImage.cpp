//
//  Image.cpp
//  Bas
//
//  Created by Jildert Viet on 18-03-16.
//
//

#include "JImage.hpp"

JImage::JImage(string filename, glm::vec3 loc){
    this->loc = loc;
    setType("JImage");

    bLoadSucces = loadImage(filename);
    color = ofColor(255,255);
}

void JImage::display(){
    if(!bLoadSucces)
        return;
    ofSetColor(color);
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
    size = glm::vec3(svg->svg.getWidth(),svg->svg.getHeight(), 0.0);
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
        size = glm::vec3(image.getWidth(), image.getHeight(), 0.0);
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

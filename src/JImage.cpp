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
        case DEFAULT:
            image.draw(0, 0, size.x, size.y);
            break;
    }
    
    ofPopMatrix();
}

void JImage::specificFunction(){
    
}

bool JImage::loadImage(string path){
    image.clear();
    if(image.load(path)){
        cout << "Image " << path << " loaded" << endl;
        size = glm::vec2(image.getWidth(), image.getHeight());
        return true;
    } else{
        return false;
    }
}

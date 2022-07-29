//
//  imageFloat.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 02-03-19.
//

#include "imageFloat.hpp"

imageFloat::imageFloat(string src){
    setType("imageFloat");
    if(!load(src))
        cout << "imageFloat: Failed to load img" << endl;
}

imageFloater::imageFloater(imageFloat* src){
    setType("imageFloater");
    img = src;
    colors[0] = ofColor::white;
    
    size = ofVec3f(200, 200, 0);
    bMove = true;
    direction = ofVec3f(1, 0, 0);
    roiSpeed = ofVec2f(ofRandom(0.1, 1.0), ofRandom(0.1, 1.0));
    roi = ofVec2f(ofRandom(0, img->getWidth() - size.x), ofRandom(0, img->getHeight() - size.y));
}

void imageFloater::display(){
    ofSetColor(colors[0]);
    img->drawSubsection(loc.x, loc.y, loc.z, size.x, size.y, roi.x, roi.y);
}

void imageFloater::imageFloating(){
    roi += roiSpeed;
    if(roi.x < 0 || roi.x > img->getWidth() - size.x){
        roiSpeed.x *= -1.;
    }
    if(roi.y < 0 || roi.y > img->getHeight() - size.y){
        roiSpeed.y *= -1.;
    }
}

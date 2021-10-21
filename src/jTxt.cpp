//
//  jTxt.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 02-03-19.
//

#include "jTxt.hpp"

jText::jText(ofTrueTypeFont* f){
    this->f = f;
}

void jText::display(){
    ofSetColor(colors[0]);
    if(!bCamEnabled){
        f->drawString(txt, loc.x, loc.y + f->getStringBoundingBox(txt, 0, 0).height);
        return;
    }
    if(*bCamEnabled == false){
        f->drawString(txt, loc.x, loc.y + f->getStringBoundingBox(txt, 0, 0).height);
    } else{
        // When ofEasyCam is used, I call ofScale(1, -1, 1), and this mirrors the txt, so this fixes it :/
        ofPushMatrix();
        ofTranslate(0, -loc.y - (f->getStringBoundingBox(txt, 0, 0).height*0.5));
        ofRotateX(180);
        f->drawString(txt, loc.x, -loc.y * 2 - (f->getStringBoundingBox(txt, 0, 0).height*1.5));
        ofPopMatrix();
    }
}

void jText::setText(string txt){
    this->txt = txt;
}

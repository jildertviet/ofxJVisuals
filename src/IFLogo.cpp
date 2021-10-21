//
//  IFLogo.cpp
//  Visualizer_2_8
//
//  Created by Jildert Viet on 14-10-19.
//

#include "IFLogo.hpp"

IFLogo::IFLogo(ofVec2f loc, ofVec2f size){
    setType("IFLogo");
    rBack = new JRectangle();
    rBack->setColor(ofColor(255));
    this->loc = loc;
    setSize(size);
}

void IFLogo::specificFunction(){
    if(envelopes.size()){
        rBack->colors[0].a = colors[0].a;
        for(char i=0; i<4; i++)
            r4[i]->colors[0].a = colors[0].a;
    }
}

void IFLogo::display(){
    ofPushMatrix();
    ofTranslate(loc);
    rBack->display();
    for(char i=0; i<4; i++)
        r4[i]->display();
    int x, y;
    ofSetColor(colors[0]);
    x = r4[0]->loc.x + (r4[0]->size.x * 0.5);
    y = r4[0]->loc.y + (r4[0]->size.y * 0.5);
    ofDrawCircle(x, y, r4[0]->size.x * 0.51);
    
    ofSetPolyMode(OF_POLY_WINDING_NONZERO);
    ofBeginShape();
        ofVertex(r4[3]->loc + ofVec2f(r4[3]->size.x * 0.5, r4[3]->size.y));
        ofVertex(r4[2]->loc + ofVec2f(r4[2]->size.x, 0));
        ofVertex(r4[3]->loc + ofVec2f(r4[3]->size.x, r4[3]->size.y));
    ofEndShape();
    ofPopMatrix();
}

void IFLogo::setSize(ofVec3f size){
    this->size = size;
    rBack->size = size;
    
    for(char i=0; i<2; i++){
        for(char j=0; j<2; j++){
            char index = i*2 + j;
            r4[index] = new JRectangle();
            int p = size.x * 0.04;
            int w = size.x * 0.5 - (p*1.5);
            r4[index]->setSize(ofVec2f(w, w));
            r4[index]->setLoc(ofVec2f(i * (w+p) + p, j * (w+(p)) + p));
            r4[index]->setColor(ofColor::black);
        }
    }
    r4[1]->setColor(ofColor::red);
}

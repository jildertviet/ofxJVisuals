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

    colors[0] = ofColor(255);
    speed = 2;
    seed = ofRandom(1000);
    loc = ofVec2f(0, ofGetHeight()/2); // Middle of screen
//    balance = ofRandom(1.0);
//    heightOffset = 100;
    size = ofGetWindowSize();
}

void JLine::specificFunction(){
    line.clear();
    switch(mode){
        case 0:
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
            line.lineTo(loc.x, loc.y);
            line.lineTo(loc.x+size.x, loc.y);
            line.lineTo(loc.x+size.x, loc.y+size.y);
            line.lineTo(loc.x, loc.y+size.y);
            line.lineTo(loc.x, loc.y);
            break;
    }
}
void JLine::display(){
    ofSetColor(colors[0]);
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

//
//  OnlyYoursLogo.cpp
//  Visualizer_2_4
//
//  Created by Jildert Viet on 06-10-17.
//
//

#include "OnlyYoursLogo.hpp"

OnlyYoursLogo::OnlyYoursLogo(){
    scale = 0.8;
    loc = ofVec2f(ofGetWidth()/2. - 250, ofGetHeight()/2. - 300);
    int a = 40;
    colors[0] = ofColor(255,a);
    colors[1] = ofColor(165, 41, 47,a);
    colors[2] = ofColor(39, 166, 151,a);
    colors[3] = ofColor(0,a);
    
    bezierPoints.push_back(ofVec2f(342,216));
    bezierPoints.push_back(ofVec2f(296,398));
    bezierPoints.push_back(ofVec2f(89,231));
    bezierPoints.push_back(ofVec2f(342,644));
}

void OnlyYoursLogo::display(){
    ofPushMatrix();
    ofScale(scale, scale, 1);
    ofTranslate(loc.x, loc.y, 0); // 250 is haf width
    ofTranslate(10,10,0);
    
    for(int i=numLayers-1; i>-1; i--){
        displayOneLayer(colors[i]);
        ofTranslate(layerOffset.x,layerOffset.y,0);
    }
    ofFill();
    ofPopMatrix();
}

void OnlyYoursLogo::displayOneLayer(ofColor c){
    displayRects(c);
    drawLinesLeft();
    drawLinesRight();
}

void OnlyYoursLogo::drawLinesLeft(){
    if(mode==0){
        ofNoFill();
        ofSetLineWidth(lineWidth);
        ofSetColor(colors[3]);
        ofBeginShape();
        ofVertex(216,178);
        ofBezierVertex(216,251,66,332,0,363);
        ofBezierVertex(0,363,216,260,216,358);
        ofEndShape();

        ofBeginShape();
        ofVertex(0,305);
        ofVertex(77,322);
        ofVertex(17,182);
        ofVertex(151,272);
        ofEndShape();
 
        ofBeginShape();
        ofVertex(32,545);
        ofVertex(0,592);
        ofVertex(92,458);
        ofVertex(216,570);
        ofBezierVertex(216,570,221,384,0,487);
        ofEndShape();
    
        ofBeginShape();
        ofVertex(0,592);
        ofVertex(216,570);
        ofEndShape();
    }else if(mode==1){
        ofNoFill();
        ofSetLineWidth(lineWidth);
        ofSetColor(colors[3]);
        
        ofBeginShape();
        ofVertex(289,118);
        ofBezierVertex(289,216,bezierPoints[2].x,bezierPoints[2].y, 0,231);
        ofEndShape();

        ofBeginShape();
        ofVertex(0,154);
        ofVertex(104,225);
        ofVertex(22,0);
        ofVertex(203,204);
        ofEndShape();

        ofBeginShape();
        ofVertex(43,495);
        ofVertex(0,538);
        ofVertex(124,416);
        ofVertex(289,644);
        ofBezierVertex(289,644,bezierPoints[1].x,bezierPoints[1].y,0,398);
        ofEndShape();
        
        ofBeginShape();
        ofVertex(0,538);
        ofVertex(289,644);
        ofEndShape();
    }
}

void OnlyYoursLogo::drawLinesRight(){
    ofNoFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(colors[3]);
    
    if(mode==0){
        ofBeginShape();
        ofVertex(255,157);
        ofBezierVertex(255, 157, 298, 190, 473, 143);
        ofEndShape();

        ofBeginShape();
        ofVertex(255,554);
        ofBezierVertex(255,554,251,287,472,143);
        ofEndShape();
        
        ofBeginShape();
        ofVertex(255,554);
        ofBezierVertex(255,554,336,306,472,300);
        ofEndShape();
        
        ofBeginShape();
        // Vink onder
            ofVertex(255,554);
            ofVertex(337, 598);
        
            ofVertex(337, 598);
            ofVertex(472, 300);
        ofEndShape();
        
        
        ofBeginShape();
    //    Z boven
        ofVertex(255,157);
        ofVertex(411, 28);
        ofVertex(411, 28);
        ofVertex(373, 163);
        ofVertex(373, 163);
        ofVertex(472, 85);
        ofEndShape();
    } else if(mode==1){
        ofBeginShape();
        ofVertex(631,231);
        ofBezierVertex(542,231,bezierPoints[0].x,bezierPoints[0].y,342,118);
        ofEndShape();

        ofBeginShape();
        ofVertex(631,398);
        ofBezierVertex(335,398,bezierPoints[3].x, bezierPoints[3].y,342,644);  //  bezierPoints.push_back(ofVec2f(342,644));
//        ofBezierVertex(335,398,342,644 + ofRandom(-150,150),342,644);  //  bezierPoints.push_back(ofVec2f(342,644));
//        ofVertex(469,757);
//        ofVertex(631,398);
        ofEndShape();

        ofBeginShape();
        ofVertex(342,87);
        ofVertex(486,219);
        ofVertex(379,38);
        ofVertex(623,231);
        ofEndShape();
    }
}

void OnlyYoursLogo::displayRects(ofColor c){
    ofFill();
    ofSetColor(c);
    ofSetLineWidth(lineWidth);
    if(mode==0){
        ofBeginShape();
        ofVertex(216,626);
        ofVertex(0,727);
        ofVertex(0,190);
        ofVertex(216,89);
        ofVertex(216,626);
        ofEndShape();
        
        ofBeginShape();
            ofVertex(472,267);
            ofVertex(472,536);
            ofVertex(350,593);
            ofVertex(255,637);
            ofVertex(255,552);
            ofVertex(255,551);
            ofVertex(255,99);
            ofVertex(472,-1);
            ofVertex(472,267);
            ofVertex(472,267);
        ofEndShape();
        
        ofNoFill();
        ofSetColor(colors[3]);
        ofBeginShape();
        ofVertex(216,626);
        ofVertex(0,727);
        ofVertex(0,190);
        ofVertex(216,89);
        ofVertex(216,626);
        ofEndShape();
        
        ofBeginShape();
        ofVertex(472,267);
        ofVertex(472,536);
        ofVertex(350,593);
        ofVertex(255,637);
        ofVertex(255,552);
        ofVertex(255,551);
        ofVertex(255,99);
        ofVertex(472,-1);
        ofVertex(472,267);
        ofVertex(472,267);
        ofEndShape();
    } else if(mode==1){
        ofBeginShape();
        ofVertex(631,757);
        ofVertex(342,757);
        ofVertex(342,38);
        ofVertex(631,38);
        ofVertex(631,757);
        ofEndShape();

        ofBeginShape();
        ofVertex(289,719);
        ofVertex(0,719);
        ofVertex(0,0);
        ofVertex(289,0);
        ofVertex(289,719);
        ofEndShape();
        
        ofNoFill();
        ofSetColor(colors[3]);
        ofBeginShape();
        ofVertex(289,719);
        ofVertex(0,719);
        ofVertex(0,0);
        ofVertex(289,0);
        ofVertex(289,719);
        ofEndShape();
        
        ofBeginShape();
        ofVertex(631,757);
        ofVertex(342,757);
        ofVertex(342,38);
        ofVertex(631,38);
        ofVertex(631,757);
        ofEndShape();
    }
}

void OnlyYoursLogo::setAlpha(char alpha, int depth){
    for(int i=0; i<depth; i++){
        colors[i].a = alpha;
    }
}

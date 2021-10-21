//
//  AlphaBlackScreen.cpp
//  Paul
//
//  Created by Jildert Viet on 14-02-16.
//
//

#include "AlphaBlackScreen.hpp"

AlphaBlackScreen::AlphaBlackScreen(bool state){
    setType("AlphaBlackScreen");
    colors[0] = ofColor::black;
    setActiveness(state);
    setAlpha(255);
}

AlphaBlackScreen::~AlphaBlackScreen(){
    setActiveness(false);
}

void AlphaBlackScreen::display(){
    if(bDoAlphaBlend){
        if(bGradient){
            ofColor edgeColor(0, 0, 0, gradientAlpha);
            ofColor center = gradientColor;
            center.a = gradientAlpha;
            ofBackgroundGradient(center, edgeColor, OF_GRADIENT_CIRCULAR);
        } else{
            ofFill();
            ofSetColor(colors[0]);
            ofDrawRectangle(0, 0, size.x, size.y);
        }
    } else{
        return;
    }
}

void AlphaBlackScreen::setActiveness(bool state){ // Weird ... @02-2020
    bDoAlphaBlend = state;
    if(state){
        ofSetBackgroundAuto(false);
        ofEnableAlphaBlending();
    } else{
        ofSetBackgroundAuto(true);
//        ofDisableAlphaBlending();
        ofBackground(colors[0]);
    }
}

void AlphaBlackScreen::setAlpha(int alpha){
    if(alpha>=255 || !bDoAlphaBlend)
        setActiveness(false);
    gradientAlpha = alpha;
    colors[0].a = alpha;
}

void AlphaBlackScreen::specificFunction(){
//    cout << "Alpha: " << colors[0].a << endl;
//    if(hasAlphaBlending){
//        if(!alphaIs255){
//            if(colors[0].a == 255){
//                alphaIs255 = true;
//                
//                ofSetBackgroundAuto(true);
//                ofDisableAlphaBlending();
//                ofBackground(0);
//            }
//        } else{
//            if(colors[0].a < 255){
//                alphaIs255 = false;
//                
//                ofSetBackgroundAuto(false);
//                ofEnableAlphaBlending();
//            }
//        }
//    } else{
//        return;
//    }
}


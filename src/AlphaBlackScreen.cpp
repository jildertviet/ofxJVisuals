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
    color = ofColor::black;
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
            ofSetColor(color);
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
        ofBackground(color);
    }
}

void AlphaBlackScreen::setAlpha(int alpha){
    if(alpha>=255 || !bDoAlphaBlend)
        setActiveness(false);
    gradientAlpha = alpha;
    color.a = alpha;
}

void AlphaBlackScreen::specificFunction(){
//    cout << "Alpha: " << color.a << endl;
//    if(hasAlphaBlending){
//        if(!alphaIs255){
//            if(color.a == 255){
//                alphaIs255 = true;
//                
//                ofSetBackgroundAuto(true);
//                ofDisableAlphaBlending();
//                ofBackground(0);
//            }
//        } else{
//            if(color.a < 255){
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


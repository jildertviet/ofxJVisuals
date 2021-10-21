
//
//  JFboWindow.cpp
//  visualizer
//
//  Created by Jildert Viet on 12-12-19.
//

#include "JFboWindow.hpp"

JFboWindow::JFboWindow(){

}

void JFboWindow::setup(){
    ofSetFrameRate(frameRate);
    ofSetVerticalSync(false);
    if(bFullScreen)
        ofSetFullscreen(true);
//    ofSetWindowPosition(ofGetScreenWidth(), 0);
}

void JFboWindow::update(){
    
}

void JFboWindow::draw(){
    ofBackground(255, 0, 0);
    fbo->getTexture().drawSubsection(0, 0, ofGetWidth(), ofGetHeight(), 0, 0);
//    fbo->getTexture().draw(0, 0);
}

void JFboWindow::keyPressed(int key){
    switch(key){
        case 'f':
            bFullScreen = !bFullScreen;
            ofSetFullscreen(bFullScreen);
            break;
    }
}


//
//  JShaderTest.cpp
//  Visualizer_2_8
//
//  Created by Jildert Viet on 14-10-19.
//

#include "JShaderTest.hpp"

JShaderTest::JShaderTest(ofVec2f size){
    setType("JShaderTest");
    shader.load("shaders/shaderTest");
    this->size = size;
    mask.allocate(size.x * 0.5, size.y * 0.5, GL_RGBA);
    src.allocate(size.x, size.y, GL_RGBA);
    noiseSeed = ofRandom(1000);
    speed = 1;
}

void JShaderTest::specificFunction(){
    mask.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(colors[0].a, 0 ,0);
    ofPushMatrix();
//    ofTranslate(size.x*0.25, size.y*0.25);
    //    ofRotateZDeg(ofNoise((float)(ofGetFrameNum()+300)*0.01)*10);
//    ofTranslate(-size.x*0.25, -size.y*0.25);
    
    for(int i=0; i<divisions.x; i++){
        int x, y;
        int w, h;
        w = (size.x * 0.5) / divisions.x * (0.1 + ofNoise(noiseSeed + (float)ofGetFrameNum()*speed*0.01)*0.9);
        h = size.x*3*0.5;
        y = -size.y * 0.5;
        x = ((size.x*0.5)/divisions.x) * i;
        ofDrawRectangle(x, y, w, h);
    }
    ofPopMatrix();
    mask.end();
    
    
    src.begin();
    ofClear(colors[0].r, colors[0].g, colors[0].b, 0);
    ofSetColor(0, 0, 0, 255);
    ofPushMatrix();
    ofTranslate(size.x*0.5, size.y*0.5);
    ofRotateZDeg(sin(noiseSeed + (float)ofGetFrameNum()*0.01*speed)*10);
    ofTranslate(-size.x*0.5, -size.y*0.5);
    for(int i=0; i<divisions.y; i++){
        int x, y;
        int w, h;
        h = size.y / (divisions.y*0.5) * (0.1 + ofNoise(noiseSeed + (float)(ofGetFrameNum()+800)*speed*0.01)*0.9);
        w = size.x * 3;
        y = (size.y/(divisions.y*0.5)) * i;
        x = -size.y*1;
        y -= (size.y * 0.50);
        ofDrawRectangle(x, y, w, h);
    }
    
    ofPopMatrix();
    src.end();
}

void JShaderTest::display(){
    ofPushMatrix();
    ofTranslate(loc);
    shader.begin();
    shader.setUniformTexture("mask", mask.getTexture(), 1);
    //    shader.setUniformTexture("src", mask.getTexture(), 2);
    src.draw(0,0);
    shader.end();
    ofPopMatrix();
}

void JShaderTest::customOne(){
    divisions = ofVec2f((int)customOneArguments[0], (int)customOneArguments[1]);
}

//
//  CounterParts.cpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#if USE_OPENCL

#include "CounterParts.hpp"

CounterParts::CounterParts(ofxJVisuals* v){
    this->v = v;
    cout << "Song: CounterParts" << endl;
    ps = (particleSystem*)v->addEvent(new particleSystem(1000000), NON_CAM_FRONT); // Was 1.000.000 // LAter 2.000.000
    ps->mouseControl = true;
    ps->color = ofColor(255, 255, 255, 100);
//    ps->forces[0]->power = 300;
    ps->bMirrorMouse = false; // Was true
//    ofHideCursor();
    
    vecField = (JVecField*)addEvent(new JVecField());
    vecField->speed = 0.01;
    vecField->setSize(ofVec3f(1280*3, 800, 0));
    vecField->setDensity(ofVec2f(128, 80));
    vecField->bDrawColors = true;
    vecField->drawMode = VECFIELD_MODE::HIDE;
    vecField->complexity = 50;
//    vecField->setPixelsToTest();
    ps->setVecField(vecField);
}

void CounterParts::F0(){ // Env 1||
//    ps->setEnvelope(100, 200, 600, &ps->centerForce, ofVec2f(ps->centerForce, ps->centerForce*0.125));
}
void CounterParts::F1(){ // Env 2||
//    ps->setEnvelope(100, 200, 300, &ps->centerForce, ofVec2f(ps->centerForce, ps->centerForce*0.25));
}
void CounterParts::F2(){ // Env 3||
//    ps->setEnvelope(5, 50, 200, &ps->centerForce, ofVec2f(ps->centerForce, ps->centerForce*0.125));
}
void CounterParts::F3(){ // Env 4||
//    ps->setEnvelope(5, 50, 50, &ps->centerForce, ofVec2f(ps->centerForce, ps->centerForce*0.125));
}
void CounterParts::F4(){ // Ball ||
//    ps->centerForce = 0.00673622;
}
void CounterParts::F5(){ // ||
    
}
void CounterParts::F6(){ // ||
    
}
void CounterParts::F7(){ // ||
    
}
void CounterParts::F8(){ // ||
    
}
void CounterParts::F9(){ // ||
    
}
void CounterParts::F10(){ // ||
    
}
void CounterParts::F11(){ // ||
    
}
void CounterParts::F12(){ // ||
    
}
void CounterParts::F13(){ // ||
    
}
void CounterParts::F14(){ // ||
    
}
void CounterParts::F15(){ // ||
    
}

void CounterParts::C0(int val){ // PS speed ||
    float speed = ofMap(val, 0, 127, 0.0005, 0.05);
//    cout << speed << endl;
//    ps->centerForce = speed;
//    ps->resetSpeedDiv = ofMap(val, 0, 127, 9, 3);
}
void CounterParts::C1(int val){ // Damp ||
//    ps->damp = ofMap(val, 0, 0127, 0.003f, 0.1f);
}
void CounterParts::C2(int val){ // Speed & Fade||
    C0(val);
    doControlFunc(56, 127-val);
}
void CounterParts::C3(int val){ // Force power ||
    int power = ofMap(val, 0, 127, 100, 600);
//    ps->forces[0]->power = power;
}
void CounterParts::C4(int val){ // ||
    
}
void CounterParts::C5(int val){ // ||
    
}
void CounterParts::C6(int val){ // ||
    
}
#endif// #ifndef TARGET_RASPBERRY_PI


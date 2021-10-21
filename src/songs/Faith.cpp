//
//  Faith.cpp
//  Bas
//
//  Created by Jildert Viet on 18-04-16.
//
//

#include "Faith.hpp"

Faith::Faith(ofxJVisuals* v){
    setName("Faith");
    this->v = v;
    
//    v->alphaScreen->bGradient = false;
    
//    v->setAlpha(100);
    makeCube();
}

void Faith::stop(){
    v->alphaScreen->bGradient = false;
}

void Faith::F0(){ // Line||
    makeVerticalParticleLine(0, 8, ofGetWindowWidth(), 0.9, 1.0);
}

void Faith::F1(){ // Line connected||
    makeVerticalLineConnectedToCube();
}

void Faith::F2(){ // Cube mirror||
    if(cube)
        cube->mirror();
}

void Faith::F3(){ // Cube switch state||
    if(cube)
        cube->switchState();
}

void Faith::F4(){ // V Mirror||
    v->bMirror = !v->bMirror;
}

void Faith::F5(){ // V Add Mirror||
    v->bAddMirror = !v->bAddMirror;
}

void Faith::F6(){ // ||
//    v->alphaScreen->gradientColor = ofFloatColor(0.90980392156863,0.74901960784314,0.33725490196078, 1);
//    v->alphaScreen->bGradient = !v->alphaScreen->bGradient;
}

void Faith::F7(){ // ||
//    v->alphaScreen->gradientColor = ofColor(69, 173, 168);
//    v->alphaScreen->bGradient = !v->alphaScreen->bGradient;
}


void Faith::makeVerticalParticleLine(int x, int amount, int destX, float speedMin, float speedMax){
    if(ofGetFrameRate()<50)
        return;
    v->addEvent(new Vorm(2, amount, ofGetWindowHeight(), ofVec2f(0,ofGetWindowHeight()/2.), true));
    Vorm* line = ((Vorm*)v->getLast());

    line->change_maxspeed(speedMin, speedMax);
    line->makeVertLine(x);
    line->instantFormVorm();
    line->lijnmax = (ofGetWindowHeight()/7.)*(ofGetWindowHeight()/7.);
    line->formVorm();
    line->addToCenter(ofVec2f(ofGetWindowWidth()+100, 0));
    line->active = true;
    line->endTime = ofGetElapsedTimeMillis()+(60000); // 60 sec lifetime
    line->lineWidth = 2;
    line->alphaAdd = 200;
    line->deleteWhenOutOfSight = true;
}

void Faith::makeVerticalLineConnectedToCube(){
    if(!cube)
        return;
    makeVerticalParticleLine(0, 8, ofGetWindowWidth(), 0.9, 1.0);
    Vorm* line = ((Vorm*)getLast());
    line->addConnection(cube);
    
    for(int i=0; i<lastLines.size(); i++){
        if(lastLines[i])
            line->addConnection(lastLines[i]);
    }
    
    lastLines.erase( std::remove_if( lastLines.begin(), lastLines.end(), MamaOtis::checkIfNull ), lastLines.end() );
    
    rememberLine(line);
    line->addVector(&lastLines);
}

void Faith::makeCube(){
    addEvent(new Vorm(4, 4, 400, ofGetWindowSize()/2., true));
    cube = ((Vorm*)getLast());
    cube->instantFormVorm();
    cube->formVorm();
}

void Faith::rememberLine(Vorm* line){
    if(lastLines.size()<1){
        lastLines.push_back(line);
    } else{
        lastLines.insert(lastLines.begin(), line);
        lastLines.pop_back();
    }
}

void Faith::exit(){
    // Fade out alphaScreen
    int fadeTime = 5000;
    
    // Fade out lines...
//    for(int i=0; i<v->events.size(); i++){
//        if(v->events[i]){
//            if(v->events[i]->type == "Vorm")
//                v->events[i]->deleteWithFade(fadeTime);
//        }
//    }
    
    if(cube)
        cube->deleteWithFade(fadeTime + 200);

    cube = nullptr;
}

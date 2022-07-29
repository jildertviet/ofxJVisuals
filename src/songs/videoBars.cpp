//
//  videoBars.cpp
//  Bas
//
//  Created by Jildert Viet on 23-09-16.
//
//

#include "videoBars.hpp"

videoBars::videoBars(ofxJVisuals* v){
    setName("videoBars");
    this->v = v;
    v->cam.reset();
    v->cam.lookAt(ofVec3f(0,0,0));
    v->cam.setDistance(680);
    v->cam.move(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0);
}

void videoBars::F0(){ // switchRandomBins ||
    video->switchRandomBins();
}

void videoBars::F1(){ // randomMirror h ||
    video->randomMirror(1,0);
}

void videoBars::F2(){ // randomMirror v ||
    video->randomMirror(0,1);
}

void videoBars::F3(){ // disableVerticalMirror ||
    video->disableVerticalMirror();
}

void videoBars::F4(){ // randomFade ||
    video->randomFade();
}

void videoBars::F5(){ // duck ||
    vector<string> names = {"Naamloos.mp4"};
    string n = "./video/";
    n += names[ofRandom(names.size())];
    loadNewMovie(n);
    video->player.setVolume(0.);
    video->setMode(1);
    video->setBins(numBins);
    video->loc.z = duckZoom;
    video->addEnv(vector<float>{0, 255, 255, 0}, vector<float>{1000, 13000, 1000}, &video->colors[0]);
}

void videoBars::F6(){ // switchDilate(1) ||
    video->switchDilate(1);
}

void videoBars::F7(){ // mirrorAllBins ||
    video->mirrorAllBins(true, false);
}

void videoBars::F8(){ // Disable Odd ||
    video->disableOddBins();
}

void videoBars::F9(){ // Samsara random ||
    string n = "";
    switch((int)ofRandom(3)){
        case 0:
            n = "video/dust.mov";
            break;
        case 1:
            n = "video/temples.mov";
            break;
        case 2:
            n = "video/desert_timelapse.mov";
            break;
    }
    loadNewMovie(n);
    video->setBins(numBins);
}

void videoBars::F10(){ // Portugal ||
    vector<string> names = {"P0.mov","P1.mov"};
//    vector<string> names = {"zee_edit.mp4"};
    string n = "./video/";
    n += names[ofRandom(names.size())];
    loadNewMovie(n);
    video->setBins(numBins);
}

void videoBars::F11(){ // Restart video ||
    video->player.setFrame(0);
}

void videoBars::F12(){ // All visible ||
    video->allBinsVisible();
}

void videoBars::F13(){ // allRandomBrightness ||
    video->allRandomBrightness();
}

void videoBars::F14(){ // Lines ||
    for(int i=0; i<video->bins.size(); i++){
        int x = video->bins[i]->xPos;
        JRectangle* line = new JRectangle(3000, ofVec2f(x, ofGetHeight() * 0.5), ofVec2f(10, ofGetHeight() * (ofRandom(0.5) + 0.5)));
        line->bMove = false;
        line->colors[0] = ofColor(255, 100);
        addEvent((JEvent*)line, 2);
        line->deleteWithFade(3000);
        line->addEnv(vector<float>{line->size.x, line->size.x * (ofRandom(4) + 1)}, vector<float>{3000}, &line->size.x);
        line->addEnv(vector<float>{line->size.y, line->size.y - 300}, vector<float>{3000}, &line->size.y);
    }
}

void videoBars::F15(){ // Peas / Alp ||
    vector<string> names = {"L0.mov","L1.mov","L2.mov","L3.mov", "A0.mov"};
    string n = "./video/";
    n += names[ofRandom(names.size())];
    loadNewMovie(n);
    video->setBins(numBins);
}

void videoBars::C0(int val){ // NumBins ||
    numBins = ofMap(val, 0, 127, 3, 144);
}

void videoBars::C1(int val){ //  duckZoom ||
    duckZoom = video->loc.z = ofMap(val,0, 127, 0, -2000);
}

void videoBars::C2(int val){ //  ||
    for(int i=0; i<video->bins.size(); i++){
        video->bins[i]->colors[0].a = ofMap(val, 0, 127, 0, 255);
    }
}

void videoBars::C3(int val){ //  ||
    
}

void videoBars::loadNewMovie(string path){
    if(video)
        video->active = true; // Kill prev vid
    addEvent(new JVideoPlayer());
    video = (JVideoPlayer*)getLast();
    video->load(path);
    video->play();
}

void videoBars::exit(){
    video->deleteWithFade(2000);
    for(int i=0; i<video->bins.size(); i++){
        video->bins[i]->doFade(2000);
    }
}

//
//  verses.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 10-09-18.
//

#include "verses.hpp"

verses::verses(ofxJVisuals* v){
    this->v = v;
    setName("verses");
    colors.push_back(ofColor::white);
//    geometricLandscape();
//    camController = new cameraController(&(v->cam));
//    v->addEvent((JEvent*)camController);
    
//    v->cam.setPosition(-3.07814, 121.163, 137.302);
//    v->cam.setOrientation(ofQuaternion(-0.0788498, 0.393574, 0.888034, 0.22423));
}

void verses::rotSquares(int numRects){
    for(short i=0; i<numRects; i++){
        meshRectangle* m = new meshRectangle(ofVec2f((20 * i * 0.3),(20 * i * 0.3)), ofVec3f(0,0, 1 * i));
        if(i!=0)
            m->connection = (meshRectangle*)v->getLast();
        ofColor c = colors[0];
        c.a = 0.4 * 255;
        m->color = ofFloatColor(c);
        m->addEnv(vector<float>{0, ofMap(i, 0, numRects, 0.5, 0.01)}, vector<float>{ofMap(i, 0, numRects, 200, 10000)}, &m->color.a);
        m->lineWidth = lineW;
        float speed;
        speed = ofMap(i, 0, numRects, 0.2, maxSpeed);
        speed = ((i/10. * 0.3 + 0.3)*0.3) * -1;
        speed *= 0.05;
        m->addEnv(vector<float>{speed, speed, (float)(speed * 0.5)}, vector<float>{8000, 8000}, &m->rotateSpeed);
        addEvent((JEvent*) m, 3);
        rects.push_back(m);
    }
}

void verses::F0(){ // rotSquares ||
    rotSquares(numRects);
}

void verses::F1(){ // Env @ camPos? ||
    if(!camController)
        return;
    // Make a camera controller-event?
    vector<float> values = {0., 1.0, 0.};
    vector<float> times = {1000, 1000};
    switch((int)ofRandom(2)){
        case 0:
            camController->addEnv(values, times, &(camController->rotationSpeed.x), 0);
            break;
        case 1:
            camController->addEnv(values, times, &(camController->rotationSpeed.y), 0);
            break;
    }
}

void verses::F2(){ // Reset rotation angle||
    for(int i=0; i<rects.size(); i++){
        rects[i]->rotateAngle = 0;
    }
}

void verses::F3(){ // Squares ||
    F5();
    
    vector<ofVec3f> locations = {
        ofVec3f(0,0,0),
        ofVec3f(-200,0,200),
        ofVec3f(-200,400,200),
        ofVec3f(400,200,0),
        ofVec3f(0,200,-200),
        ofVec3f(0,-400,200),
    };
    
    for(int i=0; i<locations.size(); i++){
        meshRectangle* r = new meshRectangle(ofVec2f(200,200), locations[i]);
        r->rotateSpeed = 0;
        r->color = ofFloatColor(1,1,1);
        if(i==0){
            r->addEnv(vector<float>{0, 1}, vector<float>{5000}, &r->color.a);
        } else{
            r->addEnv(vector<float>{0, 0, 1}, vector<float>{1000 + ofRandom(5000), 5000}, &r->color.a);
        }
        r->setPathWakersSpeed(ofMap(ofRandom(2), 0, 1, 0.35, 0.5));
        r->fillRatio = 0;
        r->lineWidth = 1;
        //        r->setPathWakersSpeed(1);
        addEvent((JEvent*) r, 2);
        rects.push_back(r);
    }
}

void verses::F4(){ // Walk ||
    for(int i=0; i<rects.size(); i++){
        rects[i]->setPathWakersSpeed(ofMap(ofRandom(2), 0, 1, 0.5, 0.65));
        rects[i]->bWalk = !rects[i]->bWalk;
    }
}

void verses::F5(){ // Kill ||
    for(int i=0; i<rects.size(); i++){
        rects[i]->setEndTime(10000);
        vector<float> l = {rects[i]->color.a, 0};
        vector<float> t = {10000};
        rects[i]->addEnv(l, t, &rects[i]->color.a, 0);
        rects[i]->active = true;
    }
    rects.clear();
}

void verses::F6(){ // ||
    
}

void verses::F7(){ // ||
    
}

void verses::F8(){ // ||
    
}

void verses::F9(){ // ||
    
}

void verses::F10(){ // ||
    
}

void verses::F11(){ // ||
    
}

void verses::F12(){ // ||
    
}

void verses::F13(){ // ||
    
}

void verses::F14(){ // ||
    
}

void verses::F15(){ // ||
    
}

void verses::C0(int val){ // numRects||
    numRects = ofMap(val, 0, 127, 1, 144);
}

void verses::C1(int val){ // lineW||
    lineW = ofMap(val, 0, 127, 1, 10);
}

void verses::C2(int val){ // maxSpeed||
    maxSpeed = ofMap(val, 0, 127, 0.2, 3.0);
}

void verses::C3(int val){ // Alpha ||
    for(int i=0; i<rects.size(); i++)
        rects[i]->color.a = ofMap(val, 0, 127, 0., 1.0);
}

void verses::C4(int val){ // ||
    
}

void verses::C5(int val){ // ||
    
}

void verses::C6(int val){ // ||
    
}

void verses::exit(){
    for(int i=0; i<rects.size(); i++){
        rects[i]->setEndTime(10000);
        vector<float> l = {rects[i]->color.a, 0};
        vector<float> t = {10000};
        rects[i]->addEnv(l, t, &rects[i]->color.a, 0);
        rects[i]->active = true;
    }
}

void verses::geometricLandscape(){
    l = new meshLandscape("hmmm.jpg", "a", 30, 1, 10); // Was 800
    //    l->bDisplayWireFrame = true;
    l->bDisplayFaces = true;
    l->setColor(ofColor(255,100));
    l->translate(ofVec3f(-l->size.x*0.5,-l->size.y*0.5,0));
    
    addEvent((JEvent*)l);
    
    v->cam.setPosition(-0.759579, 30.1639, -1064.77);
    v->cam.setOrientation(ofQuaternion(-0.214214, -0.97677, -0.0033634, 0.00564715));
}
//void geometricLandscape2();
//
//
//void whiteStart();
//void whiteSecond();
//void whiteBoth();
//void oneLine();
//void withLandscape();
//void intro();
//void withLines();
void verses::eightShort(){
    mt.clear();
    
    v->alphaScreen->setColor(ofColor(252,247,229));
    l = new meshLandscape("TexturesCom_SnowRough_1024_height.tif", "TexturesCom_SnowRough_1024_normal.tif", 48, 1, 1);
    l->bDisplayFaces = false;
    l->setColor(ofColor(255,50));
    l->translate(ofVec3f(-l->size.x*0.5,-l->size.y*0.5,0));
    addEvent((JEvent*)l);
    
    uint16 numLines = 8;
    
    for(uint16 i=0; i<numLines; i++){
        mt.push_back(new meshWalker(&l->m, l->size - ofVec2f(1,1)));
        addEvent((JEvent*)mt.back());
        mt[i]->locOnGrid = ofVec2f(0, mt[i]->dimensions.y * 0.5 + ((i+0.5)-numLines*0.5) * 512/8);
        mt[i]->bDrawCircles = false;
        mt[i]->getDestination(ofVec2f(0,0));
        mt[i]->loc = mt[i]->destination;
        mt[i]->getDestination(ofVec2f(0,1));
        mt[i]->speed = 0.2;
        mt[i]->trailLength = ofMap(i, 0, 8, 50, 150);
        mt[i]->color = ofFloatColor(1, 1, 1, 0.7);
    }
    
    for(uint16 i=0; i<numLines; i++){
        int numUpdates = 60*20;
        for(int x=0; x<numUpdates; x++){
            mt[i]->specificFunction();
            mt[i]->display();
        }
        //        mt[i]->addEnv(vector<float>{0, 0, 0.7), vector<float>{6000, 3000 + ofRandom(10000)), &mt[i]->color.a);
    }
}

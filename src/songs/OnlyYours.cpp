//
//  OnlyYours.cpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#include "OnlyYours.hpp"

OnlyYours::OnlyYours(ofxJVisuals* v){
    this->v = v;
    setName("OnlyYours");
    
    int a = 100;
    colors.push_back(ofColor(255,a));
    colors.push_back(ofColor(33, 41, 47,a));
    colors.push_back(ofColor(39, 33, 151,a));
    colors.push_back(ofColor(0,a));
    
//    F5();
    short int numBars = 20;
    
    for(char i=0; i<numBars; i++){
        JRectangle* r = (JRectangle*)addEvent(new JRectangle(
                                                             ofVec2f(i * (ofGetWidth()/(numBars)), 0),
                                                             ofVec2f(30, ofGetHeight())
                                                             ),
                                              2);
        r->colors[0] = ofColor::black;
        cout << r->loc << endl;
        r->bMove = false;
        bars.push_back(r);
    }
    
//    OnlyYoursLogo* l = (OnlyYoursLogo*)addEvent(new OnlyYoursLogo(), 2);
//    l->setAlpha(255);
}



void OnlyYours::F0(){ // Make small grid||
    short int a[4] = {950, 750, 650, 950};
    for(int i=0; i<logos.size(); i++){
        OnlyYoursLogo* l = logos[i];
        l->setEndTime(2000); l->active = true;
        for(unsigned char j=0; j<4; j++){
            l->addEnv(v->vec(l->colors[j].a, 0), v->vec(a[j]), &l->colors[j]);
        }
    }
    logos.clear();
    
    for(int y=0; y<7; y++){
        for(int i=0; i<14; i++){
            OnlyYoursLogo* l;
            logos.push_back(new OnlyYoursLogo());
            l = logos.back();
            addEvent(logos.back());
            l->scale = 0.25;
            //        int a[] = {200,-100,400,300};
            //        l->loc += ofVec2f(ofMap(i, 0, 80, -400, 1400), a[(int)ofRandom(4)]);
            l->loc += ofVec2f(i*400, y*400) - ofVec2f(400,120);
            l->loc += ofVec2f(ofRandom(-100,100), ofRandom(-100,100));
            
            l->colors[0] = l->colors[(int)ofRandom(3)];
            l->colors[0].a = 150; l->colors[3].a = 150;
            l->mode = 1;
            l->numLayers = 1;
            l->colors = colors;
            for(short int i=0; i<4; i++){
                l->addEnv(v->vec(0, l->colors[i].a), v->vec(300), &l->colors[i]); // Fade in :)
            }
        }
    }
    C0(whiteColorVal);
}


void OnlyYours::F1(){ // Make big grid ||
    for(int i=0; i<logos.size(); i++){
        OnlyYoursLogo* l = logos[i];
        l->setEndTime(2000); l->active = true;
        l->addEnv(v->vec(l->colors[0].a, 0), v->vec(950), &l->colors[0]);
        l->addEnv(v->vec(l->colors[1].a, 0), v->vec(750), &l->colors[1]);
        l->addEnv(v->vec(l->colors[2].a, 0), v->vec(650), &l->colors[2]);
        l->addEnv(v->vec(l->colors[3].a, 0), v->vec(950), &l->colors[3]);
    }
    logos.clear();
    
    for(int y=0; y<3; y++){
        for(int i=0; i<6; i++){
            OnlyYoursLogo* l;
            logos.push_back(new OnlyYoursLogo());
            l = logos.back();
            addEvent(logos.back());
            l->scale = 0.5;
            //        int a[] = {200,-100,400,300};
            //        l->loc += ofVec2f(ofMap(i, 0, 80, -400, 1400), a[(int)ofRandom(4)]);
            l->loc += ofVec2f(i*400, y*400) - ofVec2f(400,120);
            l->colors[0] = l->colors[(int)ofRandom(3)];
            l->colors[0].a = 150; l->colors[3].a = 150;
            l->mode = 1;
            l->numLayers = 1;
            for(short int i=0; i<4; i++){
                l->addEnv(v->vec(0, l->colors[i].a), v->vec(300), &l->colors[i]); // Fade in :)
            }
        }
    }
    C0(whiteColorVal);
}



void OnlyYours::F2(){ // Env @ Bezier||
    for(int i=0; i<logos.size(); i++){
        for(short int j=0; j<4; j++){
            short int start = logos[i]->bezierPoints[j].y;
            logos[i]->addEnv(v->vec(start, start+(start*ofRandom(-2,2)), start), v->vec(100, 1000+ofRandom(1000)), &(logos[i]->bezierPoints[j].y));
            //            logos[i]->env.back()->loop = true;
            start = logos[i]->bezierPoints[j].x;
            logos[i]->addEnv(v->vec(start, start+(start*ofRandom(-2,2)), start), v->vec(100, 1000+ofRandom(1000)), &(logos[i]->bezierPoints[j].x));
            //            logos[i]->env.back()->loop = true;
        }
    }
}

void OnlyYours::F3(){ // Env @ loc.x||
    for(int i=0; i<logos.size(); i++){
        int x = (logos[i]->loc.x);
        if(ofRandom(2)>1){
            logos[i]->addEnv(v->vec(x, x+100, x), v->vec(50,ofRandom(500)+xPosRelease), &(logos[i]->loc.x));
        } else{
            logos[i]->addEnv(v->vec(x, x-100, x), v->vec(50,ofRandom(500)+xPosRelease), &(logos[i]->loc.x));
        }
    }
}

void OnlyYours::F4(){ // LOGO ||
    OnlyYoursLogo* l;

//    l = logos.back();
    l = (OnlyYoursLogo*)addEvent(new OnlyYoursLogo(), 3);
    l->setEndTime(1500*2); l->active = true;
    short int a[4] = {950, 750, 650, 950};
    for(unsigned char j=0; j<4; j++){
        l->addEnv(v->vec(0, 255, 255, 0), v->vec(50, 100, a[j]*2), &(l->colors[j]));
    }
}

void OnlyYours::F5(){ // Env @ loc.y one ||
    for(char i=0; i<3; i++){
        short index = ofRandom(logos.size()-1);
        int y = logos[index]->loc.y;
        logos[index]->addEnv(v->vec(y, y+ofRandom(200)+20 - 110, y), v->vec(200,300), &logos[index]->loc.y);
    }
}


void OnlyYours::F6(){ // MODE||
    for(short int i=0; i<logos.size(); i++){
        if(i%2==0)
            logos[i]->mode = !logos[i]->mode;
    }
}

void OnlyYours::F7(){ // Env @ scale ||
    for(short int i=0; i<logos.size(); i++){
        OnlyYoursLogo* l = logos[i];
        l->addEnv(v->vec(l->scale, l->scale*1.1, l->scale), v->vec(100,300), &(l->scale));
        l->addEnv(v->vec(l->loc.x, l->loc.x-(100), l->loc.x), v->vec(100,300), &(l->loc.x));
    }
}

void OnlyYours::F8(){ // Move beziers slow ||
    for(int i=0; i<logos.size(); i++){
        // Add env (osc) to loc of logo
//        float start = logos[i]->scale;
//        logos[i]->addEnv(v->vec(start, start+(start * ofRandom(-0.1, 0.1)), start), v->vec(2000+ofRandom(3000), 2000+ofRandom(1000)), &(logos[i]->scale));
//        logos[i]->loopLastEnv();
//        start = logos[i]->loc.y;
//        logos[i]->addEnv(v->vec(start, start+ofRandom(-50,50), start), v->vec(2000+ofRandom(3000), 2000+ofRandom(1000)), &(logos[i]->loc.y));
//        logos[i]->loopLastEnv();
        
        // Add env (osc) to pos of bezier-curve points
        for(short int j=0; j<4; j++){
            short int start = logos[i]->bezierPoints[j].y;
            logos[i]->addEnv(v->vec(start, start+(start*ofRandom(-3,3)), start), v->vec(10000+ofRandom(3000), 10000+ofRandom(1000)), &(logos[i]->bezierPoints[j].y));
            logos[i]->loopLastEnv();
            start = logos[i]->bezierPoints[j].x;
            logos[i]->addEnv(v->vec(start, start+(start*ofRandom(-3,3)), start), v->vec(10000+ofRandom(3000), 10000+ofRandom(1000)), &(logos[i]->bezierPoints[j].x));
            logos[i]->loopLastEnv();
        }
    }
}

void OnlyYours::F9(){ // ||
    
}

void OnlyYours::F10(){ // ||
    
}

void OnlyYours::F11(){ // ||
    
}

void OnlyYours::F12(){ // ||
    
}

void OnlyYours::F13(){ // ||
    
}

void OnlyYours::F14(){ // ||
    
}

void OnlyYours::F15(){ // ||
    
}

void OnlyYours::C0(int val){ // White <> Color ||
    whiteColorVal = val;
    val = ofMap(val, 0, 127, 0, 255);
    for(int i=0; i<logos.size(); i++){
        logos[i]->colors[3] = ofColor(val, logos[i]->colors[3].a);
        logos[i]->setAlpha(ofMap(255-val, 0, 255, 0, 150), 3); // of val omgedraaid?
    }
}

void OnlyYours::C1(int val){ // Release ||
    xPosRelease = ofMap(val, 0, 127, 500, 4000);
}

void OnlyYours::C2(int val){ // Rect alpha||
    for(int i=0; i<bars.size(); i++){
        bars[i]->setAlpha(ofMap(val, 0, 127, 0, 255));
    }
}

void OnlyYours::C3(int val){ // ||
    
}

void OnlyYours::C4(int val){ // ||
    
}

void OnlyYours::C5(int val){ // ||
    
}

void OnlyYours::C6(int val){ // ||
    
}

void OnlyYours::exit(){
    for(int i=0; i<bars.size(); i++){
        bars[i]->deleteWithFade(3000);
    }
    bars.clear();
    
    short a[4] = {950, 750, 650, 950};
    for(int i=0; i<logos.size(); i++){
        OnlyYoursLogo* l = logos[i];
        l->setEndTime(2000); l->active = true;
        for(unsigned char j=0; j<4; j++){
            l->addEnv(v->vec(l->colors[j].a, 0), v->vec(a[j]), &l->colors[j]);
        }
    }
    logos.clear();
}

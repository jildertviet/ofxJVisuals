//
//  NewOpener.cpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#include "NewOpener.hpp"

NewOpener::NewOpener(ofxJVisuals* v){
    this->v = v;
    setName("NewOpener");
    colors.push_back(ofColor::white);
    colors.push_back(ofColor::white);
    colors.push_back(ofColor::white);
}

void NewOpener::F0(){ // add String ||
    if(lines.size()>maxNumLines)
        return;
    JLine* l = (JLine*)addEvent(new JLine());
    l->loc = ofVec3f(0, ofGetHeight()/2. + ofRandom(-300,300), ofRandom(zSpawnPos));
    lines.push_back(l);
    
    l->addEnv(v->vec(0, 255), v->vec(1000), &(l->colors[0]), -2);
    l->loc.x -= 100;
    l->balance = 0.5;
    l->size = ofVec2f((1280*3)*1.2, 1);
    ofColor c = colors[ofRandom(colors.size())];
    c.a = ofRandom(150, 255);
    l->setColor(c);
}

void NewOpener::F1(){ // Excite string ||
//    vector<float> levels = {lines[i]->amplitude, ampOffset+(ofRandom(200,400)/2), 0};
    vector<float> times = {static_cast<float>(attack), static_cast<float>(squareReleaseOffset)};
    for(int i=0; i<lines.size(); i++){
        vector<float> levels = {static_cast<float>(lines[i]->amplitude), ampOffset+(ofRandom(200,400)/2), 0};
//        lines[i]->requestEnvelope(levels, times, -2, new Pointer(&(lines[i]->amplitude)));
        lines[i]->addEnv(levels, times, &(lines[i]->amplitude), -2);
        lines[i]->speed = noiseSpeed;
    }
}

void NewOpener::F2(){ // Tilt ||
    int heightOffset = ofRandom(100,200) * (((int)ofRandom(0,2)*2)-1); // 0 or 2, -1
    for(int i=0; i<lines.size(); i++){
        lines[i]->addEnv(
                         v->vec(lines[i]->heightOffset, heightOffset*3, 0),
                         v->vec(200, ofRandom(tiltRelease*0.8,tiltRelease*1.2)*0.8), &(lines[i]->heightOffset), -2);
    }
}

void NewOpener::F3(){ // ||
    return;
    //Convert to rects
    for(int i=8; i<lines.size(); i++){
        lines[i]->deleteWithFade(500);
//        lines.erase(lines.begin()+i);
    }
//    lines.clear();
    
    vector<ofVec2f> locations;
    vector<ofVec2f> sizes;
    for(int i=0; i<lines.size(); i++){
        if(i>=maxNumLines){
            break;
        }
        sizes.push_back(ofVec2f(ofGetWidth()/4, ofGetHeight()/2));
        sizes.back() *= 0.8;
        for(int j=0; j<2; j++){
            ofVec2f loc = ofVec2f(((i%4)/4.)*ofGetWidth(), ((j/2.)*ofGetHeight()));
//            cout << loc << endl;
            locations.push_back(loc);
            locations.back() += sizes[0] * 0.1;
        }
    }
    
    int transitionTime = 800;
    
    for(int i=0; i<lines.size(); i++){
        if(i>=maxNumLines){
            break;
        }
        lines[i]->mode = 1;
        lines[i]->loc = locations[i];
        lines[i]->addEnv(v->vec(lines[i]->loc.x, locations[i].x), v->vec(transitionTime), &(lines[i]->loc.x), 0);
        lines[i]->addEnv(v->vec(lines[i]->loc.y, locations[i].y), v->vec(transitionTime*0.9), &(lines[i]->loc.y), 2);
        
        cout << sizes[i] << endl;
        lines[i]->addEnv(v->vec(lines[i]->size.x, sizes[i].x), v->vec(transitionTime), &(lines[i]->size.x),0);
        lines[i]->addEnv(v->vec(lines[i]->size.y, sizes[i].y), v->vec(transitionTime*0.9), &(lines[i]->size.y), -1);
        
        lines[i]->setEndTime(transitionTime);
        lines[i]->active = true;
    }
    
    // Make rects
    for(int i=0; i<lines.size(); i++){
        if(i>=maxNumLines){
            break;
        }
        JRectangle* r = (JRectangle*)v->addEvent(new JRectangle(locations[i], sizes[i]));
        r->colors[0] = ofColor::white;
        r->bMove = false;
//        r->requestEnvelopeDelay(v->vec(0, 255, 255, 0), v->vec(100, 1000, 10000), 0, new Pointer(&(r->colors[0])), transitionTime-100);
        r->addEnv(v->vec(0,0,255,255,0), v->vec(transitionTime-100, 100, 1000, 10000), &(r->colors[0]), 0);
        r->bFill = false;
        
        JRectangle* back = (JRectangle*)v->addEvent(new JRectangle(locations[i], sizes[i]));
        back->colors[0] = ofColor::white;
        back->colors[0].a = 0;
        back->bMove = false;
        
        rects.push_back(new JRectangle*[2]);
        
        rects.back()[0] = r;
        rects.back()[1] = back;
    }
    
    lines.clear();
}

void NewOpener::F4(){ //  ||
    //Light 2 squares
    return;
    int i = ofRandom(rects.size());
//    rects[i][1]->requestEnvelope(v->vec(0,maxBright,0), v->vec(10,squareReleaseOffset+ofRandom(200,400)), 0, new Pointer(&(rects[i][1]->colors[0])));
    rects[i][1]->addEnv(v->vec(0,maxBright,0), v->vec(10,squareReleaseOffset+ofRandom(200,400)), &(rects[i][1]->colors[0]), 0);
    int j = ofRandom(rects.size());
    while(j==i){
        j = ofRandom(rects.size());
    }
//    rects[j][1]->requestEnvelope(v->vec(0,maxBright,0), v->vec(10,squareReleaseOffset+ofRandom(200,400)), 0, new Pointer(&(rects[j][1]->colors[0])));
    rects[j][1]->addEnv(v->vec(0,maxBright,0), v->vec(10,squareReleaseOffset+ofRandom(200,400)), &(rects[j][1]->colors[0]), 0);
}

void NewOpener::F5(){ // to Faith ||
    for(int i=0; i<lines.size(); i++){
        if(lines[i]->rotation==0)
            lines[i]->addEnv(v->vec(0, 90), v->vec(10000), &(lines[i]->rotation));
        lines[i]->addEnv(v->vec(lines[i]->loc.y, i*(ofGetHeight()/lines.size())), v->vec(10000), &(lines[i]->loc.y));
        //        lines.erase(lines.begin()+i);
    }
    cout << "Rot " << endl;
}

void NewOpener::F6(){ // Mikado ||
    char dir = (ofRandom(0, 2) * 2) - 1;
    for(char i=0; i<ofRandom(2,4); i++){
        JLine* l = (JLine*)addEvent(new JLine());
        l->loc = ofVec2f(0, ofGetHeight()/2. + ofRandom(-300,300));
        
        l->addEnv(v->vec(0, 255, 0), v->vec(100, 800), &(l->colors[0]), -2);
        
        l->balance = ofRandom(0.3, 0.7);
        l->size = ofVec2f(ofGetWidth()*1.5, 1);
        l->loc.x -= 100;
        
        int heightOffset = ofRandom(100,800) * (((int)ofRandom(0,2)*2)-1); // 0 or 2, -1
        l->addEnv(
                v->vec(l->heightOffset, heightOffset*1, 0),
                v->vec(200, 5000), &(l->heightOffset), -2);
        l->addEnv(
                  v->vec(l->loc.y, l->loc.y, l->loc.y + ofRandom(10,30)),
                  v->vec(50, 800), &(l->loc.y), 0);
    }
}

void NewOpener::F7(){ // 8 lines ||
    for(char i=0; i<8; i++)
        F0();
}

void NewOpener::F8(){ // ||
    
}

void NewOpener::F9(){ // ||
    
}

void NewOpener::F10(){ // ||
    
}

void NewOpener::F11(){ // ||
    
}

void NewOpener::F12(){ // ||
    
}

void NewOpener::F13(){ // ||
    
}

void NewOpener::F14(){ // ||
    
}

void NewOpener::F15(){ // ||
    
}

void NewOpener::C0(int val){ // noiseSpeed ||
    noiseSpeed = ofMap(val, 0, 127, 0.1, 3);
}

void NewOpener::C1(int val){ // ampOffset ||
    ampOffset = ofMap(val, 0, 127, 0, 300);
}

void NewOpener::C2(int val){ // Release ||
    squareReleaseOffset = ofMap(val, 0, 127, 40, 3000);
}

void NewOpener::C3(int val){ //  Attack ||
    attack = ofMap(val, 0, 127, 50, 2000);
}

void NewOpener::C4(int val){ // Tilt release ||
    tiltRelease = ofMap(val, 0, 127, 100, 2000);
}

void NewOpener::C5(int val){ // Line visibility ||
    for(uint8 i=0; i<lines.size(); i++)
        lines[i]->setAlpha(val*2);
}

void NewOpener::C6(int val){ // zSpawnPos ||
    zSpawnPos = ofMap(val, 0, 127, 0, 1000);
}

void NewOpener::exit(){
//    for(int i=0; i<rects.size(); i++){
//        rects[i][0]->deleteWithFade(1000);
//        rects[i][1]->deleteWithFade(1000);
//    }
//    rects.clear();
    for(int i=0; i<lines.size(); i++){
        lines[i]->deleteWithFade(20000);
        lines[i]->addEnv(v->vec(lines[i]->loc.y, lines[i]->loc.y - 1000), v->vec(18000 + ofRandom(3000)), &(lines[i]->loc.y));
    }
}

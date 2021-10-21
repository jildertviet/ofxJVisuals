//
//  TeachMe
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#include "TeachMe.hpp"

TeachMe::TeachMe(ofxJVisuals* v){
    this->v = v;
    cout << "Song: TeachMe" << endl;
    
//    colorsObj = new Colors();
    
    rectMask = (RectangleMask*)v->addEvent(new RectangleMask(), 3);
    
    bg = (JRectangle*)v->addEvent(new JRectangle(ofVec2f(0,0), ofGetWindowSize()));
    bg->setColor(ofColor(255,255,255,0));
    
    F4();
    F5();
    
    rectMask->setAlpha(100);
    
//    C4(127); // bar brightness max
//    C2(0);
//    for(int i=0; i<colorsObj->getByName("BatobeTeachMe").vec.size(); i++){
//        colors.push_back(colorsObj->getByName("BatobeTeachMe").vec[i]);
//    }
    
    for(int i=0; i<20; i++)
        F1();
}

void TeachMe::F0(){ // Rect to Mask ||
    rectMask->addRect(
                      new JRectangle(
                                     ofRandom(1000,2000),
                                     ofVec2f(ofRandom(50, ofGetWindowWidth()-50),0),
                                     ofVec2f(100, ofGetWindowHeight()),
                                     ofColor(255,255,255,255),
                                     ofRandom(400,500),
                                     ofRandom(100,1400)*1.4,
                                     ofVec2f((int)ofRandom(0,2)*2-1,0),
                                     true
                                     )
                      );
}

void TeachMe::F1(){ // Horizontal bar ||
    JRectangle* bar = (JRectangle*)v->addEvent(
                new JRectangle(
                               ofVec3f(ofGetWidth() * 0.5,ofRandom(0,ofGetWindowHeight()-100)),
                               ofVec2f(ofGetWindowWidth(), 100)
                               ),
                        2
                );
    
//    bar->comment = "horizontal bar";
    bars.push_back(bar);
    bar->bCheckBordersH = false;
    bar->speed = ofRandom(5,10)/15;
    
    bar->direction = ofVec2f(0,ofRandom(-1,1));
    bar->setColor(colors[ofRandom(colors.size())]);
//    bar->setEnvelope(200, 0, 0, ofVec2f(0,100));
    bar->addEnv(v->vec(0, 100), v->vec(300), &bar->colors[0]);
    bar->active = false;
}

void TeachMe::F2(){ // Reset all envs||
    for(int i=0; i<bars.size(); i++){
        bars[i]->setEnvelope(ofRandom(2000,9000), 0, 0, ofVec2f(0,100));
        bars[i]->active = false;
    }
    
}

void TeachMe::F3(){ // Reset one bar env||
    JRectangle* b = bars[ofRandom(bars.size())];
    b->setEnvelope(ofRandom(2000,9000), 0, 0, ofVec2f(0,100));
    b->active = false;
    
}

void TeachMe::F4(){ // randomizeSpeeds||
    rectMask->randomizeSpeeds();
}

void TeachMe::F5(){ // randomizeDirections||
    rectMask->randomizeDirections();
}

void TeachMe::F6(){ // 10 rect in mask||
    for(int i=0; i<10; i++)
        F0();
}

void TeachMe::F7(){ // Clap! ||
    v->addEvent(
                new JRectangle(
                               411,
                               ofVec2f(0,0),
                               ofGetWindowSize(),
                               colors[ofRandom(colors.size())],
                               10,
                               400,
                               ofVec2f(0,0),
                               false
                               ), 2
                );
    v->getLast()->setAlpha(200);
}

void TeachMe::F8(){ // Random H-bar direction ||
    for(int i=0; i<bars.size(); i++)
        bars[i]->direction = ofVec2f(0, (int)ofRandom(0,2)*2-1);
}

void TeachMe::F9(){ // Thin Lines Mask||
    for(int i=0; i<10; i++){
        int x = ofGetWindowWidth()/10*i;
        int attack = 200 + (i*200);
        rectMask->addRect(
                          new JRectangle(
                                         attack+3000,
                                         ofVec2f(x,0),
                                         ofVec2f(10, ofGetWindowHeight()),
                                         ofColor(255,255,255,255),
                                         attack,
                                         300,
                                         ofVec2f(1,0)
                                         )
                          );
    }
}

void TeachMe::F10(){ // Thin Lines||
    for(int i=0; i<20; i++){
        int x = ofGetWindowWidth()/20*i;
        int attack = 100 + (i*200);
        v->addEvent(
                    new JRectangle(
                                   attack+401,
                                   ofVec2f(x,ofGetHeight() * 0.5),
                                   ofVec2f(10, ofGetWindowHeight()),
                                   colors[ofRandom(colors.size())],
                                   attack,
                                   400,
                                   ofVec2f(1,0),
                                   true
                                   )
                    );
        v->getLast()->setAlpha(200);
        v->getLast()->bCheckBordersV = false;
    }
}

void TeachMe::F11(){ // Change colors ||
    if(colorsName == "BatobeTeachMe"){
        colorsName = "TeachMeBlue";
//        colors.clear();
//        for(int i=0; i<colorsObj->getByName(colorsName).vec.size(); i++){
//            colors.push_back(colorsObj->getByName(colorsName).vec[i]);
//        }
    } else{
        colorsName = "BatobeTeachMe";
//        colors.clear();
//        for(int i=0; i<colorsObj->getByName(colorsName).vec.size(); i++){
//            colors.push_back(colorsObj->getByName(colorsName).vec[i]);
//        }
    }
    for(unsigned char i=0; i<bars.size(); i++){
        bars[i]->setColor(colors[ofRandom(colors.size())]);
    }
}
void TeachMe::F12(){ // ||
}
void TeachMe::F13(){ // ||
}
void TeachMe::F14(){ // ||
}
void TeachMe::F15(){ // ||
}

void TeachMe::C0(int val){ // Height ||
    float newHeight = ofMap(val, 0, 127, 3, 300);
    for(int i=0; i<bars.size(); i++)
        bars[i]->setHeight(newHeight);
}
void TeachMe::C1(int val){ // Speed ||
    float newSpeed = ofMap(val, 0, 127, 10, 800)/100;
    for(int i=0; i<bars.size(); i++)
        bars[i]->speed = newSpeed;
}
void TeachMe::C2(int val){ // RectMask fade ||
    int newAlpha = ofMap(val, 0, 127, 0, 255);
    rectMask->setAlpha(newAlpha);
}

void TeachMe::C3(int val){ // WHITE ||
    int newAlpha = ofMap(val, 0, 127, 0, 200);
    bg->colors[0].a = newAlpha;
}

void TeachMe::C4(int val){ // Bar brightness ||
    cout << "TeachMe C4()" << endl;
    int newAlpha = ofMap(val, 0, 127, 0, 100);
    for(int i=0; i<bars.size(); i++)
        bars[i]->colors[0].a = newAlpha;
}

void TeachMe::C5(int val){ // LED SCREEN||

}

void TeachMe::C6(int val){ // ||

}

void TeachMe::C7(int val){ // ||

}

void TeachMe::exit(){
    int fadeTime = 5000;
    rectMask->deleteWithFade(fadeTime + 1000);
    for(int i=0; i<bars.size(); i++)
        bars[i]->deleteWithFade(fadeTime);
    bg->deleteWithFade(100);
}


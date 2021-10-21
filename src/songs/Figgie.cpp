//
//  Figgie.cpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#include "Figgie.hpp"

Figgie::Figgie(ofxJVisuals* v){
    this->v = v;
    cout << "Song: Figgie" << endl;

//    colors = new Colors();

    v->addEvent(new RectangleMask(), 3);
    
    rectMask = (RectangleMask*)v->getLast();
    
    for(int i=0; i<40; i++)
        F0();

    F4();
    F5();
    
    rectMask->setAlpha(0);
    
    for(int i=0; i<20; i++)
        F1();
}

void Figgie::F0(){ // ||
    rectMask->addRect(
        new JRectangle(
            ofRandom(5000,6000),
            ofVec2f(ofRandom(50, ofGetWindowWidth()-50),0),
            ofVec2f(100, ofGetWindowHeight()),
            ofColor(255,255,255,255),
            ofRandom(100,500),
            ofRandom(100,2400),
            ofVec2f((int)ofRandom(0,2)*2-1,0),
            true
            )
      );
}

void Figgie::F1(){ // ||
    v->addEvent(
        new JRectangle(
            ofVec2f(0,ofRandom(0,ofGetWindowHeight()-100)),
            ofVec2f(ofGetWindowWidth(), 100)
        )
    );
    
    JRectangle* bar;
    bar = (JRectangle*)v->getLast();
//    bar->comment = "horizontal bar";
    bars.push_back(bar);
    
    bar->speed = ofRandom(5,10)/15;
    
    bar->direction = ofVec2f(0,ofRandom(-1,1));
//    bar->setColor(colors->getRandomColor("figgieDakota")); // See .h
    bar->setAlpha(100);
    bar->setEnvelope(4000, 60000*9.5, 2000, ofVec2f(0,100));
}

void Figgie::F2(){ // ||
    for(int i=0; i<bars.size(); i++)
        bars[i]->setEnvelope(ofRandom(2000,9000),600000,5000);

}

void Figgie::F3(){ // ||
    bars[ofRandom(bars.size())]->setEnvelope(ofRandom(2000,9000),600000,5000);

}

void Figgie::F4(){ // ||
    rectMask->randomizeSpeeds();

}

void Figgie::F5(){ // ||
    rectMask->randomizeDirections();

}

void Figgie::F6(){ // ||
    for(int i=0; i<10; i++)
        F0();
}

void Figgie::F7(){ // ||
    v->addEvent(
        new JRectangle(
            411,
            ofVec2f(0,0),
            ofGetWindowSize(),
//            colors->getRandomColor(""), // See .h
                       ofColor::white,
            10,
            400,
            ofVec2f(0,0),
            false
        ), 2
    );
    v->getLast()->setAlpha(200);
}

void Figgie::F8(){ // ||
    for(int i=0; i<bars.size(); i++)
        bars[i]->direction = ofVec2f(0, (int)ofRandom(0,2)*2-1);
}

void Figgie::F9(){ // ||
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

void Figgie::F10(){ // ||
    for(int i=0; i<20; i++){
        int x = ofGetWindowWidth()/20*i;
        int attack = 100 + (i*200);
        v->addEvent(
            new JRectangle(
                attack+401,
                ofVec2f(x,0),
                ofVec2f(10, ofGetWindowHeight()),
//                colors->getRandomColor(""), // See .h
                           ofColor::white,
                attack,
                400,
                ofVec2f(1,0),
                true
                           )
        );
        v->getLast()->setAlpha(200);
    }
}

void Figgie::songCCValue(int note, int value){
    switch (note) {
        case 48:{
            //
            int newHeight = ofMap(value, 0, 127, 30, 300);
            for(int i=0; i<bars.size(); i++)
                bars[i]->setHeight(newHeight);
        }
            break;
        case 49:{
            int newSpeed = ofMap(value, 0, 127, 5, 800)/100;
            for(int i=0; i<bars.size(); i++)
                bars[i]->speed = newSpeed;
                }
            break;
        case 50:{
            int newAlpha =ofMap(value, 0, 127, 0, 255);
            rectMask->setAlpha(newAlpha);
        }
            break;
            
        default:
            break;
    }
}

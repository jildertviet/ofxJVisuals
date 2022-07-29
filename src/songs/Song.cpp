//
//  Song.cpp
//  Paul
//
//  Created by Jildert Viet on 14-02-16.
//
//

#include "Song.hpp"

Song::Song(){

}

Song::~Song(){
//    v->cam.lookAt(ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.5));
}

void Song::keys(int key){
    return;
}

void Song::doFunc(int i){
    switch(i){
        case 0: F0(); break; case 1: F1(); break; case 2: F2(); break; case 3: F3(); break; case 4: F4(); break; case 5: F5(); break; case 6: F6(); break; case 7: F7(); break; case 8: F8(); break; case 9: F9(); break; case 10: F10(); break; case 11: F11(); break; case 12: F12(); break; case 13: F13(); break; case 14: F14(); break; case 15: F15(); break;
    }
}

void Song::setName(string name){
    this->name = name;
    cout << "Song: " << name << endl;
}

void Song::doControlFunc(int note, int value){
    // Store values in Bus objects in Visualizer
    if(note < 48 || note > 56)
        return;
        
    switch(note){
        case 56:
            cout << "Set brightness" << endl;
            v->setBrightness(ofMap(value, 0, 127, 0, 255));
            break;
        case 55:
            v->setAlpha(ofMap(value, 0, 127, 0, 255));
            cout << "Set alpha" << endl;
            break;
        case 48: C0(value); break; case 49: C1(value); break; case 50: C2(value); break; case 51: C3(value); break; case 52: C4(value); break; case 53: C5(value); break; case 54: C6(value); break;
    } 
}

JEvent* Song::getLast(){
    return v->getLast();
}

JEvent* Song::addEvent(JEvent* e, int index){
    return v->addEvent(e, index);
}

void Song::exit(){
    cout << "Song::exit()" << endl;
}

void Song::key(int key){
    switch(key){
        case '0':
            F0();
            break;
        case '1':
            F1();
            break;
        case '2':
            F2();
            break;
        case '3':
            F3();
            break;
        case '4':
            F4();
            break;
        case '5':
            F5();
            break;
        case '6':
            F6();
            break;
        case '7':
            F7();
            break;
        case '8':
            F8();
            break;
        case '9':
            F9();
            break;
            case 'q':
            F10();
            break;
            case 'w':
            F11();
            break;
            case 'e':
            F12();
            break;
            case 'r':
            F13();
            break;
    }
}

//ofxJSON Song::getColorsAsJson(){
//    ofxJSON json;
//    for(short i=0; i<colors.size(); i++){
//        string colorName = "color";
//        colorName += ofToString(i);
//        json[colorName]["red"] = colors[i].r;
//        json[colorName]["green"] = colors[i].g;
//        json[colorName]["blue"] = colors[i].b;
//        json[colorName]["alpha"] = colors[i].a;
//    }
//    return json;
//}

ofxOscMessage Song::getColorsAsOSC(){
    ofxOscMessage m;
    m.addStringArg("colors");
    for(short i=0; i<colors.size(); i++){
        m.addIntArg(colors[i].r);
        m.addIntArg(colors[i].g);
        m.addIntArg(colors[i].b);
        m.addIntArg(colors[i].a);
    }
    return m;
}

void Song::setColor(int index, ofColor c){
    if(index < colors.size()){
        colors[index] = c;
    }
}

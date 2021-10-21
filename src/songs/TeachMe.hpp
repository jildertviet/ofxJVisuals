//
//  TeachMe
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#ifndef TeachMe_hpp
#define TeachMe_hpp

#include <stdio.h>
#include "Song.hpp"


//vector<ofColor> colors4 {
//    ofColor(232,221,203),
//    ofColor(205,179,128),
//    ofColor(3,101,100),
//    ofColor(3,54,73),
//    ofColor(3,22,52)
//};


//                 vector<ofColor>{
//                     ofColor(232,243,248),
//                     ofColor(219,230,236),
//                     ofColor(194,203,206),
//                     ofColor(164,188,194),
//                     ofColor(129,168,184)
//                 },
//                 "TeachMeBlue"


class TeachMe: public Song{
public:
    TeachMe(ofxJVisuals* v);
//    Colors* colorsObj;
    RectangleMask* rectMask;
    vector<JRectangle*> bars;
    JRectangle* bg;
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int v); void C1(int v); void C2(int);  void C3(int v);
    void C4(int v); void C5(int v); void C6(int v); void C7(int v);
    
    void exit();
    
    string colorsName = "BatobeTeachMe";
};

#endif /* TeachMe */

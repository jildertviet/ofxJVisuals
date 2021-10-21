//
//  Figgie.hpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#ifndef Figgie_hpp
#define Figgie_hpp

#include <stdio.h>
#include "Song.hpp"


//vector<int> hexValues { // Figgie / Dakota
//    0xFFD756,
//    0xFC794C,
//    0x9B3D70,
//    0x7E48D1,
//    0x5330B2,
//    0x41608C
//};


class Figgie: public Song{
public:
    Figgie(ofxJVisuals* v);
    ofxJVisuals* v;
    RectangleMask* rectMask;
    vector<JRectangle*> bars;
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); 
    
    void songCCValue(int note, int value);
};

#endif /* Figgie_hpp */

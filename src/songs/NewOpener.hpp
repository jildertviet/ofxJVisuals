//
//  NewOpener.hpp
//  Visualizer_1_5
//
//  Created by Jildert Viet on 20-01-17.
//
//

#ifndef NewOpener_hpp
#define NewOpener_hpp

#include <stdio.h>
#include "Song.hpp"
#include "JLine.hpp"

class NewOpener: public Song{
public:
typedef unsigned char uint8;
    NewOpener(ofxJVisuals* v);
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int); void C1(int); void C2(int); void C3(int);
    void C4(int); void C5(int); void C6(int);
    
    void exit();
    
    vector<JLine*> lines;
    vector<JRectangle**> rects;
    
    short int maxNumLines = 100;
    int maxBright = 255;
    int squareReleaseOffset = 1000;
    short attack = 100;
    short tiltRelease = 2000;
    int ampOffset = 0;
    
    float noiseSpeed = 2;
    int zSpawnPos = 0;
};

#endif /* NewOpener_hpp */

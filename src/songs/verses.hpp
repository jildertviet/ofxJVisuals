#ifndef verses_hpp
#define verses_hpp

#include <stdio.h>
#include "Song.hpp"
#include "meshRectangle.hpp"
#include "meshLandscape.hpp"
#include "cameraController.hpp"

class verses: public Song{
public:
typedef unsigned short uint16;
    verses(ofxJVisuals* v);
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int); void C1(int); void C2(int); void C3(int);
    void C4(int); void C5(int); void C6(int);
    
    void exit();
    
    int numRects = 96;
    int lineW = 2;
    float maxSpeed = 0.5;
    void rotSquares(int num = 96);
    
    vector<meshRectangle*> rects;
    vector<meshWalker*> mt;
    meshLandscape* l;

    void geometricLandscape();
    void geometricLandscape2();
    
    cameraController* camController;
    
    void whiteStart();
    void whiteSecond();
    void whiteBoth();
    void oneLine();
    void withLandscape();
    void intro();
    void withLines();
    void eightShort();
};

#endif /* verses_hpp */

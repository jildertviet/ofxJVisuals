//
//  Faith.hpp
//  Bas
//
//  Created by Jildert Viet on 18-04-16.
//
//

#ifndef Faith_hpp
#define Faith_hpp

#include <stdio.h>
#include "Song.hpp"
#include "Vorm.h"
#include "MamaOtis.hpp"

#endif /* Faith_hpp */

class Faith: public Song{
public:
    Faith(ofxJVisuals* v);
    void stop();
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    
//    void F9(); void F10(); void F11();
//    void F12(); void F13();
    
    void makeVerticalParticleLine(int x, int amount, int destX, float speedMin, float speedMax);
    void makeVerticalLineConnectedToCube();
    
    Vorm* lastLine = nullptr;
    Vorm* cube = nullptr;
    void makeCube();
    
    void exit();
private:
    Vorm* lastCubeLine = nullptr;
    vector<Vorm*> lastLines;
    void rememberLine(Vorm* line);
    
    void startMove(int duration);
};

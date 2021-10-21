//
//  MamaOtis.hpp
//  Bas
//
//  Created by Jildert Viet on 24-04-16.
//
//

#ifndef MamaOtis_hpp
#define MamaOtis_hpp

#include <stdio.h>
#include "Song.hpp"
#include "Vorm.h"

class MamaOtis: public Song{
public:
    MamaOtis(ofxJVisuals* visualizer);
    void stop();
    
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    
    
    
    vector<Vorm*> vormen;
    vector<Vorm*> triangles;
    Vorm* lastTriangle = nullptr;
    
    
    void makeVorm(int numSides, int sideDivision, float radius, ofVec2f center, bool on_destination=true);
    void functionOne();
    void functionTwo();
    void changeSpeed();
    void makeThings1();
    void makeThings2();
    
    void makeCube1(); // squares
    void makeCube2();
    void showParticlesVormen();
    
    void fall();
    void thingsWithCircle();
    
    void finalVorm();
    static bool checkIfNull(Vorm* v);
    
    void exit();
    
private:
    Vorm* cube1 = nullptr; Vorm* cube2 = nullptr;
};
#endif /* MamaOtis_hpp */

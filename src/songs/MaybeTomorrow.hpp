//
//  MaybeTomorrow.hpp
//  Bas
//
//  Created by Jildert Viet on 25-04-16.
//
//

#ifndef MaybeTomorrow_hpp
#define MaybeTomorrow_hpp

#include <stdio.h>
#include "Song.hpp"
#include "MultiMeshMaybeTomorrow.hpp"

class MaybeTomorrow: public Song{
public:
    MaybeTomorrow(ofxJVisuals* v);
    int numMeshes=0;
    vector<MultiMeshMaybeTomorrow*> multiMesh;
    void stop();
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int val); void C1(int val); void C2(int val); void C3(int val);
    
    float growAmount = 1.0; // Growth on noise vertically, or hor
    
    void generateChorusMeshes();
    
    void exit();
    ofVec2f env = ofVec2f(10, 100);
    int numLines=100;

};
#endif /* MaybeTomorrow_hpp */

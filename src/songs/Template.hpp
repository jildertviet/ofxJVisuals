//
//  Template.hpp
//  Visualizer_1_5
//
//  Created by Jildert Viet on 20-01-17.
//
//

#ifndef Template_hpp
#define Template_hpp

#include <stdio.h>
#include "Song.hpp"

class Template: public Song{
public:
    Template(ofxJVisuals* v);
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int); void C1(int); void C2(int); void C3(int);
    void C4(int); void C5(int); void C6(int);
    
    void exit();
};

#endif /* Template_hpp */

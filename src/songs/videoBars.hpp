//
//  ZZB_1.hpp
//  Bas
//
//  Created by Jildert Viet on 23-09-16.
//
//

#ifndef VIDEOBARS_hpp
#define VIDEOBARS_hpp

#include <stdio.h>
#include "Song.hpp"
#include "JVideoPlayer.hpp"
#include "JRectangle.hpp"

class videoBars: public Song{
public:
    videoBars(ofxJVisuals* visualizer);
    
    JVideoPlayer* video;
    
    void loadNewMovie(string path);
    void exit();
    
    void F0(); void F1(); void F2(); void F3();
    void F4(); void F5(); void F6(); void F7();
    void F8(); void F9(); void F10(); void F11();
    void F12(); void F13(); void F14(); void F15();
    
    void C0(int);
    int numBins = 7;
    void C1(int); void C2(int); void C3(int);
    int duckZoom = 0;
//    void C4(int); void C5(int); void C6(int);
};
#endif /* VIDEOBARS_hpp */

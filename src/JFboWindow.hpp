//
//  JFboWindow.hpp
//  visualizer
//
//  Created by Jildert Viet on 12-12-19.
//

#ifndef JFboWindow_hpp
#define JFboWindow_hpp

#include <stdio.h>
#include "ofMain.h"

class JFboWindow: public ofBaseApp {
public:
    JFboWindow();
    void setup();
    void update();
    void draw();
    ofFbo* fbo;
    float frameRate = 60;
    bool bFullScreen = true;
    void keyPressed(int key);

//    void exit();
    
//    void keyPressed(int key);
//    void mousePressed(int x, int y, int button);
};

#endif /* JFboWindow_hpp */

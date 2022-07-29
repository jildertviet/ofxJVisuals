//
//  Image.hpp
//  Bas
//
//  Created by Jildert Viet on 18-03-16.
//
//

#ifndef Image_hpp
#define Image_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"

class JImage: public JEvent{
    enum DrawMode{
        DEFAULT
    };
public:
    JImage(string filename, ofVec2f loc = ofVec2f(0,0));
    JImage(){};
    ofImage image;
    
    void display();
    void specificFunction();
    
    DrawMode drawMode = JImage::DrawMode::DEFAULT;
    
    bool loadImage(string path);
    bool bLoadSucces = false;
};
#endif /* JImage_hpp */


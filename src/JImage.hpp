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
#include "Event.hpp"

class JImage: public Event{
public:
    JImage(string filename, ofVec2f loc = ofVec2f(0,0));
    JImage(){};
    ofImage image;
    ofImage backGround;
    ofMesh mesh;
    
    void display();
    void specificFunction();
    void makeImageFit();
    
    void createFullScreenCopy();
    void meshSetup();
    
    bool fillScreen = false;
    bool drawImage=true;
    bool drawMesh = 0;
    
    void loadImage(string path);
    void center();
};
#endif /* JImage_hpp */

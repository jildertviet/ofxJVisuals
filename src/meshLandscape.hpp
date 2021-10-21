//
//  meshLandscape.hpp
//  versesVideo
//
//  Created by Jildert Viet on 22-01-18.
//
//

#ifndef meshLandscape_hpp
#define meshLandscape_hpp

#include <stdio.h>
#include "Event.hpp"

class meshLandscape: public Event{
public:
    meshLandscape(string img1 = "a", string img2 = "a", int height=48, int widthBetweenNodes=1, int skip = 8);
    ofMesh m;
    void specificFunction();
    void display();
    bool bDisplayWireFrame = false;
    bool bDisplayFaces = false;
    ofVec3f translation = ofVec3f(0,0,0);
    void translate(ofVec3f t);
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c);
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d);
    ofVec3f getVertexFromImg(ofShortImage& img, int x, int y);
    float widthBetweenNodes = 1;
    bool bUseNormalImage = false;
    int height = 48;
    int skip = 8;
    void round();
};

#endif /* meshLandscape_hpp */

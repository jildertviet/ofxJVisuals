//
//  pathWalker.hpp
//  versesVideo
//
//  Created by Jildert Viet on 29-01-18.
//
//

#ifndef pathWalker_hpp
#define pathWalker_hpp

#include <stdio.h>
#include "ofMain.h"
#include "meshWalker.hpp"

class pathWalker: public meshWalker{
public:
	typedef unsigned short uint16;
    pathWalker(vector<ofVec3f> path);
    void specificFunction();
    void display();
    vector<ofVec3f> path;
    void displayTrail();

};
#endif /* pathWalker_hpp */

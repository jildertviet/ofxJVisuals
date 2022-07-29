//
//  imageFloat.hpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 02-03-19.
//

#ifndef imageFloat_hpp
#define imageFloat_hpp

#include <stdio.h>
#include "JEvent.hpp"
#include "JRectangle.hpp"

class imageFloat: public JEvent, public ofImage{
public:
    imageFloat(string src);
};

class imageFloater: public JRectangle{ // Inherits Event
public:
    imageFloater(imageFloat* src);
    imageFloat* img;
    void display();
    void imageFloating();
    ofVec2f roi, roiSpeed;
};

#endif /* imageFloat_hpp */

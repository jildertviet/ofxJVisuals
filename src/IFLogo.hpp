//
//  IFLogo.hpp
//  Visualizer_2_8
//
//  Created by Jildert Viet on 14-10-19.
//

#ifndef IFLogo_hpp
#define IFLogo_hpp

#include <stdio.h>
#include "Event.hpp"
#include "ofMain.h"
#include "JRectangle.hpp"

class IFLogo: public Event{
public:
    IFLogo(ofVec2f loc = ofVec2f(0, 0), ofVec2f size = ofVec2f(400, 400));
    void specificFunction() override;
    void display() override;
    JRectangle* rBack;
    JRectangle* r4[4];
    void setSize(ofVec3f size) override;
};

#endif /* IFLogo_hpp */

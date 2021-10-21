//
//  jTxt.hpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 02-03-19.
//

#ifndef jText_hpp
#define jText_hpp

#include <stdio.h>
#include "Event.hpp"

class jText: public Event{
public:
    jText(ofTrueTypeFont* f);
    ofTrueTypeFont* f;
    void display();
    string txt;
    void setText(string txt);
    bool* bCamEnabled = nullptr;
};
#endif /* jText_hpp */

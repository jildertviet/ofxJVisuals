//
//  jTxt.hpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 02-03-19.
//

#ifndef JText_hpp
#define JText_hpp

#include <stdio.h>
#include "JEvent.hpp"

class JText: public JEvent{
public:
    JText(ofTrueTypeFont* f);
    ofTrueTypeFont* f;
    void display();
    string txt;
    void setText(string txt);
    bool* bCamEnabled = nullptr;
};
#endif /* JText_hpp */

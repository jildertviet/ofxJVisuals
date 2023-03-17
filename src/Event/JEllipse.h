//
//  JEllipse.h
//
//  Created by Jildert Viet on 07-06-15.
//
//

#ifndef __JEllipse__
#define __JEllipse__

#include "ofMain.h"
#include "Wavetable.h"
#include "JEvent.hpp"

class JEllipse: public JEvent{
public:
    JEllipse();

    float radius;
    glm::vec3 originalSize;
    int alpha;
    int originalAlpha;

    float angle;
    void specificFunction();

    void display();

    Wavetable wavetable;
    bool LFO_alpha = false;

    void halfTime();
    bool oneFrameLarger = false;
    bool fill_oneFrame = false;

    bool smaller = false;
    int smaller_amount = 10;
    void makeSmaller();
    bool already_haftime = false;

};
#endif /* defined(__JEllipse__) */

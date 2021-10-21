//
//  Wavetable.h
//  Batobe_met_MIDI
//
//  Created by Jildert Viet on 04-06-15.
//
//

#ifndef __Batobe_met_MIDI__Wavetable__
#define __Batobe_met_MIDI__Wavetable__

#include <iostream>
#include "ofMain.h"

#define DENSITY 256

class Wavetable{
public:
    Wavetable(float freq, float offset, int mode, float pulseWidth=0.5);
    float wavetable[DENSITY];

    float getNextValue();
    float getNextValueWithOffset(float offset);
    Wavetable();
    void printTable();
    void setMode(int mode);
    
    float offset=0;
    float phase = 0;
    float pulseWidth=0.5;
    float process();
    float freq=1;
    
    void retrigger();
    bool bActive = true;
};
#endif /* defined(__Batobe_met_MIDI__Wavetable__) */

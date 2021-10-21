//
//  Wavetable.cpp
//  Batobe_met_MIDI
//
//  Created by Jildert Viet on 04-06-15.
//
//

#include "Wavetable.h"

Wavetable::Wavetable(){
//    cout << "Wavetable made, with density: " << DENSITY << endl;
    setMode(0);
}

Wavetable::Wavetable(float freq, float offset, int mode, float pulseWidth){
    this->freq = freq;
    this->offset = offset;
    this->pulseWidth = pulseWidth;
    setMode(mode);
}

float Wavetable::process(){
    if(bActive){
    float millis = ofGetElapsedTimeMillis();
    millis /= ((1.f/freq)*1000);
    phase = (int)(millis * DENSITY) % DENSITY;
    phase += (offset * DENSITY);
    phase = (int)phase % DENSITY;
    float output = wavetable[(int)phase];
    return output;
    } else {
        return 0; 
    }
}

void Wavetable::setMode(int mode){ // 0: sin, 1: cos, 2 square, 3 impulse, 4 triangle
    switch(mode){
        case 0:
            for(int i=0; i<DENSITY; i++){
                wavetable[i] = sin((i*(1.0/(float)DENSITY))*TWO_PI);
            }
            break;
        case 2:
            for(int i=0; i<DENSITY; i++){
                if(i<DENSITY*pulseWidth){
                    wavetable[i] = 1;
                } else{
                    wavetable[i] = 0;
                }
            }
            break;
        case 1:
            for(int i=0; i<DENSITY; i++){
                wavetable[i] = cos((i*(1.0/(float)DENSITY))*TWO_PI);
            }
            break;
        case 3:
            for(int i=0; i<DENSITY; i++){
                if(i==0){
                    wavetable[0] = 1;
                } else{
                    wavetable[i] = 0;
                }
            }
            break;
        case 4:
            for(int i=0; i<DENSITY; i++){
                float value;
                if(i<(DENSITY*0.5)){
                    value = i/(DENSITY*0.5);
                } else{
                    value = 1-((i-(DENSITY*0.5))/(DENSITY*0.5));
                }
                wavetable[i] = value;
            }
            break;
    }
}

void Wavetable::printTable(){
    for(int i=0; i<DENSITY; i++){
        cout << wavetable[i] << endl;
    }
}

void Wavetable::retrigger(){
    offset = phase / DENSITY;
}


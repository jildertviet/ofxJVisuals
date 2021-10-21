//
//  Noise.hpp
//  voorPaul
//
//  Created by Jildert Viet on 27-01-16.
//
//

#ifndef JNoise_hpp
#define JNoise_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Event.hpp"

enum JNOISE_MODE{
    HORIZONTAL,
    VERTICAL
};

class JNoise: public Event{
public:
    JNoise(float duration=200, ofColor color = ofColor(255), char mode = JNOISE_MODE::HORIZONTAL, short int linesAmount = 100);
    void display() override;
    void horizontalNoise(int size); void verticalNoise(int size);
    void generateSeed();
    int lineWidth = 1;
    int linesAmount = 100;
    ofColor localColor;
    ofVec2f offset = ofVec2f(0, 0);
    float seed;
    void customOne() override;
private:
};
#endif /* JNoise_hpp */

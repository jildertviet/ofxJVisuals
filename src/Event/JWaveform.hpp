//
//  JWaveform.hpp
//

#ifndef JWaveform_hpp
#define JWaveform_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"

class JWaveform: public JEvent{
public:
    JWaveform();
    void display() override;
    void specificFunction() override;
    ofPath path;

    virtual void parseBuffer();
    // void customOne(float* v = nullptr) override;
    // void customTwo(float* v = nullptr) override;
    // void customThree(float* v = nullptr) override;
};
#endif /* JWaveform_hpp */

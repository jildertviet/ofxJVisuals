//
//  AlphaBlackScreen.hpp
//
//  Created by Jildert Viet on 14-02-16.
//
//

#ifndef AlphaBlackScreen_hpp
#define AlphaBlackScreen_hpp

#include <stdio.h>
#include "JEvent.hpp"

class AlphaBlackScreen: public JEvent{
public:
    AlphaBlackScreen(bool state = true);
    ~AlphaBlackScreen();
    void display();

    void specificFunction();
    
    void setActiveness(bool state);
    void setAlpha(int alpha);
    
    bool bGradient = false;
    ofColor gradientColor = ofColor(255);
    int gradientAlpha;
private:
    bool bDoAlphaBlend = false;

};
#endif /* AlphaBlackScreen_hpp */

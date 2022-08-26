//
//  RectangleMask.hpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 15-01-17.
//
//

#ifndef RectangleMask_hpp
#define RectangleMask_hpp

#include <stdio.h>
#include "JRectangle.hpp"
#include "JEvent.hpp"

class RectangleMask: public JEvent{
public:
    RectangleMask();
    RectangleMask(JRectangle* rect);
    
    vector<JRectangle*> rects;
    void addRect(JRectangle* r);
    
    void specificFunction();
    void display();
    void addPtr(RectangleMask** p);
    
    vector<int> lefts;
    vector<int> rights;
    
    void randomizeSpeeds();
    void randomizeDirections();
    
    static bool checkIfNull(JRectangle* r);
    
    void findLeftAndRight();
};
#endif /* RectangleMask_hpp */

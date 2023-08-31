//
//  JModifier.hpp
//  exampleSimple
//
//  Created by Jildert Viet on 29/07/2022.
//

#ifndef JModifier_hpp
#define JModifier_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"

class JModifier: public JEvent{
public:
    JModifier();
    virtual void display(JEvent* e, void (JEvent::*ptr) (void));
};

#endif /* JModifier_hpp */

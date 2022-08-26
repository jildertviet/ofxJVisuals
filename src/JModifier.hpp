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

class JEvent;

class JModifier{
public:
    JModifier();
    virtual void display(JEvent* e, void (JEvent::*ptr) (void));
    virtual void setViaBusses(vector<float> busses){};
};

class JModifierArray: public JModifier{
public:
    JModifierArray();
    glm::vec2 dimensions = glm::vec2(2, 2);
    glm::vec2 spacing = glm::vec2(200, 200);
    void display(JEvent* e, void (JEvent::*ptr) (void)) override;
    void setViaBusses(vector<float> busses) override;
};

class JModifierFact{
public:
    JModifierFact(){};
    static JModifier* create(int nameID){
        switch(nameID){
            case 0:
                return new JModifierArray();
                break;
        }
    };
};
#endif /* JModifier_hpp */

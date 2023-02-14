//
//  JModifier.cpp
//  exampleSimple
//
//  Created by Jildert Viet on 29/07/2022.
//

#include "JModifier.hpp"

JModifier::JModifier(){

}

void JModifier::display(JEvent* e, void (JEvent::*ptr) (void)){

}

JModifierArray::JModifierArray(){

}

void JModifierArray::display(JEvent* e, void (JEvent::*ptr) (void)){
    for(int i=0; i<dimensions.x; i++){
        for(int j=0; j<dimensions.y; j++){
            ofPushMatrix();
            ofTranslate(i * spacing.x, j * spacing.y);
            (e->*ptr)();
            ofPopMatrix();
        }
    }
}

void JModifierArray::setViaBusses(vector<float> busses){
    if(busses.size() < 4)
        return;
    dimensions.x = busses[0];
    dimensions.y = busses[1];
    spacing.x = busses[2];
    spacing.y = busses[3];
}

void JEvent::displayModifier(){
    for(int i=0; i<modifiers.size(); i++){
        modifiers[i]->display(this, &JEvent::display);
    }
}

void JEvent::deleteModifiers(){
    for(int i=0; i<modifiers.size(); i++){
        delete modifiers[i]; // Should call ~JEvent() and remove self from ofxJVisuals::events[]
    }
}

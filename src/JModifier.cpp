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
  dimensions[0] = &busses[1];
  dimensions[1] = &busses[2];
  spacing[0] = &busses[3];
  spacing[1] = &busses[4];
}

void JModifierArray::display(JEvent* e, void (JEvent::*ptr) (void)){
    for(int i=0; i<*dimensions[0]; i++){
        for(int j=0; j<*dimensions[1]; j++){
            ofPushMatrix();
            ofTranslate(i * (*spacing[0]), j * (*spacing[1]));
            (e->*ptr)();
            ofPopMatrix();
        }
    }
}

//
// void JModifierArray::setViaBusses(vector<float> busses){
//     if(busses.size() < 4)
//         return;
//     dimensions.x = busses[0];
//     dimensions.y = busses[1];
//     spacing.x = busses[2];
//     spacing.y = busses[3];
// }

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

#ifndef JModifierArray_hpp
#define JModifierArray_hpp

#include "JModifier.hpp"

class JModifierArray: public JModifier{
public:
    JModifierArray(){
      dimensions[0] = &busses[1];
      dimensions[1] = &busses[2];
      spacing[0] = &busses[3];
      spacing[1] = &busses[4];
    }
    float* dimensions[2];
    float* spacing[2];
    void display(JEvent* e, void (JEvent::*ptr) (void)){
      for(int i=0; i<*dimensions[0]; i++){
          for(int j=0; j<*dimensions[1]; j++){
              ofPushMatrix();
              ofTranslate(i * (*spacing[0]), j * (*spacing[1]));
              (e->*ptr)();
              ofPopMatrix();
          }
      }
    };
};

#endif

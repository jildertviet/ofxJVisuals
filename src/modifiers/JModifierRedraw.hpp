#ifndef JModifierRedraw_hpp
#define JModifierRedraw_hpp

#include "JModifier.hpp"

class JModifierRedraw: public JModifier{

enum RedrawMode {
  MODE,
  COLOR
} ;

public:
    JModifierRedraw(){

    }

    void update(JEvent* e) override{
      switch((char)busses[1]){ // RedrawMode (redrawColor, redrawSize, etc)
        case JModifierRedraw::RedrawMode::MODE:{
          char prevMode = e->mode;
          e->mode = busses[2];
          e->parseBuffer();
          e->mode = prevMode;
        }
      }

    };

    void display(JEvent* e, void (JEvent::*ptr) (void)){
      // ofSetColor(255);
      // ofDrawRectangle(0, 0, 100, 100);
      // cout << "X" << endl;
      switch((char)busses[1]){ // RedrawMode (redrawColor, redrawSize, etc)
        case JModifierRedraw::RedrawMode::MODE:{
          (e->*ptr)(); // First draw normally (or should this be done w/ bus, or even JModifierClone)
          char prevMode = e->mode;
          ofColor prevColor = e->color;
          // e->color = color; // Use color of the modifier?
          e->color = ofColor::red;
          e->mode = busses[2];
          (e->*ptr)();
          e->mode = prevMode;
          e->color = prevColor;
        }
      }
    };
};

#endif

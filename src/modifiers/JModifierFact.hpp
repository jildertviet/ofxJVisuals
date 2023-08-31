#ifndef JModifierFact_hpp
#define JModifierFact_hpp

#include "JModifierArray.hpp"
#include "JModifierRedraw.hpp"

class JModifierFact{
public:
    JModifierFact(){};
    static JModifier* create(int nameID){
        switch(nameID){
            case 0:
                return new JModifierArray();
                break;
        }
        return nullptr;
    };
};

#endif

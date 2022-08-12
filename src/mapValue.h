//
//  mapValue.h
//  exampleSimple
//
//  Created by Jildert Viet on 24/07/2022.
//

#ifndef mapValue_h
#define mapValue_h

class mapValue{
public:
    mapValue(){
    }
    mapValue(float* v){
        floatPointer = v;
    }
    mapValue(ofColor* v, char type = 'a'){
        colorPtr = v;
        this->type = type;
    }
    char type = 'f'; //
    float* floatPointer;
    int* intPointer;
    ofColor* colorPtr;
    void setVal(float v){
        switch(type){
            case 'a':
                colorPtr->a = v;
                break;
            case 'r':
                colorPtr->r = v;
                break;
            case 'g':
                colorPtr->g = v;
                break;
            case 'b':
                colorPtr->b = v;
                break;
            default:
                *floatPointer = v;
        }
    }
    void setVal(int v){
        *intPointer = v;
    }
    string name = "";
};

#endif /* mapValue_h */

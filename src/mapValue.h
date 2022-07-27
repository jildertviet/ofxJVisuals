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
    mapValue(ofColor* v){
        colorPtr = v;
    }
    char type = 'f'; //
    float* floatPointer;
    int* intPointer;
    ofColor* colorPtr;
    void setVal(float v){
        *floatPointer = v;
    }
    void setVal(int v){
        *intPointer = v;
    }
    void setColor(float v, char type){
        if(!colorPtr)
            return;
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
        }
    }
    void setVal(float v, char type){ // f, i, a, r, g, b
        switch(type){
            case 'f':
                setVal(v);
                break;
            case 'i':
                setVal((int)v);
                break;
            case 'a':
            case 'r':
            case 'g':
            case 'b':{
                setColor(v, type);
            }
                break;
                
        }
    }
    string name = "";
};

#endif /* mapValue_h */

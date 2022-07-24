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
    char type = 'f'; //
    float* floatPointer;
    int* intPointer;
    void setVal(float v){
        *floatPointer = v;
    }
    void setInt(int v){
        *intPointer = v;
    }
    string name = "";
};

#endif /* mapValue_h */

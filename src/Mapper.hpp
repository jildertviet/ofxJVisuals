//
//  Mapper.hpp
//  Visualizer_1_2
//
//  Created by Jildert Viet on 07-12-16.
//
//

#ifndef Mapper_hpp
#define Mapper_hpp

#include <stdio.h>
#include "ofMain.h"

class Event;
class linkTap{
public:
    linkTap(){};
    linkTap(string name, float* floatPtr, ofVec2f range){
        this->name = name; this->floatPtr = floatPtr; this->range = range;
    }
    linkTap(string name, int* intPtr, ofVec2f range){
        this->name = name; this->intPtr = intPtr; this->range = range;
    }
    linkTap(string name, ofColor* colorPtr, ofVec2f range = ofVec2f(0,255)){
        this->name = name; this->colorPtr = colorPtr; this->range = range;
    }
    
    void setWeight(float weight){
        this->weight = weight;
    }
    
    void setValue(float value){ // Gets 0 - 1 from SC meters
        // Scale
        value = ofMap(value, 0., 1., range.x, range.y);
        value *= weight;
        // Set pointer
        if(intPtr){
            int val = value;
            *intPtr = val;
        } else if(floatPtr){
            *floatPtr = value;
        } else if(colorPtr){
            (*colorPtr).a = value;
        }
    }
    
    ofVec2f range = ofVec2f(0., 1.);
    float weight = 1;
    float* floatPtr = nullptr;
    int* intPtr = nullptr;
    ofColor* colorPtr = nullptr;
    
    string name = "null";
    
    int parentID = 0;
};

class Mapper{
public:
    Mapper();
    ~Mapper();
    Mapper(vector<Mapper*>* visualizerMappers, Event* parent, string listenID, string audioParam, linkTap* link);
    
    string listenID = "/0";
    string audioParam = "";
    
    bool process(string adress, float val);
    
    linkTap* link = nullptr;
    Event* parent = nullptr;
    
    vector<Mapper*>* visualizerMappers = nullptr;
    Mapper* mapperReferencePtr = nullptr;
    
    static bool checkIfNull(Mapper* r);
    int mode = 0; // 0=lin, 1=exp
};
#endif /* Mapper_hpp */

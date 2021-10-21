//
//  Mapper.cpp
//  Visualizer_1_2
//
//  Created by Jildert Viet on 07-12-16.
//
//

#include "Mapper.hpp"

Mapper::Mapper(){
    cout << "New Mapper" << endl;
}

Mapper::~Mapper(){
    cout << "Deleting mapper: " << listenID << " " << link->name << endl;
}

Mapper::Mapper(vector<Mapper*>* visualizerMappers, Event* parent, string listenID, string audioParam, linkTap* link){
    this->audioParam = audioParam;
    this->parent = parent;
    
    this->visualizerMappers = visualizerMappers;
    
    this->listenID = listenID;
    this->link = link;
    cout << "New Mapper" << endl;
}

bool Mapper::process(string adress, float val){
//    cout << "Mapper process function" << endl;
    
//    cout << adress << "\t " << listenID << endl;
    if(mode==0){
        // Lin, so nothing
    } else if(mode==1){
        val = val*val; // Exp
    }
    // To test... 
    if(adress==listenID){
//        cout << "Value: " << value << "\t Range: " << link->range.x << " " << link->range.y << "\t" << endl;
//        value = ofMap(value, 0, 1, link->range.x, link->range.y);
//        cout << "Set: " << value << endl;
        link->setValue(val); // Get mapped in link object
        return true;
    } else{
        return false;
    }
}

bool Mapper::checkIfNull(Mapper* r){
    if(r){
        return false;
    } else{
        return true;
    }
}
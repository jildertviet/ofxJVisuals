//
//  JDivisionGrid.cpp
//  Visualizer_2_6
//
//  Created by Jildert Viet on 23-05-19.
//

#include "JDivisionGrid.hpp"

bool sortByAreaSize(JPoly* a, JPoly* b){
    if(abs(a->p.getArea()) > abs(b->p.getArea())){
        return true;
    } else{
        return false;
    }
};

JDivisionGrid::JDivisionGrid(){
    size = glm::vec3(1080, 1080, 0);
    generateRandomRects();
}

void JDivisionGrid::display(){
    ofPushMatrix();
    ofTranslate(loc);
    for(int i=0; i<polyVec.size(); i++)
        polyVec[i]->display();
    ofPopMatrix();
}

void JDivisionGrid::specificFunction(){
    for(int i=0; i<polyVec.size(); i++){
        polyVec[i]->update(); // Env management
        polyVec[i]->generateMesh();
    }
    if(bSave){
        ofMesh rest;
        for(int i=0; i<polyVec.size(); i++){
            polyVec[i]->prepareForSave(); // Scale and add loc adjustment
            if(polyVec[i]->bHasEnv){
               polyVec[i]->save(); // Save as unique
            } else{
                rest.append(polyVec[i]->m);
            }
        }
        rest.save(polyVec[0]->getMeshName());
    }
}

void JDivisionGrid::generateRandomRects(){
    polyVec.clear();
    JPoly* poly = new JPoly();
    polyVec.push_back(poly);
    int padding = size.x * 0.05;
    // Set a path of 5 points
    ofPolyline* p = &(poly->p);
    p->addVertex(glm::vec3(padding, padding, 0));
    p->lineTo(glm::vec3(size.x - padding, padding, 0));
    p->lineTo(glm::vec3(size.x - padding, size.x - padding, 0));
    p->lineTo(glm::vec3(padding, size.x - padding, 0));
    p->lineTo(glm::vec3(padding, padding, 0));
    poly->id = id;
    
    for(int i=0; i<32; i++)
        sortAndSplit();
    
    for(int i=0; i<4; i++){
        int index = ofRandom(polyVec.size());
        while(polyVec[index]->bBlock)
            index = ofRandom(polyVec.size());
        polyVec[index]->bBlock = true;
    }
    
    for(int i=0; i<1024; i++)
        sortAndSplit();
    
    for(int i=0; i<polyVec.size(); i++){ // Apply white noise to heights
        polyVec[i]->zOffset = ofRandom(30);
    }
}


void JDivisionGrid::sortAndSplit(){
    sort(polyVec.begin(), polyVec.end(), sortByAreaSize);
    
    int toSplit = 0;
    if(ofRandom(5.) < 1.0){ // Split the same poly multiple times
        for(int i=0; i<3; i++){
            polyVec[toSplit]->prepare();
            while(!polyVec[toSplit]->split(&polyVec)){
                toSplit++;
                polyVec[toSplit]->prepare();
            }
        }
    } else{
        polyVec[toSplit]->prepare();
        while(!polyVec[toSplit]->split(&polyVec)){
            toSplit++;
            polyVec[toSplit]->prepare();
        }
    }
}

void JDivisionGrid::customOne(){
    for(int i=0; i<polyVec.size(); i++)
        polyVec[i]->bDrawWireframe = true;
}

void JDivisionGrid::customTwo(){
    for(int i=0; i<polyVec.size(); i++){
        polyVec[i]->locAdjustmentForExport = glm::vec3(size.x, size.y, 0) * 0.5;
        polyVec[i]->id = id + i;
        polyVec[i]->customTwo();
    }
}

void JDivisionGrid::customThree(){
    bSave = customOneArguments[0];
    if(bSave){
        for(int i=0; i<polyVec.size(); i++){
            polyVec[i]->locAdjustmentForExport = glm::vec3(size.x, size.y, 0) * 0.5;
            polyVec[i]->id = id + i;
        }
    }
}

void JDivisionGrid::setColor(ofColor c, int id){
    for(int i=0; i<polyVec.size(); i++)
        polyVec[i]->colors[0] = c;
}

Env* JDivisionGrid::addEnvRandomPoly(vector<float> values, vector<float> times){
    int index = ofRandom(polyVec.size());
    polyVec[index]->bHasEnv = true;
    return polyVec[index]->addEnv(values, times, &(polyVec[index]->colors[0]));
}

Env* JDivisionGrid::addEnvSelectedPoly(vector<float> values, vector<float> times){
    int index = customOneArguments[0] * polyVec.size();
    if(index >= polyVec.size() || index < 0)
        index = 0;
    polyVec[index]->bHasEnv = true;
    return polyVec[index]->addEnv(values, times, &(polyVec[index]->colors[0]));
}

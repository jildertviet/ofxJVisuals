//
//  JMesh.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 26-04-19.
//

#include "JMesh.hpp"

JMesh::JMesh(){
    size = ofVec3f(100, 100, 100);
    m = ofMesh::box(size.x, size.y, size.z);
    m.setMode(OF_PRIMITIVE_TRIANGLES);
//    createHollowRect(glm::vec3(100, 100, 20), 10);
}

void JMesh::display(){
    ofSetColor(color);
    ofPushMatrix();
    ofTranslate(loc);
    m.draw();
    ofPopMatrix();
}

void JMesh::specificFunction(){
    if(bSave){
        save();
    }
    if(envelopes.size()){
        for(short i=0; i<envelopes.size(); i++){
            for(int j=0; j<envelopes.size(); j++){
                if(envelopes[i][j]->ptr->valF == &(size.x)){
                    // ?
                }
            }
        }
    }
}

void JMesh::setLoc(glm::vec3 loc){
    // Add loc to all vertices? Now using ofTranslate :o
    this->loc = loc;
}

string JMesh::getDirectoryNameForFrame(){
    string zeroes = "";
    for(int i=0; i<7; i++)
        zeroes += "0";
    zeroes = zeroes.substr(0, zeroes.length()-(ofToString(ofGetFrameNum()).length()));
    zeroes += ofToString(ofGetFrameNum());
    string name = "./meshExport/f";
    name += zeroes;
    return name;
}

string JMesh::getMeshName(){
    string toSave = "";
    toSave += getDirectoryNameForFrame();
    toSave += "/";

    string zeroes = "";
    for(int j=0; j<4; j++)
        zeroes += "0";
    zeroes = zeroes.substr(0, zeroes.length()-(ofToString(id).length()));
    zeroes += ofToString(id);
    string newName = "m";
    newName += zeroes;

    toSave += newName;
    toSave += ".ply";
    cout << toSave << endl;
    return toSave;
}

void JMesh::customOne(float* v){
    bSave = true;
}

void JMesh::customTwo(float* v){
    prepareForSave();
    save();
}

void JMesh::customThree(float* v){
    createHollowRect(glm::vec3(busses[0], busses[1], busses[2]), busses[3]);
}

void JMesh::prepareForSave(){

}

void JMesh::setSize(glm::vec3 size){
    m.clear();
    m = ofMesh::box(size.x, size.y, size.z);
    this->size = size;
}

void JMesh::save(){
    ofDirectory d;
    if(!d.doesDirectoryExist(getDirectoryNameForFrame())){
        d.createDirectory(getDirectoryNameForFrame());
    }
    ofMesh m2 = m;
    float scale = 0.01;
    for(unsigned int i=0; i<m2.getNumVertices(); i++)
        m2.setVertex(i, (m2.getVertex(i) + loc) * scale);
    m2.save(getMeshName());
}

void JMesh::createHollowRect(glm::vec3 dimensions, float width){
    ofMesh a, b;
 //   ofxCorkCsg::box(b, dimensions.x, dimensions.y, dimensions.z);
//    dimensions -= width;
 //   ofxCorkCsg::box(a, dimensions.x - width, dimensions.y - width, dimensions.z * 2);
  //  ofxCorkCsg::computeIntersection(b, a, m);
}

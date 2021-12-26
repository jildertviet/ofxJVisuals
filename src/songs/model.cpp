//
//  model.cpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 20-01-17.
//
//

#include "model.hpp"

model::model(ofxJVisuals* v){
    this->v = v;
    setName("model");
    m.loadModel("Cello.3DS");
    v->cam.lookAt(ofVec3f(0, 0, 0));
    
    camController = new cameraController(&(v->cam));
    v->addEvent((Event*)camController);
    
    v->cam.setPosition(-3.07814, 121.163, 137.302);
    v->cam.setOrientation(ofQuaternion(-0.0788498, 0.393574, 0.888034, 0.22423));
}

void model::F0(){ // Series ||
    for(char i=0; i<meshes.size(); i++){
        meshes[i]->deleteWithFade(1000);
    }
    meshes.clear();
    
    for(char i=0; i<12; i++){
        jMesh* mesh;
        mesh = new jMesh();
        mesh->m = m.getMesh(0);
        mesh->m2 = m.getMesh(1);
        
//        mesh->offset = ofVec3f(500,0,0);
        mesh->rotationSpeed = ofVec3f(i * 0.02, 0, 0);
        mesh->addEnv(vector<float>{0, 10}, vector<float>{1000 + ofRandom(3000)}, &mesh->colors[0]);
        
        addEvent((Event*)mesh);
        meshes.push_back(mesh);
    }
}

void model::F1(){ // Rect ||
    for(char i=0; i<meshes.size(); i++){
        meshes[i]->deleteWithFade(1000);
    }
    meshes.clear();
    
    for(char i=0; i<4; i++){
        for(char j=0; j<4; j++){
            jMesh* mesh;
            mesh = new jMesh();
            mesh->m = m.getMesh(0);
            mesh->m2 = m.getMesh(1);
            mesh->colors[0] = ofColor(255, 10);
            mesh->offset = ofVec3f(100 * i,j*100,0);
            mesh->addEnv(vector<float>{0, 10}, vector<float>{1000 + ofRandom(3000)}, &mesh->colors[0]);
    //        mesh->rotationSpeed = ofVec3f(i * 0.02, 0, 0);
            
            addEvent((Event*)mesh);
            meshes.push_back(mesh);
        }
    }
}

void model::F2(){ // Random env||
    if(!meshes.size())
        return;
    int i = ofRandom(meshes.size());
    meshes[i]->addEnv(vector<float>{10, 0, 10}, vector<float>{(float)attack, (float)release}, &(meshes[i]->colors[0]));
}

void model::F3(){ // Env @ loc.x ||
    for(char i=0; i<meshes.size(); i++){
        meshes[i]->addEnv(
                          vector<float>{meshes[i]->offset.x, (ofRandom(300) + 100) * ((ofRandom(2)*2)-1), 0},
                          vector<float>{
                                (500 + ofRandom(500))*xPosAttackScale,
                                (500 + ofRandom(500))*xPosAttackScale
                            },
                          &(meshes[i]->offset.x));
    }
}

void model::F4(){ // Reset rotation ||
    for(char i=0; i<meshes.size(); i++){
        meshes[i]->rotation = ofVec3f(0, 0, 0);
    }
}

void model::F5(){ // Env @ camPos ||
    // Make a camera controller-event?
    vector<float> values = {0., 1.0, 0.};
    vector<float> times = {1000, 1000};
    switch((int)ofRandom(2)){
        case 0:
            camController->addEnv(values, times, &(camController->rotationSpeed.x), 0);
            break;
        case 1:
            camController->addEnv(values, times, &(camController->rotationSpeed.y), 0);
            break;
    }
}

void model::F6(){ // ||
    
}

void model::F7(){ // ||
    
}

void model::F8(){ // ||
    
}

void model::F9(){ // ||
    
}

void model::F10(){ // ||
    
}

void model::F11(){ // ||
    
}

void model::F12(){ // ||
    
}

void model::F13(){ // ||
    
}

void model::F14(){ // ||
    
}

void model::F15(){ // ||
    
}

void model::C0(int val){ // Attack ||
    attack = ofMap(val, 0, 127, 10, 3000);
}

void model::C1(int val){ // Release ||
    release = ofMap(val, 0, 127, 10, 3000);
}

void model::C2(int val){ // xPos env scale||
    xPosAttackScale = ofMap(val, 0, 127, 0.5, 5.0);
}

void model::C3(int val){ // ||
    
}

void model::C4(int val){ // ||
    
}

void model::C5(int val){ // ||
    
}

void model::C6(int val){ // ||
    
}

void model::exit(){
    for(char i=0; i<meshes.size(); i++){
        meshes[i]->deleteWithFade(1000);
    }
    meshes.clear();
}

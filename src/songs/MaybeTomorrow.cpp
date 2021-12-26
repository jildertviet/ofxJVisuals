//
//  MaybeTomorrow.cpp
//  Bas
//
//  Created by Jildert Viet on 25-04-16.
//
//

#include "MaybeTomorrow.hpp"

MaybeTomorrow::MaybeTomorrow(ofxJVisuals* v){
    setName("MaybeTomorrow");
    this->v = v;
}

void MaybeTomorrow::stop(){
}


void MaybeTomorrow::F0(){ // Lines fade||

    generateChorusMeshes();
    for(int i=0; i<multiMesh.size(); i++){
        MultiMeshMaybeTomorrow* mesh = multiMesh[i];
        mesh->deleteWithFade(2000, 8000+ofRandom(5000)); // Should have sustain...
    }
}

void MaybeTomorrow::F1(){ // Lines fading moving down ||
    generateChorusMeshes();
    for(int i=0; i<multiMesh.size(); i++){
        multiMesh[i]->bMoveVertical = true;
        multiMesh[i]->direction = ofVec2f(0,ofRandom(0.1, 1.0)/2.);
        multiMesh[i]->deleteWithFade(2000, 10000); // Should have sustain...
    }
}

void MaybeTomorrow::F2(){ // Lines no fade ||
    generateChorusMeshes();
}

void MaybeTomorrow::F3(){ // Lines no fade moving||
    generateChorusMeshes();
    for(int i=0; i<multiMesh.size(); i++){
        multiMesh[i]->bMoveVertical = true;
        multiMesh[i]->direction = ofVec2f(0,ofRandom(0.1, 1.0)/2.);
    }
}

    
void MaybeTomorrow::F4(){ // Move last ||
    for(int i=0; i<multiMesh.size(); i++)
        multiMesh[i]->bMoveHorizontal = true;
}


void MaybeTomorrow::F5(){ // ||
    
}

void MaybeTomorrow::F6(){ // Horizontal Noise||
    addEvent(new JNoise(100, ofColor::white, JNOISE_MODE::HORIZONTAL, numLines));
    JNoise* n = (JNoise*)v->getLast();
    n->size = ofVec3f(1280*3, 800);
    n->addEnv(
              vector<float>{n->size.y, n->size.y + growAmount * 800},
              vector<float>{ofMap(growAmount, 0., 1.0, 50, 4000)},
              &(n->size.y));
    n->addEnv(
              vector<float>{n->loc.y, (float)(-growAmount*0.5 * 800)},
              vector<float>{ofMap(growAmount, 0., 1.0, 50, 4000)},
              &(n->loc.y));
    n->setEnvelope(env[1], 100, env[0]);
}

void MaybeTomorrow::F7(){ // Vertical Noise||
    ofVec2f offsets[3] = {ofVec2f(0, 0), ofVec2f(1280, 0), ofVec2f(1280*2, 0)};
    for(int j=0; j<3; j++){
        addEvent(new JNoise(100, ofColor::white, JNOISE_MODE::VERTICAL, numLines));
        JNoise* n = (JNoise*)v->getLast();
        n->offset = offsets[j];
        n->addEnv(
                  vector<float>{n->size.x, n->size.x + growAmount * 800},
                  vector<float>{ofMap(growAmount, 0., 1.0, 50, 4000)},
                  &(n->size.x));
        n->addEnv(
                  vector<float>{n->loc.x, (float)(-growAmount*0.5 * 800)},
                  vector<float>{ofMap(growAmount, 0., 1.0, 50, 4000)},
                  &(n->loc.x));
        n->setEnvelope(env[1], 100, env[0]);
        n->seed = ofRandom(10000);
    }
}

void MaybeTomorrow::F8(){ // Sustaining Noise||
    v->addEvent(new JNoise(100));
    JNoise* n = (JNoise*)v->getLast();
    n->setEnvelope(100, 100, 3000);
}

void MaybeTomorrow::F9(){ // ||
}
void MaybeTomorrow::F10(){ // ||
}
void MaybeTomorrow::F11(){ // ||
}
void MaybeTomorrow::F12(){ // ||
}
void MaybeTomorrow::F13(){ // ||
}
void MaybeTomorrow::F14(){ // ||
}
void MaybeTomorrow::F15(){ // ||
}


void MaybeTomorrow::C0(int val){ // Release ||
    env[0] = ofMap(val, 0, 127, 10, 2000);
}

void MaybeTomorrow::C1(int val){ // Attack ||
    env[1] = ofMap(val, 0, 127, 10, 2000);
}

void MaybeTomorrow::C2(int val){ // numLines ||
    numLines = ofMap(val, 0, 127, 1, 200);
}

void MaybeTomorrow::C3(int val){ // growth ||
    growAmount = ofMap(val, 0, 127, 0., 1.0);
}

void MaybeTomorrow::generateChorusMeshes(){
    multiMesh.clear();
//    multiMesh.push_back((MultiMeshMaybeTomorrow*)addEvent(new MultiMesh(4)));
    
    multiMesh.push_back((MultiMeshMaybeTomorrow*)addEvent(new MultiMeshMaybeTomorrow(5, ofFloatColor(0.6, 0.6, 0.6, 1))));
    
    multiMesh.push_back((MultiMeshMaybeTomorrow*)addEvent(new MultiMeshMaybeTomorrow(6, ofFloatColor(0.97254901960784,0.7921568627451,0,1))));
    
    multiMesh.push_back((MultiMeshMaybeTomorrow*)addEvent(new MultiMeshMaybeTomorrow(7, ofFloatColor(0.90980392156863,0.74901960784314,0.33725490196078, 1))));
    
    for(int i=0; i<multiMesh.size(); i++){
        multiMesh[i]->generateJildert();
        multiMesh[i]->deleteWithFade(10000);
    }
}

void MaybeTomorrow::exit(){
    
}

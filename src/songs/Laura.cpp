//
//  Laura.cpp
//  Visualizer_1_8
//
//  Created by Jildert Viet on 12-05-17.
//
//

#include "Laura.hpp"

Laura::Laura(ofxJVisuals* v){
//    ofSetFrameRate(30);
    this->v = v;
    v->bAddMirror = true;
#if USE_PP
    v->bPostProcessing = true;
    v->zoomPP->setEnabled(true);
    v->noisePP->setEnabled(true);
    
    v->noisePP->setFrequency(0.01);
    v->noisePP->setAmplitude(0.0);
    
    v->zoomPP->setDensity(0.1); // Default is 0.25
    v->zoomPP->setExposure(0.53);
    v->zoomPP->setWeight(0.9);
#endif
    
    
    Vorm* prevVorm = nullptr;
    int numVormsX = 3;
    int numVormsY = 4;
    
    for(char i=0; i<numVormsX; i++){
        for(char j=0; j<numVormsY; j++){
            Vorm* vorm;
            float radius = ofGetWidth() / numVormsX * 0.3;
            
            int index = i;
            if(i > numVormsX/2.){
                index = numVormsX - (i+1);
            }
            index += 1;
            
            cout << index << endl;
            radius *= (1 + (1./ (index)) );
            cout << radius << endl;
            
            ofVec2f center = ofVec2f(ofGetWidth()/(numVormsX+1) * (i+1), ofGetHeight()/(numVormsY+1) * (j+1));
            vorm = new Vorm(3, 2, radius, center);
//            vorm->formVorm();
            vorm->change_maxspeed(0.3); // 0.3
            int lijnmax = 130;
            vorm->lijnmax = (lijnmax*lijnmax)*(1 + (1./index));
            vorm->lineWidth = 2;
            vorm->setColor(ofColor(255,255,255,255)); // Was 80
            
            v->addEvent(vorm);
        
            vormen.push_back(vorm);
        }
    }
    for(int i=0; i<vormen.size(); i++){
        for(int j=i+1; j<vormen.size(); j++){
            vormen[i]->addConnection(vormen[j]);
        }
    }
}

void Laura::F0(){ // Form vorms||
    int state = vormen[0]->particles[0]->state;
    state = !state;
    
    for(int i=0; i<vormen.size(); i++){
        vormen[i]->change_state_of_particles(state);
    }
}

void Laura::F1(){ // Lines ||
    for(int i=0; i<10; i++){
        ofVec2f loc = ofVec2f(ofGetWidth() / (10-1) * i, 0);
        ofVec2f size = ofVec2f(5, ofGetHeight());
        JRectangle* rect = (JRectangle*)v->addEvent(new JRectangle(3000, loc, size));
        rect->setEnvelope(200, 10, ofRandom(2700,2700));
        rect->setColor(ofColor(255,10));
    }
}

void Laura::F2(){ // ||
    
}

void Laura::F3(){ // ||
    
}

void Laura::F4(){ // ||

}

void Laura::F5(){ // ||
    
}

void Laura::F6(){ // ||
    
}

void Laura::F7(){ // ||
    
}

void Laura::F8(){ // ||
    
}

void Laura::F9(){ // ||
    
}

void Laura::F10(){ // ||
    
}

void Laura::F11(){ // ||
    
}

void Laura::F12(){ // ||
    
}

void Laura::F13(){ // ||
    
}

void Laura::F14(){ // ||
    
}

void Laura::F15(){ // ||
    
}

void Laura::C0(int val){ // NoiseWarp ||
    float value = ofMap(val, 0, 127, 0.01, 0.5);
    float value2 = ofMap(val, 0, 127, 0.0, 0.1);
#if USE_PP
    v->noisePP->setFrequency(value);
    v->noisePP->setAmplitude(value2);
#endif
}

void Laura::C1(int val){ // Lines-Density||
    float value = ofMap(val, 0, 127, 0.05, 0.4);
#if USE_PPP
    v->zoomPP->setDensity(value); // Default is 0.25
#endif
}

void Laura::C2(int val){ // Exposure ||
    float value = ofMap(val, 0, 127, 0.01, 1.5);
#if USE_PP
    v->zoomPP->setExposure(value);
#endif
}

void Laura::C3(int val){ // Particlespeed ||
    float value = ofMap(val, 0, 127, 0.001, 2);
    for(int i=0; i<vormen.size(); i++){
        for(int j=i+1; j<vormen.size(); j++){
            vormen[i]->change_maxspeed(value, value*2);
        }
    }
}

void Laura::C4(int val){ // ||
    
}

void Laura::C5(int val){ // ||
    
}

void Laura::C6(int val){ // ||
    
}

void Laura::exit(){
#if USE_PP
    v->bPostProcessing = false;
#endif
    v->killAll();
}

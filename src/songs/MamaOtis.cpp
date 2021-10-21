//
//  MamaOtis.cpp
//  Bas
//
//  Created by Jildert Viet on 24-04-16.
//
//

#include "MamaOtis.hpp"

MamaOtis::MamaOtis(ofxJVisuals* v){
    setName("MamaOtis");
    this->v = v;
    v->setAlpha(50);
    makeThings1();
}

void MamaOtis::stop(){
    for(int i=0; i<vormen.size(); i++){
        Vorm* vorm = vormen[i];
        vorm->active = true;
        vorm->setEnvelope(1, 1, 3000, ofVec2f(0, 230));
//        vorm->envelope->triggerEnv();
    }
}

void MamaOtis::F0(){ // Implode ||
    static int index = 0;
    if(index++>=(vormen.size()-1))
        index = 0;
    
    Vorm* vormTemp = vormen[(index + 1) % (vormen.size())];
    Vorm* prevVorm = vormen[index];
    
    prevVorm->switchConnectableness();
    //    prevVorm->formVorm();
    
    vormTemp->switchConnectableness();
    vormTemp->rotateCoordinates(20, 1);
}

void MamaOtis::F1(){ // Triangle ||
    makeThings2();
}

void MamaOtis::F2(){ // showParticlesVormen ||
    showParticlesVormen();
}

void MamaOtis::F3(){ // Cube L ||
    makeCube1();
}

void MamaOtis::F4(){ // Cube R ||
    makeCube2();
}

void MamaOtis::F5(){ // Cubes and falling ||
    makeCube1(); makeCube2();
    fall();
}

void MamaOtis::F6(){ // Final Vorm ||
    finalVorm();
}

void MamaOtis::F7(){ // Red gradient ||
//                visualizer->alphaScreen->gradientColor = ofColor(255,0,0);
    v->alphaScreen->gradientColor = ofColor(255-20,46-20,45-20);
    v->alphaScreen->bGradient = !(v->alphaScreen->bGradient);
}

void MamaOtis::makeVorm(int numSides, int sideDivision, float radius, ofVec2f center, bool on_destination){
    addEvent(new Vorm(numSides, sideDivision, radius, center, on_destination));
    vormen.push_back( (Vorm*)getLast() );
}

void MamaOtis::functionOne(){

}

void MamaOtis::functionTwo(){
    static int index = 0;
    if(index++>=vormen.size())
        index = 0;
    Vorm* vormTemp = vormen[(index + 1) % vormen.size()];
    Vorm* prevVorm = vormen[index];
    
    prevVorm->changeRadius(vormen[(index + 4 + (int)ofRandom(2)) % vormen.size()]->radius);
    vormen[(index + 4 + (int)ofRandom(2)) % vormen.size()]->particlesVisible = ofRandom(0,1);
}

void MamaOtis::changeSpeed(){
    for(int i=0; i<vormen.size(); i++){
        vormen[i]->particlesVisible = true;
        vormen[i]->change_maxspeed(2.0+(i/12.));
    }
}

void MamaOtis::makeThings1(){
    for(int i=0; i<10; i++){ // IMPLODES
        makeVorm(6,5+(i),150+(i*20),ofGetWindowSize()/2., false);
//        vormen[i]->switchConnectableness();
        vormen[i]->particlesVisible = false;
        vormen[i]->change_maxspeed(2.0-(i/10.));
        vormen[i]->formVorm();
        vormen[i]->switchConnectableness();
        vormen[i]->lijnmax = 50*50;
    }
        vormen[1]->switchConnectableness();
 }

void MamaOtis::makeThings2(){
    triangles.clear();
    float num = 16;
    for(int i=0; i<num; i++){ // BAR
        int r = ofGetWindowHeight()/2.-50;
        ofVec2f newLoc = (ofGetWindowSize()/2.)+ofVec2f(cos(i/num*TWO_PI)*r, sin(i/num*TWO_PI)*r);
        addEvent(new Vorm(3, 2, 50, newLoc, true));
        Vorm* triangle = ((Vorm*)getLast());
        
        triangles.push_back(triangle);
        if(i!=0){
            triangle->addConnection(lastTriangle);
        }
        triangle->addVector(&triangles);
        lastTriangle = triangle;
        triangle->formVorm();
        triangle->change_maxspeed(0.1);
        triangle->setState(0);
        triangle->lijnmax = pow(130,2);
//        triangle->switchConnectableness();
        triangle->alphaAdd = 0;
        triangle->active = true;
        triangle->setEnvelope(10, 100, 2000, ofVec2f(0, 100));
    }
    triangles.front()->addConnection(lastTriangle);
}

void MamaOtis::makeCube1(){
    addEvent(new Vorm(4, 10, 110, ofVec2f(125,ofGetWindowHeight()/2.), true));
    Vorm* vorm = ((Vorm*)getLast());
    cube1 = vorm;
    vorm->formVorm();
    //    vorm->switchConnectableness();
    vorm->change_maxspeed(0.5);
    vorm->lijnmax = 50*50;
    
    vorm->changeRadius(vorm->radius+100);
    vorm->rotateCoordinates(vorm->getNumParticles()-10, 1);
    vorm->active = true;
    vorm->setEnvelope(10, 200, 4000, ofVec2f(0, 230));
}

void MamaOtis::makeCube2(){
    addEvent(new Vorm(4, 10, 110, ofVec2f(ofGetWindowWidth()-125,ofGetWindowHeight()/2.), true));
    Vorm* vorm = ((Vorm*)getLast());
    cube2 = vorm;
    vorm->formVorm();
//    vorm->switchConnectableness();
    vorm->change_maxspeed(0.5);
    vorm->lijnmax = 50*50;
    
    vorm->changeRadius(vorm->radius+100);
    vorm->rotateCoordinates(10, 1);
    vorm->active = true;
    vorm->setEnvelope(10, 200, 4000, ofVec2f(0, 230));
}


void MamaOtis::showParticlesVormen(){
    for(unsigned char i=0; i<vormen.size(); i++)
        vormen[i]->switchState();
}

void MamaOtis::fall(){
    for(int i=0; i<vormen.size(); i++){
        Vorm* vorm = vormen[i];
        vorm->addToCenter(ofVec2f(0,1800));
        vorm->change_maxspeed(0.5, 2.0);
        vorm->setEnvelope(10, 10, 11000, &(vorm->lijnmax), ofVec2f(pow(1,2), pow(50,2)));
        vorm->formVorm();
        if(vorm->isConnectable() == false)
            vorm->switchConnectableness();
    }
    triangles.erase( std::remove_if( triangles.begin(), triangles.end(), MamaOtis::checkIfNull ), triangles.end() );

    for(int i=0; i<triangles.size(); i++){
        Vorm* triangle = triangles[i];
        triangle->addToCenter(ofVec2f(0,1800));
        triangle->change_maxspeed(0.5, 2.0);
        triangle->formVorm();
    }
    if(cube1){
        cube1->addToCenter(ofVec2f(0,1800));
        cube1->change_maxspeed(0.5, 2.0);
        cube1->formVorm();
    }
    
    if(cube2){
        cube2->addToCenter(ofVec2f(0,1800));
        cube2->change_maxspeed(0.5, 2.0);
        cube2->formVorm();
    }
}

void MamaOtis::thingsWithCircle(){
    for(char i=0; i<triangles.size(); i++){
        triangles[i]->switchConnectableness();
    }
}

void MamaOtis::finalVorm(){
    addEvent(new Vorm(6, 10, 110*3, ofGetWindowSize()/2., true));
    Vorm* vorm = ((Vorm*)getLast());
//    cube2 = vorm;
    vorm->formVorm();
    vorm->change_maxspeed(0.6);
    vorm->lijnmax = 50*50;
    
    vorm->changeRadius(vorm->radius/2.);
    vorm->rotateCoordinates(vorm->getNumParticles()/3., 1);
    vorm->active = true;
    vorm->setEnvelope(10, 200, 8000, ofVec2f(0, 230));
}

bool MamaOtis::checkIfNull(Vorm* r){
    if(r){
        return false;
    } else{
        return true;
    }
}

void MamaOtis::exit(){
    for(unsigned char i=0; i<vormen.size(); i++)
        delete vormen[i];

//    int fadeTime = 1000;
//    ofColor* color = &(v->alphaScreen->colors[0]);
//    v->alphaScreen->setEnvelope(0.1, 0.1, fadeTime, &(color->r));
}

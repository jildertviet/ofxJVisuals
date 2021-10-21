//
//  cameraController.cpp
//  versesVideo
//
//  Created by Jildert Viet on 24-01-18.
//
//

#include "cameraController.hpp"

cameraController::cameraController(ofEasyCam* c){
    cam = c;
}

void cameraController::specificFunction(){
    
    ofVec3f lookAt;
    if(!focus){
        lookAt = ofVec3f(0,0,0);
    } else{
        lookAt = *focus;
    }

    if(rotationSpeed.y){
        cam->rotateAround(rotationSpeed.y, ofVec3f(0, 1, 0), ofVec3f(0,0,0));
        cam->lookAt(lookAt);
    }
    if(rotationSpeed.x){
        cam->rotateAround(rotationSpeed.x, ofVec3f(1, 0, 0), ofVec3f(0,0,0));
        cam->lookAt(lookAt);
    }
    if(rotationSpeed.z){
        cam->rotateAround(rotationSpeed.z, ofVec3f(0, 0, 1), ofVec3f(0,0,0));
        cam->lookAt(lookAt);
    }

    if(rotateSpeed.x)
        cam->rotate(rotateSpeed.x, ofVec3f(1,0,0));
    if(rotateSpeed.y){
        cam->rotate(rotateSpeed.y, ofVec3f(0,1,0));
    }
    if(rotateSpeed.z)
        cam->rotate(rotateSpeed.z, ofVec3f(0,0,1));

    if(boomSpeed)
        cam->boom(boomSpeed);
    if(dollySpeed)
        cam->dolly(dollySpeed);
    if(truckSpeed)
        cam->truck(truckSpeed);
    
    if(orbitSpeeds.x || orbitSpeeds.y){
        orbitValues += orbitSpeeds;
        cam->orbit(orbitValues.x, orbitValues.y, orbitDistance);
    }
}

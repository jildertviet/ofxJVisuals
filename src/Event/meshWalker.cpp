//
//  meshWalker.cpp
//  meshTrails
//
//  Created by Jildert Viet on 18-01-18.
//
//

#include "meshWalker.hpp"

meshWalker::meshWalker(ofMesh* m, ofVec2f dimensions){
    this->m = m;
    destination = m->getVertex(0);
    
    this->dimensions = dimensions;
    numPoints = ((dimensions.x + 1) * (dimensions.y + 1));
//    cout << "Num points on mesh: " << numPoints << endl;
    trail.setMode(OF_PRIMITIVE_LINES);
    trail.enableColors();
    getDestination(ofVec2f(1,0));
    
//    color = ofFloatColor(ofRandom(0.7, 1.0), ofRandom(0.3, 0.5), ofRandom(0.9, 1.0));
    speed = ofRandom(3, 7) * 0.3;
    
    locOnGrid = ofVec2f(0,0);
    loc = m->getVertex(0);
    
//    segmentLength = m->getVertex(0).distance(m->getVertex(1));
//    diagonalSegmentLength = segmentLength * sqrt(2); // Sqrt(2)
    
//    cout << "segmentLength: " << segmentLength << endl;
//    cout << "diagonalSegmentLength: " << diagonalSegmentLength << endl;
}

void meshWalker::specificFunction(){
    if(numSteps > 1 && destination != ofVec3f(-1, -1, -1)){
        loc += direction * speed;
        numSteps--;
    } else if(numSteps <= 1){
        loc = destination;
        // Get new random destination?
        //        getDestination(getPerlinDirection());
        if(staticDirection){
            getDestination(ofVec2f(1,0));
        } else{
            while(!getDestination(getDirection()));
        }
        
        if(trailPoints.size() >= trailLength)
            trailPoints.erase(trailPoints.begin());
        trailPoints.push_back(loc);
    }
}

void meshWalker::display(){
    ofSetLineWidth(3);
    displayTrail();
    //    ofDrawRectangle(loc-ofVec3f(5,5,0), 10, 10);
    //    ofSetColor(ofColor::darkGreen);
    //    ofDrawRectangle(destination-ofVec3f(5,5,0), 10, 10);
//    ofSetColor(255);
//    ofDrawSphere(loc,3);
}

bool meshWalker::getDestination(ofVec2f d){
    if(locOnGrid.y == 0 && d.y == -1)
        return false;
    
    locOnGrid += d; // (0,0) + (0,1), down, becomes: (0,1) (4,3) + (1,0), right, becomes: (5,3)
    
    if(locOnGrid.x >= dimensions.x){
        locOnGrid.x = dimensions.x;
        meshIndex = ((locOnGrid.x-1) * dimensions.y * 6 + (locOnGrid.y * 6)) + 1;
    } else if(locOnGrid.x < 0){
        locOnGrid.x = 0;
    }
    
    if(locOnGrid.y >= dimensions.y){
        locOnGrid.y = dimensions.y;
        meshIndex =(locOnGrid.x * dimensions.y * 6 + ((locOnGrid.y - 1) *6)) + 4;
    } else if(locOnGrid.y < 0){
        locOnGrid.y = 0;
    }
    
    if(locOnGrid.x < dimensions.x && locOnGrid.y < dimensions.y){
        //        cout << "Normal move" << endl;
        meshIndex = ((locOnGrid.x * dimensions.y * 6) + (locOnGrid.y * 6));
    }
    
    if(locOnGrid == dimensions){
        //        cout << "Right bottom" << endl;
        meshIndex = m->getNumVertices() - 1;
    }
    
    destination = m->getVertex(meshIndex);
//    destination += ofVec3f(0,0,1);
    direction = destination - loc; // (2,2) - (1,1), direction: (1,1) /////   (4,0) - (0,0) = direction(4,0).norm -> (1,0)
    direction.normalize();
    
    //    cout << locOnGrid << endl;
    travelDistance = loc.distance(destination);
    numSteps = travelDistance / speed;
    numStepsStart = numSteps;
    
    if(numStepsStart == 0){
        return false;
    } else{
        return true;
    }
    
    // How many steps to take? After n steps, loc = destination, get new destination.
    // loc += direction * speed;     (0,0) + ((1,0) * (0.5)) = (0.5, 0) - (1, 0) - (1.5, 0) - (2, 0)
}

ofVec3f meshWalker::getPerlinDirection(){
    perlinReadPoint += 0.1;
    ofVec2f d = ofVec2f(ofMap(ofNoise(perlinReadPoint), 0, 1, -1, 1), ofMap(ofNoise(perlinReadPoint+5), 0, 1, -1, 1));
    d.x = round(d.x);
    d.y = round(d.y);
    return d;
}

ofVec3f meshWalker::getDirection(){
    // Get angle (45, 90, -90 etc), add or subtract 45, normalize this direction
    char seed = ofRandom(2);
    //    cout << "Seed: " << (int)seed << endl;
    if(seed >= 1){
        globalDirection.rotate(45, ofVec3f(0,0,1));
    } else{
        globalDirection.rotate(-45, ofVec3f(0,0,1));
    }
    
    globalDirection.normalize();
    globalDirection.x = round(globalDirection.x);
    globalDirection.y = round(globalDirection.y);
    
    if(globalDirection == ofVec2f(-1, 1) || globalDirection == ofVec2f(1, -1))
        return getDirection();
    return globalDirection;
}

void meshWalker::displayTrail(){
    trail.clear();
    
    if(trailPoints.size() > 1){
        float ratio = (1 - numSteps/(float)numStepsStart);
        
        ofVec3f p = trailPoints[1] - trailPoints[0];
        p *= ratio;
        
        trail.addVertex(trailPoints[0] + p);
        
        for(uint16 i=1; i<trailPoints.size(); i++){
            trail.addVertex(trailPoints[i]);
        }
        
        trail.addVertex(loc);
        
        for(uint16 i=0; i<trail.getNumVertices()-1; i++){
            trail.addIndex(i);
            trail.addIndex(i+1);
        }
        
        ofFloatColor c = color;
        c.a = 0;
        trail.addColor(c);
        
        for(uint16 i=1; i<trail.getNumVertices(); i++)
            trail.addColor(color);
        
//        addFade();
        addFadeFromStart();
        //        for(uint16 i=0; i<trail.getNumVertices(); i++)
        //            ofDrawCircle(trail.getVertex(i), 4);
    }
    
    if(bDrawCircles){
        ofSetColor(255);
        for(uint16 i=0; i<trailPoints.size(); i++)
            ofDrawCircle(trailPoints[i], 4);
        ofSetColor(ofColor(255,0,0));
        ofDrawCircle(loc, 4);
        //        cout << "Loc: " << loc << endl;
    }
    
    ofSetColor(255);
    trail.draw();
}

void meshWalker::addFade(){
//    int length;
//    length += trailPoints[0].distance(trailPoints[1]);
//    for(uint16 i=0; i<trail.getNumVertices()-2; i++){
//        if(trailPoints[i].x == trailPoints[i+1].x || trailPoints[i].y == trailPoints[i+1].y){
//            length += segmentLength;
//        } else {
//            length += diagonalSegmentLength;
//        }
//    }
//    length += trailPoints.back().distance(loc);
//    cout << "Length: " << length << endl;
    
    int toPlace = fadeLength;
    int totalLength = toPlace;
    
    uint32 writePos = 0;
//    float distance = trail.getVertex(writePos).distance(trail.getVertex(writePos+1));
    float distance = glm::distance(trail.getVertex(writePos),trail.getVertex(writePos+1));
    float prevAlpha = 0;
    
    if(!isinf(distance) && distance != 0){
        while(toPlace > distance && writePos < trail.getNumVertices()){
            if(!isinf(distance) && distance != 0){
                writePos++;
                float a = (distance / toPlace) + prevAlpha;
                a = ofMap(a, 0., 1., 0., color.a);
//                cout << a << endl;
                trail.getColors()[writePos].a = a;
                toPlace -= distance;
                
                ofVec3f p1 = trail.getVertex(writePos);
                //            cout << "P1: " << p1 << endl;
                ofVec3f p2 = trail.getVertex(writePos+1);
                //            cout << "P2: " << p2 << endl;
                
                distance = p1.distance(p2);
                //            cout << "distance: " << distance << endl;
                //            cout << "toPlace: " << toPlace << endl;
                
                if( trail.getVertex(writePos) == trail.getVertex(writePos+1) || distance == 0){
                    toPlace--;
                    break;
                }
                
                if(writePos >= trail.getNumVertices())
                    writePos = trail.getNumVertices();
                
                prevAlpha = a;
            } else{
                writePos++;
                ofVec3f p1 = trail.getVertex(writePos);
                ofVec3f p2 = trail.getVertex(writePos+1);
                distance = p1.distance(p2);
            }
        }
    }
    
    if(toPlace <= distance || writePos == trail.getNumVertices() - 1){
        //        cout << "first point: " << trail.getVertex(writePos+1) << " second: " << trail.getVertex(writePos) << endl;
        
//        ofVec3f direction = ( - .normalize();
         glm::vec3 direction = trail.getVertex(writePos+1) - glm::normalize(trail.getVertex(writePos));
        //        cout << "Direction: " << direction << endl;
        ofVec3f newPoint = trail.getVertex(writePos) + (direction * (toPlace));
        //        cout << "newPoint: " << newPoint << endl;
        
        //        ofDrawCircle(newPoint, 3);
        trail.addVertex(newPoint);
        trail.addColor(color);
        
        uint32 nextIndex = trail.getNumVertices();
        nextIndex--;
        //        cout << "nextIndex: " << nextIndex << endl;
        //        cout << "writePos: " << writePos << endl;
        trail.getIndices().insert(trail.getIndices().begin() + ((writePos)*2 + 1), nextIndex);
        trail.getIndices().insert(trail.getIndices().begin() + ((writePos)*2 + 1), nextIndex);
    }    
}

void meshWalker::addFadeFromStart(){
    for(uint32 i=0; i<trail.getNumVertices(); i++){
        float a = (float)i / trail.getNumVertices();
        a = ofMap(a, 0., 1., 0., color.a);
//        if(a<0)
//            cout << 1 << endl;
        trail.getColors()[i].a = a;
    }
}

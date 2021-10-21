//
//  pathWalker.cpp
//  versesVideo
//
//  Created by Jildert Viet on 29-01-18.
//
//

#include "pathWalker.hpp"

pathWalker::pathWalker(vector<ofVec3f> path){
    this->path = path;
    destination = path[1];
    loc = path[0];

    direction = destination - loc; // (2,2) - (1,1), direction: (1,1) /////   (4,0) - (0,0) = direction(4,0).norm -> (1,0)
    direction.normalize();
    
    travelDistance = loc.distance(destination);
    numSteps = travelDistance / speed;
    numStepsStart = numSteps;
    
    
    trail.setMode(OF_PRIMITIVE_LINES);
    trail.enableColors();

    speed = ofRandom(3, 7) * 0.3;

    locOnGrid = ofVec2f(0,0);
    
}

void pathWalker::specificFunction(){
    if(numSteps > 1){
        loc += direction * speed;
        numSteps--;
    } else if(numSteps <= 1){
        loc = destination;
        
        locOnGrid.x ++;
        destination = path[locOnGrid.x];
        
        direction = destination - loc; // (2,2) - (1,1), direction: (1,1) /////   (4,0) - (0,0) = direction(4,0).norm -> (1,0)
        direction.normalize();
        
        travelDistance = loc.distance(destination);
        numSteps = travelDistance / speed;
        numStepsStart = numSteps;
        
        if(trailPoints.size() >= trailLength)
            trailPoints.erase(trailPoints.begin());
        trailPoints.push_back(loc);
    }
//    
//    for(int i=0; i<trailPoints.size(); i++){
//        cout << trailPoints[i] << endl;
//    }
    
}

void pathWalker::display(){
//    color = ofFloatColor::red;
    ofSetLineWidth(lineWidth);
    ofSetColor(color);
    displayTrail();
}

void pathWalker::displayTrail(){
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
        
        
        float totalLength = 0;
        vector<float> distances;
        if(trail.getNumVertices() > 1){
            for(int i=0; i<trail.getNumVertices()-1; i++){
//                float d = trail.getVertex(i).distance(trail.getVertex(i+1));
                float d = glm::distance(trail.getVertex(i), trail.getVertex(i+1));
                totalLength += d;
                distances.push_back(totalLength);
            }
//            cout << "length: " << totalLength << endl;
            for(int i=0; i<trail.getNumVertices()-1; i++){
                float a = ofMap(distances[i], 0, totalLength, 0., 1.);
//                cout << i << " " << a << endl;
                ofFloatColor c = color;
                c.a = a;
                trail.setColor(i, c);
            }
        }
        //        addFade();
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
    
//    ofSetColor(255,0,0);
    trail.draw();
}

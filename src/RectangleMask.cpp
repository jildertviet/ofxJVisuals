//
//  RectangleMask.cpp
//  Visualizer_1_4
//
//  Created by Jildert Viet on 15-01-17.
//
//

#include "RectangleMask.hpp"

RectangleMask::RectangleMask(){
    setType("RectangleMask");
    colors[0] = ofColor(0,0,0,255);
}

RectangleMask::RectangleMask(JRectangle* r){
    setType("RectangleMask");
    addRect(r);
}

void RectangleMask::specificFunction(){
//    cout << "RectangleMask update()" << endl;
    rects.erase( std::remove_if( rects.begin(), rects.end(), RectangleMask::checkIfNull ), rects.end() );
    for(int i=rects.size()-1; i>=0; i--){
        rects[i]->updateMain();
    }
    findLeftAndRight();
}

void RectangleMask::display(){
//    cout << "RectangleMask display()" << endl;
//    return;
    if(rects.size()==0){ // Black
        ofSetColor(colors[0]);
        ofDrawRectangle(0,0,ofGetWindowWidth(), ofGetWindowHeight());
        return;
    }
    
    if(rects.size()==1){
        JRectangle* r = rects[0];
        ofSetColor(colors[0]);
        
        ofDrawRectangle(0, 0, r->loc.x, ofGetWindowHeight()); // From left side
        ofDrawRectangle(r->loc.x+r->size.x, 0, ofGetWindowWidth(), ofGetWindowHeight()); // From right side
        
        int alpha = colors[0].a - rects[0]->colors[0].a;
        if(alpha < 0)
            alpha = 0;
        ofSetColor(0, alpha);
        ofDrawRectangle(r->loc.x, r->loc.y, r->size.x, r->size.y);
        return;
    }
    
    ofSetColor(colors[0]);
    
    // Left side
    ofRectangle r = ofRectangle(ofVec2f(0,0), ofVec2f(lefts[0], ofGetWindowHeight()));
    ofDrawRectangle(r);
    
    for(int i=0; i<lefts.size()-1; i++){
        r = ofRectangle(ofVec2f(rights[i], 0), ofVec2f(lefts[i+1], ofGetWindowHeight()));
        ofDrawRectangle(r);
    }
    
    // Right side
    r = ofRectangle(ofVec2f(rights.back(),0), ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
    ofDrawRectangle(r);
    
    for(int i=0; i<rects.size(); i++){
        // Draw envelope of rect @ locations of rects
        int alpha = colors[0].a - rects[i]->colors[0].a;
        if(alpha < 0)
            alpha = 0;
        ofSetColor(0, alpha);
        ofDrawRectangle(rects[i]->loc.x, rects[i]->loc.y, rects[i]->size.x, rects[i]->size.y);
        ofSetColor(colors[0]);
    }

}

void RectangleMask::addPtr(RectangleMask** p){
    toClear.push_back((Event**)p);
}

void RectangleMask::addRect(JRectangle* r){
//    r-> comment = "RectMask-Rect";
//    cout << rects.size() << "is: " << r << endl;
    rects.push_back(r);
    r->addVector(&rects);
    findLeftAndRight();
}

void RectangleMask::randomizeSpeeds(){
    for(int i=0; i<rects.size(); i++){
        rects[i]->speed = ofRandom(0.5, 2.);
    }
}

void RectangleMask::randomizeDirections(){
    for(int i=0; i<rects.size(); i++){
        if(ofRandom(-1,1)>0){
            rects[i]->direction = ofVec2f(-1,0);
        } else{
            rects[i]->direction = ofVec2f(1,0);
        }
    }
}

bool RectangleMask::checkIfNull(JRectangle* r){
    if(r){
        return false;
    } else{
        return true;
    }
}

void RectangleMask::findLeftAndRight(){
    rects.erase( std::remove_if( rects.begin(), rects.end(), RectangleMask::checkIfNull ), rects.end() );
    
    vector<int>().swap(lefts);
    vector<int>().swap(rights);
    
    for(int i=0; i<rects.size(); i++){
        lefts.push_back(rects[i]->loc.x);
        rights.push_back(rects[i]->loc.x+rects[i]->size.x);
    }
    
    // Order vector, based on position
    std::sort (lefts.begin(), lefts.end());
    std::sort (rights.begin(), rights.end());
    
    // Find overlap
    if(rights.size()>1 && lefts.size()>1){ // Only when there are 2
        bool bOverlap = true;
        ofVec2f toDelete;
        
        while(bOverlap==true){
            if(rights.size()>1 && lefts.size()>1){
                for(int i=0; i<lefts.size()-1; i++){
                    if(rights[i] > lefts[i+1]){
                        bOverlap = true;
                        toDelete = ofVec2f(i+1, i);
                    } else{
                        bOverlap = false;
                    }
                    if(bOverlap){
                        if(rights.size()>1 && lefts.size()>1){
                            lefts.erase(lefts.begin()+toDelete[0]);
                            rights.erase(rights.begin()+toDelete[1]);
                        } else{
                            bOverlap = false;
                        }
                    }
                }
            } else{
                bOverlap = false;
            }
        }
        
//        cout << "Lefts:\t";
//        for(int i=0; i<lefts.size(); i++){
//            cout << lefts[i] << "\t";
//        }
//        cout << endl;
        
//        cout << "Rights:\t";
//        for(int i=0; i<rights.size(); i++){
//            cout << rights[i] << "\t";
//        }
//        cout << endl;
        
    }
    
    std::sort (lefts.begin(), lefts.end());
    std::sort (rights.begin(), rights.end());
}

//
//  dotsBlob.cpp
//  dots
//
//  Created by Jildert Viet on 10-04-18.
//

#include "dotsBlob.hpp"

dotsBlob::dotsBlob(){
    setType("dotsBlob");
    connectionL = new ofPolyline[maxConnections];
    connectionR = new ofPolyline[maxConnections];
    seed = ofRandom(1.0);
    for(char i=0; i<ofRandom(1,2); i++){
        circles.push_back(dotsCircle());
        ofVec3f ax;
        switch((int)ofRandom(3)){
            case 0:
                ax = ofVec3f(1, 0, 0);
                break;
            case 1:
                ax = ofVec3f(0, 1, 0);
                break;
            case 2:
                ax = ofVec3f(0, 0, 1);
                break;
        }
        circles.back().axis = ax;
    }
    
    dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    colors.push_back(ofColor(255, 30));
}

void dotsBlob::update(){
    calculateConnections();
    for(char i=0; i<circles.size(); i++){
        circles[i].rotation += circles[i].speed;
    }
//    calcPolyBlob();
    if(speed != 0)
        loc += speed * dir;
}

void dotsBlob::updateCircles(){
    
}

void dotsBlob::drawCircles(ofColor c){
    ofNoFill();
    ofSetColor(c);
    for(char i=0; i<circles.size(); i++){
        ofPushMatrix();
//        ofRotate
        ofTranslate(loc);
        ofRotate(circles[i].rotation, circles[i].axis.x, circles[i].axis.y, circles[i].axis.z);
        ofDrawCircle(ofVec2f(0, 0), circles[i].r);
        ofPopMatrix();
    }
    ofFill();
}

void dotsBlob::display(){
//    ofSetLineWidth(2.0);
    ofSetColor(colors[0]);
    if(!bOnlyLines)
        ofDrawSphere(loc, 10);
    
    ofColor cLines = colors[0]; // For lines and rotating circles (less bright)
    cLines.a *= 0.4;
    if(!bOnlyLines)
        drawCircles(colors[0]);
    ofSetColor(cLines); // Two lines
    
    ofDrawLine(loc.x, -ofGetWindowHeight(), loc.x, ofGetWindowHeight());
    ofDrawLine(-ofGetWindowWidth(), loc.y, ofGetWindowWidth(), loc.y);
    
    if(bOnlyLines)
        return;
    
    if(bDisplayRange){
        ofNoFill();
        if(bDrawSphere)
            ofDrawSphere(loc, range);
//        offill();
        ofColor t = colors[0];
        t.a *= 0.6;
        ofSetColor(t);
//        ofNoFill();
        ofDrawCircle(loc, range);
        ofFill();
    }
    
    if(percent!=1.0){
        ofSetColor(ofColor(255, 0, 0, 50));
        for(int i=0; i<connections.size(); i++){
            if(distances[i] < minDistance)
                continue;
//            if(i >= maxConnections) // why?
//                return;
//            ofDrawCircle(connectionL[i].getVertices().back(), 2);
//            ofDrawCircle(connectionL[i].getVertices().back(), 2);
            if(connectionL[i].getVertices().size() && i >= maxConnections)
                ofDrawCircle(connectionL[i].getVertices().back(), 2);
            if(connectionR[i].getVertices().size() && i >= maxConnections)
                ofDrawCircle(connectionR[i].getVertices()[0], 2);

         }
     }
}

void dotsBlob::drawConnections(ofColor c){
    ofSetLineWidth(3.0);

    for(char i=0; i<connections.size(); i++){
        ofColor t = c;
        int alpha = t.a;
        alpha *= 1.6;
        if(alpha > 255)
            alpha = 255;
        t.a = alpha;
        
        ofSetColor(t);
        if(i >= maxConnections) // why?
            return;
        connectionL[i].draw();
        connectionR[i].draw();
//        if(percent!=0)
    }
    ofSetLineWidth(2.);
}

void dotsBlob::calculateConnections(){
    for(char i=0; i<connections.size(); i++){
        if(i >= maxConnections)
            return;
        connectionL[i].clear();
        connectionR[i].clear();
        if(loc != connections[i]->loc){
            if(distances[i] > minDistance){
                ofVec3f dest = connections[i]->loc;
                int lineDensity = distances[i] / segmentLength;

                connectionL[i].curveTo(loc);
                for(int j=0; j<lineDensity * (percent) + 1; j++){
                    ofVec3f newPoint = loc.getInterpolated(dest, j / (float)(lineDensity-1));
                    float noiseVal = ofNoise(((float)j*0.1) + ofGetFrameNum() * 0.01 + seed) - 0.5;
                    noiseVal *= pow(sin(j / (float)lineDensity * PI), 3) * distances[i] * 0.4;
                    
                    ofVec3f rotated = ofVec3f(1, 0, 0).rotate((dest-loc).angle(ofVec2f(1, 0)) + 90, ofVec3f(0, 0, 1)) * noiseVal;
                    rotated += newPoint;
                
                    connectionL[i].curveTo(rotated);
                }

                for(int j=lineDensity * (1.0 - percent)-1; j<lineDensity; j++){
                    ofVec3f newPoint = loc.getInterpolated(dest, j / (float)(lineDensity-1));
                    float noiseVal = ofNoise(((float)j*0.1) + ofGetFrameNum() * 0.01 + seed) - 0.5;
                    noiseVal *= pow(sin(j / (float)lineDensity * PI), 3) * distances[i] * 0.4;
                    
                    ofVec3f rotated = ofVec3f(1, 0, 0).rotate((dest-loc).angle(ofVec2f(1, 0)) + 90, ofVec3f(0, 0, 1)) * noiseVal;
                    rotated += newPoint;
                    
                    connectionR[i].curveTo(rotated);
                }
                connectionR[i].curveTo(dest);
                
                

                
                return;
                float x = ofGetFrameNum() * 0.005 + seed + (i*PI);
                rotationLeft = (ofNoise(x) - 0.5) * 90.;
                rotationRight = (ofNoise(x+PI) - 0.5) * 90.;
                
//                ofVec3f dest = connections[i]->loc;
                ofVec3f middle = loc.getMiddle(dest);
                ofVec3f middleLeft = loc.getInterpolated(middle, percent + ((1.0-percent)*0.5));
                ofVec3f middleRight = dest.getInterpolated(middle, percent + ((1.0-percent)*0.5));
                middleLeft.rotate(rotationLeft, loc, ofVec3f(0, 0, 1));
                middleRight.rotate(rotationRight, dest, ofVec3f(0, 0, 1));

                ofVec3f startPointL = loc - (middleLeft - loc).normalize().rotate(startPointRotation + (rotationLeft*3), ofVec3f(0, 0, 1)) * 50;
                ofVec3f endPointL = middleLeft - (loc - middleLeft).normalize().rotate(endPointRotation - (rotationLeft*3), ofVec3f(0, 0, 1)) * 50;
                
                connectionL[i].curveTo(startPointL);
                connectionL[i].curveTo(loc);
                connectionL[i].curveTo(middleLeft);
                connectionL[i].curveTo(endPointL);
                
                ofVec3f startPointR = dest - (middleRight - dest).normalize().rotate(startPointRotation + (rotationRight*3), ofVec3f(0, 0, 1)) * 50;
                ofVec3f endPointR = middleRight - (dest - middleRight).normalize().rotate(endPointRotation - (rotationRight*3), ofVec3f(0, 0, 1)) * 50;
                
                connectionR[i].curveTo(startPointR);
                connectionR[i].curveTo(dest);
                connectionR[i].curveTo(middleRight);
                connectionR[i].curveTo(endPointR);
                
    //            connectionR.close();
    //            connectionL.close();
                for(int j=0; j<connectionL[i].getVertices().size(); j++){
                    float x = ofNoise((float)j*0.1+(ofGetFrameNum()*0.05 + seed)) - 0.5;
    //                cout << x << endl;
//                    connectionL[i].getVertices()[j] = connectionL[i].getVertices()[j].rotate(x*30, loc, ofVec3f(0, 0, 1));
                    connectionL[i].getVertices()[j] = glm::rotate(connectionL[i].getVertices()[j]-loc, x*30, glm::vec3(0, 0, 1)) + loc;
                }
                for(int j=0; j<connectionR[i].getVertices().size(); j++){
                    float x = ofNoise((float)j*0.1+(ofGetFrameNum()*0.05 + seed)) - 0.5;
                    connectionR[i].getVertices()[j] = glm::rotate(connectionR[i].getVertices()[j] - dest, x*30*-1, glm::vec3(0, 0, 1)) + dest;
                }
            } else{
                // Draw straight line?
                connectionL[i].addVertex(loc);
                connectionL[i].addVertex(connections[i]->loc);
            }
        }
    }
}

vector<dotsBlob*> dotsBlob::getBlobsInRange(){
    vector<dotsBlob*> inRange;
    vector<float*> distance;
    
    for(uint16 i=0; i<blobs->size(); i++){
        if(blobs->at(i) == this){
            // Exclude self
        } else{
            float d[2];
            d[0] = blobs->at(i)->loc.distance(loc);
//            cout << d[0] << endl;
            d[1] = i;
            if(d[0] < 100){
    //             In range
                inRange.push_back(blobs->at(i));
//                connectionR.push_back(ofPolyline());
//                connectionL.push_back(ofPolyline());
                distance.push_back(new float[2]);
                memcpy(distance.back(), d, sizeof(d));
            }
        }
    }
    cout << "Num blobs in range: " << inRange.size() << endl;
//     Sort d vec and limit to first three
//    std::sort (distance.begin(), distance.end(), sortFunction);
    for(int i=0; i<distance.size(); i++){
        cout << distance[i][0] << " " << distance[i][1] << endl;
    }
    vector<dotsBlob*> output;
    
    for(char i=0; i<maxConnections; i++){
        if(inRange.size() > i){
            output.push_back(inRange[distance[i][1]]);
        }
    }
    return output;
}


void dotsBlob::calcPolyBlob(){
    polyBlob.clear();
    for(char i=0; i<127; i++){
        ofVec2f v = ofVec2f(0, 1);
        if(i > 64){
            v.y *= ofNoise(((float)(64-(i-64))*0.05) + ofGetFrameNum() * 0.005 + seed + 4) * 0.9;
        } else{
            v.y *= ofNoise(((float)i*0.05) + ofGetFrameNum() * 0.005 + seed + 4) * 0.9;
        }
//        v.y *= sin(PI * (i/127.));
        v.y += 0.1;
        v = v.getRotated(i/127.*360, ofVec2f(0, 0));
        v *= 50;
        v += loc;
//        if(i==0)
//            polyBlob.curveTo(v);
        polyBlob.curveTo(v.x, v.y);
    }
//    polyBlob.curveTo(polyBlob.getVertices()[1]);
//    polyBlob.curveTo(polyBlob.getVertices()[1]);
}

void dotsBlob::drawPolyBlob(){
    ofNoFill();
    polyBlob.draw();
    
//    ofPushMatrix();
//    ofTranslate(loc);
//    ofRotateZ(180);
//    polyBlob.draw();
//    ofRotateZ(-180);
//    ofTranslate(-loc);
//    ofPopMatrix();
    
    ofFill();
}

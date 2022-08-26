//
//  JDivisionGrid.hpp
//  Visualizer_2_6
//
//  Created by Jildert Viet on 23-05-19.
//

#ifndef JDivisionGrid_hpp
#define JDivisionGrid_hpp

#include <stdio.h>
#include "JEvent.hpp"
#include "JMesh.hpp"

class JPoly: public JMesh{
public:
    JPoly(){ m.clear(); m.setMode(OF_PRIMITIVE_TRIANGLES);}
    bool bBlock = false; // Means it can't be split
    float z = 100;
    float zOffset = 0;
    ofPolyline p;
    
    ofPoint spawnPos;
    ofPoint endPoint;
    
    int startSide;
    int endSide;
    glm::vec3 direction;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 locAdjustmentForExport;
    bool bHasEnv = false;
    bool bDrawWireframe = false;
    void display() override{
        ofSetColor(colors[0]);
        ofSetLineWidth(1);
        p.draw();
        if(bDrawWireframe){
            m.drawWireframe();
            if(envelopes.size())
                m.draw();
        }
        
        return;
        ofSetColor(255);
        ofDrawBitmapString(ofToString(id) + " " + ofToString(p.getVertices().size()), p.getCentroid2D());
        
        ofSetColor(255, 0, 0, 200);
        ofDrawCircle(spawnPos, 10);
        ofDrawLine(spawnPos, endPoint);
        
        ofSetColor(255);
        ofDrawBitmapString(ofToString(id), spawnPos);
        
        ofSetColor(255, 100, 0, 200);
        ofDrawCircle(endPoint, 10);
        ofSetColor(255);
        ofDrawBitmapString("endpoint: " + ofToString(id), endPoint);
    }
    void prepareForSave() override{
        ofDirectory d;
        if(!d.doesDirectoryExist(getDirectoryNameForFrame()))
            d.createDirectory(getDirectoryNameForFrame());
        
        // Scale? Center?
        float scale = 0.01;
        for(int i=0; i<m.getNumVertices(); i++){
            m.setVertex(i, m.getVertex(i) - locAdjustmentForExport);
            m.setVertex(i, m.getVertex(i) * scale);
        }
    }
    
    void save() override{
        m.save(getMeshName());
    }
    
    void generateMesh(){
        z = ofNoise((float)(p.getCentroid2D().x * 0.001 + (ofGetFrameNum() * 0.001)), (float)(p.getCentroid2D().y * 0.001 + (ofGetFrameNum() * 0.001))) * 200. + zOffset;
        m.clear();
        m.setMode(OF_PRIMITIVE_TRIANGLES);
        glm::vec2 center = p.getCentroid2D();
        glm::vec3 center3D = glm::vec3(center.x, center.y, z);
        m.setupIndicesAuto();
        
        float scale = 1.0;
        for(int i=0; i<p.getVertices().size()-1; i++){
            m.addVertex(center3D * scale); // To center
            m.addVertex((p.getVertices()[i] + glm::vec3(0, 0, z)) * scale);
            m.addVertex((p.getVertices()[i + 1] + glm::vec3(0, 0, z)) * scale);
            
            m.addVertex((p.getVertices()[i] + glm::vec3(0, 0, z)) * scale); // Down left
            m.addVertex((p.getVertices()[i + 1] + glm::vec3(0, 0, z)) * scale);
            m.addVertex((p.getVertices()[i]) * scale); //
            
            m.addVertex((p.getVertices()[i]) * scale); //
            m.addVertex((p.getVertices()[i + 1]) * scale); //
            m.addVertex((p.getVertices()[i + 1] + glm::vec3(0, 0, z)) * scale);
        }
    }
    void prepare(){
        int longestSideId = 0; // find longest line
        int secondLongestSideId = 0;
        float longestDistance = 0.;
        for(int i=0; i<p.getVertices().size()-1; i++){
            if(glm::distance(p.getVertices()[i], p.getVertices()[i+1]) > longestDistance){
                longestDistance = glm::distance(p.getVertices()[i], p.getVertices()[i+1]);
                longestSideId = i;
            }
        }
        startSide = longestSideId;
        
        cout << "startSide: " << startSide << ", id: " << id << endl;
        glm::vec2 directionToPlaceSpawnPos = p.getVertices()[startSide+1] - p.getVertices()[startSide];
        directionToPlaceSpawnPos = glm::normalize(directionToPlaceSpawnPos);
        float distance = glm::distance(p.getVertices()[startSide+1], p.getVertices()[startSide]);
        spawnPos = p.getVertices()[startSide] + ((distance * (0.5)) * directionToPlaceSpawnPos);
        
        // Take another segment as endpoint
        if(ofRandom(8.) <= 1.){ // Take random
            endSide = ofRandom(p.getVertices().size()-1);
            while(endSide == startSide){
                endSide = ofRandom(p.getVertices().size()-1);
            }
        } else{ // Take second longest
            longestDistance = 0.;
            for(int i=0; i<p.getVertices().size()-1; i++){
                if(i == startSide)
                    continue;
                if(glm::distance(p.getVertices()[i], p.getVertices()[i+1]) > longestDistance){
                    longestDistance = glm::distance(p.getVertices()[i], p.getVertices()[i+1]);
                    secondLongestSideId = i;
                }
            }
            endSide = secondLongestSideId;
        }
        
        cout << "Endpoint segment: " << endSide << endl;
        cout << "Coordinates: " << p.getVertices()[endSide] << " " << p.getVertices()[endSide+1] << endl;
        endPoint = p.getVertices()[endSide] - (glm::normalize(p.getVertices()[endSide] - p.getVertices()[endSide+1]) * glm::distance(p.getVertices()[endSide], p.getVertices()[endSide+1]) * 0.5);
    }
    
    bool split(vector<JPoly*>* rects){
        if(bBlock)
            return false;
        // Create two new shapes!
        ofPolyline newLineOne;
        newLineOne.addVertex(spawnPos);
        newLineOne.lineTo(endPoint);
        for(int i=0; i<50; i++){
            if(i==49)
                cout << "error" << endl;
            cout << "p.getVertices().size(): " << p.getVertices().size() << endl;
            int index = (endSide - i);
            if(index < 0)
                index = p.getVertices().size() - 1 + index; // 7 - 1 - 1 = 5, so if there are 7 vertices, it skips index 6 (because double), and reads from 0 to 5
            //            % (p.getVertices().size() - 1); // Skip the last connecting vertex
            cout << "Index of shape 0: " << index << endl;
            if(index == startSide){
                newLineOne.lineTo(spawnPos); // End the poly
                break;
            }
            if(newLineOne.getVertices().back() != p.getVertices()[index]){ // Disable doubles
                newLineOne.lineTo(p.getVertices()[index]);
            } else{
                cout << "newLineOne skipping a double @ index: " << index << endl;
                cout << "newLineOne.getVertices().back(): " << newLineOne.getVertices().back() << endl;
                cout << "p.getVertices()[index]: " << p.getVertices()[index] << endl;
            }
        }
        
        ofPolyline newLineTwo;
        newLineTwo.addVertex(spawnPos);
        newLineTwo.lineTo(endPoint);
        cout << "p.getVertices().size(): " << p.getVertices().size() << endl;
        for(int i=1; i<50; i++){
            if(i==49)
                cout << "error" << endl;
            int index = (endSide + i) % (p.getVertices().size() - 1);
            cout << "Index of shape 1: " << index << endl;
            if(newLineTwo.getVertices().back() != p.getVertices()[index]){ // Disable doubles
                newLineTwo.lineTo(p.getVertices()[index]);
                cout << "Adding: " << p.getVertices()[index] << endl;
            } else{
                cout << "Skipping double" << endl;
                cout << "p.getVertices().front(): " << p.getVertices().back() << endl;
                cout << "p.getVertices()[index]: " << p.getVertices()[index] << endl;
            }
            if(index == startSide){
                newLineTwo.lineTo(spawnPos); // End the poly
                break;
            }
        }
        
        p = newLineOne;
        JPoly* newRect = new JPoly();
        newRect->p = newLineTwo;
        newRect->id = rects->size();
        rects->push_back(newRect);
        return true;
    }
};

class JDivisionGrid: public JEvent{
public:
    JDivisionGrid(glm::vec2 size = glm::vec2(1080));
    void display() override;
    void specificFunction() override;
    void setColor(ofColor c, int id) override;
    void customOne() override;
    void customTwo() override;
    void customThree() override;
    void customFour() override;
    bool bSave = false;
    
    vector<JPoly*> polyVec;
    void initRect();
    void generateRandomRects();
    void sortAndSplit();
    Env* addEnvRandomPoly(vector<float> values, vector<float> times);
    Env* addEnvSelectedPoly(vector<float> values, vector<float> times);
};
#endif /* JDivisionGrid_hpp */

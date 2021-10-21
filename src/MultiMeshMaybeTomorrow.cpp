//
//  MultiMeshMaybeTomorrow.cpp
//  Bas
//
//  Created by Jildert Viet on 23-03-16.
//
//

#include "MultiMeshMaybeTomorrow.hpp"

MultiMeshMaybeTomorrow::MultiMeshMaybeTomorrow(int num, ofFloatColor color){
    setType("MultiMeshMaybeTomorrow");
    globalColor = color;
    globalColor.a = 0;
    numMeshes = num;
//    fadeOutEnv = new Envelope(1, 1, 1000, 1);
//    fadeInEnv = new Envelope(250, 100, 1, 1);
//    bFadeIn = true;
//    fadeInEnv->trigger = true;
    
    globalAlphaAdd = 0.4;
    colorAlphas = new float[numMeshes*4]; // 4 vertices
    for(int i=0; i<4*numMeshes; i++)
        colorAlphas[i] = ofRandom(1.0)/numMeshes*2 + globalAlphaAdd;
    
    pulseOsc = new Wavetable(bpm/60, 0, 2); pulseOsc->pulseWidth = 1./8; pulseOsc->setMode(2);
    pulseMoveWidth = ofRandom(100);
    
    generateJildert();
}

void MultiMeshMaybeTomorrow::display(){
    ofSetColor(colors[0]);
    ofPushMatrix();
    ofTranslate(loc);
    for(short i=0; i<numMeshes; i++)
        meshes[i].draw();
    ofPopMatrix();
}

void MultiMeshMaybeTomorrow::specificFunction(){
    morph();
    growRadius(1, radiusAdd);
    moveOutVertex(3);
    centerPulse();
    moveHorizontal();
    moveVertical();
    moveWithPulse();
    
//    if(getNumEnv()>0){
//        for(int i=0; i<numMeshes; i++){
//            for(int j=0; j<meshes[i].getNumColors(); j++){
//                ofFloatColor color = meshes[i].getColor(j);
//                float alpha = colorAlphas[(i*4)+j] * globalColor.a;
//                color.a = alpha;
//                meshes[i].setColor(j, color);
//            }
//        }
//    }
//    fadeIn();
//    fadeOut();
}

void MultiMeshMaybeTomorrow::generateRandomMeshes(int num, ofFloatColor color){
    meshes = new ofMesh[num];
    wavetables = new Wavetable**[num];
    locations = new ofVec2f*[num];
    
    float radius = 100+ofRandom(300);
    float secRan = ofRandom(0,200);
    float thRan = ofRandom(180);
    float fRan = ofRandom(100);
    float freq = 0.342/4;
    
    for(int i=0; i<numMeshes; i++){
        wavetables[i] = new Wavetable*[4];
        locations[i] = new ofVec2f[4];
        
        meshes[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        meshes[i].enableColors();
        //------
        ofVec3f firstPoint = ofVec3f(ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0);
        firstPoint+= ofVec2f(sin(TWO_PI*(1./num*i))*radius,cos(TWO_PI*(1./num*i))*radius);
        meshes[i].addVertex(firstPoint); locations[i][0] = firstPoint;
        
        color.a = colorAlphas[(i*4)];
        meshes[i].addColor(color);
        wavetables[i][0] = new Wavetable(freq, ofRandom(0.f,TWO_PI), 0);
        //------
        ofVec2f secondPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        secondPoint*=200+secRan;
        secondPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(secondPoint.x, secondPoint.y, 0)); locations[i][1] = secondPoint;
        
        color.a = colorAlphas[(i*4)+1];
        meshes[i].addColor(color);
        wavetables[i][1] = new Wavetable(freq, ofRandom(0.f,TWO_PI), 1);
        //------
        ofVec2f thirdPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        thirdPoint.rotate(90+thRan);
        thirdPoint*=200;
        thirdPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(thirdPoint.x, thirdPoint.y, 0)); locations[i][2] = thirdPoint;
        
        color.a = colorAlphas[(i*4)+2];
        meshes[i].addColor(color);
        wavetables[i][2] = new Wavetable(freq, ofRandom(0.f,TWO_PI), 1);
        //------
        ofVec2f fourthPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        fourthPoint.rotate(10);
        fourthPoint*=100+fRan;
        fourthPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(fourthPoint.x, fourthPoint.y, 0)); locations[i][3] = fourthPoint;
        
        color.a = colorAlphas[(i*4)+3];
        meshes[i].addColor(color);
        wavetables[i][3] = new Wavetable(freq, ofRandom(0.f,TWO_PI), 0);
    }
}

void MultiMeshMaybeTomorrow::generateSymmetricMeshes(int num, ofFloatColor color, float radiusTemp){
    meshes = new ofMesh[num];
    wavetables = new Wavetable**[num];
    locations = new ofVec2f*[num];
    
    
    radius = (0.25*radiusTemp)+ofRandom((0.75*radiusTemp));
    float secRan = ofRandom(0,200);
    float thRan = ofRandom(90);
    float fRan = ofRandom(100);
    float freq = bpm/60/8;
    int waveTableMode = 4;
    
    for(int i=0; i<numMeshes; i++){
        wavetables[i] = new Wavetable*[4];
        locations[i] = new ofVec2f[4];
        
        meshes[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        meshes[i].enableColors();
        //------
        ofVec2f firstPoint = ofGetWindowSize()/2.;
        firstPoint+= ofVec2f(sin(TWO_PI*(1./num*i))*radius,cos(TWO_PI*(1./num*i))*radius);
        meshes[i].addVertex(glm::vec3(firstPoint.x, firstPoint.y, 0)); locations[i][0] = firstPoint;
        color.a = colorAlphas[i*4];
        meshes[i].addColor(color);
        wavetables[i][0] = new Wavetable(freq, TWO_PI*(0.5*((int)ofRandom(0,2))), waveTableMode);
        //------
        ofVec2f secondPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        secondPoint*=secRan;
        secondPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(secondPoint.x, secondPoint.y, 0)); locations[i][1] = secondPoint;
        
        color.a = colorAlphas[(i*4)+1];
        meshes[i].addColor(color);
        wavetables[i][1] = new Wavetable(freq, TWO_PI*(0.5*((int)ofRandom(0,1))), waveTableMode);
        //------
        ofVec2f thirdPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        thirdPoint.rotate(thRan);
        thirdPoint*=100;
        thirdPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(thirdPoint.x, thirdPoint.y, 0)); locations[i][2] = thirdPoint;
        
        color.a = colorAlphas[(i*4)+2];
        meshes[i].addColor(color);
        wavetables[i][2] = new Wavetable(freq*2, TWO_PI*(0.5*((int)ofRandom(0,2))), waveTableMode);
        //------
        ofVec2f fourthPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        fourthPoint.rotate(10);
        fourthPoint*=100+fRan;
        fourthPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(fourthPoint.x, fourthPoint.y, 0)); locations[i][3] = fourthPoint;
        
        color.a = colorAlphas[(i*4)+3];
        meshes[i].addColor(color);
        wavetables[i][3] = new Wavetable(freq, TWO_PI*(0.5*((int)ofRandom(0,1))), waveTableMode);
    }
}

void MultiMeshMaybeTomorrow::changeMode(ofPrimitiveMode mode){
    for(int i=0; i<numMeshes; i++){
        meshes[i].setMode(mode);
    }
}

void MultiMeshMaybeTomorrow::morph(){
    if(bMorph && ofGetElapsedTimeMillis() < morphTime){
        for(int meshNum=0; meshNum<numMeshes; meshNum++){
            for(int numVertices=0; numVertices<meshes[meshNum].getNumVertices(); numVertices++){
                //                ofVec2f vertex = meshes[meshNum].getVertex(numVertices);
                ofVec2f vertex = locations[meshNum][numVertices];
                vertex -= (ofGetWindowSize()/2.);
                vertex = vertex.rotate(morphAngle);
                vertex += (ofGetWindowSize()/2.);
                //                meshes[meshNum].setVertex(numVertices, vertex);
                locations[meshNum][numVertices]=vertex;
            }
        }
    } else{
        bMorph = false;
    }
}

void MultiMeshMaybeTomorrow::doMorph(float angle, float time){
    bMorph = true;
    int direction = (round(ofRandomuf())*2)-1;
    morphAngle = angle*direction; morphTime = ofGetElapsedTimeMillis()+time;
}

//void MultiMeshMaybeTomorrow::fadeOut(){
//    if(bFadeOut){
//        float envValue = fadeOutEnv->process();
//        for(int i=0; i<numMeshes; i++){
//            for(int j=0; j<meshes[i].getNumColors(); j++){
//                ofFloatColor color = meshes[i].getColor(j);
//                float alpha = colorAlphas[(i*4)+j] * envValue;
//                color.a = alpha;
//                meshes[i].setColor(j, color);
//            }
//        }
//        
//        if(fadeOutEnv->done){
//            endTime = 0; active = true; bFadeOut = false;
//        }
//    }
//}

void MultiMeshMaybeTomorrow::deleteWithFade(int releaseTime, int attackTime){
    bFadeOut = true;
    addEnv(vector<float>{0, 1.0, 0}, vector<float>{(float)attackTime, (float)releaseTime}, &globalColor.a);
    setEndTime(releaseTime + attackTime);
    active = true;
}

//void MultiMeshMaybeTomorrow::fadeIn(){
//    if(bFadeIn){
//        float envValue = fadeInEnv->process();
//        for(int i=0; i<numMeshes; i++){
//            for(int j=0; j<meshes[i].getNumColors(); j++){
//                ofFloatColor color = meshes[i].getColor(j);
//                float alpha = colorAlphas[(i*4)+j] * envValue;
//                color.a = alpha;
//                meshes[i].setColor(j, color);
//            }
//        }
//        
//        if(fadeInEnv->sustainBool){
//            bFadeIn = false;
//        }
//    }
//}

void MultiMeshMaybeTomorrow::addRandomMesh(float radiusTemp, ofFloatColor color){
    float* tempColorArray = new float[numMeshes*4];
    ofMesh* tempMeshArray = new ofMesh[numMeshes];
    Wavetable*** tempWavetablesArray = new Wavetable**[numMeshes];
    ofVec2f** locationsTemp = new ofVec2f*[numMeshes];
    
    for(int i=0; i<numMeshes; i++){
        tempMeshArray[i] = meshes[i];
        tempWavetablesArray[i] = wavetables[i];
        locationsTemp[i] = locations[i];
    }
    for(int i=0; i<numMeshes*4; i++)
        tempColorArray[i] = colorAlphas[i];
    
    numMeshes++;
    meshes = new ofMesh[numMeshes];
    wavetables = new Wavetable**[numMeshes];
    colorAlphas = new float[(numMeshes)*4]; // 4 vertices
    locations = new ofVec2f*[numMeshes];
    
    for(int i=0; i<numMeshes-1; i++){
        meshes[i] = tempMeshArray[i];
        wavetables[i] = tempWavetablesArray[i];
        locations[i] = locationsTemp[i];
    }
    for(int i=0; i<(numMeshes-1)*4; i++)
        colorAlphas[i] = tempColorArray[i];
    
    delete[] tempMeshArray;
    delete[] tempWavetablesArray;
    delete[] tempColorArray;
    delete[] locationsTemp;
    
    float radius = (0.25*radiusTemp)+ofRandom((0.75*radiusTemp));
    float secRan = ofRandom(0,200);
    float thRan = ofRandom(90);
    float fRan = ofRandom(100);
    float freq = bpm/60./16.+ofRandom(0.01);;
    
    for(int i=numMeshes-1; i<numMeshes; i++){
        float randomSeed = ofRandom(TWO_PI);
        for(int j=(numMeshes-1)*4; j<4+((numMeshes-1)*4); j++) // Tricky
            colorAlphas[j] = ofRandom(1.0)/numMeshes*2;
        
        wavetables[i] = new Wavetable*[4];
        locations[i] = new ofVec2f[4];
        
        meshes[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        meshes[i].enableColors();
        
        ofVec2f firstPoint = ofGetWindowSize()/2.;
        firstPoint+= ofVec2f(sin(TWO_PI*(1./numMeshes+randomSeed))*radius,cos(TWO_PI*(1./numMeshes+randomSeed))*radius);
        meshes[i].addVertex(glm::vec3(firstPoint.x, firstPoint.y, 0)); locations[i][0] = firstPoint;
        
        color.a = colorAlphas[i*4];
        meshes[i].addColor(color);
        wavetables[i][0] = new Wavetable(freq, TWO_PI*(0.5*((int)ofRandom(0,2))), 0);
        
        ofVec2f secondPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        secondPoint*=secRan;
        secondPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(secondPoint.x, secondPoint.y, 0)); locations[i][1] = secondPoint;
        
        color.a = colorAlphas[(i*4)+1];
        meshes[i].addColor(color);
        wavetables[i][1] = new Wavetable(freq, TWO_PI*(0.5*((int)ofRandom(0,2))), 0);
        
        ofVec2f thirdPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        thirdPoint.rotate(thRan);
        thirdPoint*=100;
        thirdPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(thirdPoint.x, thirdPoint.y, 0)); locations[i][2] = thirdPoint;
        
        color.a = colorAlphas[(i*4)+2];
        meshes[i].addColor(color);
        wavetables[i][2] = new Wavetable(freq*2, TWO_PI*(0.5*((int)ofRandom(0,1))), 0);
        
        ofVec2f fourthPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        fourthPoint.rotate(10);
        fourthPoint*=100+fRan;
        fourthPoint+=ofGetWindowSize()/2.;
        meshes[i].addVertex(glm::vec3(fourthPoint.x, fourthPoint.y, 0)); locations[i][3] = fourthPoint;
        
        color.a = colorAlphas[(i*4)+3];
        meshes[i].addColor(color);
        wavetables[i][3] = new Wavetable(freq, TWO_PI*(0.5*((int)ofRandom(0,1))), 0);
    }
}

void MultiMeshMaybeTomorrow::doubleTime(){
    for(int i=0; i<numMeshes; i++){
        for(int j=0; j<meshes[i].getNumVertices(); j++){
            wavetables[i][j]->freq = wavetables[i][j]->freq*2;
            wavetables[i][j]->retrigger();
        }
    }
}

void MultiMeshMaybeTomorrow::halfTime(){
    for(int i=0; i<numMeshes; i++){
        for(int j=0; j<meshes[i].getNumVertices(); j++){
            wavetables[i][j]->freq = wavetables[i][j]->freq*0.5;
        }
    }
}

void MultiMeshMaybeTomorrow::changeColor(ofFloatColor color){
    for(int i=0; i<numMeshes; i++){
        for(int j=0; j<meshes[i].getNumVertices(); j++){
            meshes[i].setColor(j, color);
        }
    }
}

void MultiMeshMaybeTomorrow::addRadius(float radiusIncrease){
    for(int i=0; i<numMeshes; i++){
        for(int j=0; j<meshes[i].getNumVertices(); j++){
            ofVec2f newLoc = meshes[i].getVertex(j);
            newLoc-=ofGetWindowSize()/2.;
            ofVec2f copy = newLoc;
            newLoc = (copy.normalize()*radiusIncrease);
            //            newLoc+=ofGetWindowSize()/2.;
            //            meshes[i].setVertex(j, newLoc);
            locations[i][j] += newLoc;
        }
    }
}

void MultiMeshMaybeTomorrow::growRadius(int direction, float speed){
    if(bGrowRadius)
        addRadius(direction*speed);
}

void MultiMeshMaybeTomorrow::centerPulse(){
//    if(bCenterPulse){
//        for(int i=0; i<numMeshes; i++){
//            for(int j=0; j<meshes[i].getNumVertices(); j++){
//                ofVec2f newLoc = locations[i][j];
//                newLoc-=ofGetWindowSize()/2.;
//                newLoc.normalize();
//                float waveTableVal = (wavetables[i][j]->process()+1);
//                newLoc*=(waveTableVal*centerPulseRadius);
//                newLoc+=locations[i][j];
//                //            newLoc+=ofGetWindowSize()/2.;
//                meshes[i].setVertex(j, newLoc);
//            }
//        }
//    }
}


void MultiMeshMaybeTomorrow::setFrequency(float freq, float* multipliers, int size){
    for(int i=0; i<numMeshes; i++){
        float multiplier = multipliers[(int)ofRandom(size)];
        for(int j=0; j<meshes[i].getNumVertices(); j++){
            wavetables[i][j]->freq = freq*multiplier;
        }
    }
    pulseOsc->freq = freq * multipliers[(int)ofRandom(size)];
}

void MultiMeshMaybeTomorrow::changeLocations(){
    for(int i=0; i<numMeshes; i++){
        
        radius = (0.25*350)+ofRandom((0.75*350));
        float secRan = ofRandom(0,200);
        float thRan = ofRandom(90);
        float fRan = ofRandom(100);
        
        ofVec2f firstPoint = ofGetWindowSize()/2.;
        firstPoint+= ofVec2f(sin(TWO_PI*(1./numMeshes*i))*radius,cos(TWO_PI*(1./numMeshes*i))*radius);
        locations[i][0] = firstPoint; meshes[i].setVertex(0, glm::vec3(firstPoint.x, firstPoint.y, 0));
        //------
        ofVec2f secondPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        secondPoint*=secRan;
        secondPoint+=ofGetWindowSize()/2.;
        locations[i][1] = secondPoint; meshes[i].setVertex(1, glm::vec3(secondPoint.x, secondPoint.y, 0));
        //------
        ofVec2f thirdPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        thirdPoint.rotate(thRan);
        thirdPoint*=100;
        thirdPoint+=ofGetWindowSize()/2.;
        locations[i][2] = thirdPoint; meshes[i].setVertex(2, glm::vec3(thirdPoint.x, thirdPoint.y, 0));
        
        //------
        ofVec2f fourthPoint = (firstPoint-(ofGetWindowSize()/2.)).normalize();
        fourthPoint.rotate(10);
        fourthPoint*=100+fRan;
        fourthPoint+=ofGetWindowSize()/2.;
        locations[i][3] = fourthPoint; meshes[i].setVertex(3, glm::vec3(fourthPoint.x, fourthPoint.y, 0));
    }
}

void MultiMeshMaybeTomorrow::moveWithPulse(){
    if(bMoveWithPulse){
        for(int i=0; i<numMeshes; i++){
            for(int j=0; j<meshes[i].getNumVertices(); j++){
                ofVec2f toAdd = locations[i][j];
                toAdd-=ofGetWindowSize()/2.;
                toAdd.normalize();
                toAdd*=pulseMoveWidth;
                toAdd*=pulseOsc->process();
                meshes[i].setVertex(j, meshes[i].getVertex(j)+glm::vec3(toAdd.x, toAdd.y, 0));
            }
        }
    }
}

void MultiMeshMaybeTomorrow::moveOutVertex(int index){
    if(bMoveOutVertext){
        for(int i=0; i<numMeshes; i++){
            ofVec2f loc = locations[i][index];
            ofVec2f direction = (loc-ofGetWindowSize()/2.).normalize();
            locations[i][index] = loc + direction;
        }
    }
}






void MultiMeshMaybeTomorrow::generateJildert(){
    int num = numMeshes;
    ofFloatColor color = ofFloatColor(1.0,0.0,0.2, 0.9);
    
    meshes = new ofMesh[numMeshes];
    wavetables = new Wavetable**[numMeshes];
    locations = new ofVec2f*[numMeshes];
    
    for(int i=0; i<numMeshes; i++){
        int numVertices = 3;
        wavetables[i] = new Wavetable*[numVertices];
        locations[i] = new ofVec2f[numVertices];
        
        meshes[i].setMode(OF_PRIMITIVE_LINE_STRIP);
//        meshes[i].enableColors();
        
        ofVec2f startPoint = ofVec2f(ofRandomWidth(), ofRandomHeight());
        meshes[i].addVertex(glm::vec3(startPoint.x, startPoint.y, 0));
//        meshes[i].addColor(ofFloatColor(1));
        locations[i][0] = startPoint;
        
        
        int seed = ofRandom(1000);
        for(int j=1; j<numVertices; j++){ // Goeie j?
            
            
            float x = j;
            float noiseVal = ofNoise(x*PI+seed);
            //            cout << noiseVal << endl;
            int index=0;
            if(i%2==0){
                index = ofMap(noiseVal, 0., 1., 6, 0);
            } else{
                index = ofMap(noiseVal, 0., 1., 0, 6);
            }

            int increaseMultiplier = ofMap(noiseVal, 0., 1., 1, 4);
            int increase = 100 * increaseMultiplier;
            ofVec2f add;
            switch(index){
                case 1:
                    add = ofVec2f(0,increase);
                    break;
                case 2:
                    add = ofVec2f(increase,0);
                    break;
                case 3:
                    add = ofVec2f(-increase,0);
                    break;
                case 4:
                    add = ofVec2f(0,-increase);
                    break;
            }
            ofVec2f p = locations[i][j-1] + add;
            
            meshes[i].addVertex(glm::vec3(p.x, p.y, 0));
//            meshes[i].addColor(ofFloatColor(1, ofRandom(1.0)));
            locations[i][j] = p;
        }
        for(int j=0; j<numVertices; j++){
            //            if(wavetables[i][j]==nullptr){
            wavetables[i][j] = new Wavetable(ofRandom(0.01, 1.), ofRandom(TWO_PI), 2);
            wavetables[i][j]->bActive = false;
            //            }
        }
        bCenterPulse = false;
        bFadeIn = false;
    }
}


void MultiMeshMaybeTomorrow::moveHorizontal(){
    if(bMoveHorizontal){
        cout<<1<<endl;
        int add = 100;
        int direction = ofRandom(2);
        switch (direction) {
            case 0:
                direction = 1;
                break;
            case 1:
                direction = -1;
                break;
            default:
                break;
        }
        for(int i=0; i<numMeshes; i++){
            cout << i << endl;
            for(int j=0; j<meshes[i].getNumVertices(); j++){
                cout << j << endl;
                meshes[i].setVertex(j, meshes[i].getVertex(j)+glm::vec3(add*direction,0, 0));
            }
        }
    }
    bMoveHorizontal = false;
}

//bool MultiMeshMaybeTomorrow::checkIfNextVertextHasSameX(ofMesh* vertex, int index){
//    int add = 100;
//    if(vertex->getVertex(index).x == vertex->getVertex(index+1).x){
//        checkIfNextVertextHasSameX(vertex, index+1);
//        vertex->setVertex(index, vertex->getVertex(index)+ofVec2f(add,0));
//    }
//}

void MultiMeshMaybeTomorrow::moveVertical(){
    if(bMove){
        for(int i=0; i<numMeshes; i++){
//            cout << i << endl;
            for(int j=0; j<meshes[i].getNumVertices(); j++){
//                cout << j << endl;
                meshes[i].setVertex(j, meshes[i].getVertex(j)+direction);
            }
        }
    }
}

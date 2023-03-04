//
//  SpaceCube.cpp
//  Shapes_met_Osc
//
//  Created by Jildert Viet on 04-05-15.
//
//

#include "SpaceCube.h"

SpaceCube::SpaceCube(ofVec3f loc_, ofVec3f size_, int points_amount_, int gridsize){
    setType("SpaceCube");
    setMode(1);
    
    size = size_;
    pointsAmount = points_amount_;
    loc = loc_;
    this->gridsize = gridsize;
    
    offset = ofVec3f(ofRandom(-size.x, size.x),ofRandom(-size.y, size.y),ofRandom(-size.z, size.z));
    offset *= 0.2;
    
    color.a = 25;
    
    for(int i=0; i<points_amount_; i++)
        placeNewPoint(ofVec3f(), true);
        
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    speed = 2.5 + (0.25*(int)ofRandom(1,4));
    
    randomChooseRotXorRotY();

//    addEnv(vector<float>{0, 0.1}, vector<float>{4000}, &(color.a));
    
    ofSetLineWidth(lineWidth);
}

void SpaceCube::ownDtor(){ removeFromVector(); }


void SpaceCube::customOne(float* v){
    bool bAdd = busses[0];
    int num = busses[1];
    for(int i=0; i<num; i++){
        if(bAdd){
//            if(mesh.getNumVertices() < 7)
            placeNewPoint(ofVec3f(), true);
        } else{
            if(mesh.getNumVertices() > 3)
                removeLastVertex();
        }
    }
}

void SpaceCube::customTwo(float* v){
    randomChooseRotXorRotY();
}

void SpaceCube::customThree(float* v){
    blink();
}

void SpaceCube::customFour(float* v){
    draw_line = true;
}

void SpaceCube::customFive(float* v){ // Resize
    size.x = busses[0];
    size.y = busses[1];
    pointsAmount = busses[2];
    gridsize = busses[3];
    mesh.clear();
    for(int i=0; i<pointsAmount; i++)
        placeNewPoint(ofVec3f(), true);
}

void SpaceCube::display(){
    ofPushMatrix();
    glm::vec3 translate = glm::vec3(ofGetWindowWidth()/2., ofGetWindowHeight()/2., 0) + offset + loc;
    ofTranslate(translate);
    rotateY();
    rotateX();

    ofSetColor(color);
    mesh.draw();
    
    if(draw_line){
        lightLine(0, true);
        lightLine(0, true);
        lightLine(0, true);
    }
    
    ofTranslate(-translate);
    ofPopMatrix();
}

void SpaceCube::rotateX(){
    if(rotX){
        ofRotateXDeg(x_angle*rotation_direction);
        x_angle += (speed/10.);
    }
}


void SpaceCube::rotateY(){
    if(rotY){
        ofRotateYDeg(y_angle*rotation_direction);
        y_angle += (speed/10.);
    }
}

void SpaceCube::specificFunction(){
    if(fillMeshOneFrameFlag)
        fillMeshOneFrame(20, true);
    if(convert_to_sphere)
        convertToSphere(radius);
}



void SpaceCube::placeNewPoint(ofVec3f loc, bool random){
    glm::vec3 loc_temp;
    if(random){
        loc_temp = ofVec3f((((int)ofRandom(gridsize))*size.x/gridsize), (((int)ofRandom(gridsize))*size.y/gridsize), (((int)ofRandom(gridsize))*size.z/gridsize));
        loc_temp -= (size/2.);
    } else{
        loc_temp = loc;
    }
    
    switch (mesh.getNumVertices()) {
        case 0:
            mesh.addVertex(loc_temp);
            break;
            
        case 1:
            mesh.addVertex(loc_temp);
            break;
            
        case 2:
        {
            ofVec3f first, second;
            first = mesh.getVertex(0);
            second = mesh.getVertex(1);
            
            mesh.addVertex(loc_temp);
            mesh.addVertex(first);
            mesh.addVertex(loc_temp);
            mesh.addVertex(second);
        }
            break;
            
        case 6:
        {
            ofVec3f first, second, third;
            first = mesh.getVertex(0);
            second = mesh.getVertex(1);
            third = mesh.getVertex(2);
            
            mesh.addVertex(loc_temp);
            mesh.addVertex(first);
            mesh.addVertex(loc_temp);
            mesh.addVertex(second);
            mesh.addVertex(loc_temp);
            mesh.addVertex(third);
        }
            break;
            
        default:
            // Find three closest, connect
        {   // Lekker 3 for-loops, waarom ook niet
            ofVec3f first, second, third;
            int firstIndex, secondIndex, thirdIndex;
            firstIndex = 0;
            secondIndex = 0;
            thirdIndex = 0;
            float distance = 999999999999;
            for(int i=0; i<mesh.getNumVertices(); i++){
//                float d = mesh.getVertex(i).distance(loc_temp);
                float d = glm::distance(mesh.getVertex(i), loc_temp);
                if(d != 0 && d < distance){
                    distance = d;
                    first = mesh.getVertex(i);
                    firstIndex = i;
                }
            }
            distance = 999999999999;
            for(int i=0; i<mesh.getNumVertices(); i++){
//                float d = mesh.getVertex(i).distance(loc_temp);
                float d = glm::distance(mesh.getVertex(i), loc_temp);
                if(d != 0 && d < distance && i != firstIndex && mesh.getVertex(i) != mesh.getVertex(firstIndex)){
                    distance = d;
                    first = mesh.getVertex(i);
                    secondIndex = i;
                }
            }
            distance = 999999999999;
            for(int i=0; i<mesh.getNumVertices(); i++){
//                float d = mesh.getVertex(i).distance(loc_temp);
                float d = glm::distance(mesh.getVertex(i), loc_temp);
                if(d != 0 && d < distance && i != firstIndex && i != secondIndex && mesh.getVertex(i) != mesh.getVertex(firstIndex) && mesh.getVertex(i) != mesh.getVertex(secondIndex)){
                    distance = d;
                    first = mesh.getVertex(i);
                    thirdIndex = i;
                }
            }
            first = mesh.getVertex(firstIndex);
            second = mesh.getVertex(secondIndex);
            third = mesh.getVertex(thirdIndex);
            
            mesh.addVertex(loc_temp);
            mesh.addVertex(first);
            mesh.addVertex(loc_temp);
            mesh.addVertex(second);
            mesh.addVertex(loc_temp);
            mesh.addVertex(third);
        }
            break;
    }
}

void SpaceCube::setMode(char m){
    ofPrimitiveMode modes[3] = {OF_PRIMITIVE_LINES, OF_PRIMITIVE_TRIANGLE_FAN, OF_PRIMITIVE_POINTS};
    mesh.setMode(modes[m]);
    mode = m;
}

void SpaceCube::blink(){
    filled = false;
    fillMeshOneFrameFlag = true;
}

void SpaceCube::rotateRotationSpeed(){
    rotation_speeds_iterator += (int)ofRandom(1,4);
    if(rotation_speeds_iterator >= 4){
        rotation_speeds_iterator = 0;
    }
    speed = rotation_speeds[rotation_speeds_iterator];
}

void SpaceCube::fillMeshOneFrame(int modulo, bool on_cue){
    if(on_cue){
        if(filled == false){
            setMode(1);
            filled = true;
        } else{
            setMode(0);
            fillMeshOneFrameFlag = false;
        }
    } else {
        if(ofGetFrameNum() % modulo == 0){
           setMode(1);
        } else{
            setMode(0);
        }
    }

}

void SpaceCube::removeLastVertex(){
    if(mesh.getNumVertices()>6){
        for(int i=0; i<6; i++){
            mesh.removeVertex(mesh.getNumVertices()-1);
        }
    }
}

void SpaceCube::lightLine(int index, bool random){
    int index1;
    int index2;
    
    if(random){
        index1 = ofRandom(mesh.getNumVertices());
    } else{
        index1 = index;
    }
    index2 = (index1+1) % mesh.getNumVertices();
    ofSetColor(255,100);
    ofSetLineWidth(lineWidth);
    ofDrawLine(mesh.getVertex(index1), mesh.getVertex(index2));
    draw_line = false;
}

void SpaceCube::getBigger(int size_){
    for(int i=0; i<size_; i++){
        placeNewPoint(ofVec3f(0,0,0), true);
    }
}

void SpaceCube::getSmaller(int size_){
    for(int i=0; i<size_; i++){
        removeLastVertex();
    }
}

void SpaceCube::randomChooseRotXorRotY(){
    if(ofRandom(-1, 1) > 0){
        rotY = true;
    } else{
        rotY = true;
    }
    if(ofRandom(-1, 1) > 0){
        rotX = true;
    } else{
        rotX = false;
        if(rotY == false){
            rotY = true;
        }
    }
}

void SpaceCube::placeVertexOnRadius(float radius){
    // Choose random vector ->norm() -> * radius, placepoint
    ofVec3f randomDirection = ofVec3f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));
    randomDirection.normalize();
    randomDirection *= radius;
    ofVec3f centerCube = ofVec3f(size.x/2., size.y/2., size.z/2.);
    placeNewPoint(centerCube+randomDirection, false);
}

void SpaceCube::convertToSphere(float radius){
    if(is_sphere)
        return;
    convert_to_sphere = false;
    mesh.removeVertex(0);
    placeVertexOnRadius(radius);
    
    num_vertices_to_sphere ++;
    if(num_vertices_to_sphere == mesh.getNumVertices())
        is_sphere = true;
}

void SpaceCube::addVector(vector<SpaceCube*>* v){
    this->v = v;
}

void SpaceCube::removeFromVector(){
    if(v){
        for(int i=0; i<v->size(); i++){
            if(v->at(i) == this){
                v->at(i) = nullptr; // Later remove the nullptrs in vec
                cout << "Found self!" << endl;
            }
        }
//        cout << "Num cubes before deleting: " << v->size() << endl;
        v->erase( std::remove_if( v->begin(), v->end(), SpaceCube::checkIfNull ), v->end() );
//        cout << "Num cubes after deleting: " << v->size() << endl;
    }
}

bool SpaceCube::checkIfNull(SpaceCube* s){
    if(!s){
        return true;
    } else{
        return false;
    }
}

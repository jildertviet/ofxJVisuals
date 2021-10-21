//
//  SpaceCube.h
//  Shapes_met_Osc
//
//  Created by Jildert Viet on 04-05-15.
//
//

#ifndef __Shapes_met_Osc__SpaceCube__
#define __Shapes_met_Osc__SpaceCube__

#include <iostream>
#include "Event.hpp"

class SpaceCube: public Event{
public:
    SpaceCube(ofVec3f loc=ofVec3f(0, 0 ,0), ofVec3f size = ofVec3f(400,400,400), int points_amount = 5, int gridsize = 6);
    void ownDtor() override;
    
    void placeNewPoint(ofVec3f loc, bool random);
    ofMesh mesh;
    
    void specificFunction() override;
    void display() override;
    
    ofVec3f offset;
        
    int rotation_direction = 1;
    float rotation_speed;
    int gridsize;
    void setMode(char m) override;
    char mode = 0;

    float rotation_speeds[4] = {2.5, 3.75, 5.0, 7.5};
    int rotation_speeds_iterator = 0;
    void rotateRotationSpeed();
    void fillMeshOneFrame(int modulo, bool on_cue);
    bool filled = false;
    float x_angle = 0;
    float y_angle = 0;
    void removeLastVertex();
    void lightLine(int index, bool random);
    bool draw_line = false;
    void getBigger(int size_);
    void getSmaller(int size_);
    int prev_type;
    bool fillMeshOneFrameFlag = false;
    void blink();
    bool rotX = false;
    bool rotY = false;
    void randomChooseRotXorRotY();
//    void setColor(ofFloatColor color);
    
    void placeVertexOnRadius(float radius);
    void convertToSphere(float radius);
    bool convert_to_sphere;
    bool is_sphere = false;
    int num_vertices_to_sphere = 0;
    float radius;
    
    void addVector(vector<SpaceCube*>* v);
    vector<SpaceCube*>* v = nullptr;
    void removeFromVector();
    static bool checkIfNull(SpaceCube* s);
    
    int lineWidth = 3;
    
    void customOne() override;
    void customTwo() override;
    void customThree() override;
    void customFour() override;
    void customFive() override;
    int pointsAmount = 0;
private:
    void rotateY();
    void rotateX();

};
#endif /* defined(__Shapes_met_Osc__SpaceCube__) */

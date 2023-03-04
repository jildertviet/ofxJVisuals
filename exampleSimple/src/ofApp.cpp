#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    // int a = 67109896;
    int b = 67109895;
    // int c = 67109894;
    // int d = 67109893;
    // int e = 67109892;
    // float aF = a;
    // float bF = b;
    // float cF = c;
    // float dF = d;
    // float eF = e;
    // cout << aF << ", " << bF << ", " << cF << ", " << dF << ", " << eF << endl;
    // int aI = aF;
    // int bI = bF;
    // int cI = cF;
    // int dI = dF;
    // int eI = eF;
    // cout << aI << ", " << bI << ", " << cI << ", " << dI << ", " << eI << endl;
    // float x;
    // memcpy(&x, &b, 4);
    // cout << x << endl;
    // int y;
    // memcpy(&y, &x, 4);
    // cout << y << endl;
    //
    // int d = 1000;
    // float z;
    // memcpy(&z, &d, 4);
    // cout << z << endl;
    // std::exit(0);
    ofSetWindowShape(1080, 1080);
    bool bSC = true;
    visualizer = new ofxJVisuals(vec2(1080), bSC); // Don't use SC
}

//--------------------------------------------------------------
void ofApp::update(){
#ifndef TARGET_RASPBERRY_PI
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
#else
//    cout << ofGetFrameRate() << endl;
#endif
    visualizer->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    visualizer->display();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    visualizer->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1080, 1080);
    bool bSC = true;
    visualizer = new ofxJVisuals(vec2(1080), bSC); // Don't use SC
    visualizer->addEvent((JEvent*)new JRectangle(glm::vec2(100,100), glm::vec2(100, 100)));
    visualizer->last->color = ofColor(255, 0, 0, 100);
    visualizer->addEvent((JEvent*)new JRectangle(glm::vec2(150,100), glm::vec2(100, 100)));
    visualizer->last->color = ofColor(0, 0, 255, 100);

    visualizer->shaders.push_back(new JShader());
    visualizer->shaders.back()->load("lines");
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

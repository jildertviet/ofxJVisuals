#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
    visualizer = new ofxJVisuals(glm::vec2(1920, 1080));
    
//    visualizer->addEvent((Event*)new JPhysarum(glm::vec2((1920-1080)/2, 0), glm::vec2(1080, 1080)), NON_CAM_FRONT);
//    visualizer->getLast()->setColor(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::update(){
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

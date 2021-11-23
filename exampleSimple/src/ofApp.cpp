#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
    visualizer = new ofxJVisuals(glm::vec2(4096, 2160));
    
//    visualizer->addEvent((Event*)new JPhysarum(glm::vec2((1920-1024)/2, 0), glm::vec2(1024, 1024)), NON_CAM_FRONT);
//    visualizer->getLast()->setColor(ofColor::white);
    
    f.allocate(4096, 2160, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
    f.begin();
    visualizer->update();
    visualizer->display();
    f.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    f.draw(0,0, ofGetWidth(), ofGetHeight());
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

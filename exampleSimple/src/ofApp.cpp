#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
//    visualizer = new ofxJVisuals(glm::vec2(1920, 1080));
//    auto e = JPhysarum(glm::vec2(0), glm::vec2(100));
}

//--------------------------------------------------------------
void ofApp::update(){
#ifndef TARGET_RASPBERRY_PI
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
#else
//    cout << ofGetFrameRate() << endl;
#endif
    visualizer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    visualizer.display();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    visualizer.keyPressed(key);
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

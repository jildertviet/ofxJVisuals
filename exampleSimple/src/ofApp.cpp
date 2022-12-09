#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1080, 1080);
    visualizer = new ofxJVisuals(glm::vec2(1080), true); // Don't use SC
    // auto i = new JImage("test_0.svg");
    // i->setSize(glm::vec3(ofGetWidth()*0.5, 100,0));
    // std::exit(0);
   // auto r = new JRectangle(vec3(100, 100, 0), vec3(100, 100, 0));
   // visualizer.addEvent(i, VisualizerLayer::DEFAULT, 1);
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

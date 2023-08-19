#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1000, 280);
    bool bSC = true;
    visualizer = new ofxJVisuals(vec2(1000, 280), bSC); // Don't use SC
    // visualizer->addEvent((JEvent*)new JPhysarum(glm::vec2(0,0), glm::vec2(512,512)));
    // visualizer->addEvent((JEvent*)new JRectangle(glm::vec2(100,100), glm::vec2(100, 100)));
    // visualizer->last->color = ofColor(255, 0, 0, 255);
    // visualizer->addEvent((JEvent*)new JRectangle(glm::vec2(200,100), glm::vec2(100, 100)));
    // visualizer->last->color = ofColor(0, 255, 0, 255);

    // visualizer->shaders.push_back(new JShader());
    // visualizer->shaders.back()->load("lines");
    // s.load("../../../../../addons/ofxJVisuals/libs/shaders/JPhysarum/posUpdate");
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
  // visualizer->last->displayMain();
  // s.begin();
  // ofSetColor(255);
  // ofDrawRectangle(0,0,100,100);
  // s.end();
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

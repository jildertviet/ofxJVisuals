#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1080, 1080);
    bool bSC = false;
    visualizer = new ofxJVisuals(glm::vec2(1080), bSC); // Don't use SC

    auto i = new JRectangle(glm::vec2(0,0), glm::vec2(100, 100));
    i->setColor(ofColor::red);
    visualizer->addEvent(i, VisualizerLayer::DEFAULT, 1);

    i = new JRectangle(glm::vec2(50,0), glm::vec2(100, 100));
    i->setColor(ofColor::blue);
    visualizer->addEvent(i, VisualizerLayer::DEFAULT, 2);
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

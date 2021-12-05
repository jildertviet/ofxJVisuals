#include "ofApp.h"

ofImage exportImage;
bool bExport = false;
double startNum = 0;
ofPixels p;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(1920, 1080);
    visualizer = new ofxJVisuals(glm::vec2(3840, 2160));

//    visualizer->addEvent((Event*)new JPhysarum(glm::vec2((1920-1024)/2, 0), glm::vec2(1024, 1024)), NON_CAM_FRONT);
//    visualizer->getLast()->setColor(ofColor::white);

    ofFboSettings settings;
    settings.width = 3840;
    settings.height = 2160;
    settings.useStencil = true;
    settings.internalformat = GL_RGB32F;
    settings.numSamples = 8;
    f.allocate(settings);

//    f.allocate(1920, 1080, GL_RGB);
    p.allocate(settings.width, settings.height, 3);

    exportImage.allocate(settings.width, settings.height, OF_IMAGE_COLOR);
//    ofSetFrameRate(3);

//    f.allocate(1920, 1080, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    f.begin();
    visualizer->update();
    visualizer->display();
    f.end();



//    if(f >= (60*40))
//        std::exit(0);
}

//--------------------------------------------------------------
void ofApp::draw(){
    f.draw(0,0, ofGetWidth(), ofGetHeight()); // ?


    if(bExport){
        float fNum = ofGetFrameNum();

        string zeroes = "00000000";
        zeroes = zeroes.substr(0, zeroes.length()-(ofToString(fNum).length()));

        string fileName = "f" + zeroes + ofToString(fNum) + ".tiff";

        p.clear();
        f.readToPixels(p);
        exportImage.setFromPixels(p);
        exportImage.save(fileName);
//        ofSaveFrame();
//        ofSaveScreen("test.tiff");
        if(ofGetFrameNum() >= startNum + (60 * 60)){
            std::exit(0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    visualizer->keyPressed(key);
    switch(key){
        case 'e':
            bExport = !bExport;
            startNum = ofGetFrameNum();
            break;
    }
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

//
//  Image.cpp
//  Bas
//
//  Created by Jildert Viet on 18-03-16.
//
//

#include "JImage.hpp"

JImage::JImage(string filename, ofVec2f loc){
    this->loc = loc;
    setType("JImage");
    image.load(filename);
//    image.mirror(true, false);
//    cout << image.getImageType() << endl;
//    image.setImageType(OF_IMAGE_COLOR_ALPHA);
    size = ofVec2f(image.getWidth(), image.getHeight());
    
    meshSetup();
    colors[0] = ofColor(255,255);
//    createFullScreenCopy();
}

void JImage::display(){
    ofSetColor(colors[0]);
    if(fillScreen)
        backGround.draw(loc);
    if(drawImage)
        image.draw(loc);
    if(drawMesh)
        mesh.draw();
}

void JImage::specificFunction(){
}

void JImage::createFullScreenCopy(){
    backGround.allocate(ofGetWindowWidth(), ofGetWindowHeight(), image.getImageType());
    int numChannels = image.getPixels().getNumChannels();
    for(int w=0; w<backGround.getWidth(); w++){
        for(int h=0; h<backGround.getHeight(); h++){
            for(int channels=0; channels<numChannels; channels++){
                backGround.getPixels()[numChannels*(w+(h*backGround.getWidth()))+channels] =
                image.getPixels()[numChannels*((w%(int)image.getWidth())+((int)((h%(int)image.getHeight())*image.getWidth())))+channels];
            }
        }
    }
    backGround.update();
}

void JImage::meshSetup(){
    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    mesh.addVertex(loc);
    mesh.addVertex(loc+ofVec2f(100,0));
    mesh.addVertex(loc+ofVec2f(0,100));
}

void JImage::loadImage(string path){
    image.clear();
    if(image.load(path))
        cout << "Image " << path << " loaded" << endl;
    image.update();
    size = ofVec2f(image.getWidth(), image.getHeight());
}

void JImage::makeImageFit(){
    float ratio = image.getWidth() / image.getHeight();
    image.resize(ofGetWindowWidth() * ratio, ofGetWindowHeight());
}

void JImage::center(){
    loc.x = ofGetWindowWidth()/2. - image.getWidth()/2.;
}

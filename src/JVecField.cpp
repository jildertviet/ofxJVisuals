//
//  vecField.cpp
//  Visualizer_2_5
//
//  Created by Jildert Viet on 09-03-19.
//

#include "JVecField.hpp"
#define DRAW_VID   false

JVecField::JVecField(){
    setType("JVecField");
    size = ofVec2f(ofGetWidth(), ofGetHeight());
    setDensity(density);
    colors[0].a = 255;
    seed = ofRandom(1000);
    
    if(!shader.load("shaders/brightnessAndSaturation")){
        cout << "Failed to load shader" << endl;
    }
    
    if(DRAW_VID){
        video = new ofVideoPlayer;
        video->load("video/mc4.mp4");
        video->play();
        video->setLoopState(OF_LOOP_NORMAL);
        video->setVolume(0.);
        
        ofPixels p; // Default
        p.allocate(1, 1, 4);
        p.setColor(0, 0, ofColor::red);
        vecTex.loadData(p);
    }
    
//    vecTex.allocate(size.x, size.y, GL_RGBA);
}

void JVecField::specificFunction(){
    switch(mode){
        case VECFIELD_MODE::PERLIN:{
            t = ofGetFrameNum() * speed + seed;
            for (int i = 0; i < p.getWidth(); i++){
                for(int j=0; j<p.getHeight(); j++){
                    ofFloatColor c = ofColor::white;
                    c.r = getField(ofVec2f(i, j)).x;
                    c.g = getField(ofVec2f(i, j)).y;
                    p.setColor(i, j, c);
                }
            }
            vecTex.loadData(p);
        }
            break;
        case VECFIELD_MODE::TEST:{
            
        }
            break;
        case VECFIELD_MODE::VIDEO:{
            if(!video)
                return;
            video->update();
            if(video->isFrameNew()){
                frame.setFromPixels(video->getPixels());
                frame.resize(density.x, density.y);
                vecTex.loadData(frame.getPixels());
            } else{

            }
        }
            break;
        case UNDERLAYING:{
            // Read underlaying layers...
            // Do this in display()? So the underlaying layers will be drawn already
        }
            break;
    }
}

void JVecField::display(){
    ofSetLineWidth(lineWidth);
    if(mode == UNDERLAYING){
//        if(vecTex.getWidth() != 1280)
//            vecTex.allocate(1280, 800, GL_RGBA);
        
//        vecTex.loadScreenData(0, 0, 1280, 800); // Read screen content into fbo
        
        resizeFbo.begin();
            ofClear(0,0);
            shader.begin();
                shader.setUniform1f("contrast", shaderContrast);
                shader.setUniform1f("brightness", shaderBrightnessAdd); // All color counts, so background as well :o
//                ofSetColor(255);
//                vecTex.draw(0, 0, density.x, density.y);
                underlayer->draw(0, 0, density.x, density.y);
            shader.end();
        resizeFbo.end();
        
        vecTex = resizeFbo.getTexture();
        
        // Draw or hide background:
        ofSetColor(colors[0]);
        ofDrawRectangle(0, 0, size.x, size.y);
        
        // Monitor, debugging
//        ofSetColor(255);
//        vecTex.draw(0, 0, ofGetWidth(), ofGetHeight());
    } else{
        switch(drawMode){
            case LINES:{
                ofSetColor(colors[0]);
                for(int i=0; i<vecTex.getWidth(); i++){ // Can be done in geom shader? ...
                    for(int j=0; j<vecTex.getHeight(); j++){
                        start = ofVec2f(i, j);
                        start *= sizeMultiplier;
                        start += offset;
                        end = start;
                        ofVec2f dir = getField(ofVec2f(i, j));
                        dir *= 2.;
                        dir -= ofVec2f(1, 1);
                        if(bNormalize){
                            end += dir.normalize() * lineLength;
                        } else{
                            end += dir * lineLength;
                        }
                        ofDrawLine(start, end); // Or as one mesh?
                    }
                }
            }
                break;
            case CIRCLES:{
                ofSetColor(colors[0]);
                ofNoFill();
                for(int i=0; i<vecTex.getWidth(); i++){ // Can be done in geom shader? ...
                    for(int j=0; j<vecTex.getHeight(); j++){
                        start = ofVec2f(i, j);
                        start *= sizeMultiplier;
                        start += offset;
                        end = start;
                        ofVec2f dir = getField(ofVec2f(i, j));
                        if(bNormalize){
                            end += dir.normalize() * lineLength;
                        } else{
                            end += dir * lineLength;
                        }
                        ofDrawCircle(start, dir.x * lineLength);
    //                    ofDrawLine(start, end); // Or as one mesh?
                    }
                }
            }
                break;
            case HIDE:{
    //            video->draw(0, 0, ofGetWidth(), ofGetHeight());
                if(DRAW_VID)
                    frame.draw(0, 0, ofGetWidth(), ofGetHeight());
                ofSetColor(colors[0]);
                ofDrawRectangle(0, 0, size.x, size.y);
            }
                break;
            case TEXTURE:{
                ofSetColor(colors[0]);
                vecTex.draw(0, 0, size.x, size.y);
            }
        }
    }
}

ofVec2f JVecField::getField(ofVec2f position) {
    float normx = ofNormalize(position.x, 0, ofGetViewportWidth());
    float normy = ofNormalize(position.y, 0, ofGetViewportHeight());
    float u = ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
    float v = ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
    
    u = ofLerp(0, 1, u);
    v = ofLerp(0, 1, v);
    return ofVec2f(u, v);
}

void JVecField::setDensity(glm::vec2 d, char numChannels){
    p.clear();
    vecTex.clear();
    p.allocate(d.x, d.y, numChannels);
    density = d;
    resizeFbo.allocate(density.x, density.y, GL_RGBA);
    
//    plane.set(density.x, density.y, 10, 10);
//    plane.mapTexCoords(0, 0, resizeFbo.getWidth(), resizeFbo.getHeight());
    
//    sizeMultiplier = ofVec2f(ofGetWidth() / p.getWidth(), ofGetHeight() / p.getHeight());
    sizeMultiplier = ofVec2f(size.x / p.getWidth(), size.y / p.getHeight());
    offset = sizeMultiplier * 0.5;
}

void JVecField::setSize(ofVec3f size){
    this->size = size;
    setDensity(density);
}

void JVecField::setPixelsToTest(){
    setDensity(glm::vec2(2, 2), 4); // 4 channels
    p.setColor(ofFloatColor(0.5, 0.5, 0., 1.0));
    p.setColor(0, 0, ofFloatColor(1.0, 0.5, 0., 1.0));
//    p.setColor(1, 0, ofFloatColor(1.0, 1.0, 0., 1.0));
    p.setColor(0, 1, ofFloatColor(0., 0.5, 0., 1.0));
//    p.setColor(1, 1, ofFloatColor(0.5, 0., 1.0));
    vecTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
}

void JVecField::setMode(char m){
    mode = m;
}

void JVecField::customOne(){
    shaderContrast = customOneArguments[0];
}

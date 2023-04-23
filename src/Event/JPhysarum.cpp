//
//  JPhysarum.cpp
//  visualizer
//
//  Created by Jildert Viet on 01-07-20.
//

#include "JPhysarum.hpp"

#ifdef JPhysarum_hpp

JPhysarum::JPhysarum(glm::vec2 loc, glm::vec2 size){
    setType("JPhysarum");
    this->size = glm::vec3(size, 0);
    this->loc = glm::vec3(loc,0);

    numParticles = (size.x*size.y)*0.5;

    string shadersFolder = SHADER_PATH;

    // Loading the Shaders
    updatePos.load(shadersFolder+"/JPhysarum/posUpdate");
    updateVel.load(shadersFolder+"/JPhysarum/velUpdate");
    alphaDecay.load(shadersFolder+"/JPhysarum/alphaDecay");

    updateRender.load(shadersFolder+"/JPhysarum/render");

    textureRes = (int)sqrt((float)numParticles);
    numParticles = textureRes * textureRes;

    ofFloatPixels p;
    p.allocate(textureRes, textureRes, 3);
    for (float x = 0; x < textureRes; x++){
        for (float y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            p.setColor(x, y,
              ofFloatColor(x / (float)textureRes, y / (float)textureRes, 1.0-pow(ofRandom(0.0001, 0.9), 3.0))
            );
        }
    }

    ofImage img;
    img.setFromPixels(p);

    posPingPong.allocate(textureRes, textureRes, GL_RGBA32F);
    posPingPong.src->begin();
    ofClear(0);
    img.draw(0,0);
    posPingPong.src->end();


    velPingPong.allocate(textureRes, textureRes, GL_RGBA32F);
    velPingPong.src->begin();
    ofClear(255, 0, 0); // All in one direction
    velPingPong.src->end();

    renderPingPong.allocate(size.x, size.y, GL_RGBA16F);
    renderPingPong.src->begin();
    ofClear(0, 0);
    ofSetColor(0, 0, 255);
    for(int i=0; i<100; i++){
        ofDrawRectangle(ofRandom(renderPingPong.src->getWidth()), ofRandom(renderPingPong.src->getHeight()), 10, 10);
    }
    renderPingPong.src->end();

    renderPingPong.dst->begin();
        ofClear(0,0);
    renderPingPong.dst->end();

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for(int x = 0; x < textureRes; x++){
        for(int y = 0; y < textureRes; y++){
            mesh.addVertex({x,y,0});
            mesh.addTexCoord({x, y});
        }
    }

//    gui.setup();
//    gui.add(sensorAngle.setup("sensorAngle", 45, 1, 360));
//    gui.add(sensorDistance.setup("sensorDistance", 50, 1, 500));
//    gui.add(turnAngle.setup("turnAngle", 45, 0, 360));
//    gui.add(speedMul.setup("speedMul", 2.5, 0.2, 30.0));
//    gui.add(decay.setup("decay", 0.5, 0, 1));
//    gui.add(depositAmount.setup("depositAmount", 40, 1., 255.));
//    gui.add(balance.setup("balance", 0.,0., 1.0));
//    gui.add(blurMix.setup("blurMix", 0.,0., 1.0));
    setColor(ofColor::white);
}

void JPhysarum::display(){
    ofFill();
    ofSetColor(color);
    renderPingPong.src->draw(0, 0);
    // ofNoFill();
    // ofSetColor(255, 100);
    // ofDrawRectangle(0, 0, renderPingPong.src->getWidth(), renderPingPong.src->getHeight());

   // velPingPong.src->draw(0, 0);
//    velPingPong.dst->draw(velPingPong.dst->getWidth(), 0);
   // posPingPong.src->draw(0, velPingPong.dst->getHeight() * 3);


   // posPingPong.src->draw(0, 0);
//    posPingPong.dst->draw(velPingPong.dst->getWidth(), velPingPong.dst->getHeight());

//    if(bDrawGui){
//        posPingPong.src->draw(0, 0);
//        velPingPong.src->draw(0, posPingPong.src->getHeight());
//        ofPushMatrix();
//        ofScale(2);
//        gui.draw();
//        ofPopMatrix();
//    }
}

void JPhysarum::specificFunction(){
    velPingPong.dst->begin();
        ofClear(0);
        updateVel.begin();
            updateVel.setUniformTexture("velocityTex", velPingPong.src->getTexture(), 0); // To read current vel
            updateVel.setUniformTexture("positionTex", posPingPong.src->getTexture(), 1); // To read current pos
            updateVel.setUniformTexture("trailTex", renderPingPong.src->getTexture(), 2); // To read what's happening around that pos
            if(externalVelocity)
                updateVel.setUniformTexture("externalVelocity", *externalVelocity, 3); // To read what's happening around that pos

            updateVel.setUniform1f("sensorAngle", sensorAngle);
            updateVel.setUniform1f("time", ofGetElapsedTimeMillis());
            updateVel.setUniform1f("sensorDistance", sensorDistance);
            updateVel.setUniform1f("turnAngle", turnAngle);

            updateVel.setUniform2f("resolution", glm::vec2(size.x, size.y));
            updateVel.setUniform1f("speedMul", speed);
            updateVel.setUniform1f("balance", balance);
            updateVel.setUniform1f("depositAmount", (float)depositAmount);
            updateVel.setUniform1f("blurMix", (float)blurMix);

            bool bExternalVelocity = false;
            if(externalVelocity)
                bExternalVelocity = true;
            updateVel.setUniform1i("bExternalVelocity", (int)bExternalVelocity);

            velPingPong.src->draw(0, 0);
        updateVel.end();
        velPingPong.dst->end();
    velPingPong.swap();

    posPingPong.dst->begin();
        ofClear(0);
        updatePos.begin();
            updatePos.setUniformTexture("velocityTex", velPingPong.src->getTexture(), 1);
            updatePos.setUniformTexture("positionTex", posPingPong.src->getTexture(), 0);
            // updatePos.setUniformTexture("trailTex", renderPingPong.src->getTexture(), 2);
            updatePos.setUniform1f("speedMul", speed);
            posPingPong.src->draw(0, 0);
        updatePos.end();
    posPingPong.dst->end();
    posPingPong.swap();




    renderPingPong.dst->begin();
        ofClear(0, 255);

        // ofEnableAlphaBlending();
        alphaDecay.begin();
          alphaDecay.setUniform1f("decay", decay);
          ofSetColor(255);
          renderPingPong.src->draw(0,0);
        alphaDecay.end();

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        // ofEnableAlphaBlending();
        // glEnable(GL_BLEND);

        // ofSetColor(255);
        // ofPushMatrix();
        // ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
        // ofRotateDeg(ofGetFrameNum()*0.3);
        // ofDrawLine(0, 0, ofGetWidth()*0.5, 0);
        // ofPopMatrix();


        // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        // glBlendFunc(GL_ONE, GL_ZERO);


        updateRender.begin();
            updateRender.setUniformTexture("posTex", posPingPong.src->getTexture(), 0);
            updateRender.setUniformTexture("alpha", velPingPong.src->getTexture(), 1);
            updateRender.setUniform2f("screen", (float)size.x, (float)size.y);
            ofSetColor(255);
            mesh.draw();
        updateRender.end();

    renderPingPong.dst->end();

    renderPingPong.swap();


    // glDisable(GL_BLEND);
    ofDisableBlendMode();
    // ofDisableAlphaBlending();
}
#endif

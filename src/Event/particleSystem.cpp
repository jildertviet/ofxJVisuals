#include "particleSystem.h"
#ifndef USE_OPENCL

particleSystem::particleSystem(){
  init(numParticles);
}

particleSystem::particleSystem(int numParticles, glm::vec2 size) : numParticles(numParticles){
  this->size = size;
  init(numParticles);
}

void particleSystem::init(int numParticles){
  timeStep = 0.005f;
  numParticles = 1000;

  string shadersFolder;
  if(ofIsGLProgrammableRenderer()){

  }else{
    shadersFolder="../../../../../addons/ofxJVisuals/libs/shaders/particleSystem";
  }

  // Loading the Shaders
  if(ofIsGLProgrammableRenderer()){
      updatePos.load(shadersFolder+"/passthru.vert", shadersFolder+"/posUpdate.frag");// shader for updating the texture that store the particles position on RG channels
      updateVel.load(shadersFolder+"/passthru.vert", shadersFolder+"/velUpdate.frag");// shader for updating the texture that store the particles velocity on RG channels
  }else{
      updatePos.load("",shadersFolder+"/posUpdate.frag");// shader for updating the texture that store the particles position on RG channels
      updateVel.load("",shadersFolder+"/velUpdate.frag");// shader for updating the texture that store the particles velocity on RG channels
  }

  // Frag, Vert and Geo shaders for the rendering process of the spark image
  // updateRender.setGeometryInputType(GL_POINTS);
  // updateRender.setGeometryOutputType(GL_TRIANGLE_STRIP);
  // updateRender.setGeometryOutputCount(6);
  updateRender.load(shadersFolder+"/render");

  // Seting the textures where the information ( position and velocity ) will be
  textureRes = (int)sqrt((float)numParticles);
  numParticles = textureRes * textureRes;

  // 1. Making arrays of float pixels with position information
  vector<float> pos(numParticles*3);
  for (int x = 0; x < textureRes; x++){
      for (int y = 0; y < textureRes; y++){
          int i = textureRes * y + x;

          pos[i*3 + 0] = ofRandom(1.0); //x*offset;
          pos[i*3 + 1] = ofRandom(1.0); //y*offset;
          pos[i*3 + 2] = 0.0;
      }
  }
  // Load this information in to the FBO's texture
  posPingPong.allocate(textureRes, textureRes, GL_RGB32F);
  posPingPong.src->getTexture().loadData(pos.data(), textureRes, textureRes, GL_RGB);
  posPingPong.dst->getTexture().loadData(pos.data(), textureRes, textureRes, GL_RGB);


  // 2. Making arrays of float pixels with velocity information and the load it to a texture
  vector<float> vel(numParticles*3);
  for (int i = 0; i < numParticles; i++){
      vel[i*3 + 0] = ofRandom(-1.0,1.0);
      vel[i*3 + 1] = ofRandom(-1.0,1.0);
      vel[i*3 + 2] = 1.0;
  }
  // Load this information in to the FBO's texture
  velPingPong.allocate(textureRes, textureRes, GL_RGB32F);
  velPingPong.src->getTexture().loadData(vel.data(), textureRes, textureRes, GL_RGB);
  velPingPong.dst->getTexture().loadData(vel.data(), textureRes, textureRes, GL_RGB);

  // Loading and setings of the variables of the textures of the particles

  // Allocate the final
  renderFBO.allocate(size[0], size[1], GL_RGB32F);
  renderFBO.begin();
  ofClear(0, 0, 0, 255);
  renderFBO.end();


  mesh.setMode(OF_PRIMITIVE_POINTS);
  for(int x = 0; x < textureRes; x++){
      for(int y = 0; y < textureRes; y++){
          mesh.addVertex({x,y,0});
          mesh.addTexCoord({x, y});
      }
  }
}

void particleSystem::setVecField(JVecField* vF){
    // vecField = vF;
    //
    // cout << vF->density << endl;
    // pixels = new unsigned char[(int)vF->density.x*(int)vF->density.y*4];
};

void particleSystem::specificFunction(){
  // In each cycle it's going to update the velocity first and the the position
  // Each one one with a different shader and FBO.
  // Because on GPU you can't write over the texture that you are reading we are
  // using to pair of ofFbo attached together on what we call pingPongBuffer
  // Learn more about Ping-Pong at:
  //
  // http://www.comp.nus.edu/~ashwinna/docs/PingPong_FBO.pdf
  // http://www.seas.upenn.edu/~cis565/fbo.htm#setupgl4

  // Velocities PingPong
  //
  // It calculates the next frame and see if it's there any collition
  // then updates the velocity with that information
  //
  velPingPong.dst->begin();
  ofClear(0);
  updateVel.begin();
  updateVel.setUniformTexture("backbuffer", velPingPong.src->getTexture(), 0);   // passing the previus velocity information
  updateVel.setUniformTexture("posData", posPingPong.src->getTexture(), 1);  // passing the position information
  updateVel.setUniform1i("resolution", (int)textureRes);
  updateVel.setUniform2f("screen", (float)size[0], (float)size[1]);
  updateVel.setUniform1f("timestep", (float)timeStep);

  // draw the source velocity texture to be updated
  velPingPong.src->draw(0, 0);

  updateVel.end();
  velPingPong.dst->end();

  velPingPong.swap();


  // Positions PingPong
  //
  // With the velocity calculated updates the position
  //
  posPingPong.dst->begin();
  ofClear(0);
  updatePos.begin();
  updatePos.setUniformTexture("prevPosData", posPingPong.src->getTexture(), 0); // Previus position
  updatePos.setUniformTexture("velData", velPingPong.src->getTexture(), 1);  // Velocity
  updatePos.setUniform1f("timestep",(float) timeStep );

  // draw the source position texture to be updated
  posPingPong.src->draw(0, 0);

  updatePos.end();
  posPingPong.dst->end();

  posPingPong.swap();


  // Rendering
  //
  // 1.   Sending this vertex to the Vertex Shader.
  //      Each one it's draw exactly on the position that match where it's stored on both vel and pos textures
  //      So on the Vertex Shader (that's is first at the pipeline) can search for it information and move it
  //      to it right position.
  // 2.   Once it's in the right place the Geometry Shader make 6 more vertex in order to make a billboard
  // 3.   that then on the Fragment Shader is going to be filled with the pixels of sparkImg texture
  //
  renderFBO.begin();
  ofClear(0,0,0,0);
  updateRender.begin();
  updateRender.setUniformTexture("posTex", posPingPong.dst->getTexture(), 0);
  updateRender.setUniform1i("resolution", (float)textureRes);
  updateRender.setUniform2f("screen", (float)size[0], (float)size[1]);

  ofPushStyle();
  ofEnableBlendMode( OF_BLENDMODE_ADD );
  ofSetColor(255);

  mesh.draw();

  ofDisableBlendMode();
  glEnd();

  updateRender.end();
  renderFBO.end();
  ofPopStyle();
}

void particleSystem::display(){
  ofSetColor(colors[0]);
  renderFBO.draw(0,0);
}

//
// void particleSystem::customOne(){
//     globalForce = glm::vec2(customOneArguments[0], customOneArguments[1]);
// }
//
// void particleSystem::customTwo(){
//     forceMultiplier = glm::vec2(customOneArguments[2], customOneArguments[3]);
// }
//
// void particleSystem::customThree(){ // Set traagheid
//     traagheid = customOneArguments[4];
// }
//
// void particleSystem::customFour(){ // Write alpha to fbo...
// //     float alpha = colors[0].a / 255.;
// //     cout << "Set alpha: " << alpha << endl;
// //     for(unsigned int i=0; i<numParticles; i++) {
// //         particleSystemParticle &p = particles[i];
// //         particleCos[i].set(colors[0].r / 255., colors[0].g / 255., colors[0].b / 255., alpha);
// //         p.alpha = alpha; // Takes this as a "fade in to" value
// //     }
// // //    particleCos.initFromGLObject(cbo, numParticles);
// //     opencl.kernel("updateParticle")->setArg(4, particleCos);
// //     particleCos.writeToDevice(0, numParticles);
// //     destAlpha = alpha;
// }
//
// void particleSystem::customFive(){
//     fadeTime = customOneArguments[0];
// }
//
// void particleSystem::setColor(ofColor c, int index){
//     colors[0] = c;
//     customFour();
// }
//
// void particleSystem::setSize(glm::vec3 size){
//     this->size = size;
// }

#endif

//
//  particleSystem.cpp
//  Visualizer_1_2
//
//  Created by Jildert Viet on 04-12-16.
//
//
#if USE_OPENCL
#include "particleSystemOpenCL.hpp"

typedef struct{
    float2 vel;
    float2 spawnPos;
    float mass;
    float alpha;        // need this to make sure the float2 vel is aligned to a 16 byte boundary
//    float fadeInValue;
//    float dummy;
} particleSystemParticle;

msa::OpenCLBufferManagedT<particleSystemParticle> particles; // vector of Particles on host and corresponding clBuffer on device
msa::OpenCLBufferManagedT<float2> particlePos; // vector of particle positions on host and corresponding clBuffer, and vbo on device
msa::OpenCLBufferManagedT<float4> particleCos;

particleSystem::particleSystem(){
    init(numParticles);
}

particleSystem::particleSystem(int numParticles, ofVec2f size, ofFloatColor color, int testIndex, string clFile){
    this->numParticles = numParticles;
    this->color = color;
    dimensions = size;
    busses[0] = testIndex;
    init(numParticles);
    clImage.initWithTexture(128, 80, GL_RGBA);
    clImage.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR); // Remove this later :
    // init particles
}

void particleSystem::init(int numParticles, string clFile){
    setType("particleSystem");
    particlesPos = new float2[numParticles];
    
    opencl.setupFromOpenGL(1); // 1 is NVIDIA?

    // create vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * numParticles, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Sort of unbind?
    
    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float4) * numParticles, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
//    particleCos.initBuffer(numParticles);
    
    // init host and CL buffers
    particles.initBuffer(numParticles);
    particlePos.initFromGLObject(vbo, numParticles);
    particleCos.initFromGLObject(cbo, numParticles);
    
//    clImage.initWithTexture(10, 10, GL_RGBA);
//    clImage.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR); // Remove this later
    
    for(int i=0; i<numParticles; i++) {
        particleSystemParticle &p = particles[i];
        p.vel.set(ofRandom(10000.), ofRandom(10000.));
        p.mass = ofRandom(0.00001, 1.0) / 10.;
        
        int x,y;
        x = sin(i)*ofGetHeight()*0.4; // For circle resetPos
        y = cos(i)*ofGetHeight()*0.4;
        x += (ofGetWidth()/2.) + ofRandom(-2.5, 2.5); // Bit of noise on the spawncircle
        y += (ofGetHeight()/2.) + ofRandom(-2.5, 2.5);
        p.spawnPos = float2(x,y);
        
        x = ofRandom(dimensions.x);
        y = ofRandom(dimensions.y);
        p.spawnPos = float2(x,y);
        particlePos[i].set(x,y);
        particleCos[i].set(1, 1.0, 1.0,0.4);
        p.alpha = 0.4; // Takes this as a "fade in to" value
    }
    
    particles.writeToDevice();
    particlePos.writeToDevice(); ///< uploads buffer data to shared CL/GL memory, so the vbo and the cl buffer are written in one go, since they occupy the same memory locations.
    particleCos.writeToDevice();
    
    
    opencl.loadProgramFromFile(clFile);
    opencl.loadKernel("updateParticle");

    opencl.kernel("updateParticle")->setArg(0, particles);
    opencl.kernel("updateParticle")->setArg(1, particlePos);
    opencl.kernel("updateParticle")->setArg(2, dimensions);
//    opencl.kernel("updateParticle")->setArg(3, clImage); // Only set it when the clImage is allocated (by setVecField function)
    opencl.kernel("updateParticle")->setArg(4, particleCos);
    opencl.kernel("updateParticle")->setArg(5, globalForce);
    opencl.kernel("updateParticle")->setArg(6, forceMultiplier);
    opencl.kernel("updateParticle")->setArg(7, traagheid);
    opencl.kernel("updateParticle")->setArg(8, fadeTime);
    
    glPointSize(2);
}

void particleSystem::setVecField(JVecField* vF){
    vecField = vF;
    
    cout << vF->density << endl;
    pixels = new unsigned char[(int)vF->density.x*(int)vF->density.y*4];
    return;
//    clImage.reset();
    cout << "initWithTexture" << endl;
//    clImage.initWithTexture(vF->texture); // GL_RGBA
    clImage.initFromTexture(vF->vecTex);
    cout << "setTextureMinMagFilter" << endl;
    clImage.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_LINEAR); // Remove this later :
};

bool particleSystem::copyVecFieldToCLImage(){
    if(vecField && vecField->vecTex.getWidth() != 0){ // Update vecField-info
        ofPixels p;
        vecField->vecTex.readToPixels(p);
        memcpy(pixels, p.getData(), p.getWidth() * p.getHeight() * 4);
        clImage.write(pixels, true);
        return true;
    } else{
        return false; // Test: only work with vecField for now
    }
}

void particleSystem::specificFunction(){
    if(!copyVecFieldToCLImage())
        return;
    
    opencl.kernel("updateParticle")->setArg(2, dimensions);
    opencl.kernel("updateParticle")->setArg(3, clImage);
    opencl.kernel("updateParticle")->setArg(5, globalForce);
    opencl.kernel("updateParticle")->setArg(6, forceMultiplier);
    opencl.kernel("updateParticle")->setArg(7, traagheid);
    opencl.kernel("updateParticle")->setArg(8, fadeTime);
    opencl.kernel("updateParticle")->setArg(9, destAlpha);
    
    glFlush();
    
    opencl.kernel("updateParticle")->run1D(numParticles);
//    opencl.flush();
}

void particleSystem::display(){
    ofSetColor(255);
    opencl.finish();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glColorPointer(4, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, numParticles);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    opencl.finish();
    
//    glColor4f(color.r, color.g, color.b, color.a);
//    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
//
//    opencl.finish();
//
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(2, GL_FLOAT, 0, 0);
//    glDrawArrays(GL_POINTS, 0, numParticles);
//    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void particleSystem::customOne(float* v){
    globalForce = float2(busses[0], busses[1]);
}

void particleSystem::customTwo(float* v){
    forceMultiplier = float2(busses[2], busses[3]);
}

void particleSystem::customThree(float* v){ // Set traagheid
    traagheid = busses[4];
}

void particleSystem::customFour(float* v){ // Set alpha
    float alpha = color.a / 255.;
    cout << "Set alpha: " << alpha << endl;
    for(unsigned int i=0; i<numParticles; i++) {
        particleSystemParticle &p = particles[i];
        particleCos[i].set(color.r / 255., color.g / 255., color.b / 255., alpha);
        p.alpha = alpha; // Takes this as a "fade in to" value
    }
//    particleCos.initFromGLObject(cbo, numParticles);
    opencl.kernel("updateParticle")->setArg(4, particleCos);
    particleCos.writeToDevice(0, numParticles);
    destAlpha = alpha;
}

void particleSystem::customFive(float* v){
    fadeTime = busses[0];
}

void particleSystem::setColor(ofColor c, int index){
    color = c;
    customFour(float* v);
}

void particleSystem::setSize(glm::vec3 size){
    dimensions = size;
    this->size = size;
}

#endif

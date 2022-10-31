#include "particleSystem.h"

typedef struct{
    glm::vec2 vel;
    glm::vec2 origin;
    float mass;
    float alpha;        // need this to make sure the float2 vel is aligned to a 16 byte boundary
} particleSystemParticle;


particleSystem::particleSystem(){
    init(numParticles);
}

particleSystem::particleSystem(int numParticles, glm::vec2 size, ofFloatColor color, int testIndex, string clFile) : numParticles(numParticles), color(color){
    init(numParticles);
}

void particleSystem::init(int numParticles, string clFile){
    setType("particleSystem");
    string shadersFolder;
    if(ofIsGLProgrammableRenderer()){
        shadersFolder="../../../addons/ofxJVisuals/libs/shaders_gl3"; //  Why less .. ???
    }else{
        shadersFolder="../../../../../addons/ofxJVisuals/libs/shaders";
    }

    // Loading the Shaders
    if(ofIsGLProgrammableRenderer()){
        particleShader.load(shadersFolder+"/particleShader.vert", shadersFolder+"/particleShader.frag");
    } else{
        particleShader.load("",shadersFolder+"/particleShader.frag");
    }
    int textureRes = (int)sqrt((float)numParticles);

    velocityAndOrigin.allocate(textureRes, textureRes, GL_RGBA);
    massAndAlpha.allocate(textureRes, textureRes, GL_RGBA);

    ofPixels p, q;
    p.allocate(textureRes, textureRes, GL_RGBA);
    q.allocate(textureRes, textureRes, GL_RGBA);
    for(int x=0; x<textureRes; x++) {
      for(int y=0; y<textureRes; y++) {
        particleSystemParticle p;
        p.vel.set(ofRandom(10000.), ofRandom(10000.));
        p.mass = ofRandom(0.00001, 1.0) / 10.;

        int x,y;
        x = ofRandom(size.x);
        y = ofRandom(size.y);
        p.origin = glm::vec2(x,y);
        p.alpha = 0.4; // Takes this as a "fade in to" value

        p.setColor(x,y,ofColor(p.vel.x, p.vel.y, p.origin.x, p.origin.y));
        q.setColor(x,y,ofColor(p.mass, 1.0, p.alpha, 1.0));
      }
    }

    specificFunction();

    glPointSize(2);
}

void particleSystem::setVecField(JVecField* vF){
    vecField = vF;

    cout << vF->density << endl;
    pixels = new unsigned char[(int)vF->density.x*(int)vF->density.y*4];
};

void particleSystem::specificFunction(){
  particleShader.setUniform(0, particles);
  particleShader.setUniform(1, particlePos);
  particleShader.setUniform(2, size);
  particleShader.setUniform(3, globalForce);
  particleShader.setUniform(4, forceMultiplier);
  particleShader.setUniform(5, traagheid);
  particleShader.setUniform(6, fadeTime);
  particleShader.setUniform(9, destAlpha);
}

void particleSystem::display(){
    ofSetColor(255);
}

void particleSystem::customOne(){
    globalForce = glm::vec2(customOneArguments[0], customOneArguments[1]);
}

void particleSystem::customTwo(){
    forceMultiplier = glm::vec2(customOneArguments[2], customOneArguments[3]);
}

void particleSystem::customThree(){ // Set traagheid
    traagheid = customOneArguments[4];
}

void particleSystem::customFour(){ // Write alpha to fbo...
//     float alpha = colors[0].a / 255.;
//     cout << "Set alpha: " << alpha << endl;
//     for(unsigned int i=0; i<numParticles; i++) {
//         particleSystemParticle &p = particles[i];
//         particleCos[i].set(colors[0].r / 255., colors[0].g / 255., colors[0].b / 255., alpha);
//         p.alpha = alpha; // Takes this as a "fade in to" value
//     }
// //    particleCos.initFromGLObject(cbo, numParticles);
//     opencl.kernel("updateParticle")->setArg(4, particleCos);
//     particleCos.writeToDevice(0, numParticles);
//     destAlpha = alpha;
}

void particleSystem::customFive(){
    fadeTime = customOneArguments[0];
}

void particleSystem::setColor(ofColor c, int index){
    colors[0] = c;
    customFour();
}

void particleSystem::setSize(glm::vec3 size){
    this->size = size;
}

#endif

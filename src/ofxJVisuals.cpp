//  Created by Jildert Viet on 24-01-16.

#include "ofxJVisuals.hpp"

#define RECEIVER_PORT 6061

ofxJVisuals::ofxJVisuals(glm::vec2 size, bool bUseSC) : size(size) {
  initMesh();
  for (int i = 0; i < MAX_EVENTS_PTRS; i++)
    events[i] = nullptr;
  for (int i = 0; i < NUMLAYERS; i++)
    layers[i] = new JLayer(); // Dummy events

  negative.load((string)SHADER_PATH + "/negative150");
  brightnessAndSaturation.load((string)SHADER_PATH +
                               "/brightnessAndSaturation");

#if USE_PP
  post.init(ofGetWidth(), ofGetHeight());
  noisePP = post.createPass<NoiseWarpPass>();
  noisePP->setEnabled(false);
  fxaaPP = post.createPass<FxaaPass>();
  fxaaPP->setEnabled(false);
  rgbPP = post.createPass<RGBShiftPass>();
  rgbPP->setEnabled(false);

  zoomPP = post.createPass<ZoomBlurPass>();
  zoomPP->setEnabled(false);
#endif

  alphaScreen = new AlphaBlackScreen(true);
  alphaScreen->size = glm::vec3(size, 0.0);
  addEvent((JEvent *)alphaScreen, 0);
  setAlpha(255);

  loadLastMaskFile();

  avenir30.load("/System/Library/Fonts/avenir.ttc", 30, true, true);
  avenir30.setLineHeight(34.0f);
  avenir30.setLetterSpacing(1.035);
  avenir300.load("/System/Library/Fonts/avenir.ttc", 300, true, true);
  avenir300.setLineHeight(34.0f);
  avenir300.setLetterSpacing(1.035);
  selectedFont = &avenir300;

  cout << "ofxJVisuals made" << endl;
#ifndef TARGET_RASPBERRY_PI
  ofFbo::Settings fs; // For export quality
  fs.numSamples = 8;
  fs.width = size.x;
  fs.height = size.y;
  fs.internalformat = GL_RGBA16F;
  fs.useStencil = true;
  fbo.allocate(fs);
  renderFbo.allocate(fs);
#else
  fbo.allocate(size.x, size.y, GL_RGBA);
  renderFbo.allocate(size.x, size.y, GL_RGBA);
#endif
  negativeMask.allocate(size.x, size.y, GL_RGBA);

  fbo.begin();
  ofClear(0, 0);
  fbo.end();
  negativeMask.begin();
  ofClear(0, 0);
  // ofSetColor(255);
  // ofDrawRectangle(negativeMask.getWidth()*0.25, 0,
  // negativeMask.getWidth()*0.5, negativeMask.getHeight());
  negativeMask.end();

  // initCam();

  receiver.setup(RECEIVER_PORT);

  msgParser = new MsgParser(this, bUseSC);

  ofSetCircleResolution(90);
  ofEnableAlphaBlending();
  // ofEnableDepthTest();
  //    sharedFbo.allocate(2560, 800, GL_RGBA);
  //    sharedFbo2.allocate(2560, 800, GL_RGBA);
}

void ofxJVisuals::initCam() {
  cam.reset();
  //    cam.rotate(-180, ofVec3f(0,1,0));
  //    cam.setPosition(0, 0, 0);
  //    cam.rotate(-180, ofVec3f(1,0,0));
  //    cam.setDistance(fbo.getWidth() * 0.25);
  //    cam.lookAt(ofVec3f(0,0,0));

  //    cam.move(0, ofGetHeight() * -0.5, 0);
  //    cam.move(fbo.getWidth() * 0.5, fbo.getHeight() * 0.5, 0); // Temp for
  //    ADEtje

  if (!camController) {
    camController = new cameraController(&cam);
    addEvent((JEvent *)camController, 0);
  }
}

ofxJVisuals::~ofxJVisuals() {
  cout << "ofxJVisuals deleted" << endl;
  // Delete all events?
  for (int i = 0; i < NUMLAYERS; i++)
    layers[i][0].deleteNext();
  delete msgParser;
}

void ofxJVisuals::deconstructor() { delete this; }

void ofxJVisuals::update() {
  msgParser->update();

  while (receiver.hasWaitingMessages()) {
    ofxOscMessage m;
    receiver.getNextMessage(m);
    cout << m << endl;
    msgParser->parseMsg(m);
  }

  if (bRotate)
    rotationAngle += rotationAngleIcrement;

  for (int i = 0; i < NUMLAYERS; i++)
    layers[i]->updateMain();
  negativeLayer.updateMain();

  //    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
  //    GL_ONE_MINUS_SRC_ALPHA); // Found this on the OF forum
  // Goal: two grey area's should be brighter when they overlap, currently they
  // merge in the same greyness

  //    ofEnableBlendMode(OF_BLENDMODE_ADD);
  //    ofEnableDepthTest();
  negativeMask.begin();
  ofClear(0, 255);
  negativeLayer.displayMain();
  negativeMask.end();

  fbo.begin();

#if USE_PP
  glPushAttrib(GL_ENABLE_BIT);
  // setup gl state
  //        glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  post.begin();
#endif
  ofClear(0, 0);
  //        ofEnableSmoothing();
  alphaScreen->displayMain();

  // brightnessAndSaturation.begin();
  // brightnessAndSaturation.setUniform1f("brightness", 0); // 0 = normal
  // brightnessAndSaturation.setUniform1f("contrast", 1.0); // 1 = normal
  // brightnessAndSaturation.setUniform1f("saturation", 1); // 1 = normal

  ofEnableBlendMode(OF_BLENDMODE_ALPHA);
  layers[1]->displayMain(); // Non-cam layer back
  for (int i = 0; i < generativeShaders.size(); i++) {
    generativeShaders[i]->begin();
    generativeShaders[i]->update();
    ofSetColor(255);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    generativeShaders[i]->end();
  }

  //        if(bCam){
  //            ofPushMatrix();
  //            cam.begin();
  //            ofScale(1, -1, 1);
  //            ofTranslate(-ofGetWidth()/2., -ofGetHeight()/2.);
  //        }
  for (int i = 2; i < NUMLAYERS; i++) // Don't draw layer 0: nonCamFront
    layers[i]->displayMain();
  ofDisableDepthTest();
  //        if(bCam){
  //            cam.end();
  //            ofPopMatrix();
  //        }

  // layers[NUMLAYERS-1]->displayMain(); // Non-cam layer front

//        ofClearAlpha();
#if USE_PP
  post.end();
  // set gl state back to original
  glPopAttrib();
#endif
  // brightnessAndSaturation.end();
  fbo.end();

  renderFbo.dst->begin();
  negative.begin();
  negative.setUniformTexture("mask", negativeMask.getTexture(), 1);
  fbo.draw(0, 0);
  negative.end();
  renderFbo.dst->end();

  renderFbo.swap();

  for (int i = 0; i < fxShaders.size(); i++) {
    renderFbo.dst->begin();
    ofSetColor(255);
    fxShaders[i]->begin();
    fxShaders[i]->update();

    renderFbo.src->draw(0, 0);
    fxShaders[i]->end();
    renderFbo.dst->end();
    renderFbo.swap();
  }
  renderFbo.swap();
}

void ofxJVisuals::display() {
  ofSetColor(255, brightness); // Brightness = alpha of fbo w/ black background?
  switch (fboDisplayMode) {
  case 0:
    renderFbo.getTexture().bind();
    mesh.draw();
    renderFbo.getTexture().unbind();
    if (bEditMode) {
      for (char i = 0; i < 4; i++)
        ofDrawCircle(meshVertices[i], 10);
    }
    break;
  case 1: // Stretch
    renderFbo.draw(0, 0);
    break;
  case 2: //
    cout << "Display mode not supported" << endl;
    //            fbo.draw(0, ofGetHeight() * 0.5 - (0.5 * (ofGetHeight() /
    //            (f.getWidth() / ofGetWidth()))), ofGetWidth(), ofGetHeight() /
    //            (f.getWidth() / ofGetWidth()));
    break;
  }
  drawMask();
}

bool ofxJVisuals::checkIfNull(JEvent *e) { return (bool)e; }

JEvent *ofxJVisuals::addEvent(JEvent *e, int layerIndex, int index,
                              bool bOldVersion) { // Index 0 means: don't save
  if (index && bOldVersion) {
    if (index < MAX_EVENTS_PTRS) {
      if (events[index]) {
        cout << "Position in array is occupied, delete" << endl;
        // This deletes multiple J_UGens w/ the same ID (from Synth)...
        delete events[index]; // Sets events[index] to nullptr? Doesn't
                              // matter...
        events[index] = nullptr;
      }
      events[index] = e; // For accessing events later, with id
      e->events = events;
      cout << "Added to ptrs[] w/ index: " << index << endl;
    } else {
      cout
          << "Can't save the eventID in the events[], because the ID is too big"
          << endl;
      // return nullptr;
    }
  }

  if (layerIndex >= 0) {
    if (layerIndex == VisualizerLayer::NEGATIVE) {
      negativeLayer.addEvent(e);
    } else {
      layers[layerIndex]->addEvent(e);
    }
  }
  last = e;
  e->lastPtr = &last; // For setting to nullptr if event is deleted
  e->lastFound = &lastFound;
  cout << e->type << endl;
  numEvents++;
  // cout << "event ID: " << e->id << endl;
  // Used ?

  //    e->SCsender = SCsender;
  e->numEventsPtr = &numEvents;

  e->id = index;
  e->v = this;

  //    e->receivingPointers = &receivingPointers;
  return e;
}

JEvent *ofxJVisuals::addEvent(JEvent *e, VisualizerLayer l, int index) {
  switch (l) {
  case FUNCTIONAL:
    return addEvent(e, 0, index);
  case NON_CAM_BACK: // Is draw first
    return addEvent(e, 1, index);
  case NON_CAM_FRONT: // Drawn as last
    return addEvent(e, NUMLAYERS - 1, index);
  case DEFAULT:
    return addEvent(e, 2, index);
  case NEGATIVE:
    negativeLayer.addEvent(e);
    return addEvent(e, -1, index); // Don't store it in a layer again
  }
  return nullptr;
}

void ofxJVisuals::setAlpha(int alpha, bool bDo) {
  alphaScreen->setAlpha(alpha);
  alphaScreen->setActiveness(bDo);
}

void ofxJVisuals::setBrightness(unsigned char b) { brightness = b; }

void ofxJVisuals::killAll() {
  alphaScreen->bGradient = false;
#if USE_PP
  bPostProcessing = false;
#endif
  bAddMirror = false;

  for (uint8 i = 1; i < NUMLAYERS - 1; i++) { // Skip layer 0 'Functional'
    if (layers[i]->next)
      layers[i]->next->deleteNext(); // Don't delete self, only next events
  }
  if (negativeLayer.next)
    negativeLayer.next->deleteNext();

  for (unsigned short i = 0; i < MAX_EVENTS_PTRS; i++)
    events[i] = nullptr;
}

void ofxJVisuals::makeFit(glm::vec2 size) {
  if (this->size == size)
    return;
  alphaScreen->size = glm::vec3(size, 0.0);
  if (fbo.getWidth() != size.x || fbo.getHeight() != size.y) {
    fbo.allocate(size.x, size.y,
                 fbo.getTexture().getTextureData().glInternalFormat);
  }
  this->size = size;
}
//
// vector<float> ofxJVisuals::vec(float a){
//    vector<float> v;
//    v.push_back(a);
//    return v;
//}

// vector<float> ofxJVisuals::vec(float a, float b){
//     return vector<float>({a, b});
// }
// vector<float> ofxJVisuals::vec(float a, float b, float c){
//     return vector<float>({a, b, c});
// }
// vector<float> ofxJVisuals::vec(float a, float b, float c, float d){
//     return vector<float>({a, b, c, d});
// }
// vector<float> ofxJVisuals::vec(float a, float b, float c, float d, float e){
//     return vector<float>({a, b, c, d, e});
// }

void ofxJVisuals::loadLastMaskFile() {
  ofFile f;
  f.open("./maskFile.txt", ofFile::Mode::ReadOnly);
  ofBuffer b;
  b = f.readToBuffer();
  string path = b.getText();
  if (path.length() > 5) {
    mask.load(path);
    maskBrightness = 0;
    bMask = true;
  }
}

ofxOscMessage ofxJVisuals::getAllEvents() {
  ofxOscMessage m;
  m.setAddress("/allEvents");
  for (unsigned short i = 1; i < NUMLAYERS - 1; i++) {
    // A, B, C
    if (layers[i]->next) {
      JEvent *lastRead = layers[i]->next; // First: A (skip dummy Event)
      m.addIntArg(lastRead->id);
      m.addStringArg(lastRead->type);

      while (lastRead->next) {     // Is there a B? Yes.
        lastRead = lastRead->next; // So this is B.
        m.addIntArg(lastRead->id);
        m.addStringArg(lastRead->type);
        // has B a next? Yes. Has C a next? No
      }
    } else {
      // Only the dummy Event
    }
  }
  return m;
}

JEvent *ofxJVisuals::getEventById(unsigned int idToFind, unsigned int subID) {
  if (getLastAdded() && getLastAdded()->id == idToFind &&
      getLastAdded()->subID == subID) {
    return getLastAdded();
  }
  if (getLastFound() && getLastFound()->id == idToFind &&
      getLastFound()->subID == subID) {
    return getLastFound();
  }

  // if(idToFind<MAX_EVENTS_PTRS){
  //     if(events[idToFind]){
  //         return events[idToFind];
  //     } else{
  //
  //     }
  // } else{
  //
  // }

  JEvent *toCheck = &negativeLayer; // Dummy
  while (toCheck->next) {
    if (toCheck->next->id == idToFind && toCheck->next->subID == subID) {
      lastFound = toCheck->next;
      return toCheck->next;
    }
    toCheck = toCheck->next;
  }
  for (int i = 0; i < NUMLAYERS; i++) {
    JEvent *toCheck = layers[i]; // Dummy
    while (toCheck->next) {
      // cout << "Layer: " << i << ", id: " << toCheck->next->id << endl;
      if (toCheck->next->id == idToFind && toCheck->next->subID == subID) {
        lastFound = toCheck->next;
        return toCheck->next;
      }
      toCheck = toCheck->next;
    }
  }
  return nullptr;

  //    if(last){
  //        if(last->id == idToFind){
  //            return last;
  //        }
  //    }
  //
  //    for(int i=0; i<NUMLAYERS; i++){
  //        JEvent* toCheck = layers[i]; // Dummy
  //        while(toCheck->next){
  //            cout << "test: layer: " << i << " " << toCheck->next->id <<
  //            endl; if(toCheck->next->id == idToFind){
  //                return toCheck->next;
  //            }
  //            toCheck = toCheck->next;
  //        }
  //    }
  //    return nullptr;
}

void ofxJVisuals::getFreePointers(string ip, int port) {
  unsigned short freeEvents[MAX_EVENTS_PTRS];
  unsigned short numFreeEvents = 0;
  for (unsigned short i = 1; i < MAX_EVENTS_PTRS; i++) { // Keep 0 free...
    if (events[i] == nullptr) {
      freeEvents[numFreeEvents] = i;
      numFreeEvents++;
    }
  }
  ofxOscMessage m;
  m.setAddress("/freePointers");
  for (unsigned short i = 0; i < numFreeEvents; i++) {
    m.addInt32Arg(freeEvents[i]);
  }
  if (!SCsender) {
    SCsender = msgParser->initSCsender(ip, port);
  }
  if (SCsender) {
    SCsender->sendMessage(m);
  } else {
    cout << "Failed to init SC sender, with ip: " << ip << " and port: " << port
         << endl;
  }
  cout << "Reply with free pointers" << endl;
  // [3, 4, 5 .. 512], when only 0, 1, 2 are still occupied / alive.
  // numFreeEvents should 509

  // So should have a 'blank list', of all ptrs[x] that are free...
  // @ Reset receive a bunch of numbers that are free, use that for setting new
  // addresses @ SC...
}

void ofxJVisuals::initCircularMaskFbo(glm::vec2 size, int num) {
  cout << "create circle mask w/ size: " << size << endl;
  // create one circle first
  circularMask.allocate(size.x, size.y, GL_RGBA);
  ofPixels p;
  p.allocate(size.x, size.y, OF_PIXELS_RGBA);
  float yRatio, xRatio;
  for (int i = 0; i < p.getWidth(); i++) {
    xRatio = (float)i / p.getWidth();
    xRatio = pow((1 - abs(sin(xRatio * (num * PI)))), 0.5);
    for (int j = 0; j < p.getHeight(); j++) {
      yRatio = float(j) / p.getHeight();
      yRatio = pow((1 - sin(yRatio * (PI))), 0.5);
      float toWrite = (yRatio + xRatio) * 0.5;
      ofColor c = ofColor(0, 255 * toWrite); // Half phase of sine
      p.setColor(i, j, c);
    }
  }
  circularMask.getTexture().loadData(p);
}

void ofxJVisuals::drawMask() {
  if (bMask) {
    ofSetColor(255, maskBrightness);
    mask.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
  }
  if (bDrawCirclularMask) {
    ofSetColor(255);
    circularMask.draw(0, 0);
  }
}

void ofxJVisuals::keyPressed(int key) {
  switch (key) {
  case 'q':
    fboDisplayMode = 0;
    break;
  case 'w':
    fboDisplayMode = 1;
    break;
  case 'e':
    fboDisplayMode = 2;
    break;
  case 'a':
    bEditMode = !bEditMode;
    break;
  }
}

void ofxJVisuals::initMesh() {
  mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
  texCoords = {glm::vec2(0, 0), glm::vec2(size.x, 0), glm::vec2(size.x, size.y),
               glm::vec2(0, size.y)};
  meshVertices = {glm::vec3(0, 0, 0), glm::vec3(size.x, 0, 0),
                  glm::vec3(size.x, size.y, 0), glm::vec3(0, size.y, 0)};
  for (char i = 0; i < 4; i++) {
    //        mesh.addTexCoord(texCoords[i] + glm::vec2(size.x, 0)); // Center
    //        piece
    mesh.addTexCoord(texCoords[i]); // Center piece
    mesh.addVertex(meshVertices[i]);
  }
}

void ofxJVisuals::mousePressed(int x, int y, int button) {
  if (bEditMode) {
    unsigned char indexOfClosest = 0;
    float minDistance = 99999999;
    for (char i = 0; i < 4; i++) {
      if (ofVec2f(x, y).distance(meshVertices[i]) < minDistance) {
        minDistance = ofVec2f(x, y).distance(meshVertices[i]);
        indexOfClosest = i;
      }
    }
    cout << "Selected: " << (int)indexOfClosest << endl;
    meshVertices[indexOfClosest] = glm::vec3(x, y, 0);
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

    for (char i = 0; i < 4; i++) {
      mesh.addTexCoord(texCoords[i]);
      mesh.addVertex(meshVertices[i]);
    }
  }
}

// Wrappers
JEvent *MsgParser::getEventById(unsigned int id, unsigned int subID) {
  return v->getEventById(id, subID);
}
JEvent *MsgParser::addEvent(JEvent *e, int layerIndex, int index,
                            bool bOldVersion) {
  return v->addEvent(e, layerIndex, e->id, bOldVersion);
}
ofTrueTypeFont *MsgParser::getSelectedFont() { return v->selectedFont; }
// vector<JShader *> *MsgParser::getShaders() { return &(v->shaders); }
JEvent *MsgParser::addShader(JShader *s, int layer) {
  auto &shaderList = (layer == 2) ? v->generativeShaders : v->fxShaders;
  shaderList.push_back(s);
  s->parent = &shaderList;
  return (JEvent *)s;
}

ofFbo *MsgParser::getFbo() { return &(v->fbo); }
void MsgParser::killAll() { v->killAll(); }
void MsgParser::getFreePointers(string host, int port) {
  v->getFreePointers(host, port);
}
void MsgParser::setBrightness(float b) { v->setBrightness(b); }
void MsgParser::initCam() { v->initCam(); }
void MsgParser::setAlpha(float a) { v->setAlpha(a); }
void MsgParser::setCirclularMaskState(bool bState) {
  v->bDrawCirclularMask = bState;
}
void MsgParser::setCameraState(bool bState) { v->bCam = bState; }
ofEasyCam *MsgParser::getCam() { return &(v->cam); }
AlphaBlackScreen *MsgParser::getAlphaScreen() { return v->alphaScreen; }

JEvent *JEvent::getEventById(unsigned int id, unsigned int subID) {
  return v->getEventById(id, subID);
}

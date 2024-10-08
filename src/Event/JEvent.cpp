#include "JEvent.hpp"

JEvent::JEvent() {
  color = ofColor::white;
  setStartTime();
  setDefaultMappers(); // Will this happen on subclasses?
}

JEvent::~JEvent() {
  cout << "Start of ~JEvent()" << endl;
  if (lastPtr)
    *lastPtr = nullptr;
  if (lastFound)
    *lastFound = nullptr;

  if (numEventsPtr)
    *numEventsPtr = *numEventsPtr - 1;
  if (!next) {
    if (previous)
      previous->next = nullptr;
  } else {
    previous->next = next;
    next->previous = previous;
  }

  deleteModifiers();

  if (events)
    events[id] = nullptr; // Only if it has this ptr! Child JEvents (inside an
                          // JEvent) don't hve this set...

  clearEnv();
  printf("dtor %p\n", this); // in destructor
  cout << "JEvent '" << type << "', id: " << id << " deleted!" << endl;
}

void JEvent::deleteNext() {
  if (next)
    next->deleteNext();
  ownDtor();
  delete this;
}

void JEvent::update() {
  if (getTimeMillis() > endTime && active) {
    cout << "JEvent (id:" << id << ") has passed!" << endl;
    ownDtor();

    // Remove from 'vector<JEvent*> JEvents' in Visualizer (trying to remove,
    // 05-03-19)
    if (parentContainer) {
      for (int i = 0; i < parentContainer->size(); i++) {
        if (parentContainer->at(i) == this) {
          parentContainer->at(i) = nullptr;
        }
      }
    }

    delete this;
    return;
  }
#ifdef JV_SHAREDMEM
  if (reader) {
    // float *r = reader->readFloatArray();
    // if (r) {
    // setValuesFromFloatArray(r);
    // }
    reader->readFloatArrayBuf(valuesToSend); // Re-use array
    cout << "Set " << ofToString(id) << ", " << ofToString(subID) << endl;
    setValuesFromFloatArray(valuesToSend);
    cout << "Done set " << endl;
    // delete r;
  }
#endif

  for (int i = envelopes.size() - 1; i >= 0; i--) {
    for (int j = envelopes[i].size() - 1; j >= 0; j--) {
      if (!envelopes[i][j]->process()) { // Process
        if (lastEnv == envelopes[i][j])
          lastEnv = nullptr;
        envelopes[i].erase(envelopes[i].begin() +
                           j); // If env is done: remove from vec
      }
    }
    if (envelopes[i].size() ==
        0) { // If all envelopes are done, remove this vec
      envelopes.erase(envelopes.begin() + i);
    } else {
      std::sort(envelopes[i].begin(), envelopes[i].end(),
                customLess); // Active envelopes, so let's get highest value
      envelopes[i].back()->ptr->writeValue(envelopes[i].back()->value);
      if (envelopes[i].size() > 1) {
        //                cout << 0 << endl;
      }
    }
  }
}

void JEvent::updateMain() {
  if (next)
    next->updateMain();
  specificFunction();
  if (modifiers.size()) {
    updateModifiers();
  }
  update();
}

void JEvent::displayMain() {
  if (modifiers.size()) {
    displayModifiers();
  } else {
    display();
  }
  if (next)
    next->displayMain();
}

void JEvent::setEnvelope(
    int attack, int sustain, int release,
    ofVec2f range) { // Link envelope to alpha of color & lifetime of object
  Env *e = new Env(vector<float>{range.x, range.y, range.y, range.x},
                   vector<float>{(float)attack, (float)sustain, (float)release},
                   &color, 0);
  int totalTime = attack + sustain + release;
  setEndTime(totalTime);
  addEnv(e);
}

//
void JEvent::setEnvelope(int attack, int sustain, int release, int *value,
                         ofVec2f range) { // Link envelope to int
  Env *e = new Env(vector<float>{range.x, range.y, range.y, range.x},
                   vector<float>{(float)attack, (float)sustain, (float)release},
                   value, 0);
  int totalTime = attack + sustain + release;
  setEndTime(totalTime);
  addEnv(e);
}

void JEvent::setEnvelope(int attack, int sustain, int release, float *value,
                         ofVec2f range, int id,
                         bool bSetEndTime) { // Link envelope to float
  Env *e = new Env(vector<float>{range.x, range.y, range.y, range.x},
                   vector<float>{(float)attack, (float)sustain, (float)release},
                   value, 0);
  int totalTime = attack + sustain + release;
  setEndTime(totalTime);
  addEnv(e);
}

void JEvent::checkBorders() {
  if (bCheckBordersH) {
    if (loc.x < 0) {
      loc.x = 0;
      direction.x *= -1;
    }
    if ((loc.x + size.x) > ofGetViewportWidth()) {
      loc.x = ofGetViewportWidth() - size.x;
      direction.x *= -1;
    }
  }
  if (bCheckBordersV) {
    if ((loc.y + size.y) > ofGetViewportHeight()) {
      loc.y = ofGetViewportHeight() - size.y;
      direction.y *= -1;
    }
    if (loc.y < 0) {
      loc.y = 0;
      direction.y *= -1;
    }
  }
}

void JEvent::setType(string type) {
  this->type = type;
  cout << "Made an JEvent of type: " << type << endl;
};

JEvent *JEvent::getEvent(int index) {
  JEvent *JEvent = this;
  for (int i = 0; i < index; i++) {
    if (JEvent->next) {
      JEvent = JEvent->next;
    }
  }
  return JEvent;
}

void JEvent::addEvent(JEvent *toAdd) {
  if (!next) {
    next = toAdd;
    toAdd->previous = this;
  } else {
    next->addEvent(toAdd);
  }
  //    if(next)
  //        next->id = id+1;
}

JEvent *JEvent::getLast() {
  // Als een JEvent geen 'next' heeft, returned ie zichzelf, omdat ie de laatste
  // is
  if (next) {
    return next->getLast();
  } else {
    return this;
  }
}

void JEvent::setEndTime(float duration) {
  setStartTime();
  endTime = startTime + duration;
}

void JEvent::addEventAsFirst(JEvent *toAdd) {
  if (next)
    next->previous = toAdd;
  next = toAdd;
}

void JEvent::deleteWithFade(short int releaseTime) {
  addEnvAlpha(vector<float>{(float)color.a, 0},
              vector<float>{(float)releaseTime});
  endTime = getTimeMillis() + releaseTime;
  active = true;
}

void JEvent::setColor(ofColor color) { this->color = color; }

void JEvent::addPtr(JEvent **p) { toClear.push_back(p); }

void JEvent::setAlpha(unsigned char alpha) { color.a = alpha; }

Env *JEvent::addEnv(Env *e) {
  e->id = getNumEnv();
  e->parentID = &id;
  lastEnv = e;

  for (int i = 0; i < envelopes.size(); i++) {
    for (int j = 0; j < envelopes[i].size(); j++) {
      if (envelopes[i][j]->ptr->valF &&
          envelopes[i][j]->ptr->valF == e->ptr->valF) {
        envelopes[i].push_back(e);
        return e;
      } else if (envelopes[i][j]->ptr->valI &&
                 envelopes[i][j]->ptr->valI == e->ptr->valI) {
        envelopes[i].push_back(e);
        return e;
      } else if (envelopes[i][j]->ptr->valC &&
                 envelopes[i][j]->ptr->valC == e->ptr->valC) {
        envelopes[i].push_back(e);
        return e;
      }
    }
  }

  envelopes.push_back(vector<Env *>({e}));
  return e;
}

Env *JEvent::addEnv(vector<float> levels, vector<float> times, float *f,
                    char curve) {
  Env *e = new Env(levels, times, f, curve);
  addEnv(e);
  return e;
}
Env *JEvent::addEnv(vector<float> levels, vector<float> times, int *i,
                    char curve) {
  Env *e = new Env(levels, times, i, curve);
  addEnv(e);
  return e;
}
Env *JEvent::addEnv(vector<float> levels, vector<float> times, ofColor *c,
                    char curve) {
  Env *e = new Env(levels, times, c, curve);
  addEnv(e);
  return e;
}
void JEvent::addEnvAlpha(vector<float> levels, vector<float> times,
                         char curve) {
  Env *e = new Env(levels, times, &color, curve);
  addEnv(e);
}

void JEvent::addEnvAlpha(float a, float s, float r, float alpha) {
  Env *e = new Env(vector<float>{0, (float)color.a, (float)color.a, 0},
                   vector<float>{a, s, r}, &color, 0);
  addEnv(e);
}

short JEvent::getNumEnv() {
  short num = 0;
  for (int i = 0; i < envelopes.size(); i++) {
    for (int j = 0; j < envelopes[i].size(); j++) {
      num++;
    }
  }
  return num;
}

Env *JEvent::getLastEnv() { return lastEnv; }

void JEvent::clearEnv() { vector<vector<Env *>>().swap(envelopes); }

bool JEvent::checkIfEnvExists(char id) {
  for (int i = 0; i < envelopes.size(); i++) {
    for (int j = 0; j < envelopes.size(); j++) {
      if (envelopes[i][j]->id == id) {
        return true;
      }
    }
  }
  return false;
}

void JEvent::setSize(glm::vec3 s) { size = s; }
void JEvent::setMode(char m) { mode = m; }
void JEvent::setLoc(glm::vec3 loc) { this->loc = loc; }
void JEvent::setSpeed(float speed) { this->speed = speed; }

unsigned long JEvent::getTimeMillis() {
  if (USE_FRAMES_AS_CLOCK) {
    return ofGetFrameNum() * (1000. / ofGetTargetFrameRate());
  } else {
    return ofGetElapsedTimeMillis();
  }
}

void JEvent::setStartTime() {
  if (USE_FRAMES_AS_CLOCK) {
    startTime = ofGetFrameNum() * (1000. / ofGetTargetFrameRate());
  } else {
    startTime = ofGetElapsedTimeMillis();
  }
}

void JEvent::setDefaultMappers() {
  mapValues[0] = new mapValue(&size.x);
  mapValues[1] = new mapValue(&size.y);
  mapValues[2] = new mapValue(&size.z);
  mapValues[3] = new mapValue(&loc.x);
  mapValues[4] = new mapValue(&loc.y);
  mapValues[5] = new mapValue(&loc.z);
  mapValues[6] =
      new mapValue(&color, 'a'); // = 'a'; // This will be accessed with an
                                 // extra character, identifying a, r, g or b
  mapValues[7] = new mapValue(&color, 'r');
  mapValues[8] = new mapValue(&color, 'g');
  mapValues[9] = new mapValue(&color, 'b');
  mapValues[10] = new mapValue(&speed);
  mapValues[11] = new mapValue(&direction.x);
  mapValues[12] = new mapValue(&direction.y);
  mapValues[13] = new mapValue(&direction.z);
  mapValues[14] = new mapValue(&zoom);
}

void JEvent::fillBuffer(ofxOscMessage &m) {
  if (buffer.size()) {
    buffer.clear();
  }
  for (int i = 1; i < m.getNumArgs(); i++) { // Skip 0: id of JEvent
    buffer.push_back(m.getArgAsFloat(i));
  }
  // cout << buffer[0] << endl;
}

void JEvent::doFunc(int id, float *v) {
  switch (id) {
  case 0:
    customOne(v);
    break;
  case 1:
    customTwo(v);
    break;
  case 2:
    customThree(v);
    break;
  case 3:
    customFour(v);
    break;
  case 4:
    customFive(v);
    break;
  }
}

#ifdef JV_SHAREDMEM
void JEvent::createMemoryReader() {
  string idString = "";
  idString += ofToString(id);
  idString += ",";
  idString += ofToString(subID);
  cout << "Subscribe to shared mem: " << idString << endl;
  reader =
      std::make_shared<lsm::SharedMemoryReadStream>(idString, 64 * 4, false);
  cout << "After subscribe" << endl;
}
#endif

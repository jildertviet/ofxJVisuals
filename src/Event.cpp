//
//  Event.cpp
//  AudioAnalyzer
//
//  Created by Jildert Viet on 24-01-16.
//
//

#include "Event.hpp"

Event::Event(){
    setStartTime();
}

Event::~Event(){
    if(lastPtr)
        *lastPtr = nullptr;
    
    if(numEventsPtr)
        *numEventsPtr = *numEventsPtr - 1;
    if(!next){
        if(previous)
            previous->next = nullptr;
    } else{
        previous->next = next;
        next->previous = previous;
    }
    
    if(events)
        events[id] = nullptr; // Only if it has this ptr! Child events (inside an Event) don't hve this set...
    
    clearEnv();

    cout << "Event '" << type << "', id: " << id << " deleted!" << endl;
}

void Event::deleteNext(){
    if(next)
        next->deleteNext();
    ownDtor();
    delete this;
}

void Event::update(){
    if(getTimeMillis() > endTime && active){
        cout << "Event (id:"<<id<<") has passed!" << endl;
        ownDtor();
        
        // Remove from 'vector<Event*> events' in Visualizer (trying to remove, 05-03-19)
        if(parentContainer){
            for(int i=0; i<parentContainer->size(); i++){
                if(parentContainer->at(i) == this){
                    parentContainer->at(i) = nullptr;
                }
            }
        }
        
        delete this;
        return;
    }
    
    for(int i=envelopes.size()-1; i>=0; i--){
        for(int j=envelopes[i].size()-1; j>=0; j--){
            if(!envelopes[i][j]->process()){ // Process
                if(lastEnv == envelopes[i][j])
                    lastEnv = nullptr;
                envelopes[i].erase(envelopes[i].begin() + j); // If env is done: remove from vec
            }
        }
        if(envelopes[i].size()==0){ // If all envelopes are done, remove this vec
            envelopes.erase(envelopes.begin()+i);
        } else{
            std::sort(envelopes[i].begin(), envelopes[i].end(), customLess); // Active envelopes, so let's get highest value
            envelopes[i].back()->ptr->writeValue(envelopes[i].back()->value);
            if(envelopes[i].size() > 1){
//                cout << 0 << endl;
            }
        }
    }
}

void Event::updateMain(){
    if(next)
        next->updateMain();
    specificFunction();
    update();
}

void Event::displayMain(){
    if(next)
        next->displayMain();
    display();
}

void Event::setEnvelope(int attack, int sustain, int release, ofVec2f range){ // Link envelope to alpha of color & lifetime of object
    Env* e = new Env(vector<float>{range.x, range.y, range.y, range.x}, vector<float>{(float)attack, (float)sustain, (float)release}, &colors[0], 0);
    int totalTime = attack + sustain + release;
    setEndTime(totalTime);
    addEnv(e);
}

//
void Event::setEnvelope(int attack, int sustain, int release, int* value, ofVec2f range){ // Link envelope to int
    Env* e = new Env(vector<float>{range.x, range.y, range.y, range.x}, vector<float>{(float)attack, (float)sustain, (float)release}, value, 0);
    int totalTime = attack + sustain + release;
    setEndTime(totalTime);
    addEnv(e);
}

void Event::setEnvelope(int attack, int sustain, int release, float* value, ofVec2f range, int id, bool bSetEndTime){ // Link envelope to float
    Env* e = new Env(vector<float>{range.x, range.y, range.y, range.x}, vector<float>{(float)attack, (float)sustain, (float)release}, value, 0);
    int totalTime = attack + sustain + release;
    setEndTime(totalTime);
    addEnv(e);
}

void Event::checkBorders(){
    if(bCheckBordersH){
        if(loc.x < 0){
            loc.x = 0;
            direction.x *= -1;
        }
        if((loc.x+size.x) > ofGetViewportWidth()){
            loc.x = ofGetViewportWidth()-size.x;
            direction.x *= -1;
        }
    }
    if(bCheckBordersV){
        if((loc.y+size.y) > ofGetViewportHeight()){
            loc.y = ofGetViewportHeight()-size.y;
            direction.y *= -1;

        }
        if(loc.y < 0){
            loc.y = 0;
            direction.y *= -1;
        }
    }
}

void Event::setType(string type){
    this->type=type;
    cout<<"Made an event of type: " << type << endl;;
};

void Event::test(){
        cout << "Event test()" << endl;
}

Event* Event::getEvent(int index){
    Event* event = this;
    for(int i=0; i<index; i++){
        if(event->next){
            event = event->next;
        }
    }
    return event;
}

void Event::addEvent(Event* toAdd){
    if(!next){
        next = toAdd;
        toAdd->previous = this;
    } else{
        next->addEvent(toAdd);
    }
//    if(next)
//        next->id = id+1;
}

Event* Event::getLast(){
    // Als een Event geen 'next' heeft, returned ie zichzelf, omdat ie de laatste is
    if(next){
        return next->getLast();
    } else{
        return this;
    }
}

void Event::setEndTime(float duration){
    setStartTime();
    endTime = startTime + duration;
}

void Event::addEventAsFirst(Event* toAdd){
    if(next)
        next->previous = toAdd;
    next = toAdd;
}

void Event::deleteWithFade(short int releaseTime){
    addEnvAlpha(vector<float>{(float)colors[0].a, 0}, vector<float>{(float)releaseTime});
    endTime = getTimeMillis() + releaseTime;
    active = true;
}

void Event::setColor(ofColor color, int index){
    colors[index] = color;
//    cout << (int)index << " " << color << endl;
}

void Event::addPtr(Event** p){
    toClear.push_back(p);
}

void Event::setAlpha(unsigned char alpha){
    colors[0].a = alpha;
}

Env* Event::addEnv(Env* e){
    e->id = getNumEnv();
    e->parentID = &id;
    lastEnv = e;
    
    for(int i=0; i<envelopes.size(); i++){
        for(int j=0; j<envelopes[i].size(); j++){
            if(envelopes[i][j]->ptr->valF && envelopes[i][j]->ptr->valF == e->ptr->valF){
                envelopes[i].push_back(e);
                return e;
            } else if(envelopes[i][j]->ptr->valI && envelopes[i][j]->ptr->valI == e->ptr->valI){
                envelopes[i].push_back(e);
                return e;
            } else if(envelopes[i][j]->ptr->valC && envelopes[i][j]->ptr->valC == e->ptr->valC){
                envelopes[i].push_back(e);
                return e;
            }
        }
    }
    
    envelopes.push_back(vector<Env*>({e}));
    return e;
}

Env* Event::addEnv(vector<float> levels, vector<float> times, float* f, char curve){
    Env* e = new Env(levels, times, f, curve);
    addEnv(e);
    return e;
}
Env* Event::addEnv(vector<float> levels, vector<float> times, int* i, char curve){
    Env* e = new Env(levels, times, i, curve);
    addEnv(e);
    return e;
}
Env* Event::addEnv(vector<float> levels, vector<float> times, ofColor* c, char curve){
    Env* e = new Env(levels, times, c, curve);
    addEnv(e);
    return e;
}

void Event::addEnvAlpha(vector<float> levels, vector<float> times, char curve){
    Env* e = new Env(levels, times, &colors[0], curve);
    addEnv(e);
}

void Event::addEnvAlpha(float a, float s, float r, float alpha){
    Env* e = new Env(
                     vector<float>{0, (float)colors[0].a, (float)colors[0].a, 0},
                     vector<float>{a, s, r},
                     &colors[0], 0);
    addEnv(e);
}

short Event::getNumEnv(){
    short num = 0;
    for(int i=0; i<envelopes.size(); i++){
        for(int j=0; j<envelopes[i].size(); j++){
            num++;
        }
    }
    return num;
}

Env* Event::getLastEnv(){
    return lastEnv;
}

void Event::clearEnv(){
    vector<vector<Env*>>().swap(envelopes);
}

bool Event::checkIfEnvExists(char id){
    for(int i=0; i<envelopes.size(); i++){
        for(int j=0; j<envelopes.size(); j++){
            if(envelopes[i][j]->id == id){
                return true;
            }
        }
    }
    return false;
}

void Event::setSize(ofVec3f s){size = s;}
void Event::setMode(char m){ mode = m; }
void Event::setLoc(ofVec3f loc){this->loc = loc;}
void Event::setSpeed(float speed){this->speed = speed;}

unsigned long Event::getTimeMillis(){
    if(USE_FRAMES_AS_CLOCK){
        return ofGetFrameNum() * (1000./ofGetTargetFrameRate());
    } else{
        return ofGetElapsedTimeMillis();
    }
}

void Event::setStartTime(){
    if(USE_FRAMES_AS_CLOCK){
        startTime = ofGetFrameNum() * (1000./ofGetTargetFrameRate());
    } else{
        startTime = ofGetElapsedTimeMillis();
    }
}

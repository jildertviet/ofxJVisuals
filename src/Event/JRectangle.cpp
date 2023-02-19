//
//  JRectangle
//
//  Created by Jildert Viet on 24-01-16.
//
//

#include "JRectangle.hpp"

JRectangle::JRectangle(){
    setType("JRectangle");
//    JModifier j;
//    modifiers.push_back((JModifier*)new JModifierArray());
}

JRectangle::JRectangle(float millisTime, glm::vec3 loc, glm::vec3 size, ofColor color, float attack, float release, glm::vec3 direction, bool move) : JRectangle(){
    setEndTime(millisTime);
    this->loc = loc;
    this->size = size;
    this->direction = direction;

    active=true;
//    setEnvelope(attack, millisTime-attack-release,release);
    addEnvAlpha(vector<float>{0, (float)color.a, (float)color.a, 0}, vector<float>{attack, millisTime-attack-release,release});

    speed = 1;
    bMove = move;
}

JRectangle::JRectangle(glm::vec3 loc, glm::vec3 size) : JRectangle(){
    this->loc = loc; this->size = size; active=false;
    bMove = false;
    direction = glm::vec3(-1,0,0);
    speed = 1;
}

void JRectangle::ownDtor(){
    removeFromVector();
}

void JRectangle::setAlpha(unsigned char alpha){
    if(m){
        for(char i=0; i<m->getNumColors(); i++){
            ofColor c = m->getColor(i);
            c.a = alpha;
            m->setColor(i, c);
        }
    }
}

void JRectangle::setQuadColor(ofColor a, ofColor b, ofColor c, ofColor d){
    if(m)
        delete m;
    m = new ofMesh();
    m->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    m->addVertex(glm::vec3(0,0,0));
    m->addColor(a);
    m->addVertex(glm::vec3(size.x, 0, 0));
    m->addColor(b);
    m->addVertex(glm::vec3(0, size.y, 0));
    m->addColor(d); // Swapped, so color adding is clockwise
    m->addVertex(glm::vec3(size.x, size.y, 0));
    m->addColor(c);
}

void JRectangle::display(){
    ofSetColor(color);

    ofPushMatrix();

    if(!m){
        ofTranslate(loc + (size*0.5));
        ofRotateXDeg(rotation.x);
        ofRotateYDeg(rotation.y);
        ofRotateZDeg(rotation.z);
        ofTranslate(-(size*0.5));

//        ofTranslate(loc);
        if(size.z){
            ofDrawBox(0, 0, 0, size.x, size.y, size.z);
        } else{
            ofPushStyle();
            ofSetLineWidth(10);
            switch(mode){
                case 0:
                    break;
                case 1:
                    ofSetRectMode(OF_RECTMODE_CENTER);
                    break;
            }

            // convertRectToMesh(asOfRectangle(), lineWidth);
            // mesh.drawWireframe();
            // mesh = ofMesh::plane(100, 100, 1, 1);
            // ofTranslate(loc);
            // mesh.draw();
            if(bFill){
              ofDrawRectangle(0, 0, size.x, size.y);
            } else{
              ofPath path;
              path.setFillColor(color);
              path.rectangle(0, 0, size.x, size.y);
              path.rectangle(lineWidth*0.5, lineWidth*0.5, size.x-(lineWidth), size.y-lineWidth);
              path.draw();
            }
            ofPopStyle();
        }
//        ofTranslate(-size*0.5); // ?
//        ofRotateZDeg(rotation.z);
//        ofTranslate(size*0.5);
    } else{
        ofTranslate(loc + (size*0.5)); // This wasn't here before... (13-01-2021_
        ofRotateXDeg(rotation.x);
        ofRotateYDeg(rotation.y);
        ofRotateZDeg(rotation.z);
        ofTranslate(-(size*0.5));

        m->draw();
    }

    ofPopMatrix();

    ofFill();
}

void JRectangle::specificFunction(){
    if(getNumEnv()){
        if(m){
            setAlpha(color.a); // Transfer color alpha to mesh alpha
        }
    }
    move();
    imageFloating(); // virtual
}

void JRectangle::jump(glm::vec2 distance){
    loc += glm::vec3(distance, 0);
}

void JRectangle::noDank(){
    setEndTime(400);
    loc = glm::vec3(ofRandomWidth(), 0, 0);
    size = glm::vec3(100, ofGetWindowHeight(), 0);
    active=true;
    int attack = 10;
    int release = 300;
    int millisTime = 400;
    addEnvAlpha(attack, millisTime-attack-release,release);
    if(ofRandom(-1,1)>0){
        direction = glm::vec3(-1,0,0);
    } else{
        direction = glm::vec3(1,0,0);
    }
    speed = 1;
    bMove = true;
}

void JRectangle::addPtr(JRectangle** p){
    toClear.push_back((JEvent**)p);
}

void JRectangle::addVector(vector<JRectangle*>* v){
    this->v = v;
}

void JRectangle::divide(){
    glm::vec3 newSize = size/2.;
    color.a = ofRandom(100)+100;
    for(int i=0; i<2; i++){
        glm::vec3 newLoc = loc+glm::vec3(i*newSize.x, i*newSize.y, 0.0);
        if(newLoc.x<ofGetWindowWidth() && newLoc.y<ofGetWindowHeight()){
            children.push_back(new JRectangle(newSize, newLoc));
            numChildren++;
            JRectangle* c = children.back();
            if(c->bMove){
                c->bMove = true;
            } else{
                c->bMove = false;
            }
            c->speed = speed;
            c->color = color;
        }


        newLoc = glm::vec3(newSize.x*2,0,0)+loc+glm::vec3(i*newSize.x, i*newSize.y, 0);
        if(newLoc.x<ofGetWindowWidth() && newLoc.y<ofGetWindowHeight()){
            children.push_back(new JRectangle(newSize, newLoc));
            numChildren++;
            JRectangle* c = children.back();
            if(c->bMove){
                c->bMove = true;
            } else{
                c->bMove = false;
            }
            c->speed = speed;
            c->color = color;
        }

        newLoc = glm::vec3(0, newSize.y*2, 0.0) + loc + glm::vec3(i*newSize.x, i*newSize.y, 0.0);
        if(newLoc.x<ofGetWindowWidth() && newLoc.y<ofGetWindowHeight()){
            children.push_back(new JRectangle(newSize, newLoc));
            numChildren++;
            JRectangle* c = children.back();
            if(c->bMove){
                c->bMove = true;
            } else{
                c->bMove = false;
            }
            c->speed = speed;
            c->color = color;
        }
    }

    cout << "Num children: " << children.size() << endl;
}

JRectangle* JRectangle::getChild(){
    if(numChildren){
        return children.back();
        children.pop_back();
        numChildren--;
    } else{
        return nullptr;
    }
}

void JRectangle::removeFromVector(){
    if(v){
        for(int i=0; i<v->size(); i++){
            if(v->at(i) == this){
                v->at(i) = nullptr; // Later remove the nullptrs in vec
                cout << "Found self!" << endl;
                return;
            }
        }
    }
}

ofRectangle JRectangle::asOfRectangle(){
  ofRectangle r(vec2(0, 0), size.x, size.y);
  return r;
}

void JRectangle::setHeight(int height){ size.y = height; }
void JRectangle::setWidth(int width){ size.x = width; }

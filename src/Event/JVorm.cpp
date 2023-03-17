//
//  JVorm.cpp
//  Shapes
//
//  Created by Jildert Viet on 15-02-15.
//

#include "JVorm.h"
typedef unsigned short uint16;
JVorm::~JVorm(){
//    cout << "JVorm " << JVorm_index << " being deleted" << endl;
}

void JVorm::ownDtor(){
    removeFromVector();
    unConnect();
}

JVorm::JVorm(uint8 numSides, int sideDiv, float radius, glm::vec3 loc, bool on_destination){
    setType("JVorm");
    this->radius = radius;
    this->loc = loc;
    size = glm::vec3(radius);
    maxDistance = &busses[4];

    makeCoordinates(numSides, sideDiv, radius, on_destination); // Coordinates

    color = ofColor(255, 230);
}

void JVorm::init(){
  // makeCoordinates(numSides, sideDiv, radius, on_destination);
  makeCoordinates(busses[0], busses[1], busses[2], busses[3]);
  changeMaxspeed(speed);
}

void JVorm::specificFunction(){
    for(uint16 i=0; i<particles.size(); i++){
        particles[i]->update();
    }
}

void JVorm::display(){
    ofSetColor(color);

    connectParticles();
    if(particlesVisible){
        for(uint16 i=0; i<particles.size(); i++){
            particles[i]->display();
        }
    }
}

void JVorm::customOne(float* values){
  int targetID;
  float targetIDf = values[0];
  memcpy(&targetID, &targetIDf, sizeof(float));
  JVorm* target = (JVorm*)getEventById(targetID, values[1]);
  cout << this << " connects to: " << target << endl;
  addConnection(target);
}

void JVorm::customTwo(float* v){
    // Place all particles at the border :)
    for(short i=0; i<particles.size(); i++){
        JParticle* p = particles[i];
        switch((int)ofRandom(4)){
            case 0:
                p->loc.x = 0;
                p->loc.y = ofRandomHeight();
                break;
            case 1:
                p->loc.x = ofGetWidth();
                p->loc.y = ofRandomHeight();
                break;
            case 2:
                p->loc.x = ofRandomWidth();
                p->loc.y = 0;
                break;
            case 3:
                p->loc.x = ofRandomWidth();
                p->loc.y = ofGetHeight();
                break;
        }
    }
}

void JVorm::customThree(float* v){}

void JVorm::customFour(float* v){}

void JVorm::customFive(float* v){
    instantFormJVorm();
}

void JVorm::makeCoordinates(uint8 numSides, int sideDiv, float radius_, bool onDestination){
    coordinates.clear();
    particles.clear();

    this->numSides = numSides;
    this->sideDiv = sideDiv;
    radius = radius_; // radius of the circle on which the corner-points lay
    angle = 360. / numSides;

    if(numSides == 1 || sideDiv == 0){
        coordinates[0] = loc;
        return;
    }

    if(numSides<2 || sideDiv <2){
        cout << "Number of sides or side division is too low, number of sides should be >2, side division >2" << endl;
        if(numSides < 2){
            numSides = 2;
        }
        if(sideDiv < 2){
            sideDiv = 2;
        }
    }

    switch(numSides){ // angleOffset is changed, so a rectangle will have horizontal and vertical sides
        case 4:
            angleOffset = 45;
            break;
        default:
            angleOffset = -90;
            break;
    }

    for(uint8 i=0; i<numSides; i++) { // calculate coordinates
        coordinates.push_back(
        glm::vec3(
            loc.x + (cos((angleOffset + (angle*i)) * (PI/180.)) * radius), // xPos
            loc.y + (sin((angleOffset + (angle*i)) * (PI/180.)) * radius), // yPos
            0
            )
        );
    }

    tussenCoordinaten(); // Places points on lines

    for(short i=0; i<coordinates.size(); i++){
        particles.push_back(new JParticle(&(coordinates[i]), !onDestination));   // Particles
        particles.back()->speed = &speed;
    }
}

void JVorm::setCenter(glm::vec3 newLoc){
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] += -loc + newLoc;
    }
}
void JVorm::addToCenter(glm::vec3 add){
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] += add;
    }
}

void JVorm::changeRadius(float radius_){ // Could be improved by only adding radius increasement to coordinates ...
//    makeCoordinates(numSides, side_division, radius_);
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] -= glm::vec3(ofGetWindowSize()/2., 0.0);
        float radiusDelta = radius_ / (float)this->radius;
        coordinates[i] *= (radiusDelta);
        coordinates[i] += glm::vec3(ofGetWindowSize()/2., 0.0);
    }
    radius = radius_;
}

void JVorm::rotateCoordinates(int places, int direction){
    vector<glm::vec3> temp = coordinates;
    if(direction > 0){
        rotate(coordinates.begin(),coordinates.begin()+(places % coordinates.size()),coordinates.end());
    } else{
        if(direction < 0){
            for(int i=0; i<particles.size(); i++){
                if(i==0){
                    coordinates[i] = temp[particles.size()-places];
                } else{
                    coordinates[i] = temp[(i-places)%particles.size()];
                }
            }
        }
    }
    vector<glm::vec3>().swap(temp);
    rotating = true;
}

void JVorm::moveCorner(int corner_, glm::vec3 distance_){
    coordinates[corner_] += distance_;
    tussenCoordinaten();
}

void JVorm::tussenCoordinaten(){
    for (uint8 i=0; i<numSides; i++) {
        for (uint8 j=0; j<sideDiv-1; j++) {
//            int index = numSides+(   (i*((int)side_division-1))+j ); // KLOPT

            glm::vec3 delta = coordinates[((i+1) % numSides)] -  coordinates[(i % numSides)];
            delta = delta * ((1/sideDiv)*(j+1));
            coordinates.push_back(coordinates[i] + delta);
            // *(particlespointer+index) = new Particle(coordinates, index);
            // cout << "Coordinaten_" << index << " " << coordinates[index] << ", met i: " << i << " en j: " << j  << endl;
        }
    }
    vector<glm::vec3> temp;
    for(short i=0; i<coordinates.size(); i++){
        temp.push_back(coordinates[i]);
    }

    for(short i=1; i<particles.size(); i++){ // Start with i=1, because 0th stays 0th
        if(i%(int)sideDiv == 0){
            coordinates[i] = temp[i/(int)sideDiv];
        } else{
            int nth_division = (i%(int)sideDiv);
            int side = ((i-nth_division)/(int)sideDiv);
//            cout << "Side: " << side << endl;
//            cout << "Nth division: " << nth_division << endl;
//            cout << "Number of sides: " << numSides << endl;
            int indexTempCoordinates = numSides+(side*(sideDiv-1))+nth_division;
//            cout << "indexTempCoordinates: " << indexTempCoordinates-1 << endl;
            coordinates[i] = temp[indexTempCoordinates-1];
//            cout << "Setting " << i << " to: " << coordinates[i] << endl;
        }
    }
    vector<glm::vec3>().swap(temp);
}

void JVorm::changeMaxspeed(float maxspeed_){
    // for(int i=0; i<particles.size(); i++)
      speed = maxspeed_;
}

void JVorm::switchConnectableness(){
    for(int i=0; i<particles.size(); i++){
        particles[i]->connectable = !particles[i]->connectable;
    }
}

void JVorm::changeMaxspeed(float minspeed_, float maxspeed_){
    speed = ofRandom(minspeed_, maxspeed_);
}

void JVorm::changeStateOfParticles(bool state){
    for(int i=0; i<particles.size(); i++)
        particles[i]->state = state;
}

void JVorm::setState(bool state){
    for(int i=0; i<particles.size(); i++)
        particles[i]->state = state;
}

bool JVorm::checkIfInFormation(){
    float sum_distance = 0;
    if(ofGetFrameNum() % 2 == 1){
        for(int i=0; i<coordinates.size(); i++){
            float distance = glm::distance(particles[i]->loc, (*(*(particles[i])).destination));
            sum_distance += distance;
        }
        if(sum_distance/coordinates.size() < 4.){
            return true;
        } else{
            return false;
        }
    }
return false;
}

void JVorm::switchRadiusses(JVorm* JVorm){
    if(!JVorm)
        return;
    int radiusTemp = radius;
    changeRadius(JVorm->radius);
    JVorm->changeRadius(radiusTemp);
}

void JVorm::mirror(){
//    cout << "MIRROR" << endl;
    rotateCoordinates((particles.size())/2., true);
}

void JVorm::formJVorm(){
    changeStateOfParticles(1);
}

void JVorm::changeAngleOffset(float angle_){
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] -= loc;
        glm::vec3 c = coordinates[i];
        c = glm::rotate(c, angle_, glm::vec3(0, 0, 1));
        coordinates[i] = c; // Klopt dit? ofVec2f naar ofVec3f, Z = 0 ?
        coordinates[i] += loc;
    }
}

bool JVorm::isConnectable(){
    for(int i=0; i<particles.size(); i++)
        return particles[i]->connectable;
    return true;
}

void JVorm::instantFormJVorm(){
    for(int i=0; i<particles.size(); i++)
        particles[i]->locationIsDestination();
}

void JVorm::connectParticles(){
    // Connect own particles
    for(uint16 i=0; i<particles.size(); i++){
        for(uint16 j=i; j<particles.size(); j++){
            drawLines(particles[i], particles[j]);
        }
    }

    // Connect with particles of other JVorm objects
    connectWith();
}

void JVorm::drawLines(JParticle *p1, JParticle *p2){
    float d = glm::distance(p1->loc,p2->loc);
    if(d < *maxDistance && p1->connectable && p2->connectable){
        ofSetLineWidth(lineWidth);

        ofSetColor(color.r, color.g, color.b,
                   ofMap(d, 0, *maxDistance, color.a, 0));
        ofDrawLine(p1->loc, p2->loc);
    }
}

void JVorm::unConnect(){ // Say to all connected objects: remove me from your vector
    for(uint16 i=0; i<connections.size(); i++){
        JVorm* connection = connections[i].vorm;
        connection->removeConnection(this);
    }
}

void JVorm::removeConnection(JVorm* v){
    int index; // Index to remove
    for(int i=0; i<connections.size(); i++){
        JVorm* connection = connections[i].vorm;
        if(connection == v){
            index = i;
        }
    }
    if(connections.size())
        connections.erase(connections.begin() + index);
}

void JVorm::connectWith(){
    // If A and B are not connected: draw lines from A to B, set both to 'connected' on both objects. B will do nothing with A.
    for(int i=0; i<connections.size(); i++){
        if(connections[i].connected == false){
            // DRAW LINES (A to B)
            bDrawLines = true;
//            cout << this << " bDrawLines = true" << endl;

            // CONNECT
            // Returns the pointer from the other object, pointing to this object   (A finds A* in B)
            JVorm::connection* b = connections[i].vorm->findSelf(this);

//            cout << "B points to A with adress: " << b->JVorm << " is this " << this << endl;
            b->connected = true; // B knows it is connected to A
            connections[i].connected = true; // A knows it is connected to B
        }

        if(bDrawLines){
//            cout << "Lines:" << endl;
            for(int j=0; j<particles.size(); j++){
                // A0 to B0, B1, etc. A1 to B0, B1, etc
                for(int k=0; k<connections[i].vorm->particles.size(); k++){
                    drawLines(particles[j], connections[i].vorm->particles[k]);
                }
            }
        }
    }
}

void JVorm::addConnection(JVorm* JVorm, bool repeat){ // repeat = true
    if(JVorm==this || JVorm == nullptr) // Can't connect with self or nullptr
        return;
    JVorm::connection c;

    // Add to own reference
    c.vorm = JVorm;
    c.connected = false;
    connections.push_back(c);
    // Add this to other reference
    if(repeat){
        JVorm->addConnection(this, false); // Second time, only the first 4 lines will be evaluated
    }
//    cout << "This: " << this << " is connected to: " << JVorm << endl;
}

JVorm::connection* JVorm::findSelf(JVorm* pointer){
    for(short i=0; i<connections.size(); i++){
        if(connections[i].vorm == pointer){
            return &connections[i];
        }
    }
return nullptr;
}

void JVorm::switchState(){
    for(short i=0; i<particles.size(); i++)
        particles[i]->state = !particles[i]->state;
}

void JVorm::addVector(vector<JVorm*>* v){
    this->connectionPointers = v;
}

void JVorm::removeFromVector(){
    if(v){
        for(int i=0; i<connectionPointers->size(); i++){
            if(connectionPointers->at(i) == this){
                connectionPointers->at(i) = nullptr; // Later remove the nullptrs in vec
//                cout << "Found self!" << endl;
                return;
            }
        }
    }
}

uint16 JVorm::getNumParticles(){
    return particles.size();
}

void JVorm::setZ(int z){
    for(short i=0; i<coordinates.size(); i++)
        coordinates[i].z = z;
}

void JVorm::setLoc(glm::vec3 newLoc){ // Get distance from new loc
    glm::vec3 diff = newLoc - loc;
    // Add this to all particles?
    for(short i=0; i<coordinates.size(); i++)
        coordinates[i] += diff;
    loc = newLoc;
}

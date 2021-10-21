//
//  Vorm.cpp
//  Shapes
//
//  Created by Jildert Viet on 15-02-15.
//

#include "Vorm.h"
typedef unsigned short uint16;
Vorm::~Vorm(){
//    cout << "Vorm " << vorm_index << " being deleted" << endl;
}

void Vorm::ownDtor(){
    removeFromVector();
    unConnect();
}

Vorm::Vorm(uint8 numSides, int sideDiv, float radius, ofVec2f loc, bool on_destination){
    setType("Vorm");
    this->radius = radius;
    
    this->loc = loc;
    loc = loc;
    size = ofVec2f(radius, radius);
    
    makeCoordinates(numSides, sideDiv, radius, on_destination); // Coordinates

    colors[0] = ofColor(255, 230);
    
    makeLinkTap("lijnmax", &lijnmax, ofVec2f(pow(40, 2), pow(110, 2)));
}

void Vorm::specificFunction(){
    for(uint16 i=0; i<particles.size(); i++){
        particles[i]->update();
    }
    if(deleteWhenOutOfSight){
        if(checkIfOutOfSight()){
            active = true; endTime = ofGetElapsedTimeMillis() + 3000;
            deleteWhenOutOfSight = false;
        }
    }
}

void Vorm::display(){
    ofSetColor(colors[0]);
    
//    ofPushMatrix();
//    ofTranslate(loc);
    connectParticles();

    if(showFrame){
        for(uint16 i=0; i<particles.size(); i++){
            particles[i]->display();
        }
        showFrame = false;
    }
    
    if(particlesVisible){
        for(uint16 i=0; i<particles.size(); i++){
            particles[i]->display();
        }
    }
//    ofPopMatrix();
}

void Vorm::displayShape(int mode, float division){
    if(shape_visible){
        ofSetPolyMode(OF_POLY_WINDING_NONZERO);
        switch (mode) {
            case 0:
                // Display shape
                ofSetColor(colors[0]);
                ofBeginShape();
                for(uint8 i=0; i<numSides; i++){
                    int index = i*sideDiv;
                    ofVertex(coordinates[index].x,coordinates[index].y);
                }
                ofEndShape();
                break;
                
            default:
                break;
            case 1:
                
                // Display shape in n triangles
                for(uint8 i=0; i<numSides; i++){
                    ofColor newcolor = colors[0];
                    newcolor.setBrightness(colors[0].a+(15*i));
                    ofSetColor(newcolor);
                    ofBeginShape();
                    int index = i*sideDiv;
                    int index_one_up = (i+1)*sideDiv;
                    float distance = loc.distance(coordinates[index]);
                    distance *= division;
                    if(index_one_up == particles.size())
                        index_one_up = 0;
                    ofVec2f tempVec = coordinates[index];
                    tempVec -= loc;
                    
                    tempVec.normalize();
//                    cout << distance << endl;
                    ofVertex(loc.x+(distance*tempVec.x), loc.y+(distance*tempVec.y));
                    ofVertex(coordinates[index].x, coordinates[index].y);
                    ofVertex(coordinates[index_one_up].x, coordinates[index_one_up].y);
                    ofEndShape();
                }
                
                for(uint8 i=0; i<numSides; i++){
                    ofColor newcolor = colors[0];
                    newcolor.setBrightness(colors[0].a+(15*(i+1)));
                    ofSetColor(newcolor);
                    ofBeginShape();
                    int index = i*sideDiv;
                    int index_one_up = (i+1)*sideDiv;
                    float distance = loc.distance(coordinates[index]);
                    distance *= division;
                    if(index_one_up == particles.size())
                        index_one_up = 0;
                    ofVec2f tempVec = coordinates[index];
                    tempVec -= loc;
                    
                    tempVec.normalize();
                    cout << distance << endl;
                    ofVertex(loc.x+(distance*tempVec.x), loc.y+(distance*tempVec.y));
                    ofVertex(loc.x, loc.y);
                    ofVertex(coordinates[index_one_up].x, coordinates[index_one_up].y);
                    ofEndShape();
                }

                break;
        }
    }
}

void Vorm::changeShape(uint8 numSides, int sideDiv, float r){ //particles.size() in ofApp veranderen met pointer
    coordinates.clear();
    makeCoordinates(numSides, sideDiv, r); // Only new coordinates
    short delta = abs(getNumParticles() - (numSides * sideDiv));
    if(numSides*sideDiv > coordinates.size()){
        for(short i = 0; i<delta; i++){ // Add particles
            particles.push_back(new Particle(&(coordinates[coordinates.size()-1-i])));
        }
    } else if(numSides*sideDiv < coordinates.size()){
        // Remove particles;
        short numParticles = getNumParticles();
        for(short i = 0; i<delta; i++){ // Add particles
            particles.erase(particles.begin() + numParticles - i);
        }
    } else{
        // Do nothing
    }
}

void Vorm::customOne(){
    makeCoordinates(customOneArguments[0], customOneArguments[1], customOneArguments[2], true);
}

void Vorm::customTwo(){
    // Place all particles at the border :)
    for(short i=0; i<particles.size(); i++){
        Particle* p = particles[i];
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

void Vorm::customThree(){
    for(short i=0; i<particles.size(); i++)
        particles[i]->addNoise = true;
}

void Vorm::customFour(){
    showFrame = true;
}

void Vorm::customFive(){
    instantFormVorm();
}

void Vorm::makeCoordinates(uint8 numSides, int sideDiv, float radius_, bool onDestination){
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
    
    switch(numSides){ // angle_offset is changed, so a rectangle will have horizontal and vertical sides
        case 4:
            angle_offset = 45;
            break;
        default:
            angle_offset = -90;
            break;
    }
    
    for(uint8 i=0; i<numSides; i++) { // calculate coordinates
        coordinates.push_back(
        ofVec2f(
            loc.x + (cos((angle_offset + (angle*i)) * (PI/180.)) * radius), // xPos
            loc.y + (sin((angle_offset + (angle*i)) * (PI/180.)) * radius) // yPos
            )
        );
        coordinates.back() *= circleRatio;
    }
    
    tussenCoordinaten(); // Places points on lines
    
    for(short i=0; i<coordinates.size(); i++){
        particles.push_back(new Particle(&(coordinates[i]), !onDestination));   // Particles
    }
}

void Vorm::setCenter(ofVec2f newloc){
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] -= loc;
    } // All around (0,0)
    
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] += newloc;
    }
}
void Vorm::addToCenter(ofVec2f add){
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] += add;
    }
}

void Vorm::changeRadius(float radius_){ // Could be improved by only adding radius increasement to coordinates ...
//    makeCoordinates(numSides, side_division, radius_);
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] -= ofGetWindowSize()/2.;
//        ofVec3f dir = coordinates[i];
//        dir = dir.normalize();
        float radiusDelta = radius_ / (float)this->radius;
        coordinates[i] *= (radiusDelta);
        coordinates[i] += ofGetWindowSize()/2.;
    }
    radius = radius_;
}

void Vorm::rotateCoordinates(int places, int direction){
    vector<ofVec3f> temp = coordinates;
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
    vector<ofVec3f>().swap(temp);
    rotating = true;
}

void Vorm::moveCorner(int corner_, ofVec2f distance_){
    coordinates[corner_] += distance_;
    tussenCoordinaten();
}

void Vorm::tussenCoordinaten(){
    for (uint8 i=0; i<numSides; i++) {
        for (uint8 j=0; j<sideDiv-1; j++) {
//            int index = numSides+(   (i*((int)side_division-1))+j ); // KLOPT
            
            ofVec2f delta = ofVec2f(coordinates[((i+1) % numSides)]) -  ofVec2f(coordinates[(i % numSides)]);
            delta = delta * ((1/sideDiv)*(j+1));
            coordinates.push_back(coordinates[i] + delta);
            // *(particlespointer+index) = new Particle(coordinates, index);
            // cout << "Coordinaten_" << index << " " << coordinates[index] << ", met i: " << i << " en j: " << j  << endl;
        }
    }
    vector<ofVec3f> temp;
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
    vector<ofVec3f>().swap(temp);
}

void Vorm::change_maxspeed(float maxspeed_){
    for(int i=0; i<particles.size(); i++)
        particles[i]->topspeed = maxspeed_;
}

void Vorm::switchConnectableness(){
    for(int i=0; i<particles.size(); i++){
        particles[i]->connectable = !particles[i]->connectable;
    }
}

void Vorm::change_maxspeed(float minspeed_, float maxspeed_){
    for(int i=0; i<particles.size(); i++){
        particles[i]->topspeed = ofRandom(minspeed_, maxspeed_);
    }
}

void Vorm::change_state_of_particles(bool state){
    for(int i=0; i<particles.size(); i++)
        particles[i]->state = state;
}

void Vorm::setState(bool state){
    for(int i=0; i<particles.size(); i++)
        particles[i]->state = state;
}

bool Vorm::checkIfInFormation(){
    float sum_distance = 0;
    if(ofGetFrameNum() % 2 == 1){
        for(int i=0; i<coordinates.size(); i++){
            float distance = particles[i]->loc.distance((*(*(particles[i])).destination));
            // cout << (*(*(particlespointer+array_index+i))).location << endl;
            // cout << (*(*(*(particlespointer+array_index+i))).destination) << endl;
            sum_distance += distance;
        }
        // cout << "Distance to destination for vorm " << vorm_index << " is: " << sum_distance/coordinates_amount     << endl;
        if(sum_distance/coordinates.size() < 4.){
            // return true;
            return true;
        } else{
            // return false;
            return false;
        }
    }
return false;
}

void Vorm::switchRadiusses(Vorm* vorm){
    if(!vorm)
        return;
    int radiusTemp = radius;
    changeRadius(vorm->radius);
    vorm->changeRadius(radiusTemp);
}

void Vorm::mirror(){
//    cout << "MIRROR" << endl;
    rotateCoordinates((particles.size())/2., true);
}

void Vorm::formVorm(){
    change_state_of_particles(1);
}

void Vorm::changeAngleOffset(float angle_){
    for(short i=0; i<coordinates.size(); i++){
        coordinates[i] -= loc;
        ofVec2f c = coordinates[i];
        c.rotate(angle_);
        coordinates[i] = c; // Klopt dit? ofVec2f naar ofVec3f, Z = 0 ?
        coordinates[i] += loc;
    }
}

bool Vorm::isConnectable(){
    for(int i=0; i<particles.size(); i++){
        if(particles[i]->connectable == false){
            return false;
        }
    }
    return true;
}

void Vorm::instantFormVorm(){
    for(int i=0; i<particles.size(); i++)
        particles[i]->locationIsDestination();
}

void Vorm::connectParticles(){
    // Connect own particles
    for(uint16 i=0; i<particles.size(); i++){
        for(uint16 j=i; j<particles.size(); j++){
            drawLines(particles[i], particles[j]);
        }
    }

    // Connect with particles of other vorm objects
    connectWith();
}

void Vorm::drawLines(Particle *p1, Particle *p2){
    float lijn_lengte = p1->loc.squareDistance( p2->loc );
    if(lijn_lengte < lijnmax && p1->connectable && p2->connectable){
        ofSetLineWidth(lineWidth);
        uint8 alpha = colors[0].a + alphaAdd;
        
        ofSetColor(colors[0].r, colors[0].g, colors[0].b,
                   ofMap(lijn_lengte, 0, lijnmax, alpha, 0));
        ofDrawLine(p1->loc, p2->loc);
    }
}

void Vorm::unConnect(){ // Say to all connected objects: remove me from your vector
    for(uint16 i=0; i<connections.size(); i++){
        Vorm* v = connections[i].vorm;
        v->removeConnection(this);
    }
}

void Vorm::removeConnection(Vorm* v){
    int index; // Index to remove
    for(int i=0; i<connections.size(); i++){
        Vorm* vorm = connections[i].vorm;
        if(vorm == v){
            index = i;
        }
    }
    if(connections.size())
        connections.erase(connections.begin() + index);
}

void Vorm::connectWith(){
    // If A and B are not connected: draw lines from A to B, set both to 'connected' on both objects. B will do nothing with A. 
    for(int i=0; i<connections.size(); i++){
        if(connections[i].connected == false){
            // DRAW LINES (A to B)
            bDrawLines = true;
//            cout << this << " bDrawLines = true" << endl;
            
            // CONNECT
            // Returns the pointer from the other object, pointing to this object   (A finds A* in B)
            Vorm::connection* b = connections[i].vorm->findSelf(this);
            
//            cout << "B points to A with adress: " << b->vorm << " is this " << this << endl;
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

void Vorm::addConnection(Vorm* vorm, bool repeat){ // repeat = true
    if(vorm==this || vorm == nullptr) // Can't connect with self or nullptr
        return;
    Vorm::connection c;
    
    // Add to own reference
    c.vorm = vorm;
    c.connected = false;
    connections.push_back(c);
    // Add this to other reference
    if(repeat){
        vorm->addConnection(this, false); // Second time, only the first 4 lines will be evaluated
    }
//    cout << "This: " << this << " is connected to: " << vorm << endl;
}

Vorm::connection* Vorm::findSelf(Vorm* pointer){
    for(short i=0; i<connections.size(); i++){
        if(connections[i].vorm == pointer){
            return &connections[i];
        }
    }
return nullptr;
}

void Vorm::switchState(){
    for(short i=0; i<particles.size(); i++){
        particles[i]->state = !particles[i]->state;
    }
}

void Vorm::addNoiseToAllParticles(){
    for(short i=0; i<particles.size(); i++){
        particles[i]->addNoise = true;
    }
}

void Vorm::makeVertLine(int x){
    for(int i=0; i<particles.size(); i++){
        coordinates[i] = ofVec2f(x, i * (ofGetWindowHeight()/(particles.size()-1)));
    }
}

bool Vorm::checkIfOutOfSight(){
    bool outOfSight = false;
    for(int i=0; i<particles.size(); i++){
        if(particles[i]->loc.x > ofGetWindowWidth()){
            outOfSight = true;
        } else{
            return false;
        }
        return outOfSight;
    }
return false;
}

void Vorm::addVector(vector<Vorm*>* v){
    this->v = v;
}

void Vorm::removeFromVector(){
    if(v){
        for(int i=0; i<v->size(); i++){
            if(v->at(i) == this){
                v->at(i) = nullptr; // Later remove the nullptrs in vec
//                cout << "Found self!" << endl;
                return;
            }
        }
    }
}

uint16 Vorm::getNumParticles(){
    return particles.size();
}

void Vorm::setZ(int z){
    for(short i=0; i<coordinates.size(); i++)
        coordinates[i].z = z;
}

void Vorm::setLoc(ofVec3f newLoc){ // Get distance from new loc
    ofVec3f diff = newLoc - loc;
    // Add this to all particles?
    for(short i=0; i<coordinates.size(); i++)
        coordinates[i] += diff;
}

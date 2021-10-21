//
//  JVideoPlayer.cpp
//
//  Created by Jildert Viet on 07-05-16.
//
// Edit: 15-01-2021: removed ofxOpenCv things to compile on Linux (trouble installing OpenCV there), and since it's not really needed here

#include "JVideoPlayer.hpp"

JVideoPlayer::JVideoPlayer(){
    setType("VideoPlayer");
    colors[0] = ofColor(255);
    
    setMode(JVIDEO_MODE_NORMAL);
    
//    ofSetFullscreen(true);
    
    font.load("Courier New Bold.ttf", 9);
    asciiCharacters =  string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
}

void JVideoPlayer::ownDtor(){
    player.closeMovie();
}

bool JVideoPlayer::load(string path){
    player.setLoopState(OF_LOOP_NORMAL);
    player.setPixelFormat(OF_PIXELS_RGB); // Needed?
    player.load(path);
    player.setVolume(0);
//    frame.allocate(player.getWidth(), player.getHeight());
    
    // HELLO!?
    play(0);
    playing = true;
return true;
}

void JVideoPlayer::customOne(){load(path);}
void JVideoPlayer::customTwo(){
    player.setPosition(customOneArguments[0]);
}

void JVideoPlayer::specificFunction(){
    if(!player.isLoaded())
        return;
    if(playing){
        if(loop){
            if(player.getCurrentFrame()>loopPoints[1]){
                player.setFrame(loopPoints[0]);
            }
        }
        
        for(int i=0; i<bins.size(); i++){
            bins[i]->update();
            bins[i]->specificFunction();
        }
        
        player.update();

        if(player.getIsMovieDone()){
            active = true;
        }
    }
}

void JVideoPlayer::display(){
    if(!player.isLoaded())
        return;
    if(playing){
        ofSetColor(colors[0]);
//        ofSetColor(ofColor(255,100));
//        ofDrawRectangle(0, 0, 100, 100);
        switch(mode){
            case JVIDEO_MODE_NORMAL: // Normal mode
                displayNormal();
            break; // Cut mode
            case JVIDEO_MODE_BINS:
                displayCut();
            break;
            case JVIDEO_MODE_ASCII:{
                displayNormal();
                ofPixelsRef pixelsRef = player.getPixels();
                
                ofSetHexColor(0xffffff);
                ofPushMatrix();
                ofRotateXDeg(180);
                ofTranslate(0, -ofGetHeight(), 0);
                for (int i = 0; i < player.getWidth(); i+= 7){
                    for (int j = 0; j < player.getHeight(); j+= 9){
                        // get the pixel and its lightness (lightness is the average of its RGB values)
                        float lightness = pixelsRef.getColor(i,j).getLightness();
                        // calculate the index of the character from our asciiCharacters array
                        int character = powf( ofMap(lightness, 0, 255, 0, 1), 2.5) * asciiCharacters.size();
                        // draw the character at the correct location
                        font.drawString(ofToString(asciiCharacters[character]), i, j);
                    }
                }
                ofPopMatrix();
            }
                break;
        }
        
    }
}

void JVideoPlayer::displayNormal(){
//    ofPushMatrix();
    
//    ofTranslate(loc + ofVec2f(0, -ofGetHeight()));
//    ofRotateXDeg(180);
    ofSetColor(colors[0]);
//    ofDrawRectangle(0, 0, 400, 400);
    player.draw(0, 0, size.x, size.y);
//    ofPopMatrix();
}

void JVideoPlayer::displayCut(){
//    frame.setFromPixels(player.getPixels());
    for(int i=0; i<bins.size(); i++){
        bins[i]->display();
    }
    
//    ofSetColor(ofColor::red);
//    ofDrawRectangle(100, 100, 3500, 2000);
}

void JVideoPlayer::play(float pct){
    playing = true;
    player.setPosition(pct);
    player.play();
}

void JVideoPlayer::setMode(int mode){
    switch(mode){
        case 0:
            this->mode = 0;
        break;
        case 1:
            this->mode = 1;
            setBins(7);
        break;
        default:
            this->mode = mode;
            break;
    }
}

void JVideoPlayer::setBins(int numBins){
//    bins.clear();
//    int binWidth = ofGetWindowWidth()/numBins;
////    int binWidth = player.getWidth()/numBins;
//
//    vector<int> xPosSource;
//    for(int i=0; i<numBins; i++){
//        xPosSource.push_back(i*binWidth);
//    }
//    //    Shuffle bins
//    auto engine = default_random_engine{};
//    shuffle(begin(xPosSource), end(xPosSource), engine);
//
//    for(int i=0; i<numBins; i++){
//        bins.push_back(new Bin(&frame, binWidth*i, xPosSource[i], binWidth));
//    }
}

//void VideoPlayer::switchRandomBins(){
//    int one, second;
//    one = second = ofRandom(xPos.size());
//    while(second==one){
//        second = ofRandom(xPos.size());
//    }
//    cout << one << " " << second << endl;
//    int tempXPos = xPos[one];
//    xPos[one] = xPos[second];
//    xPos[second] = tempXPos;
//}

vector<int> JVideoPlayer::chooseTwoRandomBins(){
    int one, second;
    one = ofRandom(bins.size());
    second = ofRandom(bins.size());
    while(second==one){
        second = ofRandom(bins.size());
    }
    vector<int> result { one, second };
    return result;
}

void JVideoPlayer::switchRandomBins(){
    cout << "switchRandomBins" << endl;
    switchBins(chooseTwoRandomBins());
}

void JVideoPlayer::switchBins(vector<int> array){
    // Only if no bin is already switching
    if(!(bins[array[0]]->envelopes.size()) && !(bins[array[1]]->envelopes.size())){
        bins[array[0]]->doSwitch(bins[array[1]]->xPos);
        bins[array[1]]->doSwitch(bins[array[0]]->xPos);
    }
}

void JVideoPlayer::randomMirror(bool h, bool v){
    Bin* bin = randomBin();
    if(h)
        bin->bMirrorH = !bin->bMirrorH;
    if(v)
        bin->bMirrorV = !bin->bMirrorV;
    if(h&&v){
        bin->bMirrorH = !bin->bMirrorH;
        bin->bMirrorV = !bin->bMirrorV;
    }
}

void JVideoPlayer::allRandomBrightness(int min, int max){
    for(int i=0; i<bins.size(); i++){
        bins[i]->colors[0].a = ofRandom(min, max);
    }
}

void JVideoPlayer::randomFade(){
    for(int i=0; i<bins.size(); i++){
        bins[i]->doFade(ofRandom(100,1000), 0, ofRandom(100,1000));
    }
}

void JVideoPlayer::startFromLoopPoint(){
    player.setFrame(loopPoints[0]);
}

void JVideoPlayer::switchDilate(bool all){
    int dilateFactor = ofRandom(1, 6);
    if(all){
        for(int i=0; i<bins.size(); i++){
            bins[i]->bDilate = !(bins[i]->bDilate);
            bins[i]->dilateFactor = dilateFactor;
        }
    } else{
        Bin* b = bins[ofRandom(bins.size())];
        b->bDilate = !(b->bDilate);
        b->dilateFactor = dilateFactor;
    }
}

void JVideoPlayer::disableVerticalMirror(){
    for(int i=0; i<bins.size(); i++){
        bins[i]->bMirrorV = false;
    }
}


void JVideoPlayer::setLoopPoint(int point){
    loopPoints[point] = player.getCurrentFrame();
    cout << "LoopPoint " << point << " is: " << loopPoints[point] << endl;
}

Bin* JVideoPlayer::getRandomBin(){
    return bins[ofRandom(bins.size())];
}

void JVideoPlayer::switchBinState(){
    Bin* b = getRandomBin();
    b->bDisplay = !b->bDisplay;
}

void JVideoPlayer::switchBinColor(){
    Bin* b = getRandomBin();
    b->bGray = !b->bGray;
}

void JVideoPlayer::switchAllBinColor(){
    for(int i=0; i<bins.size(); i++){
        bins[i]->bGray = !bins[i]->bGray;
    }
}

void JVideoPlayer::mirrorAllBins(bool h, bool v){
    for(int i=0; i<bins.size(); i++){
        bins[i]->bMirrorH = !bins[i]->bMirrorH;
        bins[i]->bMirrorV = !bins[i]->bMirrorV;
    }
}

void JVideoPlayer::allBinsVisible(){
    for(int i=0; i<bins.size(); i++){
        bins[i]->bDisplay = true;
    }
}

void JVideoPlayer::disableOddBins(){
    for(int i=0; i<bins.size(); i+=2){
        bins[i]->bDisplay = false;
    }
}

void JVideoPlayer::fadePhase(){
    for(int i=0; i<bins.size(); i++){
        float ratio = (1./bins.size()*i);
        bins[i]->doFade(1000 - (800*ratio), 0, 200+(ratio*ofRandom(1000,2000)));
    }
}










Bin* JVideoPlayer::randomBin(){
    return bins[ofRandom(bins.size())];
}

void Bin::display(){
//    if(!bDisplay)
//        return;
//
//    frame->setROI(xPosSource, 0, binWidth, frame->getHeight());
//    ofSetColor(colors[0]);
//    if(bMirrorH)
//        frame->mirror(0, 1);
//    if(bMirrorV)
//        frame->mirror(1, 0);
//    if(bMirrorV && bMirrorH)
//        frame->mirror(1, 1);
//
////    frame->blur(10);
//
//    if(bDilate)
//        frame->dilate(); // Used to have 'dilateFactor' as arg
//    if(bGray){
//        ofxCvGrayscaleImage gray;
//        gray.allocate(binWidth, ofGetWindowHeight());
//        gray.setFromPixels(frame->getRoiPixels().getChannel(0));
//        gray.draw(xPos, 0);
//    } else{
//        frame->drawROI(xPos, 0, binWidth, ofGetWindowHeight());
////        frame->drawROI(xPos, 0, binWidth, frame->getHeight());
//
//
////        ofSetColor(xPos, binWidth, 200);
////        ofDrawRectangle(xPos, 0, binWidth, frame->getHeight());
//    }
//
//    // Undor mirror
//    frame->mirror(0, 0);
}

//Bin::Bin(ofxCvColorImage* frame, int xPos, int xPosSource, int binWidth){
//    this->frame = frame;
//    this->xPos = xPos;
//    this->binWidth = binWidth;
//    this->xPosSource = xPosSource;
//    colors[0] = ofColor(255);
//}

void Bin::doFade(float a, float s, float r){
    vector<float> values = {(float)colors[0].a, 0, 0, 255};
    vector<float> times = {a, s, r};
    addEnv(values, times, &colors[0]);
    
//    if(envelope==nullptr){
//        envelope = new Envelope(a,s,r);
//        envelope->triggerEnv();
//    }
}

void Bin::specificFunction(){
    if(bSwitch && envelopes.size() == 0){
        vector<float> values = {0, 0, 255};
        vector<float> times = {1, 200};
        addEnv(values, times, &colors[0]);
        
        xPos = newXPos;
        bSwitch = false;
    }
}

void Bin::doSwitch(int x){
    bSwitch = true;
    doFade(10,100,300);
    newXPos = x;
}


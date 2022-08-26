//
//  VideoPlayer.hpp
//  Bas
//
//  Created by Jildert Viet on 07-05-16.
//
//

#ifndef JVideoPlayer_hpp
#define JVideoPlayer_hpp

#include <stdio.h>
#include "ofMain.h"
#include "JEvent.hpp"
#include <algorithm>
#include <random>
//#include "ofxOpenCv.h"
#include "Env.hpp"

class Bin: public JEvent{
public:
    Bin(){};
//    Bin(ofxCvColorImage* frame, int xPos, int xPosSource, int binWidth);
    void specificFunction();
    void doFade(float a=100, float s=0, float r=200);
    int binWidth;
    int xPos;
    
    int newXPos;
    void doSwitch(int x);
    bool bSwitch = false;
    
    int xPosSource;
    void display();
    
//    Env* envelope = nullptr;
    bool bMirrorH = false; bool bMirrorV = false;
    
    int brightness = 255;
//    ofxCvColorImage* frame;
    
    bool bDilate = false;
    int dilateFactor = 4;
    
    bool bGray = false;
    bool bDisplay = true;
};


//============================================================================================================
enum{
    JVIDEO_MODE_NORMAL,
    JVIDEO_MODE_BINS,
    JVIDEO_MODE_ASCII
};

class JVideoPlayer: public JEvent{
public:
    JVideoPlayer();
    
    string path;
    bool load(string path);
    
    void ownDtor() override;
    void display() override;
    void specificFunction() override;
    
    ofVideoPlayer player;
    
    bool playing = false;
    void play(float pct = 0);
    
    void displayNormal();
    void displayCut();
    char mode = JVIDEO_MODE_NORMAL;
    
    void setMode(int mode);
    void setBins(int numBins);
    Bin* randomBin();
    
    // Modulation
    vector<int> chooseTwoRandomBins();
    void switchBins(vector<int> bins);
    void switchRandomBins();
    void allRandomBrightness(int min=0, int max=255);
    
    void switchDilate(bool all=true);
    
    void randomMirror(bool h, bool v);
    void randomFade();
    
    void switchBinState();
    void allBinsVisible();
    void switchBinColor();
    
    void switchAllBinColor();
    void mirrorAllBins(bool h, bool v);
    void disableVerticalMirror();
    
    void disableOddBins();
    
    vector<Bin*> bins;
    Bin* getRandomBin();
    
    bool loop = false;
    vector<int> loopPoints {0,60};
    void startFromLoopPoint();
    void setLoopPoint(int point=0);
    
    void fadePhase();
    
//    ofxCvColorImage frame;
    
    string asciiCharacters;
    ofTrueTypeFont  font;
    
    void customOne() override;
    void customTwo() override;
};

#endif /* JVideoPlayer_hpp */

//  Created by Jildert Viet on 24-01-16.

#ifndef ofxJVisuals_hpp
#define ofxJVisuals_hpp

#define NUMLAYERS   7
#define MAX_EVENTS_PTRS 1025

#define USE_PP  false // ofxPostProcessing

#ifndef TARGET_RASPBERRY_PI
//    #define USE_PARTICLES true // Comment this to exclude the particles
#endif

#ifdef USE_PARTICLES
//    #include "particleSystem.hpp"
#endif

#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"

#if USE_PP
    #include "ofxPostProcessing.h"
#endif

#include "Event.hpp"
#include "Vorm.h"
#include "JNoise.hpp"
#include "JImage.hpp"
#include "SpaceCube.h"
#include "JMirror.h"
#include "MultiMesh.hpp"
#include "MultiMeshMaybeTomorrow.hpp"
#include "JEllipse.h"
#include "JRectangle.hpp"
#include "RectangleMask.hpp"

#include "AlphaBlackScreen.hpp"
#include "Mapper.hpp"
//#include "Wavetables.hpp"
#include "Env.hpp"
#include "cameraController.hpp"

#include "JPhysarum.hpp"

enum VisualizerLayer {
    FUNCTIONAL,
    NON_CAM_BACK,
    NON_CAM_FRONT,
    NEGATIVE,
    DEFAULT
};

class Bus;
class JLayer: public Event{
public:
    JLayer(){};
};

class ofxJVisuals{
public:
typedef unsigned char uint8;
    ofxJVisuals(glm::vec2 size);
    ~ofxJVisuals();
    ofFbo fbo, renderFbo;
    void deconstructor();
    void update();
    void display();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    
    glm::vec2 size;
    
    void setAlpha(int alpha, bool bDo = true);
    void setBrightness(unsigned char b);
    int brightness = 255;
    
    void killAll();

    int numEvents = 0;
    Event* last;
    ofxOscMessage getAllEvents();
    
    Event* events[MAX_EVENTS_PTRS];
    void getFreePointers();
    unsigned short numEventsAdded = 0;
    
    Event* addEvent(Event* e, int layerIndex=1, unsigned short index = 0);
    Event* addEvent(Event* e, VisualizerLayer l, unsigned short index = 0);
    
    Event* getLast(){return last;}
    Event* getEventById(int id);
    Event* lastCalled = nullptr;
    
    static bool checkIfNull(Event* e);
    static bool checkIfNullM(Mapper* m);
    
    bool bMirror = false;
    bool bAddMirror = false;

//    void fitFadeScreen(glm::vec2 size = glm::vec2(0, 0));
    void makeFit(glm::vec2 size);
    
    AlphaBlackScreen* alphaScreen = nullptr;
    
    vector<Mapper*> mappers;
    void addMapper(Mapper* m);
    
    vector<Bus*> busses;
    
    // PostProcessing
#if USE_PP
    ofxPostProcessing post;
    ContrastPass::Ptr contrastPP = nullptr;
    ZoomBlurPass::Ptr zoomPP = nullptr;
    RGBShiftPass::Ptr rgbPP = nullptr;
    FxaaPass::Ptr fxaaPP = nullptr;
    NoiseWarpPass::Ptr noisePP = nullptr;
    
    bool bPostProcessing = false;
#endif
    
    bool bRotate = false;
    float rotationAngle = 0;
    float rotationAngleIcrement = 0.1;
    
    ofxOscSender* SCsender;
    ofxOscReceiver receiver;
    
//    vector<Pointer*> receivingPointers;
    vector<float> vec(float a);
    vector<float> vec(float a, float b);
    vector<float> vec(float a, float b, float c);
    vector<float> vec(float a, float b, float c, float d);
    vector<float> vec(float a, float b, float c, float d, float e);

//    Wavetables wavetable;
    
    bool bMask = false;
    ofImage mask;
    unsigned char maskBrightness = 0;
    bool bCam = true;
    ofEasyCam cam;
    cameraController* camController;
    void initCam();
    
    ofTrueTypeFont verdana30;
    
//    ofFbo sharedFbo, sharedFbo2;
    ofFbo circularMask;
    bool bDrawCirclularMask = false;
    void drawMask();
    
    void initCircularMaskFbo(glm::vec2 size, int num=1);
    
    bool bNegativeShader = true;
    ofFbo negativeMask;
    bool bEditMode = false;
    char fboDisplayMode = 1;
    vector<glm::vec3> meshVertices;
    vector<glm::vec2> texCoords;
    ofMesh mesh;
    void initMesh();
private:
    void loadLastMaskFile();
    JLayer* layers[NUMLAYERS]; // linked list, use AddEvent to add
    JLayer negativeLayer;
    ofShader negative;
};

class Bus{
public:
    Bus(){
        tap = new linkTap("value", &value, ofVec2f(0., 1.0));
    }
    float value = 0.5;
    linkTap* tap;
};

#endif /* ofxJVisuals_hpp */

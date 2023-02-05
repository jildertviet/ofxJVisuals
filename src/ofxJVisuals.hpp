//  Created by Jildert Viet on 24-01-16.

#ifndef ofxJVisuals_hpp
#define ofxJVisuals_hpp

#include "config.h"

#if USE_OPENCL
   #include "particleSystemOpenCL.hpp"
#else
   #include "particleSystem.h"
#endif

#include <stdio.h>
#include "ofMain.h"

#ifdef  USE_SC // SC link is handled in MsgParser
#include "ofxOscBidirectional.h"
#else
#include "ofxOsc.h"
#endif

#if USE_PP
    #include "ofxPostProcessing.h"
#endif

#include "JEvent.hpp"
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
//#include "Wavetables.hpp"
#include "Env.hpp"
#include "cameraController.hpp"

#ifndef TARGET_RASPBERRY_PI
    #include "JPhysarum.hpp"
#else
    class JPhysarum;
#endif

#include "MsgParser.hpp"

enum VisualizerLayer {
    FUNCTIONAL, // Should be deleted?
    NON_CAM_BACK,
    NON_CAM_FRONT,
    NEGATIVE,
    DEFAULT
};

class JLayer: public JEvent{ // What does it do?
public:
    JLayer(){};
};

class ofxJVisuals{
public:
typedef unsigned char uint8;
    ofxJVisuals(glm::vec2 size = glm::vec2(1920,1080), bool bUseSC=true);
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
    JEvent* last;
    ofxOscMessage getAllEvents();

    JEvent* events[MAX_EVENTS_PTRS];
    void getFreePointers(string ip, int port);
    unsigned short numEventsAdded = 0;

    JEvent* addEvent(JEvent* e, int layerIndex=1, unsigned short index = 0);
    JEvent* addEvent(JEvent* e, VisualizerLayer l, unsigned short index = 0);

    JEvent* getLast(){return last;}
    JEvent* getEventById(int id);
    JEvent* lastCalled = nullptr;

    static bool checkIfNull(JEvent* e);

    bool bMirror = false;
    bool bAddMirror = false;

//    void fitFadeScreen(glm::vec2 size = glm::vec2(0, 0));
    void makeFit(glm::vec2 size);

    AlphaBlackScreen* alphaScreen = nullptr;

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

    MsgParser* msgParser;
    ofxOscSender* SCsender;
    ofxOscReceiver receiver;
//
////    vector<Pointer*> receivingPointers;
//    vector<float> vec(float a);
//    vector<float> vec(float a, float b);
//    vector<float> vec(float a, float b, float c);
//    vector<float> vec(float a, float b, float c, float d);
//    vector<float> vec(float a, float b, float c, float d, float e);

//    Wavetables wavetable;

    bool bMask = false;
    ofImage mask;
    unsigned char maskBrightness = 0;
    bool bCam = true;
    ofEasyCam cam;
    cameraController* camController;
    void initCam();

    ofTrueTypeFont avenir30;
    ofTrueTypeFont avenir300;
    ofTrueTypeFont* selectedFont;

//    ofFbo sharedFbo, sharedFbo2;
    ofFbo circularMask;
    bool bDrawCirclularMask = false;
    void drawMask();

    void initCircularMaskFbo(glm::vec2 size, int num=1);

    ofFbo negativeMask;
    bool bEditMode = false;
    char fboDisplayMode = 1;
    vector<glm::vec3> meshVertices;
    vector<glm::vec2> texCoords;
    ofMesh mesh;
    void initMesh();
    vector<JShader*> shaders;
private:
    void loadLastMaskFile();
    JLayer* layers[NUMLAYERS]; // linked list, use AddEvent to add
    JLayer negativeLayer;
    ofShader negative;
};

#endif /* ofxJVisuals_hpp */

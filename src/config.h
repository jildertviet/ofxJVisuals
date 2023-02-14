//
//  config.h
//  exampleSimple
//
//  Created by Jildert Viet on 24/07/2022.
//

#ifndef config_h
#define config_h

// Visuals
#define NUMLAYERS   7
#define MAX_EVENTS_PTRS 1024

#define USE_PP  false // ofxPostProcessing

#define USE_OPENCL false // Comment this to exclude the particles (RPI?)

// SuperCollider
#define USE_SC  true
#ifdef __APPLE__
#define SC_FOLDER "~/../../Applications/SuperCollider.app/Contents/Resources/"
#else
#define SC_FOLDER "/usr/local/bin/"
#endif
#define SC_PORT 9993

#endif /* config_h */

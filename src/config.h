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
#define MAX_EVENTS_PTRS 1025

#define USE_PP  false // ofxPostProcessing

// SuperCollider
#define USE_SC  true
#ifdef __APPLE__
#define SC_FOLDER "~/../../Applications/SuperCollider.app/Contents/Resources/"
#else
#define SC_FOLDER "/usr/bin/"
#endif
#define SC_PORT 9999

#endif /* config_h */

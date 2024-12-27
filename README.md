ofxJVisuals
=====================================

Introduction
------------

<img src="images/preview.png" alt="ofxJVisuals preview" width="400" style="display: block; margin: auto"/>  

For now this is still a non-released addon for creating visuals with SuperCollider and openFrameworks. 

### Workflow
I launch an ofApp similar to _exampleSimple_.
This ofApp creates a SuperCollider server in a thread. 

In sclang (I'm running SCnvim) I create a remote server, with the UDP-address I've set in src/config.h.

Now I can create a synth that uses my UGens from [J_Ugen](https://github.com/jildertviet/J_Ugen/). 
This is very undocumented, but some of these UGens have some helpfiles...

These UGens let the server send init-OSC messages ("/create") to OF and create the required objects (JEvent).
They also update the state of the UGen via OSC on a framerate set in the UGen src. 

Dependencies
------------
- ofxJVisuals
- [ofxOscBidirectional](https://github.com/elgiano/ofxOscBidirectional.git)

Mac (2024: not sure if this is outdated)

- [ofxSyphon](https://github.com/astellato/ofxSyphon)
- [ofxMSAPingPong](https://github.com/memo/ofxMSAPingPong)
- [ofxMSAOpenCL](https://github.com/memo/ofxMSAOpenCL)
Previously:
- [ofxEasing](https://github.com/arturoc/ofxEasing)
- [ofxPostProcessing](https://github.com/neilmendoza/ofxPostProcessing)

Standard in OF:  

- ofxGui
- ofxJSON
- ofxMidi
- ofxOpenCv
- ofxAssimpModelLoader

Compatibility
------------
Mainly developed on Linux. Sometimes on Mac. Sometimes on Raspberry Pi.
Never on Windows.

Known issues
------------

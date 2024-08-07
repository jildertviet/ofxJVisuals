//
//  scSynth.h
//
//  Created by Jildert Viet on 17/07/2022.
//

#ifndef scSynth_h
#define scSynth_h

#include "config.h"
#include "ofMain.h"
#include <atomic>

class scSynth : public ofThread {
public:
  string r = "";
  bool bDoOnce = false;
  ~scSynth() {
    stop();
    waitForThread(true);
  }

  void setup() {}

  void start() { startThread(); }

  void stop() {
    std::unique_lock<std::mutex> lck(
        mutex); // Test to see if this does something
    stopThread();
    cout << r << endl;
    //        condition.notify_all();
  }

  void threadedFunction() {
    while (isThreadRunning()) {
      if (!bDoOnce) {
        bDoOnce = true;
        cout << "Starting a SuperCollider server in the thread" << endl;
        vector<ofSoundDevice> devices = ofSoundStreamListDevices();
#ifndef __APPLE__
        ofSystem("export SC_JACK_DEFAULT_INPUTS=system; export "
                 "SC_JACK_DEFAULT_OUTPUTS=system");
#endif
        string cmd = "pw-jack " + ofToString(SC_FOLDER) + "scsynth -u " +
                     ofToString(SC_PORT) + " -l 4 -i 0";
        // for(auto d : devices){
        // if(ofStringTimesInString(d.name, "WH-1000XM4")){
        // cmd += " -H Sony";
        // }
        // }
        cout << cmd << endl;
        r = ofSystem(cmd.c_str());
      }
    }
  }

  void update() {}
};

#endif /* scSynth_h */

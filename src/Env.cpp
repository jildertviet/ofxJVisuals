//
//  Env.cpp
//  Visualizer_2_4
//
//  Created by Jildert Viet on 25-09-17.
//
//

#include "Env.hpp"

Env::Env(){
    setStartTime();
}

Env::Env(vector<float> levels, vector<float> times) : levels(levels), times(times){
    setStartTime();
    totalRunTime = times[0];
    getDirection();
}

Env::~Env(){
    // Save the buffer
    if(bSave){
        ofFile f;
        if(f.doesFileExist(fileName)){
            fileName = ofSplitString(fileName, ".txt")[0];
            fileName += "_";
            fileName += ofToString(*parentID);
            fileName += ".txt";
        }
        f.create(fileName); // What if there's already a file w/ that name?
        f.open(fileName, ofFile::Mode::WriteOnly);

        string toSave = ofToString(*parentID);
        toSave += ",";
        for(int i=0; i<saveBufferWritePos; i++){
            toSave += ofToString(saveBuffer[i]);
            if(i < saveBufferWritePos - 1)
                toSave += ",";
        }

        ofBuffer b;
        b.append(toSave);

        f.writeFromBuffer(b);
        f.close();
    }

    if(saveBuffer)
        delete saveBuffer;
}

void Env::trigger(vector<float> levels, vector<float> times){
    this->levels = levels; this->times = times;
    setStartTime();
    totalRunTime = times[0]; getDirection();
    active = true; timesIndex = 0;
}

Env::Env(vector<float> levels, vector<float> times, float* f, char curve){
    this->levels = levels; this->times = times; ptr = new Pointer(f); this->curve = curve;
    setStartTime();

    totalRunTime = times[0]; getDirection();
}

Env::Env(vector<float> levels, vector<float> times, int* i, char curve){
    this->levels = levels; this->times = times; ptr = new Pointer(i); this->curve = curve;
    setStartTime();
    totalRunTime = times[0]; getDirection();
}

Env::Env(vector<float> levels, vector<float> times, ofColor* c, char curve){
    this->levels = levels; this->times = times; ptr = new Pointer(c); this->curve = curve;
    setStartTime();
    totalRunTime = times[0]; getDirection();
}

#ifdef  ENV_EASING
Env::Env(vector<float> levels, vector<float> times, float* f, ofxeasing::function func){
    this->levels = levels; this->times = times; ptr = new Pointer(f);
    setStartTime();

    totalRunTime = times[0]; getDirection();
    this->curveType = func;
    bEasingSet = true;
}
#endif

bool Env::process(bool bWrite){
    if(active){
//        cout << "timesIndex: " << timesIndex << endl;
//        cout << times[timesIndex] << endl;
        if(getTime() > startTime + totalRunTime){
            timesIndex++;
            if(timesIndex >= times.size()){
                value = levels[timesIndex];
                if(ptr && bWrite)
                    ptr->writeValue(value);
                if(!loop){
                    active = false;
                    value = levels.back();
                    if(ptr && bWrite)
                        ptr->writeValue(value);
                    if(bSave){
                        saveValueToBuffer(value);
                    }
                    bDone = true;
                    return false;
                } else{
                    timesIndex = 0;
                    setStartTime();
                    totalRunTime = times[0]; getDirection();
                    return true;
                }
            }
//            getDirection(timesIndex);
            totalRunTime += times[timesIndex];
//            cout << "totalRunTime: " << totalRunTime << endl;
        }

        // Get ratio:
        float ratio;
        ratio = ((int)getTime() - startTime - totalRunTime + times[timesIndex]);
        ratio /= times[timesIndex];

        float output;
        if(bEasingSet){
#ifdef  ENV_EASING
            output = levels[timesIndex] + ofxeasing::map_clamp(ratio, 0., 1., 0, levels[timesIndex+1]-levels[timesIndex], curveType);
#endif
        } else{
            output = levels[timesIndex] + ofMap(ratio, 0., 1., 0., levels[timesIndex+1]-levels[timesIndex]);
        }

        if(ptr && bWrite)
            ptr->writeValue(output);
        value = output;

//        cout << (int)id << " " << value << endl;

        if(bSave)
            saveValueToBuffer(output);
    }
    return true;
}

void Env::getDirection(uint8 ti){
    if(levels[ti] - levels[ti+1] < 0){
        // Direction is up
        direction = 1;
    } else{
        direction = 0;
    }
//    cout << "direction: " << direction << endl;
}

void Env::setLoop(bool b){
    loop = b;
}

void Pointer::writeValue(float value){
    if(valF){
        *valF = value;
        //        cout << "Write float to: " << value << endl;
    } else if(valI){
        *valI = value;
    } else if(valC){
        ofColor c = *valC;
        c.a = value;
        //        cout << c << endl;
        (*valC) = c;
    }
}

void* Pointer::getPtr(){
    if(valF){
        return valF;
    } else if(valI){
        return valI;
    } else if(valC){
        return valC;
    }
return nullptr;
}

void Env::setSave(){
    /*
     If times = [1000]
     totalTime = 1000 in ms
     totalTime = 1 in sec
     numFrames = 1 * frameRate = 60 frames
     */

    fileName = "envExport/";
//    fileName += ofGetFrameNumStringWithZeroes();
    fileName += ".txt";
    cout << "toSave env: " << fileName << endl;

    // Add zeroes to it
    // 0300env0.txt
    // How to identify the env? Env-id? Parent-id? <- better. Set custom fileName w/ setSave()? <-better?
    bSave = true;
    float totalTime = 0 ;
    for(int i=0; i<times.size(); i++)
        totalTime += times[i]; // Is now in ms
    totalTime /= 1000.; // Now in seconds
    saveBufferSize  = totalTime * ofGetTargetFrameRate();
    saveBufferSize += 30; // Overhead

    saveBuffer = new float[saveBufferSize];
}

void Env::saveValueToBuffer(float v){
    if(saveBufferWritePos < saveBufferSize){
        saveBuffer[saveBufferWritePos] = v;
        saveBufferWritePos++;
    } else{
        cout << "not recording anymore" << endl;
    }
}

void Env::setStartTime(){
    if(USE_FRAMES_AS_CLOCK){
        startTime = ofGetFrameNum() * (1000./ofGetTargetFrameRate());
    } else{
        startTime = ofGetElapsedTimeMillis();
    }
}
unsigned long Env::getTime(){
    if(USE_FRAMES_AS_CLOCK){
        return ofGetFrameNum() * (1000./ofGetTargetFrameRate());
    } else{
        return ofGetElapsedTimeMillis();
    }
}

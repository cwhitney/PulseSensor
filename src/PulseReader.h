//
//  PulseReader.h
//  PulseSense
//
//  Created by Charlie Whitney on 10/25/12.
//
//

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Serial.h"
#include "cinder/Utilities.h"

#include <boost/algorithm/string.hpp>
#include <boost/signals2.hpp>

#define BOOST_INTEL_STDCXX0X
#define BAUD_RATE 115200
#define BUFFER_SIZE 3

using namespace std;
using namespace ci;
using namespace ci::app;

class PulseReader
{
  public:
    PulseReader() : bIsFingerOver(false){};
    
    void setup();
    void update();
    
    bool isConnected(){ return bIsConnected; };
    bool isFingerOver(){ return bIsFingerOver; };
    
    int getBpm(){ return bpm; };
    int getSensorData(){ return sensorData; };
    int getHRV(){ return hrv; };
    
    boost::signals2::signal<void ()> sOnFingerOver, sOnFingerOut;  
    boost::signals2::signal<void ()> sOnPulseStart, sOnPulseBeat;
    
  protected:
    void            checkForFinger();
    Serial          mSerial;
    bool            bIsConnected;
    deque<char>     mByteData;
    int             bpm, hrv, sensorData;
    int             mBuffer[BUFFER_SIZE];
    
    float           mLastBeat;
    bool            bLastFingerStatus;
    bool            bIsFingerOver;
};
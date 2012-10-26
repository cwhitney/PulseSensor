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

using namespace std;
using namespace ci;
using namespace ci::app;

class PulseReader
{
  public:
    PulseReader(){};
    
    void setup();
    void update();
    
    bool isConnected(){ return bIsConnected; };
    bool isFingerOver(){ return (sensorData < 850); };
    
    int getBpm(){ return bpm; };
    int getSensorData(){ return sensorData; };
    int getHRV(){ return hrv; };
    
    boost::signals2::signal<void ()> sOnFingerOver, sOnFingerOut;
    boost::signals2::signal<void ()> sOnPulseStart, sOnPulseBeat;
    
  protected:
    Serial          mSerial;
    bool            bIsConnected;
    deque<char>     mByteData;
    int bpm, hrv, sensorData;
    
    float           mLastBeat;
    bool            bLastFingerStatus;
};
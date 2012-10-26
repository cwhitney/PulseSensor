//
//  PulseReader.cpp
//  PulseSense
//
//  Created by Charlie Whitney on 10/25/12.
//
//

#include "PulseReader.h"

void PulseReader::setup()
{
    console() << "PulseReader :: finding devices" << endl;
    
    bpm = 0;
    hrv = 0;
    sensorData = 999;
    
    mLastBeat = 0.0f;
    bIsConnected = false;
    bLastFingerStatus = false;
    
    int i = 0;
    const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		console() << "PulseReader :: [" << i++ << "] " << deviceIt->getName() << endl;
	}
    
    try {
		Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial");
		mSerial = Serial( dev, BAUD_RATE );
        
        console() << "PulseReader :: Device connected to :: " << dev.getName() << endl;
        bIsConnected = true;
	}
	catch( ... ) {
		console() << "PulseReader :: There was an error initializing the serial device!" << std::endl;
	}
}

void PulseReader::update()
{
    if( !bIsConnected )
        return;
    
    string inData;
    
    while( mSerial.getNumBytesAvailable() ){
        inData = mSerial.readStringUntil('\n');
        
        // trim
        inData.erase(inData.find_last_not_of(" \n\r\t")+1);
        
        // SENSOR DATA
        if (inData[0] == 'S'){      // leading 'S' for sensor data
            inData.erase(0,1);      // cut off the leading 'S'
            try{
                sensorData = fromString<int>(inData);
            }catch(...){ continue; }
        }
        
        // BEATS PER MIN
        else if (inData[0] == 'B'){
            inData.erase(0,1);
            try{
                bpm = fromString<int>( inData );
                
                if( bpm < 0 ){
                    bpm = 0;
                    continue;
                }
                
                sOnPulseBeat();
                
                if( getElapsedSeconds() - mLastBeat > 3.0 )
                    sOnPulseStart();
                
                mLastBeat = getElapsedSeconds();
                
            }catch(...){ continue; }
        }
        
        // HRV
        else if (inData[0] == 'Q'){
            inData.erase(0,1);
            try{
                hrv = fromString<int>( inData );
            }catch(...){ continue; }
        }
    }
    
    // CHECK FOR FINGER STATUS
    if( isFingerOver() != bLastFingerStatus ){
        if( isFingerOver() )    sOnFingerOver();
        else                    sOnFingerOut();
    }
    
    bLastFingerStatus = isFingerOver();
    
}
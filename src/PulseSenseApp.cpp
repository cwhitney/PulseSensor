#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "cinder/Timeline.h"
#include "cinder/Font.h"

#include "PulseReader.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PulseSenseApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    void onHeartBeat();
    void onPulseStart();
    void onFingerOver();
    void onFingerOut();
    
    // PULSE
    PulseReader     mPulseReader;
    
    // HEART GRAPHIC
    Anim<float>     mHeartAlpha, mEventAlpha;
    
    string          eventStr;
};

void PulseSenseApp::setup()
{
    gl::enableAlphaBlending();
    
    mPulseReader.setup();
    
    eventStr = "";
    mHeartAlpha = 0;
    mEventAlpha = 0;
}

void PulseSenseApp::mouseDown( MouseEvent event )
{
}

void PulseSenseApp::update()
{
    mPulseReader.update();
    
    // PULSE EVENTS
    mPulseReader.sOnPulseBeat.connect( bind(&PulseSenseApp::onHeartBeat, this ) );
    mPulseReader.sOnPulseStart.connect( bind(&PulseSenseApp::onPulseStart, this ) );
    
    // FINGER EVENTS
    mPulseReader.sOnFingerOver.connect( bind(&PulseSenseApp::onFingerOver, this ) );
    mPulseReader.sOnFingerOut.connect( bind(&PulseSenseApp::onFingerOut, this ) );
}

void PulseSenseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    // DRAW TEXT
    gl::color( Color::white() );
    if( mPulseReader.getBpm() != 0) {
        gl::drawString( "BPM: " + toString(mPulseReader.getBpm()), Vec2f(115,20), Color::white(), Font("Helvetica", 24) );
    }else{
        gl::drawString( "BPM: Waiting", Vec2f(115,20), Color::white(), Font("Helvetica", 24));
    }
    
    gl::color( Color::white() );
    string yesno = mPulseReader.isFingerOver()?"yes":"no";
    gl::drawString( "Finger over sensor? " + yesno, Vec2f(115,40), Color::white(), Font("Helvetica", 24));
    gl::drawString( "Raw data: " + toString( mPulseReader.getSensorData() ) , Vec2f(115,60), Color::white(), Font("Helvetica", 24));
    
    gl::drawString( "Event: " + eventStr, Vec2f(115,80), ColorA(1,1,1,mEventAlpha ), Font("Helvetica", 24) );
    
    // DRAW HEART
    gl::color( ColorA(1,0,0,mHeartAlpha ) );
    gl::drawSolidRect( Rectf(0,0,100,100) );
}


void PulseSenseApp::onHeartBeat()
{
    mHeartAlpha = 1.0f;
    timeline().apply( &mHeartAlpha, 1.0f, 0.0f, 1.0f );
}

void PulseSenseApp::onPulseStart()
{
    eventStr = "Pulse start";
    timeline().apply( &mEventAlpha, 1.0f, 0.0f, 3.0f );
}

void PulseSenseApp::onFingerOver()
{
    eventStr = "Finger over";
    timeline().apply( &mEventAlpha, 1.0f, 0.0f, 3.0f );
}

void PulseSenseApp::onFingerOut()
{
    eventStr = "Finger out";
    timeline().apply( &mEventAlpha, 1.0f, 0.0f, 3.0f );
}



CINDER_APP_BASIC( PulseSenseApp, RendererGl )

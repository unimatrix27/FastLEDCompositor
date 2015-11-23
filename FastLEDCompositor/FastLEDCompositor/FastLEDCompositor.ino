/*
Testprogram for Composition - Framwwork
Author: Sebastian Stuecker

This code is heavily work in progress

Main File. does not do a lot except if used for testing stuff. 
Reads Midi data each loop and let the midi code (disco.cpp) handle it.

*/


#define USE_OCTOWS2811
#include "OctoWS2811.h"		 // Use OctoWS2811 Adaptor for parallel output
#include "FastLED.h"		 //	Include FastLED used for the fun part
#include "disco.h"			 // some defines etc.

//#include "Note.h"			// a side project, nut used currently
//#include "noteplayer.h"   // --

#include "Compositor.h"		// Definitions for the main Compositor class

//std::vector<uint16_t> notes;  // currently not used


#if FASTLED_VERSION < 3001000                                 // This guarantees the person will have to use FastLED 3.1
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Initialize changeable global variables.

uint8_t max_bright = 255;                   // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.
struct CRGB leds[NUM_LEDS];					// this is the array for all LEDs
uint8_t g_bpm_cnt = 0;						// the sub-beat counter as received via MIDI clock. increase 24 times per beat. (see MIDI specs)
uint16_t g_bpm_beatnr = 0;					// A Beat counter, useful for automatic patterns (not yet implemented)
long g_bpm_timebase = 0;					// set to the start of a beat to influence behavior of beatsin8 etc.
uint16_t g_bpm = BPM;						// global varialbe which is set to the BPM detected via MIDI clock as soon as it is received.
// Initialize our LED array.


// SETUP -------------------------------------------------------------------------------------------------------

// myComposition is going to be the main "canvas" where everything is put together
Compositor myComposition(leds, NUM_LEDS);





void setup() {
	//Serial.begin(115200);
	
	// Receive MIDI Clock information to create BPM
	// not natively supported in TeensyDuino
	// see blog post at
	// http://little-scale.blogspot.de/2011/08/how-to-deal-with-real-time-midi-beat.html
	usbMIDI.setHandleRealTimeSystem(RealTimeSystem); 

	// Give functions that handle the different MIDI receive events. 
	usbMIDI.setHandleControlChange(OnControlChange);  // handling of parameter changes through MIDI CC commands.
	usbMIDI.setHandleNoteOn(OnNoteOn);				  // called when a NOTE ON is received (and will thus enable a pattern)
	usbMIDI.setHandleNoteOff(OnNoteOff);			  // called when not off is received (disabling/fading out pattern)
	delay(500);
	FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.

} // setup()



void loop() {
	while(usbMIDI.read());			// read MIDI each loop. MIDI could be faster than the FPS, therefore the while loop.
	myComposition.draw();	        // activate drawing function of composition
	FastLED.show();					// output the array to the actual LED stripe
    //ChangeMe();		// doing some demo stuff, should be commented out in MIDI configuration


} // loop()

// below just some code for experimenting and testing, should be removed in final configuration


void ChangeMe() {
	ParameterSet* mp;
	uint8_t secondHand = (millis() / 1000) % 60;
	static uint8_t lastSecond = 99;
	if (lastSecond != secondHand) {
		lastSecond = secondHand;
		switch (secondHand) {
		case 5:
			mp = myComposition.getParams(0);
			mp->effectType = ET_RAINBOW; mp->fadeTime = 1000; mp->fadeType = FT_WIPE; mp->blendType = BT_SUM;
			mp->channelSpeed = 0; mp->clonecount = 0; mp->speed = 10; mp->setTargetPalette(PN_FOREST, 0);
			mp->brightness = 250;
			myComposition.addChannel(0);
			break;
		case 10:
			mp = myComposition.getParams(1);
			mp->numLeds = 30; mp->fadeTime = 5000; mp->fadeType = FT_FADE; mp->blendType = BT_SUM;
			mp->channelSpeed = 0; mp->clonecount = 0; mp->clonedistance = 150; mp->channelSpeed = 7; mp->speed = 0; mp->effectType = ET_RAINBOW;
			mp->brightness = 210;
			myComposition.addChannel(1);
			break;
		case 20:
			mp = myComposition.getParams(1); mp->blendType = BT_OVERLAY;
			break;
		case 30:
			mp = myComposition.getParams(0);
			mp->setTargetPalette(PN_PARTY,5000);
		case 40:
			mp = myComposition.getParams(1); mp->setTargetPalette(PN_CPT_OCEAN, 5000);
			break;
		case 50:
			myComposition.setFade(1,  2);
			myComposition.setFade(0,  2);
			break;
		default:
			break;
		}
	}
}

void chanTog(int tog);
void ChangeMe2() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
	uint8_t secondHand = (millis() / 1000) % 44;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
	static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
	if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
		lastSecond = secondHand;
		if(lastSecond%3==0) chanTog(lastSecond);
		switch (secondHand) {
		case  10: myComposition.getParams(2)->channelSpeed = 2; break;
		case 20: myComposition.getParams(2)->speed = 20; break;
		case 15: myComposition.getParams(2)->setTargetPalette(PN_AUTO_RANDOM); 
			myComposition.getParams(2)->numLeds = 100;
			break;
		case 16: myComposition.getParams(2)->paletteBlendDuration = 5000; break;

		case 25: myComposition.getParams(2)->setTargetPalette(PN_AUTO_RANDOM); break;

		default:
			break;
		}
	}
}


void chanOn() {
	int channel = 2;
	myComposition.addChannel(channel);
}

void chanOff() {
	int channel = 2;
	myComposition.setFade(channel,2);
}
void chanTog(int tog) {
	if (tog % 2) {
		chanOn();
	}
	else {
		chanOff();
	}
}
void changePalette(PaletteName pn) {
	if (myComposition.getParams(1) != NULL) {
		myComposition.getParams(1)->setTargetPalette(pn,7000);
	}
}
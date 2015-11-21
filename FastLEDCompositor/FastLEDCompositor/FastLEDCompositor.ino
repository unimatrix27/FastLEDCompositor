/*
Testprogram for Composition - Framwwork
Author: Sebastian Stuecker

This code is heavily work in progress

Main File to test the framework

*/

#include "MIDIPlayer.h"
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

uint8_t max_bright = 250;                   // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.
struct CRGB leds[NUM_LEDS]; 
uint8_t g_bpm_cnt = 0;
uint16_t g_bpm_beatnr = 0;
long g_bpm_timebase = 0;
uint16_t g_bpm =120;
// Initialize our LED array.


// SETUP -------------------------------------------------------------------------------------------------------

// myComposition is going to be the main "canvas" where everything is put together
Compositor myComposition(leds, NUM_LEDS);





void setup() {
	//Serial.begin(115200);
	usbMIDI.setHandleRealTimeSystem(RealTimeSystem);
	usbMIDI.setHandleControlChange(OnControlChange);
	usbMIDI.setHandleNoteOn(OnNoteOn);
	usbMIDI.setHandleNoteOff(OnNoteOff);
	delay(1000);
	// Below add some channels to the composition for basic testing. All parameters of addChannel have defaults
	//myComposition.addChannel(0, 3, BT_SUM, FT_NOFADE, 30, 40, 1000,TB_MILLIS);
	//myComposition.addChannel(1, 1, BT_SUM, FT_FADE, 30, 0, 5000, TB_MILLIS);
	//myComposition.addChannel(1, 1, BT_SUM, FT_FADE,300, 0, 5000, TB_MILLIS);
	//myComposition.addChannel(2, 0, BT_SUM, FT_FADE, 5, 150, 2000, TB_MILLIS);
	//myComposition.addChannel(3, 1, BT_SUM, FT_FADE, 30, 20, 2000,TB_MILLIS);
	//myComposition.addChannel(4, 0, BT_SUM, FT_FADE, 30, 200, 1300, TB_MILLIS);
	//myComposition.addChannel(5, 1, BT_SUM, FT_FADE, 40, 200, 1300, TB_MILLIS);
	//myComposition.addChannel(6, 1, BT_SUM, FT_FADE, 20, 200, 1300, TB_MILLIS);
	//myComposition.addChannel(7, 1, BT_SUM, FT_FADE, 5, 100, 1300, TB_MILLIS);
	//myComposition.addChannel(8, 1, BT_SUM, FT_FADE, 3, 90, 1300, TB_MILLIS);
	//myComposition.addChannel(1, 2);
	//myComposition.addChannel(1, 1,BT_SUM,FT_WIPE,5,17,1000,TB_MILLIS);
	//myComposition.getParams(3)->setTargetPalette(PN_AUTO_RANDOM);
	//myComposition.getParams(2)->setTargetPalette(PN_AUTO_RANDOM);

	//myComposition.getParams(2)->channelSpeed = -50;
	//myComposition.getParams(3)->channelSpeed = 40;
	//myComposition.getParams(1)->channelSpeed = 30;
	//myComposition.getParams(3)->channelSpeed = 3;
	//myComposition.getParams(4)->channelSpeed = 8;
	//myComposition.getParams(5)->channelSpeed = 5;
	//myComposition.getParams(5)->speed = 15;
	//myComposition.getParams(3)->speed = 4;
	//myComposition.getParams(6)->channelSpeed = 4;
	//myComposition.getParams(7)->channelSpeed = 44;
	//myComposition.getParams(8)->channelSpeed = 80;
	// Add the LED array to FastLED
	FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.

} // setup()



void loop() {
	while(usbMIDI.read());
	myComposition.draw();					 // activate drawing function of composition

	FastLED.show(); // output the array to the actual LED stripe
	//	 FastLED.delay(1000 / FRAMES_PER_SECOND);

		//myComposition.moveChannel(1, newpos++);  // move one of the channels arround each second as a test
		//ChangeMe();
		//myComposition.setFade(0, FT_WIPE, TB_MILLIS, 3000);

} // loop()


void chanTog(int tog);
void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
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
	myComposition.addChannel(channel,
		myComposition.getParams(channel)->effectType,
		myComposition.getParams(channel)->blendType,
		myComposition.getParams(channel)->fadeType,
		myComposition.getParams(channel)->numLeds,
		myComposition.getParams(channel)->startPos,
		myComposition.getParams(channel)->fadeTime,
		myComposition.getParams(channel)->fadeTimeBase);
}

void chanOff() {
	int channel = 2;
	myComposition.setFade(channel,
		FT_WIPE,
		TB_MILLIS,2000,2);
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
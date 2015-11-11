/*
Testprogram for Composition - Framwwork
Author: Sebastian Stuecker

This code is heavily work in progress

Main File to test the framework

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

uint8_t max_bright = 100;                   // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.
struct CRGB leds[NUM_LEDS];                 // Initialize our LED array.

// SETUP -------------------------------------------------------------------------------------------------------

// myComposition is going to be the main "canvas" where everything is put together
Compositor myComposition(leds, NUM_LEDS);

void setup() {
	// Below add some channels to the composition for basic testing. All parameters of addChannel have defaults
	//myComposition.addChannel(0, 1, BT_SUM, FT_NOFADE, 250, 0, 900,TB_MILLIS);
	myComposition.addChannel(1, 0, BT_SUM, FT_NOFADE, 50, 50, 5000, TB_MILLIS);
	//myComposition.addChannel(2, 0, BT_SUM, FT_FADE, 30, 100, 17000, TB_MILLIS);
	//myComposition.addChannel(3, 1, BT_SUM, FT_FADE, 30, 150, 19000, TB_MILLIS);
	//myComposition.addChannel(4, 0, BT_SUM, FT_FADE, 30, 200, 11000, TB_MILLIS);
	//myComposition.addChannel(5, 1, BT_SUM, FT_FADE, 30, 250, 13000, TB_MILLIS);

	delay(100);                                                // If things go bad, you can shutdown before the LED's start drawing power.
	// Add the LED array to FastLED
	FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.
	

} // setup()



  // LOOP -------------------------------------------------------------------------------------------------------

void loop() {
	static int newpos = 0; // test variable to move one channel
	myComposition.draw();					 // activate drawing function of composition

	FastLED.show(); // output the array to the actual LED stripe
	//	 FastLED.delay(1000 / FRAMES_PER_SECOND);

		//myComposition.moveChannel(1, newpos++);  // move one of the channels arround each second as a test
		ChangeMe();
		//myComposition.setFade(0, FT_WIPE, TB_MILLIS, 3000);

} // loop()

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
	uint8_t secondHand = (millis() / 1000) % 10;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
	static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
	if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
		lastSecond = secondHand;
		switch (secondHand) {
		case  2: myComposition.setFade(1, FT_WIPE, TB_MILLIS, 1000); break;  // You can change values here, one at a time , or altogether.
		case  4: myComposition.setFade(1, FT_WIPE, TB_MILLIS, 1000); break;
		case 6: myComposition.setFade(1, FT_FADE, TB_MILLIS, 1000);  break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
		case 8: myComposition.setFade(1, FT_FADE, TB_MILLIS, 1000); break;
		default: break;// Here's the matching 15 for the other one.
		}
	}
}
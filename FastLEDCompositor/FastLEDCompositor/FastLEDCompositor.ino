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
	//myComposition.addChannel(0, 0, BT_SUM, FT_FADEIN, 30, 0,13000,TB_MILLIS);
	myComposition.addChannel(1, 1, BT_SUM, FT_FADEIN, 200, 50, 25000, TB_MILLIS);
	//myComposition.addChannel(2, 0, BT_SUM, FT_FADEIN, 30, 100, 17000, TB_MILLIS);
	//myComposition.addChannel(3, 1, BT_SUM, FT_FADEIN, 30, 150, 19000, TB_MILLIS);
	//myComposition.addChannel(4, 0, BT_SUM, FT_FADEIN, 30, 200, 11000, TB_MILLIS);
	//myComposition.addChannel(5, 1, BT_SUM, FT_FADEIN, 30, 250, 13000, TB_MILLIS);


	delay(100);                                                // If things go bad, you can shutdown before the LED's start drawing power.
	// Add the LED array to FastLED
	FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.
	

} // setup()



  // LOOP -------------------------------------------------------------------------------------------------------

void loop() {
	static int newpos = 0; // test variable to move one channel
	fill_solid(leds, NUM_LEDS, CRGB::Black); // will be removed later, blank out array each loop start
	myComposition.draw();					 // activate drawing function of composition
	EVERY_N_MILLISECONDS(10) {
		myComposition.moveChannel(1, newpos++);  // move one of the channels arround each second as a test
	}
	FastLED.show(); // output the array to the actual LED stripe
	//	 FastLED.delay(1000 / FRAMES_PER_SECOND);
} // loop()


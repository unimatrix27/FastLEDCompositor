/*
Testprogram for Composition - Framwwork
Author: Sebastian Stuecker

This code is heavily work in progress

Main File to test the framework

*/
#include "ChannelMasks.h"
#define USE_OCTOWS2811
#include "OctoWS2811.h"
#include "FastLED.h"


// #include "FastLED.h"    // FastLED library. Preferably the latest copy of FastLED 2.1.
#include "disco.h"
//#include "Note.h"
//#include "noteplayer.h"
#include "Compositor.h"
//#include "Channel.h"

//std::vector<uint16_t> notes;


#if FASTLED_VERSION < 3001000                                 // This guarantees the person will have to use FastLED 3.1
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.


// Initialize changeable global variables.

uint8_t max_bright = 100;                                      // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.
uint16_t g_bpm = BPM;
struct CRGB leds[NUM_LEDS];                                  // Initialize our LED array.
unsigned long g_beatstart = 0;											// timebase for MIDI Clock BPM.
uint8_t g_midicounter = 0;
uint16_t g_beatcount = 0;
uint16_t g_number = 0;
uint16_t g_brigthness = 250;
//Noteplayer myNoteplayer;
// VARIABLES - In general, use the smallest and most accurate variable definition you can, i.e. Localize them to your routine if you can.
//uint8_t myVar = 0;                                            // for a variable that ranges from 0 to 255.
// uint8_t thisdelay=10;





CRGBPalette16 currentPalette;
TBlendType    currentBlending;
CRGBPalette16 targetPalette;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

uint32_t resetTime;

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void(*SimplePatternList[])();
SimplePatternList gPatterns = { fill_strip, bummer, Glitter, ChangeMe, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm,numberdraw };

uint8_t gCurrentPatternNumber = 10; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint16_t pixel = 5;
uint16_t endpos = NUM_LEDS - 1;

// SETUP -------------------------------------------------------------------------------------------------------

Compositor myComposition(leds, NUM_LEDS);


LedRange* myLedRange;
void setup() {
	myComposition.addChannel(0, 0, BT_SUM, FT_NOFADE, 30, 0);
	myComposition.addChannel(1, 1, BT_SUM, FT_NOFADE, 30, 50);
	myComposition.addChannel(2, 0, BT_SUM, FT_NOFADE, 30, 100);
	myComposition.addChannel(3, 1, BT_SUM, FT_NOFADE, 30, 150);
	myComposition.addChannel(4, 0, BT_SUM, FT_NOFADE, 30, 200);
	myComposition.addChannel(5, 1, BT_SUM, FT_NOFADE, 30, 250);


	delay(100);                                                // If things go bad, you can shutdown before the LED's start drawing power.
															   //Serial.begin(115200);                                        // Get the serial port running for debugging

															   // FastLED.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
	FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.
	set_max_power_in_volts_and_milliamps(5, 2000);               // This is used by the power management functionality and is currently set at 5V, 500mA.


																 // Optional randomization
	random16_set_seed(4832);                                    // Awesome randomizer
	random16_add_entropy(analogRead(2));

	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);
	usbMIDI.setHandleRealTimeSystem(RealTimeSystem);
	usbMIDI.setHandleNoteOff(OnNoteOff);
	usbMIDI.setHandleNoteOn(OnNoteOn);


	digitalWrite(ledPin, LOW);
	currentPalette = CloudColors_p;
	currentBlending = LINEARBLEND;



} // setup()



  // LOOP -------------------------------------------------------------------------------------------------------

void loop() {
	static int newpos = 0;
	fill_solid(leds, NUM_LEDS, CRGB::Black);
	myComposition.draw();
	//usbMIDI.read();
	static uint8_t startIndex = 0;
	startIndex = startIndex - 4; /* motion speed */
	EVERY_N_SECONDS(1) {
		myComposition.moveChannel(1, newpos++);
	}
	//gPatterns[gCurrentPatternNumber]();
	//confetti();
	//   EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
	//     fadeToBlackBy (leds,NUM_LEDS, 2);
	//   }
	//myNoteplayer.draw();
	// blur1d(leds, NUM_LEDS, 44);
	//FillLEDsFromPaletteColors(startIndex);

	FastLED.show();
	//	 FastLED.delay(1000 / FRAMES_PER_SECOND);
	//ChangePalettePeriodically();
	//FastLED.delay(1000/FRAMES_PER_SECOND);
	//show_at_max_brightness_for_power();                         // Run the FastLED.show() at full loop speed.
	//delay_at_max_brightness_for_power(1000/FRAMES_PER_SECOND);
	// Serial.println(LEDS.getFPS());                              // Display frames per second on the serial monitor.  
	//    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
	//    EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
} // loop()


void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
	uint8_t brightness = 255;

	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
		colorIndex += 1;
	}
}

void ChangePalettePeriodically()
{
	uint8_t secondHand = (millis() / 1000) % 60;
	static uint8_t lastSecond = 99;

	if (lastSecond != secondHand) {
		lastSecond = secondHand;
		if (secondHand == 0) { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
		if (secondHand == 10) { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND; }
		if (secondHand == 15) { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
		if (secondHand == 20) { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
		if (secondHand == 25) { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
		if (secondHand == 30) { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
		if (secondHand == 35) { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
		if (secondHand == 40) { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
		if (secondHand == 45) { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
		if (secondHand == 50) { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND; }
		if (secondHand == 55) { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
	}
}
// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
	for (int i = 0; i < 16; i++) {
		currentPalette[i] = CHSV(random8(), 255, random8());
	}
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
	// 'black out' all 16 palette entries...
	fill_solid(currentPalette, 16, CRGB::Black);
	// and set every fourth one to white.
	currentPalette[0] = CRGB::White;
	currentPalette[4] = CRGB::White;
	currentPalette[8] = CRGB::White;
	currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
	CRGB purple = CHSV(HUE_PURPLE, 255, 255);
	CRGB green = CHSV(HUE_GREEN, 255, 255);
	CRGB black = CRGB::Black;

	currentPalette = CRGBPalette16(
		green, green, black, black,
		purple, purple, black, black,
		green, green, black, black,
		purple, purple, black, black);
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
	CRGB::Red,
	CRGB::Gray, // 'white' is too bright compared to red and blue
	CRGB::Blue,
	CRGB::Black,

	CRGB::Red,
	CRGB::Gray,
	CRGB::Blue,
	CRGB::Black,

	CRGB::Red,
	CRGB::Red,
	CRGB::Gray,
	CRGB::Gray,
	CRGB::Blue,
	CRGB::Blue,
	CRGB::Black,
	CRGB::Black
};
// CHANGEME -----------------------------------------------------------------------------------------------------

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
	uint8_t secondHand = (millis() / 1000) % 30;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
	static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
	if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
		lastSecond = secondHand;
		switch (secondHand) {
		case 3: fill_gradient(leds, 1, CHSV(gHue, 255, 250), 75, CHSV(gHue + 30, 250, 250), SHORTEST_HUES); break;
		case 6: fill_gradient(leds, 1, CHSV(gHue, 255, 250), 75, CHSV(gHue + 80, 250, 250), LONGEST_HUES); break;
		case 8: fill_gradient(leds, 1, CHSV(gHue, 255, 250), 75, CHSV(gHue + 140, 250, 250), SHORTEST_HUES); break;
		case 12: fill_rainbow(leds, NUM_LEDS, gHue, 8); break;
		case 15: fill_rainbow(leds, NUM_LEDS, gHue, 6); break;
		case 20: fill_rainbow(leds, NUM_LEDS, gHue, 5); break;
		case 25: fill_rainbow(leds, NUM_LEDS, gHue, 1); break;
		case 30: fill_solid(leds, NUM_LEDS, CHSV(0, 0, 32)); break;
		}
	}
} // ChangeMe()


void fill_strip() {
	leds[20] = CRGB::White;
	if (pixel == endpos) {
		if (endpos > 0) {
			endpos--;
			pixel = 0;
		}
		else {
			endpos = NUM_LEDS - 1;
			pixel = 0;
			fill_solid(leds, NUM_LEDS, CHSV(0, 0, 32));
			return;
		}
	}
	leds[pixel++] = CRGB::White;
	if (pixel > 1) {
		leds[(pixel - 2)] = CRGB::Black;
	}
}




#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
	// add one to the current pattern number, and wrap around at the end
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow()
{
	// FastLED's built-in rainbow generator
	fill_rainbow(leds, NUM_LEDS, gHue, 7);
}



void addGlitter(fract8 chance_of_glitter = 80, int fade = 0)
{
	if (fade) {
		fadeToBlackBy(leds, NUM_LEDS, fade);
	}
	int number_of_glitters = random(NUM_LEDS / 50); // number of glitter sparkles to add

	int r = random8(100);
	if (r < chance_of_glitter) {
		for (int j = 0; j < number_of_glitters; j++) {
			int pos = random16(NUM_LEDS);
			leds[pos] = CRGB::White; // very bright glitter
		}
	}
}
void rainbowWithGlitter()
{
	// built-in FastLED rainbow, plus some random sparkly glitter
	rainbow();
	addGlitter(80);
}

void Glitter() {
	addGlitter(80, 200);
}


void confetti()
{
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, NUM_LEDS, 10);
	int pos = random16(NUM_LEDS);
	leds[pos] += CHSV(gHue + random8(40), 255, 255);
}

void sinelon()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(g_bpm / 4, 0, NUM_LEDS, g_beatstart);
	leds[pos] += CHSV(gHue, 255, 192);
}

void bpm()
{
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8(g_bpm / 2, 64, 255, g_beatstart);
	for (int i = 0; i < NUM_LEDS; i++) { //9948
		leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void juggle() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy(leds, NUM_LEDS, 20);
	byte dothue = 0;
	for (int i = 0; i < 8; i++) {
		leds[beatsin16(g_bpm / 8, 0, NUM_LEDS, g_beatstart)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

void bummer() {
	static uint16_t hue16 = 0;
	hue16 += 9;
	fill_rainbow(leds, NUM_LEDS, hue16 / 256, 3);
	uint8_t bright = beatsin8(g_bpm, 0, 255, g_beatstart, 192);
	FastLED.setBrightness(dim8_video(bright));
}

void numberdraw() {
	//fill_solid(leds, NUM_LEDS, CRGB::Black);
	fill_solid(leds, g_number, CHSV(0, 0, g_brigthness));
}


// Non-power managed display -------------------------------
//FastLED.show();                                             // I don't use this anymore, I use the power managed display instead.
//  FastLED.delay(thisdelay);


// Power managed display -----------------------------------

//  set_max_power_in_volts_and_milliamps(5, 500);               // This is defined in setup and used by the power management functionality and is currently set at 5V, 500mA.
//  show_at_max_brightness_for_power();                         // This is used in loop for power managed display of LED's.



// Palettes -----------------------------------------------------------------------------------------
/*
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;        // NOBLEND or LINEARBLEND


// In setup
// RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p

currentPalette  = CRGBPalette16(CRGB::Black);
targetPalette   = RainbowColors_p;                            // Used for smooth transitioning.
currentBlending = LINEARBLEND;

// In loop

EVERY_N_MILLISECONDS(100) {
uint8_t maxChanges = 24;
nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
}

// In loop for testing
EVERY_N_MILLISECONDS(5000) {
targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255));
}



// In routine
leds[i] = ColorFromPalette(currentPalette,thishue,thisbri, currentBlending);



// Creating a palette
void SetupRandomPalette() {
int myHue = random8();
targetPalette = CRGBPalette16(CHSV(myHue, 255, 32), CHSV(myHue, random8(64)+192, 255), CHSV(myHue, 255, 32), CHSV(myHue, random8(64)+192, 255));
}

void SetupRandomPalette() {
for (int i = 0; i < 16; i++) {
targetPalette[i] = CHSV(random8(), 255, 255);
}
}

void SetupRandomPalette() {
int myHue = random8();
targetPalette = CRGBPalette16(CRGB::Green);
}





// Timing ----------------------------------------------------------------------------------------------------------


EVERY_N_MILLISECONDS(10) {run_function();}        // Whatever function call you want.




// Assigning Colors -------------------------------------------------------------------------------------------------

leds[k] += CHSV(thathue, allsat, thatbright);
leds[k] = CRGB::Red;





// colorutils.h is at http://fastled.io/docs/3.1/group___colorutils.html

fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.

fill_solid(leds,NUM_LEDS, 0xff00ff);                        //A long RGB value
fill_solid(leds, NUM_LEDS, CRGB(50,0,200));                 // 8 bit values
fill_solid(leds, NUM_LEDS, CHSV(150,255,200));              // 8 bit values


fill_gradient_RGB(leds, startpos, 0x000011, endpos, 0x110000);
fill_gradient_RGB(leds, NUM_LEDS, CRGB(50,0,200), CRGB(80,200,240));    // up to 4 of these

//FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES
fill_gradient(leds, startpos, CHSV(50, 255,255) , endpos, CHSV(150,255,255), SHORTEST_HUES);
fill_gradient(leds, NUM_LEDS, CHSV(50, 255,255), CHSV(100,255,255), LONGEST_HUES);    // up to 4 of these



// Fade, Scale
fadeToBlackBy(leds, NUM_LEDS, fadeval);                     // 8 bit, 1 = slow, 255 = fast
nscale8(leds,NUM_LEDS,fadeval);                             // 8 bit, 1 = fast, 255 = slow
leds[i].fadeToBlackBy(fadeval);

// Blend
leds[i] = blend(CRGB::Red, CRGB::Blue, sin8(mysine));




// Randomization
uint8_t myval = random8(0, 255);
int myval = random16(NUM_LEDS);



// Beats

int beat = beatsin16(BeatsPerMinute,0,NUM_LEDS);
uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);




// Noise http://fastled.io/docs/3.1/group___noise.html
fill_noise16(leds, NUM_LEDS, octaves, x, xscale, hue_octaves, hxy, hue_scale, hue_time);



// Math http://fastled.io/docs/3.1/lib8tion_8h_source.html

// Limiting math
#define qsubd(x, b)  ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.
uint8_t sum = qadd8( 200, 200);                               // --> saturated at 255
sum = qsub8(i, j);                                            // Does not go below 0


// Memory management

memset(leds, 0, NUM_LEDS * 3);                                // Quick clearing of the LED's.
memcpy8(temp, leds, sizeof(leds));                            // Copy values from temp to leds



// PRINTF capability for debugging
// If you want to use printf capability, put this in your code:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

int serial_putchar(char c, FILE* f) {
if (c == '\n') serial_putchar('\r', f);
return !Serial.write(c);
} //serial_putchar()
FILE serial_stdout;

void setup() {
Serial.begin(57600);
fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE); // Set up stdout
stdout = &serial_stdout;
} // setup()

void loop() {
printf("My favorite number is %6d!\n", 12);                 // This is just an example
} // loop()


// Other ---------------------------------------------------------------------------------------------------------------------------------

// An add glitter function.
void addGlitter( uint8_t chanceOfGlitter) {
if(random8() < chanceOfGlitter) {
leds[ random16(NUM_LEDS) ] += CRGB::White;
}
}


// Beats Information ---------------------------------------------------------------------------------------------------------------------


uint8_t wave = beatsin8(

accum88 beats_per_minute,                                   // I'd use an int or uint8_t
uint8_t lowest=0,
uint8_t highest=255,
uint32_t timebase=0,                                        // Set to millis() to zero out the beat
uint8_t phase_offset=0)                                     // This is kind of cool

When you want to 'zero' out the beat:

uint8_t wave = beatsin8(60, 0, 255, millis());


// Easing & lerping -------------------------------------------------------------------------------------------------------

easeOutVal = ease8InOutQuad(easeInVal);                       // Start with easeInVal at 0 and then go to 255 for the full easing.
ledNum = lerp8by8(0, NUM_LEDS, easeOutVal);                   // Map it to the number of LED's you have.




// CHSV to CRGB scaling -----------------------------------------------------------
/*
If you want any level of hue rotation, then keep CHSV brightness at 32
Table of CHSV brightness to CRGB value
0 == (0,0,0)
1 == (1,0,0)
16 == (2,0,0)
23 == (3,0,0)
28 == (4,0,0)
32 == (5,0,0)
*/


void RealTimeSystem(byte realtimebyte) {
#define CLOCK 248
#define START 250
#define CONTINUE 251
#define STOP 252

	if (realtimebyte == 248) {
		g_midicounter++;
		if (g_midicounter == 24) {
			g_beatcount++;
			g_midicounter = 0;
			digitalWrite(ledPin, HIGH);


		}
		if (g_beatcount>3) {
			g_beatcount = 0;
			g_bpm = 240000 / (millis() - g_beatstart);
			g_beatstart = millis();
		}
		if (g_midicounter == 4) { digitalWrite(ledPin, LOW); }
	}
	if (realtimebyte == START || realtimebyte == CONTINUE) {
		g_beatstart = millis();
		g_midicounter = 0;
		g_beatcount = 0;
		digitalWrite(ledPin, HIGH);
	}
	if (realtimebyte == STOP) {
		digitalWrite(ledPin, LOW);
	}
	//if (g_beatcount == 32768) {
	//	g_beatcount = 0;
	//}
}

void OnNoteOn(byte channel, byte note, byte velocity)
{
	digitalWrite(ledPin, HIGH);
	//myNoteplayer.NoteOn(note, velocity);
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
	digitalWrite(ledPin, LOW);
	//myNoteplayer.NoteOff(note, velocity);
}
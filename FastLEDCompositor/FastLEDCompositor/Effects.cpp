#include "Effects.h"
#include "FastLED.h"

void SolidEffect::draw() 
{
	static uint8_t test = 0;
	fill_solid(canvas->getLeds(), canvas->getNumLeds(),CRGB(0,0,222));
	test++; test++;
}

void RainbowEffect::draw()
{
	static uint8_t start = 0;
	start++;
	fill_rainbow(canvas->getLeds(), canvas->getNumLeds(), start, 255/ canvas->getNumLeds());
}

void ConfettiEffect::draw() {
	uint8_t  thisfade = 8;                                        // How quickly does it fade? Lower = slower fade rate.
	static int       thishue = 50;                                       // Starting hue.
	uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
	uint8_t   thissat = 100;                                      // The saturation, where 255 = brilliant colours.
	uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
	int       huediff = 128;                                      // Range of random #'s to use for hue
	uint8_t thisdelay = 5;                                        // We don't need much delay (if any)
	int pos;
	EVERY_N_MILLISECONDS(thisdelay) {
		fadeToBlackBy(canvas->getLeds(), canvas->getNumLeds(), thisfade);                    // Low values = slower fade.
		pos = random16(canvas->getNumLeds());                               // Pick an LED at random.
		canvas->getLeds()[pos] += CHSV((thishue + random16(huediff)) / 4, thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
		thishue = thishue + thisinc;                                // It increments here.
	}
}

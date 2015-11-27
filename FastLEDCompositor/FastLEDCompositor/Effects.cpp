#include "Effects.h"
#include "disco.h"
#include "FastLED.h"

void SolidEffect::draw() 
{
	fill_solid(canvas->getLeds(),
		canvas->getNumLeds(),
		myParams->getColor(myParams->hue));
}


void RainbowEffect::draw()
{
	uint16_t num = canvas->getNumLeds();
	//fill_rainbow(canvas->getLeds(), canvas->getNumLeds(), start, 255/ canvas->getNumLeds());
	for (int i = 0; i < num; i++) {
		uint16_t colorIndex = i*255 / num;
		canvas->getLeds()[i] = myParams->getColor(colorIndex+myParams->hue);
	}
}

void ConfettiEffect::draw() {
	uint8_t  thisfade = 12;                                        // How quickly does it fade? Lower = slower fade rate.
	static int       thishue = 50;                                       // Starting hue.
	uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
	uint8_t   thissat = 100;                                      // The saturation, where 255 = brilliant colours.
	uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
	int       huediff = 240;                                      // Range of random #'s to use for hue
	uint8_t thisdelay = 15;                                        // We don't need much delay (if any)
	int pos;
	EVERY_N_MILLISECONDS(thisdelay) {
		fadeToBlackBy(canvas->getLeds(), canvas->getNumLeds(), thisfade);                    // Low values = slower fade.
		pos = random16(canvas->getNumLeds()-1);                               // Pick an LED at random.
		canvas->getLeds()[pos] += CHSV((thishue + random16(huediff)) / 4, thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
		thishue = thishue + thisinc;                                // It increments here.
	}
}
BaseBeatEffect::BaseBeatEffect() {
	timebase = g_bpm_timebase;
	beat = g_bpm;
	firstRun = true;
}

void BaseBeatEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	uint16_t mid = n / 2;
	uint16_t val;
	if (firstRun == true && myParams->velocity & 0x40) {						// if effect was just created, initialize timebase based on parameters in velocity. 
		timebase = millis();
	}
	val = (0xff xor beat8(beat, timebase));
	val = map8(val, 0, 150);
	val = val << 1;

	uint8_t capVal;
	for (uint16_t i=0; i < mid; i++) {
		if (val > 240) { capVal = 240; } else { capVal = val; }
		l[mid + i] = myParams->getColor(capVal);
		l[mid - i] = myParams->getColor(capVal);
		if (val >  480 / n) { val -= 480 / n; }
		else { val = 0; }
	}
	firstRun = false;
}

void TwoColorsEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	uint8_t speed = myParams->speed;
	static long tb = 0;
	static uint16_t b = 0;
	if (b == 0) {
		tb = g_bpm_timebase;
		b = g_bpm;
	}
	uint16_t val = beat8(b, tb);
	fill_solid(l, n, CRGB::Black);
//	if ((g_bpm_beatnr & 1) xor (((myParams->vitality)&(0x01)) == 0)) {
//		val = (0xff ^ val);
//	}
	l[((n-1)*val / 256)] = myParams->getColor(myParams->hue);
	blur1d(l, n, myParams->blurryness);
}


SineloinEffect::SineloinEffect() {
	timebase = g_bpm_timebase;
	beat = g_bpm;
	prevPos = 0;
	firstRun = true;
}
void SineloinEffect::draw() {
	// Velocity Options:
	// Bits 3...0 ->  to influence the fade to black amount (00 = max fade, no slow fading tail, 11, maximum tail, slow fade)
	// Bit 5...4 -> phase offset 0,90,180,270 degrees		 (	
	// Bit 6 -> timebase on MIDI clock or effect start time
	// Channel Speed param effects movement speed of pixel
	// Vitality param affects number of pixels and their distance etc.

	uint16_t n = canvas->getNumLeds();					// how big is the pattern
	CRGB* l = canvas->getLeds();						// where to render the pattern onto
	uint16_t offset;									// used for phase offset of the sine wave
	uint16_t myBpm = (beat*(myParams->speed)) / 32;
	uint16_t pos;										// where to put the current pixel
	
	if (firstRun == true && myParams->velocity & 0x40) {						// if effect was just created, initialize timebase based on parameters in velocity. 
		timebase = millis();
	}
	fadeToBlackBy(l, n, (myParams->velocity&0x0F)*8);		// fade canvas down based on parameter setting
	offset = 64 * (myParams->velocity & 0x30)>>4;           // determine if offset needs to be applied based on parameter setting
	pos = beatsin8(myBpm, 0, 255, timebase, offset);		// determine the current position of the wave
	pos = pos*n / 256;
	if (firstRun) prevPos = pos;							// if the effect was just created, there is no tail to fill. 
	if (pos <= prevPos) {									// depending on direction of movement
		for (uint8_t i = 0; i <= (prevPos - pos); i++) {	// iterate through the pixel along the distance since the last frame
			l[pos+i]=myParams->getColor(myParams->hue);		// and fill those pixels with the current color
		}
	}
	else {													// same as above for other direction
		for (uint8_t i = 0; i <= (pos - prevPos); i++) {
			l[prevPos + i] = myParams->getColor(myParams->hue);
		}
	}
	prevPos = pos;											// memorize the current position for the next frame
	firstRun = false;										// set this to false as the first run is now over. 
}
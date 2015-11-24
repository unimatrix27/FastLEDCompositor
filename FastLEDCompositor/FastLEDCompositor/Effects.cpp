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

void BaseBeatEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	uint16_t mid = n / 2;
	uint16_t val = (0xff xor beat8(g_bpm, g_bpm_timebase));
	val = map8(val, 0, 150);
	val = val << 1;

	uint8_t capVal;
	for (uint16_t i=0; i < mid; i++) {
		if (val > 240) { capVal = 240; } else { capVal = val; }
		l[mid + i] = myParams->getColor(capVal);
		l[mid - i] = myParams->getColor(capVal);
		//l[mid + i] = CHSV(0, 255, dim8_video(capVal));
		//l[mid - i] = CHSV(0, 255, dim8_video(capVal));
		if (val >  480 / n) { val -= 480 / n; }
		else { val = 0; }
	}
}

void CylonEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	uint8_t speed = myParams->speed;
	uint8_t val = beat8(g_bpm, g_bpm_timebase);
	fill_solid(l, n, CRGB::Black);
	if ((g_bpm_beatnr & 1) xor ((myParams->vitality)&(0x01) == 0)) {
		val = (0xff ^ val);
	}
	l[(n*val / 256)] = myParams->getColor(myParams->hue);
	blur1d(l, n, myParams->blurryness);
}

void JuggleEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	fadeToBlackBy(l, n, 20);
	byte dothue = myParams->hue;
	for (int i = 0; i < 8; i++) {
		l[beatsin16(g_bpm/8,0, n)] |= myParams->getColor(dothue);
		dothue += myParams->vitality;
	}
}
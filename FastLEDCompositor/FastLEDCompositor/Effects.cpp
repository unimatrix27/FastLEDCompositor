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

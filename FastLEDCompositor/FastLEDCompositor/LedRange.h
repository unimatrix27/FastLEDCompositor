// LEDRange class that does not do more than hold a number of CRGB values and its size. 


#ifndef _LEDRANGE_h
#define _LEDRANGE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "disco.h"

class LedRange
{
protected:

	uint16_t num_leds;
public:
	CRGB* leds;
	LedRange(uint16_t num_leds = NUM_LEDS)
	{
		this->num_leds = num_leds;
		leds = new struct CRGB[num_leds];
		fill_solid(leds, num_leds, CRGB::Black);   // initialize all pixels with black.
	}

	~LedRange()
	{
		delete[] leds;
	}
	struct CRGB* getLeds() {
		return leds;
	}
	uint16_t getNumLeds() {
		return num_leds;
	}
};



#endif
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
	uint16_t startpos;
public:
	CRGB* leds;
	LedRange(uint16_t num_leds = NUM_LEDS, uint16_t startpos=0)
	{
		this->num_leds = num_leds;
		leds = new struct CRGB[num_leds];
		fill_solid(leds, num_leds, CRGB::Black);
		this->startpos = startpos;
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
	uint16_t getStartPos() {
		return startpos;
	}
	void moveStartPos(uint16_t position) {
		this->startpos = position;
	}
};



#endif
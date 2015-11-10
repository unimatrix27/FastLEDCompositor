#ifndef _CHANNEL_h
#define _CHANNEL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "disco.h"
#include "Effect.h"
#include "EffectFactory.h"
#include "ChannelMask.h"
#include "ChannelMaskFactory.h"

class Channel
{
protected:
	boolean is_active = true;
	Effect* effect;
	LedRange* ledrange;
public:

	Channel(uint16_t effect = 0, uint16_t num_leds = NUM_LEDS/4, uint16_t position = 20);
	~Channel();
	void move(uint16_t position = 0);
	void draw();
	LedRange* getLedRange();
};

#endif
// ChannelMasks.h
// Channel mask collection. To be extended a lot. Just some stubs for now.
// getVal will return the brightness of each pixel at a specific point in time during the lifetime of the mask
// The mask is supposed to be deleted when the duration is over

#ifndef _CHANNELMASKS_h
#define _CHANNELMASKS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ChannelMask.h"

class FadeChannelMask:public ChannelMask
{

 public:
	uint8_t getVal(uint16_t num_leds, uint16_t lednum);
};

class WipeChannelMask :public ChannelMask
{

public:
	uint8_t getVal(uint16_t num_leds, uint16_t lednum);
};


#endif


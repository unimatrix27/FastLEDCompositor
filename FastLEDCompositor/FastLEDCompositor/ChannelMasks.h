// ChannelMasks.h

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


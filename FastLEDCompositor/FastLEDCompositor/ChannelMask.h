// ChannelMask.h

#ifndef _CHANNELMASK_h
#define _CHANNELMASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class ChannelMask {
private: 
	long duration;
	long endtime;
	long const starttime = millis();
public:
	ChannelMask();
	void setDuration(long duration = 2000);
	virtual uint8_t getVal(uint16_t num_leds, uint16_t lednum) = 0;

};

#endif


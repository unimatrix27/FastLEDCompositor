// ChannelMask.h
// Channel Mask can be used to lower the brightness of each pixel based on a mask pattern and on time
// with this smooth fade ins and fade outs can be realized
// ChannelMask defines the interface to be implemented by each concrete channel mask and is thus an abstract class.

#ifndef _CHANNELMASK_h
#define _CHANNELMASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class ChannelMask {
protected:
	long duration;
	long endtime;
	long const starttime = millis();
public:
	ChannelMask();
	void setDuration(long duration = 2000);  // channelmasks work in millis
	virtual uint8_t getVal(uint16_t num_leds, uint16_t lednum, boolean direction) = 0;
	uint8_t getValInt(uint16_t num_leds, uint16_t lednum, boolean direction = 0);
	boolean isOver();

};

#endif


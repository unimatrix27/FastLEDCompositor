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
	unsigned long duration;
	unsigned long endtime;
	unsigned long starttime;
	uint8_t startPercent;
public:
	ChannelMask();
	virtual ~ChannelMask() = 0;
	void setDuration(long duration = 2000);  // channelmasks work in millis
	void setStartTime(long myTime);
	uint8_t getValInt(boolean direction = 0);
	void setPercent(uint8_t percent);
	virtual uint8_t getVal(uint16_t num_leds, uint16_t lednum, boolean direction) = 0;
	boolean isOver();

};

#endif


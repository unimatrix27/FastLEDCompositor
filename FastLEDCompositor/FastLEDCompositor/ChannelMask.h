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
	unsigned long duration;					// duration of the channel mask effect in seconds
	unsigned long endtime;					// when will the effect end
	unsigned long starttime;				// when did it start
	uint8_t startPercent;					// start not at the beginning but somewhere in the middle. usefull if during a fade in, a fade out is initiated.
public:
	ChannelMask();
	virtual ~ChannelMask() = 0;				// virtual destructor
	void setDuration(long duration = 2000);  // channelmasks work in millis
	void setStartTime(long myTime);			 
	uint8_t getValInt(boolean direction = 0);   // get value between - and 255 showing how far the effect is already going (e.g. 128 =  50%)
	void setPercent(uint8_t percent);			// set the effect to a specific percentage (if a fade in was just 70% complete, we want to start fade out at 30%)
	virtual uint8_t getVal(uint16_t num_leds, uint16_t lednum, boolean direction) = 0;    // give value of each pixel depending on effect type
	boolean isOver();						// helper function to check if an effect is over, so that it can be deleted.

};

#endif


// Compositor.h
// A compositor holds MAX_CHANNELS channels and blends them into each other
// Many things not yet implemented but the basic idea works

#ifndef _COMPOSITOR_h
#define _COMPOSITOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "disco.h"
#include "Channel.h"
#include "ChannelMask.h"



class Compositor
{
protected:
	CRGB* leds;										// canvas to draw on
	uint16_t num_leds;								// helper info to directly know num of leds
	Channel* channels[NUM_CHANNELS];				// static amount of channel pointers (could be dynamic, but not sure if it makes sense)
	ChannelMask* channelMasks[NUM_CHANNELS];		// number is limited by memory. 
	ParameterSet* chanParams[NUM_CHANNELS];			// channal parameters are static memory for each channel to be able to prepare params before creating the channel
public:
	Compositor(CRGB* leds, uint16_t num_leds);		// assign target LED pattern and num during construction of object. 
	void addChannel(uint8_t channelId = 0, uint8_t velocity = 127);			// add a new channel (and overwrite the pattern that might already be played on the channel)
	void moveChannel(uint8_t channelId=0, uint16_t newPos = 0); // deprecated, use chanParams directly instead
	void setFade(uint8_t channelId=0,uint8_t autoInOut = 0); // fade in or out the channel
	void draw(); // draw all the channels together
	ParameterSet* getParams(uint8_t channel);			// give access to params 
};


#endif


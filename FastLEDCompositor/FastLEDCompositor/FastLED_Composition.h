// FastLED_Composition.h
// A compositor holds MAX_CHANNELS channels and blends them into each other
// Many things not yet implemented but the basic idea works

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "deftypes.h"
#include "Channel.h"
#include "ChannelMask.h"


extern CRGB ColorFromPalette(const CRGBPalette256& pal, uint8_t index, uint8_t brightness, uint8_t saturation);





class FastLED_Composition
{
protected:
	CRGB* leds;										// canvas to draw on
	uint16_t numLeds;								// helper info to directly know num of leds
	Channel* channels[NUM_CHANNELS];				// static amount of channel pointers (could be dynamic, but not sure if it makes sense)
	ChannelMask* channelMasks[NUM_CHANNELS];		// number is limited by memory. 
	ParameterSet* chanParams[NUM_CHANNELS];			// channal parameters are static memory for each channel to be able to prepare params before creating the channel
	uint16_t _beatnr = 0;					// A Beat counter, useful for automatic patterns (not yet implemented)
	long _timebase = 0;					// set to the start of a beat to influence behavior of beatsin8 etc.
	uint16_t _bpm = 80;						// current bpm
	uint8_t _bpmCnt = 0;					// current beat subcount
public:
	FastLED_Composition	(CRGB* leds, uint16_t num_leds);		// assign target LED pattern and num during construction of object. 
	void addChannel(uint8_t channelId = 0, uint8_t velocity = 127);			// add a new channel (and overwrite the pattern that might already be played on the channel)
	void moveChannel(uint8_t channelId=0, uint16_t newPos = 0); // deprecated, use chanParams directly instead
	void setFade(uint8_t channelId=0,uint8_t autoInOut = 0); // fade in or out the channel
	void draw(); // draw all the channels together
	Channel* getChannel(uint8_t channelid);
	ParameterSet* getParams(uint8_t channel);			// give access to params 
	uint16_t getBeatnr();
	void setBeatnr(uint16_t beatnr);
	uint8_t getBpmCnt();
	void setBpmCnt(uint8_t beatnr);
	long getTimebase();
	void setTimebase(long timebase);
	uint16_t getBpm();
	void setBpm(uint16_t bpm);
	int16_t midiToInt(int8_t byte);
	uint16_t getNumLeds();
	
};





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
	Channel* channels[NUM_CHANNELS];				// static amount of channels (could be dynamic, but not sure if it makes sense)
	ChannelMask* channelMasks[NUM_CHANNELS];		// number is limited by memory. 
	BlendType blendTypes[NUM_CHANNELS];				// for each channel there is also one "slot" for a channel mask and a blend type
													// blend type defines, how the pixels are added to the canvas, e.g. overwriting, or averaging, etc.
public:
	Compositor(CRGB* leds, uint16_t num_leds);
	void addChannel(
		uint8_t channelId =0,						// id of new channel
		uint8_t effect = 0,							// which effect to use (maybe change to an enum)
		BlendType blendType = BT_SUM,				// which blend type to use (SUM basically just adds the values and saturates at white)
		FadeType fadeType = FT_NOFADE,				// if there is any fading (during creation a fade in makes sense)
		uint16_t num_leds = NUM_LEDS,				// how many leds...
		uint16_t position = 0,						// where to position the channel
		long fadeDuration = DEFAULT_FADE_TIME,		// if a fade is taken, how long should it take
		TimeBase timeBase = TB_BEATS		);		// fade can mean "amount of beats divided by 24" or "amount of milliseconds"
	void moveChannel(uint8_t channelId=0, uint16_t newPos = 0);
	void setBlendType(uint8_t channelId=0, BlendType blendType=BT_SUM);  // change blend type later
	void setFade(uint8_t channelId=0, FadeType fadeType=FT_FADEIN, TimeBase timebase=TB_BEATS, long time=DEFAULT_FADE_TIME); // set a fade thing later
	void draw(); // draw all the channels together

};


#endif


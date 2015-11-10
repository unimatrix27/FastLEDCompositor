// Compositor.h

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
	CRGB* leds;
	uint16_t num_leds;
	Channel* channels[NUM_CHANNELS];
	ChannelMask* channelMasks[NUM_CHANNELS];
	BlendType blendTypes[NUM_CHANNELS];
public:
	Compositor(CRGB* leds, uint16_t num_leds);
	void addChannel(
		uint8_t channelId =0,
		uint8_t effect = 0, 
		BlendType blendType = BT_SUM,
		FadeType fadeType = FT_NOFADE,
		uint16_t num_leds = NUM_LEDS,
		uint16_t position = 0,
		long fadeDuration = DEFAULT_FADE_TIME,
		TimeBase timeBase = TB_BEATS		);
	void moveChannel(uint8_t channelId=0, uint16_t newPos = 0);
	void setBlendType(uint8_t channelId=0, BlendType blendType=BT_SUM);
	void setFade(uint8_t channelId=0, FadeType fadeType=FT_FADEIN, TimeBase timebase=TB_BEATS, long time=DEFAULT_FADE_TIME);
	void draw();

};


#endif


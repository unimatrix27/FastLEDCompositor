#ifndef _CHANNEL_h
#define _CHANNEL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// include needed definitions
#include "disco.h"							
#include "Effect.h"
#include "EffectFactory.h"
#include "ChannelMask.h"
#include "ChannelMaskFactory.h"
#include "ParameterSet.h"
// Channel Class: This is a container for one effect of one type.
//		It draws to its own ledrange
//		Channels are added to a compositor-Object.

class Channel
{
protected:
	boolean is_active = true;			// activate/deactivate (not yet implemented)
	Effect* effect;						// pointer to an effect object
	LedRange* ledrange;					// pointer to the ledrange used
	ParameterSet* myParams;				// parameters to be used
	long lastMillis;
	long speedBuffer;
	long hueBuffer;
	void processParams();
public:
	// Constructor: select an effect, a size and position
	Channel(ParameterSet* params,uint16_t effect = 0, uint16_t num_leds = NUM_LEDS, uint16_t position = 0);
	~Channel();
	void move(uint16_t position = 0);  // move the channel arround to a new offset
	void draw();					   // Call the effect-draw methode and put everything together
	LedRange* getLedRange();		   // give access to ledrange (used by compositor object for blending channels into each other)
	boolean isActive();				   // return 1 if the channel is active. 
	ParameterSet* getParams();
	void toggle();					   // activate or deactivate channel (automatic after transition with ChannelMask)
	void setActive(bool active);
};

#endif
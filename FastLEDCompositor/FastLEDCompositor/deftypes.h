#pragma once



#include <FastLED.h>
#define NUM_CHANNELS 16					// how much channels a compositor can hold (MIDI supports up to 16 channels)
#define DEFAULT_FADE_TIME 96			// default value for Fade time is 4 full beats (4*24)
enum BlendType {
	BT_SUM,
	BT_OVERLAY, 
	BT_INVERT,
	BT_NEGATIVE
};	// some basic blend types (to be added many more)

enum FadeType {
	FT_NOFADE, 
	FT_FADE, 
	FT_WIPE };  // some very basic fade types (to be added many more)

enum TimeBase { 
	TB_BEATS,
	TB_MILLIS
};  // timebase for fading (either beats or milliseconds)

enum EffectType {
	ET_SOLID, 
	ET_RAINBOW,
	ET_CONFETTI,
	ET_BASEBEAT,
	ET_TWOCOLORS, 
	ET_SINELOIN,
	ET_FIRE,
	ET_CANDLE,
	ET_NOISECANDLE,
	ET_MASKBEATSTROBE,
	ET_MASKBEATMOVER,
	ET_BARMOVER = 128 
}; // all effects (to be added a lot more of course)






  // ALl pallettes used must be listed here to be accessible later. 
enum PaletteName {
	PN_RAINBOW,
	PN_RAINBOWSTRIPE,
	PN_LAVA,
	PN_OCEAN,
	PN_CLOUD,
	PN_FOREST,
	PN_PARTY,
	PN_HEAT,
	PN_CPT_OCEAN,
	PN_CPT_BLUE,
	PN_CPT_FRENCH,
	PN_CPT_GERMAN,
	PN_CPT_SUNSET,
	PN_CPT_BHW,
	PN_AUTO_NEARHUE,
	PN_WHITE,
	PN_AUTO_RANDOM,
	PN_BW=127,
	PN_NUM_VALUES
};




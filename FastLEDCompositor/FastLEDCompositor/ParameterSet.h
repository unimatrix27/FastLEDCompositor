// ParameterSet.h
// A ParameterSet Object holds all configuration data for a channel and the effect played on it. All paramters to be influenced by MIDI or other sequencing means. 

#ifndef _PARAMETERSET_h
#define _PARAMETERSET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "disco.h"

// ALl pallettes used must be listed here to be accessible later. 
enum PaletteName { PN_RAINBOW, PN_RAINBOWSTRIPE, PN_LAVA, PN_OCEAN, PN_CLOUD, PN_FOREST, PN_PARTY, PN_HEAT, PN_CPT_OCEAN, PN_AUTO_RANDOM, PN_AUTO_NEARHUE };

class ParameterSet
{
protected:

	unsigned long paletteBlendStarttime; // used for blending from one palette into another one
	unsigned long paletteBlendEndtime;
	CRGBPalette16 currentPalette;	      // palette in use
	CRGBPalette16 targetPalette;          // palette being blended into
	void makeRandom(CRGBPalette16* targetPalette);  // create random palette

public:
	 // Effect-Parameters
	 // Effects all have access to a fixed defined group of parameters. 
	  // Simple effects will not use all of them and some parameters might have to be added here when more effects are being developed
	 EffectType effectType;		// which effect do we want
	 uint8_t hue;				// 
	 uint8_t saturation;		// not implemented yet
	 uint8_t brightness;
	 uint8_t vitality;			// not used yet
	 int8_t speed;				// speed of whatever happens in the effect (e.g. hue rotation)
	 uint8_t randomness;		// how random should the effect behave. not implemented yet. 
	 uint8_t hueVariability;	// how fluid should the hue change. not sure yet what it should do. not implemented
	 uint8_t blurryness;		// to be used to somehow blur2d the effects. not usd yet. 

	 // Channel-Parameters
	 uint8_t clonecount;		// copies one pattern to more than one position on the target array
	 uint8_t clonedistance;		// distance between such copies
	 uint8_t mirroraxes;		// cuts the strip into pieces with mirror effects. not yet implemetned
	 int8_t	 channelSpeed;		// move the effect around on the full strip (signed value both directions possible)
	 TimeBase fadeTimeBase;		// shoudl the fading be done in miliseconds or in beats
	 FadeType fadeType;			// what fading to use (-> channel masks)
	 long fadeTime;				// how long of short to fade in and out. 0 = no fading
	 long fadeOutTime;			// fade Out could be different time than fade in
	 BlendType blendType;		// how is the channel blended onto the already existing pixels. channels are processed starting with 0.
	 uint16_t numLeds;			// how big is the effect.
	 uint16_t startPos;			// where is it starting on the target strip

	 // Palette Parameters
	 TBlendType paletteBlendType;	// LINEAR or None
	 long paletteBlendDuration;		// how slow should a palette blend into a new one. 

	 // velocity - transfered with every note played if MIDI is used. 7 bits to be used only by a specific effect class for configuration
	 uint8_t velocity;
 public:
	 ParameterSet();
	 ~ParameterSet();
	 struct CRGB getColor(uint8_t index);  // get Color based on Palette 
	 void setTargetPalette(PaletteName myPalette, long duration = 0);  // blend into a new palette

};



#endif


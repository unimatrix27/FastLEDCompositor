// ParameterSet.h
// A ParameterSet Object holds all configuration data for a channel and the effect played on it. All paramters to be influenced by MIDI or other sequencing means. 

#ifndef _PARAMETERSET_h
#define _PARAMETERSET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "deftypes.h"



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
	 uint8_t hue;				// this hue value is based as starting hue. For an ET_SOLID it is the hue of the solid color. 
	 uint8_t saturation;		// not implemented yet
	 uint8_t brightness;		
	 uint8_t vitality;			// not used yet
	 int8_t speed;				// speed of whatever happens in the effect (e.g. hue rotation)
	 uint8_t randomness;		// how random should the effect behave. not implemented yet. 
	 int8_t hueVariability;		// speed of hue change
	 uint8_t blurryness;		// to be used to somehow blur2d the effects. not usd yet. 
	 uint8_t custom1;			// three more custom parameters to be used be specific effects if needed
	 uint8_t custom2;
	 uint8_t custom3;
	 uint8_t effectconf;		   // general configuration parameter, to be used by each effect differently. Often specific bits have different functions.
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
	 uint16_t startPos;			// where is it starting on the target strip. cannot be changed without re-adding the channel because it defines the size of dynamic memory.

	 // Palette Parameters
	 TBlendType paletteBlendType;	// LINEAR or None
	 long paletteBlendDuration;		// how slow should a palette blend into a new one. 


 public:
	 ParameterSet();
	 ~ParameterSet();
	 struct CRGB getColor(uint8_t index,uint8_t brightness=255,uint8_t saturation=255);  // get Color based on Palette. Used for most drawing since everything should be using palettes.
	 void setTargetPalette(PaletteName myPalette, long duration = 0);  // blend into a new palette

};



#endif


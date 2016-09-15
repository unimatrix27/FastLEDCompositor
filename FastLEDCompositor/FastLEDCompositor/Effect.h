// Effect.h
// Abstract Class defining interface for all Effects.

#ifndef _EFFECT_h
#define _EFFECT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "deftypes.h"
#include "LedRange.h"
#include "ParameterSet.h"
class FastLED_Composition;

class Effect
{
protected:
	LedRange* canvas;
	FastLED_Composition* myComp;
public:
	ParameterSet* myParams;
	virtual void draw() = 0;
	Effect();
	virtual ~Effect() = 0;
	void setCanvas(LedRange*);						// assign an LED range to the effect (this is where the effect will draw itself onto)
	void setParameters(ParameterSet* myParams);     // assign direct access to a parameter set.
	void setComp(FastLED_Composition* comp);
	uint16_t getNumLeds();
};




#endif


#ifndef _EFFECT_h
#define _EFFECT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "disco.h"
#include "LedRange.h"

class Effect
{
protected:
	LedRange* canvas;
public:
	virtual void draw() = 0;
	Effect();
	~Effect();
	void setCanvas(LedRange*);
};




#endif


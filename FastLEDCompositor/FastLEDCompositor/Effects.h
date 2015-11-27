// Definitions for all effect classes. Each effect just has to implement its specific draw function
// (should be put in a template)

#pragma once
#include "Effect.h"
class SolidEffect :
	public Effect
{
public:

	void draw();
};

class RainbowEffect :
	public Effect
{
public:
	void draw();
};

class ConfettiEffect :
	public Effect
{
public:
	void draw();
};

class BaseBeatEffect :
	public Effect
{
protected:
	uint32_t timebase;
	uint16_t beat;
	boolean firstRun;
public:
	BaseBeatEffect();
	void draw();
};

class TwoColorsEffect :
	public Effect
{
public:
	void draw();
};

class SineloinEffect :
	public Effect
{
protected:
	uint32_t timebase;
	uint16_t beat;
	boolean firstRun;
	uint16_t prevPos;
public:
	SineloinEffect();
	void draw();
};
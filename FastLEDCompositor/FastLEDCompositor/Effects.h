// Definitions for all effect classes. Each effect just has to implement its specific draw function
// (should be put in a template)

#pragma once
#include "deftypes.h"
#include "Effect.h"
#include "MovingBar.h"
class MovingBar;

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

class MaskBeatMover :
	public Effect
{
protected:
	uint32_t timebase;
	uint32_t beat;
	boolean firstRun;
public :
	void draw();
	void move();
};

class MaskBeatStrobe :
	public Effect
{
protected:
	unsigned long  nextHit;
	unsigned long hitLength;
public:
	void draw();
	void hit(uint8_t beatcounts);
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


class FireEffect :
	public Effect
{
protected:
	boolean firstRun;
	uint8_t* heat;
public:
	FireEffect();
	~FireEffect();
	void init();
	void draw();
};

class CandleEffect : public Effect {
protected:
	long lastMillis = 0;
public:
	void draw();
};

class NoiseCandleEffect : public Effect {
protected:
	uint16_t x, y, z;
	boolean firstRun;
	uint8_t *noise;
public:
	NoiseCandleEffect();
	~NoiseCandleEffect(); 
	void draw();
	void init();
};

class BarMover :
	public Effect {
protected:
	MovingBar* firstBar;
	MovingBar* currentBar;
	MovingBar* lastBar;
	MovingBar* removeBar(MovingBar* bar);
	MovingBar* findPrev(MovingBar * bar);
	uint8_t currentNote;
	long speedBuffer = 0;
	long lastMillis = 0;
public:
	BarMover();
	~BarMover();
	void init();
	MovingBar* addBar(uint8_t startHue, uint16_t size, uint8_t startingHue);
	MovingBar* rezizeBar(MovingBar* bar, uint16_t size);
	void draw();
	void noteOn(uint8_t signature, uint8_t startHue);
	void noteOff(uint8_t signature);
	uint8_t isFree(uint16_t pixelnum,uint8_t signature);
	boolean checkOverlap(MovingBar* bar);
	void process();
};
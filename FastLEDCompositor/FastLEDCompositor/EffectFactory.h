// EffectFactory.h
// Factory Class to get new effects. Singleton

#ifndef _EFFECTFACTORY_h
#define _EFFECTFACTORY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "disco.h"
#include "Effect.h"
#include "Effects.h"


class EffectFactory {
private:
	static  EffectFactory* instance;
protected:
	EffectFactory() {}
public:
	static EffectFactory* getInstance();
	Effect* orderTheEffect(uint16_t effectNumber, LedRange* ledrange, ParameterSet* params);
};



#endif


#include "EffectFactory.h"



EffectFactory* EffectFactory::instance = NULL;
EffectFactory* EffectFactory::getInstance() {
	if (instance == NULL) instance = new EffectFactory();
	return instance;
}

Effect* EffectFactory::orderTheEffect(uint16_t effectNumber, LedRange* ledrange) {
	Effect* myEffect;
	switch (effectNumber) {
	case  1:
		myEffect = new RainbowEffect();
		break;
	default:
		myEffect = new SolidEffect();
		break;
	}
	myEffect->setCanvas(ledrange);
	return myEffect;
}




#include "EffectFactory.h"



EffectFactory* EffectFactory::instance = NULL;
EffectFactory* EffectFactory::getInstance() {
	if (instance == NULL) instance = new EffectFactory();
	return instance;
}

Effect* EffectFactory::orderTheEffect(uint16_t effectNumber, LedRange* ledrange, ParameterSet* params) {
	Effect* myEffect;
	switch (effectNumber) {
	case 3:
		myEffect = new BaseBeatEffect();
		break;
	case 2:
		myEffect = new ConfettiEffect();
		break;
	case  1:
		myEffect = new RainbowEffect();
		break;
	default:
		myEffect = new SolidEffect();
		break;
	}
	myEffect->setCanvas(ledrange);
	myEffect->setParameters(params);
	return myEffect;
}




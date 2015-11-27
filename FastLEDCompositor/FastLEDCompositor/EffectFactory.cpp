#include "EffectFactory.h"



EffectFactory* EffectFactory::instance = NULL;
EffectFactory* EffectFactory::getInstance() {
	if (instance == NULL) instance = new EffectFactory();
	return instance;
}

Effect* EffectFactory::orderTheEffect(uint16_t effectNumber, LedRange* ledrange, ParameterSet* params) {
	Effect* myEffect;
	switch (effectNumber) {
	case ET_SINELOIN:
		myEffect = new SineloinEffect();
		break;
	case ET_TWOCOLORS:
		myEffect = new TwoColorsEffect();
		break;
	case ET_BASEBEAT:
		myEffect = new BaseBeatEffect();
		break;
	case ET_CONFETTI:
		myEffect = new ConfettiEffect();
		break;
	case  ET_RAINBOW:
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




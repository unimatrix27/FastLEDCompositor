#include "EffectFactory.h"



EffectFactory* EffectFactory::instance = NULL;
EffectFactory* EffectFactory::getInstance() {
	if (instance == NULL) instance = new EffectFactory();
	return instance;
}

Effect* EffectFactory::orderTheEffect(uint16_t effectNumber, LedRange* ledrange, ParameterSet* params) {
	Effect* myEffect;
	switch (effectNumber) {
	case ET_JUGGLE:
		myEffect = new JuggleEffect();
		break;
	case ET_CYLON:
		myEffect = new CylonEffect();
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




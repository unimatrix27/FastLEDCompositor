#include "EffectFactory.h"
#include "FastLED_Composition.h"



EffectFactory* EffectFactory::instance = NULL;
EffectFactory* EffectFactory::getInstance() {
	if (instance == NULL) instance = new EffectFactory();
	return instance;
}

Effect* EffectFactory::orderTheEffect(uint16_t effectNumber, LedRange* ledrange, ParameterSet* params, FastLED_Composition* comp) {
	Effect* myEffect;
	switch (effectNumber) {
	case ET_CANDLE:
		myEffect = new CandleEffect();
		break;
	case ET_NOISECANDLE:
		myEffect = new NoiseCandleEffect();
		break;
	case ET_FIRE:
		myEffect = new FireEffect();
		break;
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
	case  ET_BARMOVER:
		myEffect = new BarMover();
		break;
	case ET_MASKBEATMOVER:
		myEffect = new MaskBeatMover();
		break;
	case ET_MASKBEATSTROBE:
		myEffect = new MaskBeatStrobe();
		break;
	default:
		myEffect = new SolidEffect();
		break;
	}
	myEffect->setCanvas(ledrange);
	myEffect->setParameters(params);
	myEffect->setComp(comp);
	return myEffect;
}




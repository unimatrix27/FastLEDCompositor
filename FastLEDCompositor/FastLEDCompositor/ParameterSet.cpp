// 
// 
// 

#include "ParameterSet.h"
#include "disco.h"
DEFINE_GRADIENT_PALETTE(GMT_ocean_gp) {
	0, 0, 0, 0,
		31, 0, 1, 1,
		63, 0, 1, 4,
		95, 0, 19, 42,
		127, 0, 79, 138,
		159, 15, 144, 112,
		191, 91, 233, 89,
		223, 155, 244, 158,
		255, 242, 255, 255
};
ParameterSet::ParameterSet()
{
	effectType = ET_RAINBOW;
	hue = 0;
	saturation = 255;
	brightness = 128;
	vitality = 24;
	speed = 0;
	randomness = 0;
	hueVariability = 0;
	blurryness = 0;
	currentPalette = RainbowColors_p;
	targetPalette = RainbowColors_p;
	paletteBlendType = LINEARBLEND;
	paletteBlendStarttime = 0;
	paletteBlendEndtime = 0;
	clonecount = 0;
	clonedistance = 50;
	mirroraxes = 0;
	channelSpeed = 0;
	fadeTimeBase = TB_MILLIS;
	fadeTime = 0;
	fadeOutTime = 0;
	paletteBlendDuration = 4;
	fadeType = FT_NOFADE;
	blendType = BT_OVERLAY;
	numLeds = NUM_LEDS;
	startPos = 0;

}
ParameterSet::~ParameterSet() {

}

void ParameterSet::setTargetPalette(PaletteName myPalette, long duration) {
	CRGBPalette16 blendedPalette;
	switch (myPalette) {
	default:
	case PN_RAINBOW: targetPalette = RainbowColors_p; break;
	case PN_RAINBOWSTRIPE: targetPalette = RainbowStripeColors_p; break;
	case PN_FOREST: targetPalette = ForestColors_p; break;
	case PN_LAVA: targetPalette = LavaColors_p; break;
	case PN_CLOUD:  targetPalette = CloudColors_p; break;
	case PN_OCEAN:  targetPalette = OceanColors_p; break;
	case PN_PARTY:  targetPalette = PartyColors_p; break;
	case PN_HEAT:  targetPalette = HeatColors_p; break;
	case PN_CPT_OCEAN: targetPalette = GMT_ocean_gp; break;
	case PN_AUTO_RANDOM:  makeRandom(&targetPalette); break;
	}
	if (duration == 0) {
		currentPalette = targetPalette;
		paletteBlendEndtime = paletteBlendStarttime = 0;
	}
	else {
		if (paletteBlendEndtime > paletteBlendStarttime && millis() < paletteBlendEndtime) {
			blend(currentPalette, targetPalette, blendedPalette, 16, (millis() - paletteBlendStarttime) / (paletteBlendEndtime - paletteBlendStarttime));
			currentPalette = blendedPalette;
		}
		paletteBlendStarttime = millis();
		paletteBlendEndtime = paletteBlendStarttime + duration;
	}
}

void ParameterSet::makeRandom(CRGBPalette16* target) {
	for (int i = 0; i < 16; i++) {
		(*target)[i] =  CHSV(random8(), 255, 255);
	}
}

struct CRGB ParameterSet::getColor(uint8_t index) {
	struct CRGB result;

	if (paletteBlendEndtime != paletteBlendStarttime && millis() >= paletteBlendEndtime) {
		paletteBlendEndtime = paletteBlendStarttime;
		currentPalette = targetPalette;
	}
	if (paletteBlendEndtime != paletteBlendStarttime) {
		uint8_t passed = 255* (millis() - paletteBlendStarttime) / (paletteBlendEndtime - paletteBlendStarttime);
		result = blend(ColorFromPalette(currentPalette, index, 255, this->paletteBlendType),
			ColorFromPalette(targetPalette, index, 255, this->paletteBlendType),passed);
		return result;
	}
	else {
		result = ColorFromPalette(currentPalette, index,255, this->paletteBlendType);
		return result;
	}
}
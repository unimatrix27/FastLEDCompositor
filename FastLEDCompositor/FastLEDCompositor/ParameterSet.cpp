// 
// 
// 

#include "ParameterSet.h"
#include "deftypes.h"

#include "FastLED_Composition.h"

// Gradient palette "French_flag_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/French_flag.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE(GMT_French_flag_gp) {
	0, 0, 0, 255,
		42, 0, 0, 255,
		84, 0, 0, 255,
		84, 255, 255, 255,
		127, 255, 255, 255,
		170, 255, 255, 255,
		170, 255, 0, 0,
		212, 255, 0, 0,
		255, 255, 0, 0
};



// Gradient palette "German_flag_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/German_flag.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE(GMT_German_flag_gp ) {
    0,   0,  0,  0,
   42,   0,  0,  0,
   84,   0,  0,  0,
   84, 255,  0,  0,
  127, 255,  0,  0,
  170, 255,  0,  0,
  170, 255,255,  0,
  212, 255,255,  0,
  255, 255,255,  0};
// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE(GMT_Sunset_Real_gp) {
	0, 120, 0, 0,
		22, 179, 22, 0,
		51, 255, 104, 0,
		85, 167, 22, 18,
		135, 100, 0, 103,
		198, 16, 0, 130,
		255, 0, 0, 160
};

// Gradient palette "bhw4_039_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_039.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE(GMT_bhw4_039_gp) {
	0, 7, 4, 30,
		40, 1, 37, 91,
		47, 7, 37, 84,
		71, 25, 38, 78,
		107, 20, 56, 119,
		137, 42, 95, 144,
		175, 0, 52, 99,
		192, 2, 40, 88,
		221, 15, 30, 78,
		253, 13, 12, 54,
		255, 12, 2, 36
};


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

DEFINE_GRADIENT_PALETTE(GMT_blue_gp) {
	0, 12, 22, 33,
		127, 71, 90, 142,
		255, 146, 166, 212
};

DEFINE_GRADIENT_PALETTE(custom_bw) {
	0, 0, 0, 0,
		128, 255, 255, 255,
		255, 0, 0, 0
};

ParameterSet::ParameterSet()
{
	effectType = ET_RAINBOW;
	hue = 0;
	saturation = 255;
	brightness = 128;
	vitality = 0;
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
	fadeTime = 1;
	fadeOutTime = 1;
	paletteBlendDuration = 4;
	fadeType = FT_FADE;
	blendType = BT_OVERLAY;
	numLeds = 50;
	startPos = 0;
	effectconf = 0;
	custom1 = 0;
	custom2 = 0;
	custom3 = 0;

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
	case PN_CPT_BLUE: targetPalette = GMT_blue_gp; break;
	case PN_CPT_FRENCH: targetPalette = GMT_French_flag_gp; break;
	case PN_CPT_GERMAN: targetPalette = GMT_German_flag_gp; break;
	case PN_CPT_SUNSET: targetPalette = GMT_Sunset_Real_gp; break;
	case PN_CPT_BHW: targetPalette = GMT_bhw4_039_gp; break;
	case PN_AUTO_RANDOM:  makeRandom(&targetPalette); break;
	case PN_BW: targetPalette = custom_bw; break;
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

struct CRGB ParameterSet::getColor(uint8_t index,uint8_t brightness,uint8_t saturation) {
	// saturation not yet supported, will always return saturation as defined in palette
	struct CRGB result;

	if (paletteBlendEndtime != paletteBlendStarttime && millis() >= paletteBlendEndtime) {
		paletteBlendEndtime = paletteBlendStarttime;
		currentPalette = targetPalette;
	}
	if (paletteBlendEndtime != paletteBlendStarttime) {
		uint8_t passed = 255* (millis() - paletteBlendStarttime) / (paletteBlendEndtime - paletteBlendStarttime);
		result = blend(ColorFromPalette(currentPalette, index,brightness, this->paletteBlendType),
			ColorFromPalette(targetPalette, index, brightness, this->paletteBlendType),passed);
	}
	else {
		result = ColorFromPalette(currentPalette, index,brightness, this->paletteBlendType);
	}
	return result;

}
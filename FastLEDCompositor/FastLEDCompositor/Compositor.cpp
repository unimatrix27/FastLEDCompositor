// 
// 
// 

#include "Compositor.h"
#include "Channel.h"

Compositor::Compositor(CRGB* leds, uint16_t num_leds) {
	this->leds = leds;
	this->num_leds = num_leds;
}

void Compositor::addChannel(
	uint8_t channelId ,
	uint8_t effect ,
	BlendType blendType ,
	FadeType fadeType ,
	uint16_t num_leds ,
	uint16_t position ,
	long fadeDuration ,
	TimeBase timeBase ){

	if (channels[channelId] != NULL) {
		delete channels[channelId];
	}
	channels[channelId] = new Channel(effect, num_leds, position);
	setBlendType(channelId, blendType);
	if (fadeType != FT_NOFADE) {
		setFade(channelId, fadeType, timeBase, fadeDuration);
	}
}

void Compositor::moveChannel(uint8_t channelId, uint16_t newPos) {
	if (channels[channelId] != NULL) {
		channels[channelId]->move(newPos);
	}
}

void Compositor::setBlendType(uint8_t channelId, BlendType blendType) {
	this->blendTypes[channelId] = blendType;
}
void Compositor::setFade(uint8_t channelId, FadeType fadeType, TimeBase timebase, long time) {
	if (channels[channelId] != NULL) {
		channelMasks[channelId] = ChannelMaskFactory::getInstance()->orderTheChannelMask(fadeType, time);
	}
}

void Compositor::draw() {
	LedRange* myLedRange;
	for (uint8_t i= 0; i < NUM_CHANNELS; i++) {
		if (channels[i] == NULL) continue;
		channels[i]->draw();
		myLedRange = channels[i]->getLedRange();
		for (int l = 0; l < myLedRange->getNumLeds(); l++) {
			leds[(l + (myLedRange->getStartPos())) % (NUM_LEDS)] += myLedRange->getLeds()[l];
		}
	}

}
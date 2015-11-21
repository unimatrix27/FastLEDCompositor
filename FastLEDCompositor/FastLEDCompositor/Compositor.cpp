// 
// 
// 

#include "Compositor.h"
#include "Channel.h"

Compositor::Compositor(CRGB* leds, uint16_t num_leds) {
	this->leds = leds;
	this->num_leds = num_leds;
	for (int i = 0; i < NUM_CHANNELS; i++) {
		channels[i] = nullptr;
		channelMasks[i] = NULL;
		chanParams[i] = new ParameterSet();
	}
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
	if (channelId >= NUM_CHANNELS) {
		return;
	}
	if (channels[channelId] != NULL) {
		delete channels[channelId];
		channels[channelId] = NULL;
	}
	channels[channelId] = new Channel(chanParams[channelId],effect, num_leds, position);
	setBlendType(channelId, blendType);
	if (fadeType != FT_NOFADE) {
		setFade(channelId, fadeType, timeBase, fadeDuration,1);
	}
	else {
		channels[channelId]->setActive(1);
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
void Compositor::setFade(uint8_t channelId, FadeType fadeType, TimeBase timebase, long time, uint8_t autoInOut) {
	uint8_t oldPercentage = 255;
	if (channels[channelId] != NULL) {
		if (autoInOut == 1) {
			channels[channelId]->setActive(0);
		}
		if (autoInOut == 2) {
			channels[channelId]->setActive(1);
		}
		if (timebase == TB_BEATS) {
			time = time * (60000 / (4 * g_bpm));
		}
		if (channelMasks[channelId] != NULL) {
			oldPercentage = channelMasks[channelId]->getValInt();
			delete channelMasks[channelId];
		}
		channelMasks[channelId] = ChannelMaskFactory::getInstance()->orderTheChannelMask(fadeType, time);
		channelMasks[channelId]->setPercent(oldPercentage);
	}
}

void Compositor::draw() {
	LedRange* myLedRange;
	uint16_t myStartPos;
	CRGB* myLeds;
	boolean applyMask = false;
	fill_solid(leds, NUM_LEDS, CRGB::Black);
	for (uint8_t i= 0; i < NUM_CHANNELS; i++) {
		if (channels[i] == NULL) continue;
		if (channelMasks[i] != NULL) {
			if (channelMasks[i]->isOver()) {
				//delete channelMasks[i];
				channelMasks[i] = NULL;
				channels[i]->toggle();
				applyMask = false;
				if(channels[i]->isActive()==false) continue;
			}
			else {
				applyMask = true;
			}
		}
		else {
			applyMask = false;
			if (channels[i]->isActive() == false) continue;
		}
		channels[i]->draw();
		myLedRange = channels[i]->getLedRange();
		myStartPos = myLedRange->getStartPos();
		myLeds = myLedRange->getLeds();
		for (int l = 0; l < myLedRange->getNumLeds(); l++) {
			if (applyMask) {
				nscale8x3(myLeds[l].r, (myLeds[l].g), myLeds[l].b, dim8_raw(channelMasks[i]->getVal(myLedRange->getNumLeds(), l,channels[i]->isActive())));
			}
			for (uint8_t s = 0; s <= channels[i]->getParams()->clonecount; s++) {
				uint16_t offset = s*channels[i]->getParams()->clonedistance;
				leds[(l + myStartPos + offset) % (NUM_LEDS)] += myLeds[l];
			}
		}
	}

}

ParameterSet* Compositor::getParams(uint8_t channel) {
	if (channels[channel] != NULL) {
		return channels[channel]->getParams();
	}
	else {
		return NULL;
	}
}
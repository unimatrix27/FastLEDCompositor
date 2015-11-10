#include "Channel.h"



Channel::Channel(uint16_t effect, uint16_t num_leds,uint16_t position) 
{
	ledrange = new LedRange(num_leds, position);
	this->effect = EffectFactory::getInstance()->orderTheEffect(effect,ledrange);
	is_active = true;
}


Channel::~Channel()
{
}

void Channel::draw() {
	effect->draw();
}

LedRange* Channel::getLedRange() {
	return ledrange;
}

void Channel::move(uint16_t position) {
	ledrange->moveStartPos(position);
}
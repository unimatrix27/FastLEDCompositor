#include "Channel.h"



Channel::Channel(uint16_t effect, uint16_t num_leds,uint16_t position) 
{
	ledrange = new LedRange(num_leds, position);
	this->effect = EffectFactory::getInstance()->orderTheEffect(effect,ledrange);
	is_active = false;
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

boolean Channel::isActive() {
	return is_active;
}

void Channel::toggle() {
	if (is_active) {
		is_active = false;
	}
	else {
		is_active = true;
	}
}
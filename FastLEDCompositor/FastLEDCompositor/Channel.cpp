#include "Channel.h"
#include "MIDIPlayer.h"

Channel::Channel( ParameterSet* params,uint16_t effect, uint16_t num_leds,uint16_t position)
{
	ledrange = new LedRange(num_leds, position);
	myParams = params;
	this->effect = EffectFactory::getInstance()->orderTheEffect(effect,ledrange,myParams);
	is_active = false;
	lastMillis = millis();
	speedBuffer = 0;
	hueBuffer = 0;
}


Channel::~Channel()
{
	if (this->effect != NULL) {
		delete effect;
		effect = NULL;
	}
	if (this->ledrange != NULL) {
		delete ledrange;
		effect = NULL;
	}
}

void Channel::processParams() {
	// process channel speed and move animation including its clones around if speed is given
	int8_t  moveAmount;
	long timePassed;
	timePassed = 256 * (millis() - lastMillis);     // hoping this is faster than calling millis a lot of times.
	// Channel movement based on Channel Speed
	speedBuffer += MIDIPlayer::getInstance()->midiToInt(myParams->channelSpeed);
	if (speedBuffer > timePassed || (-1*speedBuffer)>timePassed) {
		moveAmount = speedBuffer / timePassed;
		speedBuffer -= moveAmount*timePassed;
		move(ledrange->getStartPos() + (moveAmount));
	}
	hueBuffer += MIDIPlayer::getInstance()->midiToInt(myParams->speed);
	if (hueBuffer > timePassed || (-1 * hueBuffer)>timePassed) {
		moveAmount = hueBuffer / timePassed;
		hueBuffer -= moveAmount*timePassed;
		myParams->hue += moveAmount;
	}
	lastMillis = millis();
	
	//
	//

	// Hue changes based on hue Variability and Vitality
	hueBuffer += MIDIPlayer::getInstance()->midiToInt(myParams->speed);
	
}

ParameterSet* Channel::getParams() {
	return myParams;
}
void Channel::draw() {
	processParams();
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

void Channel::setActive(bool active) {
	is_active = active;
}

void Channel::toggle() {
	if (is_active) {
		is_active = false;
	}
	else {
		is_active = true;
	}
}
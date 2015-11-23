#include "Channel.h"
#include "MIDIPlayer.h"

Channel::Channel( ParameterSet* params)  // Construct a new channel and initialise some values
{
	myParams = params;
	ledrange = new LedRange(myParams->numLeds);
	this->effect = EffectFactory::getInstance()->orderTheEffect(myParams->effectType,ledrange,myParams);  // get a new effect object
	is_active = false;	// a channel is always initialized as not active, but will be set active by the composition object
	lastMillis = millis();	// timestamp of when the channel was created for parameter processing
	speedBuffer = 0; // buffers for automatic movement of things (used in processParams)
	hueBuffer = 0;
}


Channel::~Channel()  // make sure there is no mem leak
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
	speedBuffer += midiToInt(myParams->channelSpeed);			// accumulate speed in an internal buffer and move x pixels depending on how far we got. 
	if (speedBuffer > timePassed || (-1*speedBuffer)>timePassed) {
		moveAmount = speedBuffer / timePassed;
		speedBuffer -= moveAmount*timePassed;
		move(this->getParams()->startPos + (moveAmount));
	}
	hueBuffer += midiToInt(myParams->speed);					// same as for channel speed, we increase the hue based on speed (speed moves hue - maybe should be done differently)
	if (hueBuffer > timePassed || (-1 * hueBuffer)>timePassed) {
		moveAmount = hueBuffer / timePassed;
		hueBuffer -= moveAmount*timePassed;
		myParams->hue += moveAmount;
	}
	lastMillis = millis();
	
	//
	//

	// Hue changes based on hue Variability and Vitality
	hueBuffer += midiToInt(myParams->speed);   
	
}

ParameterSet* Channel::getParams() {
	return myParams;
}
void Channel::draw() {   // each frame, process the parameters first, then draw the effect
	processParams();
	effect->draw();
}

LedRange* Channel::getLedRange() {
	return ledrange;
}

void Channel::move(uint16_t position) {
	this->getParams()->startPos = position;			// set a new static position
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
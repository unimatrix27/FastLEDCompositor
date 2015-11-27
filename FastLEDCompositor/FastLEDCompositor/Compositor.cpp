// Main Class to do most of the work
// A Composition holds all the channel pointers and calls the drawing functions of all channels
// It then blends the information together into one target array
// channels are stacked on top of each other starting 0. 
// channels can have non-overlapping or overlapping positions on the main canvas

#include "Compositor.h"
#include "Channel.h"

Compositor::Compositor(CRGB* leds, uint16_t num_leds) {
	this->leds = leds;
	this->num_leds = num_leds;
	for (int i = 0; i < NUM_CHANNELS; i++) {   // create a ParameterSet for each channel, so that they can be manipulated even if the channel is not there yet 
		channels[i] = nullptr;
		channelMasks[i] = NULL;
		chanParams[i] = new ParameterSet();
	}
}

void Compositor::addChannel(uint8_t channelId,uint8_t velocity ){		// add and display a new channel. All parameters for the creation are taken from the parameterset object.
	if (channelId >= NUM_CHANNELS) {
		return;
	}
	if (channels[channelId] != NULL) {
		delete channels[channelId];
		channels[channelId] = NULL;
	}
	channels[channelId] = new Channel(chanParams[channelId]);
	chanParams[channelId]->velocity = velocity;
	if (chanParams[channelId]->fadeType != FT_NOFADE) {
		setFade(channelId,1);
	}
	else {
		channels[channelId]->setActive(1);
	}
}

void Compositor::moveChannel(uint8_t channelId, uint16_t newPos) {  // channel to new position
	if (channels[channelId] != NULL) {
		channels[channelId]->move(newPos);
	}
}

void Compositor::setFade(uint8_t channelId,uint8_t autoInOut) {			// start a new fading in or out. 
	uint8_t oldPercentage = 255;
	long time;
	if (channels[channelId] != NULL) {									// is the channel even there yet?
		if (autoInOut == 1) {
			channels[channelId]->setActive(0);							// it is a fade in
		}
		if (autoInOut == 2) {
			channels[channelId]->setActive(1);							// it is a fade out
		}
		if (channels[channelId]->getParams()->fadeTimeBase == TB_BEATS) {	// how to count - beats or millis?
			time = time * (60000 / (4 * g_bpm));							// transfer beats into millis ( time in beat mode is given as amount of  1/16th notes (1 beat = 1/4 note)
		}
		if (channelMasks[channelId] != NULL) {							// is there already a fade running?
			oldPercentage = channelMasks[channelId]->getValInt();		// how far is it already progressed
			delete channelMasks[channelId];								// delete the old fade
		}
		if (channels[channelId]->isActive()) {
			time = channels[channelId]->getParams()->fadeOutTime;
		}else{
			time = channels[channelId]->getParams()->fadeTime;
		}
		channelMasks[channelId] = ChannelMaskFactory::getInstance()->orderTheChannelMask(channels[channelId]->getParams()->fadeType, time); // get a new channel mask object
		channelMasks[channelId]->setPercent(oldPercentage);				// init the new object with the starting time 
	}
}


// draw all channels together
void Compositor::draw() {
	LedRange* myLedRange;	// buffer for pointer to simplify code
	uint16_t myStartPos;	// buffer for startpos to simplify code
	CRGB* myLeds;			// buffer for pointer to LEDs to simplify code
	boolean applyMask = false;	// is there a channel mask to apply
	fill_solid(leds, NUM_LEDS, CRGB::Black);	// initialize the whole thing with black each frame
	//fill_solid(leds, g_bpm, CRGB::Gray);		// for testing
	for (uint8_t i= 0; i < NUM_CHANNELS; i++) {	// go through all channels
		if (channels[i] == NULL) continue;	// if the channel is not active, continue
		if (channelMasks[i] != NULL) {		// is there a channel mask active?
			if (channelMasks[i]->isOver()) {	// is it maybe already over? then delete it
				delete channelMasks[i];
				channelMasks[i] = NULL;
				channels[i]->toggle();		// toggle the status, because the fade is now over
				applyMask = false;			// and therefore there is no mask to apply in this frame
				if(channels[i]->isActive()==false) continue;	// maybe we did just deactivate? then continue
			}
			else {
				applyMask = true;
			}
		}
		else {
			applyMask = false;
			if (channels[i]->isActive() == false) continue;		// was it not active anyway? then continue
		}
		channels[i]->draw();			// now draw the actual channel
		myLedRange = channels[i]->getLedRange();	// get the LED range info about the channel that was just processed with draw()
		myStartPos = channels[i]->getParams()->startPos;
		myLeds = myLedRange->getLeds();
		for (int l = 0; l < myLedRange->getNumLeds(); l++) {		// go through each LED individually
			if (applyMask) {			// scale it down if there is a mask active based on the mask byte for the LED number
				nscale8x3(myLeds[l].r, (myLeds[l].g), myLeds[l].b, dim8_raw(channelMasks[i]->getVal(myLedRange->getNumLeds(), l,channels[i]->isActive())));
			}
			for (uint8_t s = 0; s <= channels[i]->getParams()->clonecount; s++) {	// progress through all clones of the channel if there are any
				uint16_t offset = s*channels[i]->getParams()->clonedistance;		// set an offset based on the clonedistance param
				CRGB p = myLeds[l];
				switch (channels[i]->getParams()->blendType) {						// depending on blend type, use different ways to put pixels onto the canvas
				case BT_SUM:														// BT SU = just add the values
					if(channels[i]->getParams()->brightness<255) nscale8x3(p.r, p.g, p.b, dim8_raw(channels[i]->getParams()->brightness));
					leds[(l + myStartPos + offset) % (NUM_LEDS)] += p;
					break;
				case BT_OVERLAY:													// BT OVERLAY - use the blend function
					leds[(l + myStartPos + offset) % (NUM_LEDS)]=blend(leds[(l + myStartPos + offset) % (NUM_LEDS)], p, channels[i]->getParams()->brightness);
					break;
			default:
					leds[(l + myStartPos + offset) % (NUM_LEDS)] = p;
					break;
				}
			}
		}
	}

}

ParameterSet* Compositor::getParams(uint8_t channel) {
	if (channel >= NUM_CHANNELS) {
		return NULL;
	}
	return chanParams[channel];
}


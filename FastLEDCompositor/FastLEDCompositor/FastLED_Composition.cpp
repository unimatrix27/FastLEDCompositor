// Main Class to do most of the work
// A Composition holds all the channel pointers and calls the drawing functions of all channels
// It then blends the information together into one target array
// channels are stacked on top of each other starting 0. 
// channels can have non-overlapping or overlapping positions on the main canvas

#include "FastLED_Composition.h"
#include "Channel.h"


CRGB ColorFromPalette(const CRGBPalette256& pal, uint8_t index, uint8_t brightness, uint8_t saturation)
{
	const CRGB* entry = &(pal[0]) + index;

	uint8_t red = entry->red;
	uint8_t green = entry->green;
	uint8_t blue = entry->blue;

	// added manipulation of saturation
	// variables need to become externally accessible
	// just for testing
	uint8_t maxSaturation = 255;
	uint8_t minSaturation = 0;

	// scale saturation in the right range. 
	// and inverse the value (255-saturation)
	saturation = lerp8by8(255 - maxSaturation, 255 - minSaturation, 255 - saturation);

	// make saturation appear linear
	// I picked now raw, since we we won't have issues like with dimming brightness:
	// https://plus.google.com/112889495015752015404/posts/UGXWJtFvz61
	saturation = dim8_raw(saturation);

	// apply saturation
	// qadd8 works faster? Because we just add saturation
	red = lerp8by8(red, 255, saturation);
	green = lerp8by8(green, 255, saturation);
	blue = lerp8by8(blue, 255, saturation);




	if (brightness != 255) {
		nscale8x3_video(red, green, blue, brightness);
	}

	return CRGB(red, green, blue);
}

FastLED_Composition::FastLED_Composition(CRGB* leds, uint16_t numLeds) {
	this->leds = leds;
	this->numLeds = numLeds;
	for (int i = 0; i < NUM_CHANNELS; i++) {   // create a ParameterSet for each channel, so that they can be manipulated even if the channel is not there yet 
		channels[i] = nullptr;
		channelMasks[i] = NULL;
		chanParams[i] = new ParameterSet();
	}
}

void FastLED_Composition::addChannel(uint8_t channelId,uint8_t effectconf ){		// add and display a new channel. All parameters for the creation are taken from the parameterset object.
	if (channelId >= NUM_CHANNELS) {
		return;
	}
	if (channels[channelId] != NULL) {
		delete channels[channelId]; // delete channel, if there existed one already. 
		channels[channelId] = NULL;
	}
	channels[channelId] = new Channel(chanParams[channelId], this);
	chanParams[channelId]->effectconf = effectconf;
	if (chanParams[channelId]->fadeType != FT_NOFADE) {
		setFade(channelId,1);
	}
	else {
		channels[channelId]->setActive(1);
	}
}

void FastLED_Composition::moveChannel(uint8_t channelId, uint16_t newPos) {  // channel to new position
	if (channels[channelId] != NULL) {
		channels[channelId]->move(newPos);
	}
}

void FastLED_Composition::setFade(uint8_t channelId,uint8_t autoInOut) {			// start a new fading in or out. 
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
			time = time * (60000 / (4 * _bpm));							// transfer beats into millis ( time in beat mode is given as amount of  1/16th notes (1 beat = 1/4 note)
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
void FastLED_Composition::draw() {
	LedRange* myLedRange;	// buffer for pointer to simplify code
	uint16_t myStartPos;	// buffer for startpos to simplify code
	CRGB* myLeds;			// buffer for pointer to LEDs to simplify code
	boolean applyMask = false;	// is there a channel mask to apply
	fill_solid(leds, this->numLeds, CRGB::Black);	// initialize the whole thing with black each frame
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
					leds[(l + myStartPos + offset) % (this->numLeds)] += p;
					break;
				case BT_NEGATIVE:														// BT SU = substract the values
					if (channels[i]->getParams()->brightness<255) nscale8x3(p.r, p.g, p.b, dim8_raw(channels[i]->getParams()->brightness));
					leds[(l + myStartPos + offset) % (this->numLeds)] -= p;
					break;
				case BT_OVERLAY:													// BT OVERLAY - use the blend function
					leds[(l + myStartPos + offset) % (this->numLeds)]=blend(leds[(l + myStartPos + offset) % (this->numLeds)], p, channels[i]->getParams()->brightness);
					break;
			default:
					leds[(l + myStartPos + offset) % (this->numLeds)] = p;
					break;
				}
			}
		}
	}

}

uint16_t FastLED_Composition::getNumLeds() {
	return numLeds;
}

ParameterSet* FastLED_Composition::getParams(uint8_t channel) {
	if (channel >= NUM_CHANNELS) {
		return NULL;
	}
	return chanParams[channel];
}

Channel* FastLED_Composition::getChannel(uint8_t channel) {
	return channels[channel];
}

uint16_t  FastLED_Composition::getBeatnr() {
	return _beatnr;
}
void FastLED_Composition::setBeatnr(uint16_t beatnr) {
	_beatnr = beatnr;
}

uint8_t  FastLED_Composition::getBpmCnt() {
	return _bpmCnt;
}
void FastLED_Composition::setBpmCnt(uint8_t beatnr) {
	_bpmCnt = beatnr;
}

long  FastLED_Composition::getTimebase() {
	return _timebase;
}

void FastLED_Composition::setTimebase(long timebase) {
	_timebase = timebase;
}

uint16_t  FastLED_Composition::getBpm() {
	return _bpm;
}

void FastLED_Composition::setBpm(uint16_t bpm) {
	_bpm = bpm;
}
 
int16_t FastLED_Composition::midiToInt(int8_t byte) {
	int16_t s1;
	byte = byte << 1;
	s1 = byte *byte;
	if (byte < 0) {
		s1 = -1 * s1;
	}
	s1 = s1;
	return s1;
}
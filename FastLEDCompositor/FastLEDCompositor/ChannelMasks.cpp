// Collection of Channel Mask Classes
// 
// 

#include "ChannelMasks.h"


// simply a smooth fade in and fade out
uint8_t FadeChannelMask::getVal(uint16_t num_leds, uint16_t lednum,boolean direction) {
	return ChannelMask::getValInt(direction);
}


// wipe in from start to end (without any blurring or smoothing)
uint8_t WipeChannelMask::getVal(uint16_t num_leds, uint16_t lednum, boolean direction) {
	uint8_t temp = ChannelMask::getValInt(direction);
	if (lednum > (num_leds*temp / 256)) {
		return 0;
	}
	else {
		return 255;
	}
}


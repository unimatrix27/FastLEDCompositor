// 
// 
// 

#include "ChannelMasks.h"

uint8_t FadeChannelMask::getVal(uint16_t num_leds, uint16_t lednum) {
	return 128;
}

uint8_t WipeChannelMask::getVal(uint16_t num_leds, uint16_t lednum) {
	return 255;
}


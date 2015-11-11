// 
// 
// 

#include "ChannelMasks.h"

uint8_t FadeChannelMask::getVal(uint16_t num_leds, uint16_t lednum) {
	double temp;
	if (millis() - starttime > duration) { return 255; }
	temp = (millis() - starttime);
	temp = temp * 256;
	temp = temp / duration;
	return temp;
}

uint8_t WipeChannelMask::getVal(uint16_t num_leds, uint16_t lednum) {
	return 255;
}


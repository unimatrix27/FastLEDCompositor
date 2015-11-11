// 
// 
// 

#include "ChannelMask.h"

ChannelMask::ChannelMask() {}

void ChannelMask::setDuration(long duration) {
	this->duration = duration;
	this->endtime = this->starttime + duration;
}

boolean ChannelMask::isOver() {
	if (millis()>=endtime) {
		return true;
	}
	else {
		return false;
	}
	
}

uint8_t ChannelMask::getValInt(uint16_t num_leds, uint16_t lednum, boolean direction) {
	double temp;
	if (millis() - starttime > duration) { return 0; }
	if (direction) {
		temp = endtime - millis();
	}
	else {
		temp = (millis() - starttime);
	}
	temp = temp * 256 / duration;
	return temp;
}
// 
// 
// 

#include "ChannelMask.h"

ChannelMask::ChannelMask() {}

void ChannelMask::setDuration(long duration) {
	this->duration = duration;
	this->endtime = this->starttime + duration;
}


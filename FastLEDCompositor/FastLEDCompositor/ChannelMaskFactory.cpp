// 
// 
// 

#include "ChannelMaskFactory.h"

ChannelMaskFactory* ChannelMaskFactory::instance = NULL;
ChannelMaskFactory* ChannelMaskFactory::getInstance() {
	if (instance == NULL) instance = new ChannelMaskFactory();
	return instance;
}

ChannelMask* ChannelMaskFactory::orderTheChannelMask(FadeType fadeType, long duration) {
	ChannelMask* myChannelMask;
	switch (fadeType) {
	case  FT_FADEIN:
		myChannelMask = new FadeChannelMask();
		break;
	case  FT_FADEOUT:
	default:
		myChannelMask = new WipeChannelMask();
		break;
	}
	myChannelMask->setDuration(duration);
	return myChannelMask;
}



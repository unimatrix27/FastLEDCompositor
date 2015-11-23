// Singleton Channel Mask Factory to create new channel Mask objects.
// More of those Channel mask Classes should be implemented.
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
	case  FT_FADE:
		myChannelMask = new FadeChannelMask();
		break;
	case  FT_WIPE:
		myChannelMask = new WipeChannelMask();
		break;
	default:
		myChannelMask = new FadeChannelMask();
		break;
	}
	myChannelMask->setStartTime(millis());
	myChannelMask->setDuration(duration);

	return myChannelMask;
}



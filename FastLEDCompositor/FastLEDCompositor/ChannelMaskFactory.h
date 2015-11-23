// ChannelMaskFactory.h
// Factory class for creation ChannelMask Objects
// Type of object to be determined by FadeType
// Implemented as Singleton

#ifndef _CHANNELMASKFACTORY_h
#define _CHANNELMASKFACTORY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "disco.h"
#include "ChannelMask.h"
#include "ChannelMasks.h"

// Factory Class to create factory masks of the requested type. Singleton


class ChannelMaskFactory
{
private:
	static  ChannelMaskFactory* instance;
protected:
	ChannelMaskFactory() {}
public:
	static ChannelMaskFactory* getInstance();
	ChannelMask* orderTheChannelMask(FadeType fadeType, long duration);
};


#endif


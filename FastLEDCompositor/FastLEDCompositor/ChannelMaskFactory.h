// ChannelFactory.h

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


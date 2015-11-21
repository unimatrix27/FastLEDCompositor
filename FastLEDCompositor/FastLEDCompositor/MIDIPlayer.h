// MIDIPlayer.h

#ifndef _MIDIPLAYER_h
#define _MIDIPLAYER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MIDIPlayer
{
private:
	static  MIDIPlayer* instance;
 protected:
	MIDIPlayer() {}

 public:
	 static MIDIPlayer* getInstance();
	 int16_t midiToInt(int8_t byte);
	 void OnControlChange(byte channel, byte control, byte value); 
	 void RealTimeSystem(byte realtimebyte);
};


#endif


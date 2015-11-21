// 
// 
// 

#include "MIDIPlayer.h"

MIDIPlayer* MIDIPlayer::instance = NULL;
MIDIPlayer* MIDIPlayer::getInstance()
{
	if (instance == NULL) instance = new MIDIPlayer();
	return instance;

}



int16_t MIDIPlayer::midiToInt(int8_t byte) {
	int16_t s1;
	byte = byte << 1;
	s1 = byte *byte;
	if (byte < 0) {
		s1 = -1 * s1;
	}
	s1 = s1;
	return s1;
}


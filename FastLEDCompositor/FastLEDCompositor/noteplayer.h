#ifndef _NOTEPLAYER_h
#define _NOTEPLAYER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "disco.h"
#include "Note.h"

#define MAX_NOTES 20


class Noteplayer
{
protected:
	NoteClass *notes[MAX_NOTES];
	uint8_t n_notes = 0;

	
public:
	Noteplayer();
	~Noteplayer();
	bool NoteOn(uint8_t tone, uint8_t velocity);
	bool NoteOff(uint8_t tone, uint8_t velocity);
	bool NoteRemove(uint8_t tone);
	uint8_t NoteFind(uint8_t tone);
	void draw();
	
};

#endif
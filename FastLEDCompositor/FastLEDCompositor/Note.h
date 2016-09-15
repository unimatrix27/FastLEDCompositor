// Note.h

#ifndef _NOTE_h
#define _NOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "deftypes.h"

class NoteClass
{
protected:
	unsigned long m_notestart;
	uint8_t m_tone;
	uint16_t m_toneposition;
	uint8_t m_velocity;
	bool m_pressed = true;
	uint16_t toneposition(uint8_t note);


public:
	NoteClass(const uint16_t tone, const uint8_t velocity);
	bool draw();
	void unpress();
	void press(const uint8_t velocity);
	uint8_t getTone();

};

extern NoteClass Note;
#endif


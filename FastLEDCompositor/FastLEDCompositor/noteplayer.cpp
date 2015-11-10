#include "Noteplayer.h"
#include "FastLED.h"



Noteplayer::Noteplayer()
{

}


Noteplayer::~Noteplayer()
{
}


void Noteplayer::draw()
{
	for (int i = 0; i < n_notes; i++) {
		if (notes[i]->draw() == false) {
			delete notes[i];
			notes[i] = notes[n_notes];
			notes[n_notes--] = NULL;
			break;
		}
	}
	fill_solid(leds, n_notes, CRGB::Red);
}

uint8_t Noteplayer::NoteFind(uint8_t tone) {
	for (int i = 0; i < n_notes; i++) {
		if (notes[i]->getTone() == tone) {
			return i;
		}
	}
	return MAX_NOTES;
}

bool Noteplayer::NoteRemove(uint8_t m_tone) {
	return true;
}

bool Noteplayer::NoteOn(uint8_t tone, uint8_t velocity) {
	uint8_t i  = NoteFind(tone);

	if (i != MAX_NOTES) {
		notes[i]->press(velocity);
		return true;
	}
	if (n_notes == 10) {
		return false;
	}
	else {
		notes[n_notes++] = new NoteClass(tone, velocity);
	}
	return true;
}

bool Noteplayer::NoteOff(uint8_t tone, uint8_t velocity) {
	uint8_t i = NoteFind(tone);
	if (i != MAX_NOTES) {
		notes[i]->unpress();
		return true;
	}
	return false;
}


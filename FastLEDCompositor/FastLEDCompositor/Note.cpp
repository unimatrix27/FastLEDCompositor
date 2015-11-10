// 
// 
// 

#include "Note.h"
#include "FastLED.h"

NoteClass::NoteClass(uint16_t tone, uint8_t velocity) {
	m_tone = tone;
	m_toneposition = toneposition(tone);
	press(velocity);
}

void NoteClass::unpress() {
	m_pressed = false;
	m_notestart = millis();
}

void NoteClass::press(uint8_t velocity) {
	m_pressed = true;
	m_velocity = velocity;
	m_notestart = millis();
}

uint8_t NoteClass::getTone() {
	return m_tone;
}

bool NoteClass::draw() {
	if (m_pressed == true) {
		leds[m_toneposition] += CHSV(0, 0, 2*m_velocity);
		if (millis() - m_notestart > 5000) {
			return false;
		}
		return true;
	}
	else {
		return false;
		if ((millis() - m_notestart) > 10){
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

uint16_t NoteClass::toneposition(uint8_t note) {
	return (note - 35) * (NUM_LEDS / 68);
}
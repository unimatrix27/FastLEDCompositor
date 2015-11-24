
#include "disco.h"
#include "Compositor.h"
extern Compositor myComposition;

#define MIDI_CC_START 40 // we start to interpret MIDI CC commands with value 40.


void OnControlChange(byte channel, byte control, byte value) {			// called whenever a MIDI CC is received
	static PaletteName lastPaletteName[NUM_CHANNELS] = { PN_RAINBOW };	// variable to prevent sending setTargetPalette again for values received more than once. 
	if (channel >= NUM_CHANNELS) {
		return;
	}
	switch (control) {
	case MIDI_CC_START:
		myComposition.getParams(channel)->hue = 2 * value;		// multiply with 2 because MIDI values are 7 bit. 
		break;
	case MIDI_CC_START + 1:
		myComposition.getParams(channel)->saturation = 2 * value;
		break;
	case MIDI_CC_START + 2:
		myComposition.getParams(channel)->brightness = 2 * value;
		break;
	case MIDI_CC_START + 3:
		myComposition.getParams(channel)->vitality = 2*value;
		break;
	case MIDI_CC_START + 4:
		myComposition.getParams(channel)->speed = value;
		break;
	case MIDI_CC_START + 5:
		myComposition.getParams(channel)->randomness = 2*value;
		break;
	case MIDI_CC_START + 6:
		myComposition.getParams(channel)->hueVariability =2* value;
		break;
	case MIDI_CC_START + 7:
		myComposition.getParams(channel)->blurryness = 2*value;
		break;
	case MIDI_CC_START + 8:
		myComposition.getParams(channel)->clonecount = value;
		break;
	case MIDI_CC_START + 9:
		myComposition.getParams(channel)->clonedistance = value;
		break;
	case MIDI_CC_START + 10:
		myComposition.getParams(channel)->mirroraxes = value;
		break;
	case MIDI_CC_START + 11:
		myComposition.getParams(channel)->channelSpeed = value;
		break;
	case MIDI_CC_START + 12:
		if (myComposition.getParams(channel)->fadeTimeBase == TB_BEATS) {
			myComposition.getParams(channel)->fadeTime = value;
		}
		else {
			myComposition.getParams(channel)->fadeTime = 100 * value;
		}
		break;
	case MIDI_CC_START + 13:
		myComposition.getParams(channel)->fadeTimeBase = (TimeBase)value;
		break;
	case MIDI_CC_START + 14:
		if ((PaletteName)value != lastPaletteName[channel]) {
			myComposition.getParams(channel)->setTargetPalette((PaletteName)value, myComposition.getParams(channel)->paletteBlendDuration);
			lastPaletteName[channel] = (PaletteName)value;
		}
		break;
	case MIDI_CC_START + 15:
		if (myComposition.getParams(channel)->fadeTimeBase == TB_BEATS) {
			myComposition.getParams(channel)->paletteBlendDuration = value;
		}
		else {
			myComposition.getParams(channel)->paletteBlendDuration = 100 * value;
		}
		break;
	case MIDI_CC_START + 16:
		myComposition.getParams(channel)->fadeType = (FadeType)value;
		break;
	case MIDI_CC_START + 17:
		myComposition.getParams(channel)->blendType = (BlendType)value;
		break;
	case MIDI_CC_START + 18:
		myComposition.getParams(channel)->numLeds = value;
		break;
	case MIDI_CC_START + 19:
		myComposition.getParams(channel)->startPos = value;
		break;
	case MIDI_CC_START + 20:
		myComposition.getParams(channel)->numLeds = NUM_LEDS * value / 127;
		break;
	case MIDI_CC_START + 21:
		if (myComposition.getParams(channel)->fadeTimeBase == TB_BEATS) {
			myComposition.getParams(channel)->fadeOutTime = value;
		}
		else {
			myComposition.getParams(channel)->fadeOutTime = 100 * value;
		}
		break;
	}
}


// Receive MIDI Clock information to create BPM
// not natively supported in TeensyDuino
// see blog post at
// http://little-scale.blogspot.de/2011/08/how-to-deal-with-real-time-midi-beat.html
// thanks for this great addition!

void RealTimeSystem(byte realtimebyte) {
	if (realtimebyte == 248) {
		g_bpm_cnt++; if (g_bpm_cnt == 24) {
			g_bpm_cnt = 0;
			g_bpm_beatnr++;
			g_bpm = 60000 / (millis() - g_bpm_timebase);
			g_bpm_timebase = millis();
		}
	}
	if (realtimebyte == MIDI_START || realtimebyte == MIDI_CONTINUE) {
		g_bpm_cnt = 0;
		g_bpm_beatnr = 0;
	}
	if (realtimebyte == MIDI_STOP) { g_bpm_cnt = 0; }

}

// for a new note, activate the channel. currently we do not care which note is played as long as it is above 36
// For notes below 36 we treat them the same way as MIDI CC
// this way it is the choice of the user if she wants to use MIDI CC or just Notes.
// depending on the sequencer software used
// the Author uses Renoise which perfectly supports MIDI CC in a great way
// https://www.renoise.com/

void OnNoteOn(byte channel, byte note, byte velocity) {
	if (channel >= NUM_CHANNELS) {
		return;
	}
	if (note < 36) {
		OnControlChange(channel, MIDI_CC_START + note, velocity-1);
		return;
	}
	myComposition.getParams(channel)->effectType = (EffectType)velocity;
	myComposition.addChannel(channel);
}

// note off deactivates or fades out the channel
void OnNoteOff(byte channel, byte note, byte velocity) {
	if (channel >= NUM_CHANNELS) {
		return;
	}
	if (note < 36) {
		return;
	}
	myComposition.setFade(channel, 2);
}

// we dont do anything for program changes right now. But maybe we should.
void OnProgramChange(byte channel, byte program) {

}

// helper function to transfer 7 bit values into signed integers (used for parameters which go in 2 directions, e.g. speed.
int16_t midiToInt(int8_t byte) {
	int16_t s1;
	byte = byte << 1;
	s1 = byte *byte;
	if (byte < 0) {
		s1 = -1 * s1;
	}
	s1 = s1;
	return s1;
}

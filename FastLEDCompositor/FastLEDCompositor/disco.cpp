
#include "disco.h"
#include "Compositor.h"
extern Compositor myComposition;

#define MIDI_CC_START 40

// Effect-Parameters
uint8_t hue;
uint8_t saturation;
uint8_t brightness;
uint8_t vitality;
int8_t speed;
uint8_t randomness;
uint8_t hueVariability;
uint8_t blurryness;

// Channel-Parameters
uint8_t clonecount;
uint8_t clonedistance;
uint8_t mirroraxes;
int8_t	 channelSpeed;
TBlendType paletteBlendType;

void OnControlChange(byte channel, byte control, byte value) {
	static PaletteName lastPaletteName[NUM_CHANNELS] = { PN_RAINBOW };
	channel = 1;
	if (channel >= NUM_CHANNELS) {
		return;
	}
	switch (control) {
	case MIDI_CC_START:
		myComposition.getParams(channel)->hue = 2 * value;
		break;
	case MIDI_CC_START + 1:
		myComposition.getParams(channel)->saturation = 2 * value;
		break;
	case MIDI_CC_START + 2:
		myComposition.getParams(channel)->brightness = 2 * value;
		break;
	case MIDI_CC_START + 3:
		myComposition.getParams(channel)->vitality = value;
		break;
	case MIDI_CC_START + 4:
		myComposition.getParams(channel)->speed = value;
		break;
	case MIDI_CC_START + 5:
		myComposition.getParams(channel)->randomness = value;
		break;
	case MIDI_CC_START + 6:
		myComposition.getParams(channel)->hueVariability = value;
		break;
	case MIDI_CC_START + 7:
		myComposition.getParams(channel)->blurryness = value;
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
	}
}

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

void OnNoteOn(byte channel, byte note, byte velocity) {
	if (channel >= NUM_CHANNELS) {
		return;
	}
	if (note < 36) {
		OnControlChange(channel, MIDI_CC_START + note, velocity-1);
	}
	myComposition.addChannel(channel,
		velocity,
		myComposition.getParams(channel)->blendType,
		myComposition.getParams(channel)->fadeType,
		myComposition.getParams(channel)->numLeds,
		myComposition.getParams(channel)->startPos,
		myComposition.getParams(channel)->fadeTime,
		myComposition.getParams(channel)->fadeTimeBase);
}
void OnNoteOff(byte channel, byte note, byte velocity) {
	if (channel >= NUM_CHANNELS) {
		return;
	}
	if (note < 36) {
		return;
	}
	myComposition.setFade(channel,
		myComposition.getParams(channel)->fadeType,
		myComposition.getParams(channel)->fadeTimeBase,
		myComposition.getParams(channel)->fadeTime, 2);
}
void OnProgramChange(byte channel, byte program) {

}

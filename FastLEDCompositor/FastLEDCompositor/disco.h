#pragma once
#define NUM_LEDS_PER_STRIP 300			// how much LEDs per strip/segment (parallel output)
#define NUM_STRIPS 1					// how many strips or outputs in use.
#define NUM_CHANNELS 16					// how much channels a compositor can hold (MIDI supports up to 16 channels)
#define LED_DT 14                       // Data pin to connect to the strip. (not used for OctoWS2811)
#define ledPin 13						// Teensy LED 

#define COLOR_ORDER GRB                 // Use BGR for APA102 and GRB for WS2812
#define LED_TYPE WS2812B                 // Or WS2812. Don't forget to change the FastLED.addLeds line as well.
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP       // Total Number of LED's.
#define FRAMES_PER_SECOND 100			// Target FPS (not used here)
#define BPM 80							// Basic BPM if there is no music giving one
#define DEFAULT_FADE_TIME 96			// default value for Fade time is 4 full beats (4*24)

#define MIDI_START 250					// defines for MIDI commands (never change them)
#define MIDI_STOP 252
#define MIDI_CONTINUE 251

enum BlendType { BT_SUM, BT_OVERLAY, BT_INVERT };	// some basic blend types (to be added many more)
enum FadeType { FT_NOFADE, FT_FADE, FT_WIPE };  // some very basic fade types (to be added many more)
enum TimeBase { TB_BEATS, TB_MILLIS };  // timebase for fading (either beats or milliseconds)
enum EffectType {ET_SOLID,ET_RAINBOW,ET_CONFETTI,ET_BASEBEAT}; // all effects (to be added a lot more of course)

#include <FastLED.h>

int16_t midiToInt(int8_t byte); //helper function to transform midi 7 bit value into a signed integer
extern uint16_t g_bpm;
extern uint8_t g_bpm_cnt;
extern uint16_t g_bpm_beatnr;
extern long g_bpm_timebase;
extern struct CRGB leds[NUM_LEDS];  // to be removed, legacy for note-stuff


// functions for handling of MIDI events
void OnNoteOn(byte channel, byte note, byte velocity);
void OnNoteOff(byte channel, byte note, byte velocity);
void OnProgramChange(byte channel, byte program);
void OnControlChange(byte channel, byte control, byte value);
void RealTimeSystem(byte realtimebyte);


#pragma once
#define NUM_LEDS_PER_STRIP 300
#define NUM_STRIPS 1
#define NUM_CHANNELS 8
#define LED_DT 14                                          // Data pin to connect to the strip.
#define ledPin 13											   // Teensy LED 

#define COLOR_ORDER GRB                                       // Use BGR for APA102 and GRB for WS2812
#define LED_TYPE WS2812B                                      // Or WS2812. Don't forget to change the FastLED.addLeds line as well.
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP                                    // Number of LED's.
#define FRAMES_PER_SECOND 100
#define BPM 80
#define DEFAULT_FADE_TIME 4

enum BlendType { BT_SUM, BT_OVERLAY, BT_INVERT };
enum FadeType { FT_NOFADE, FT_FADEIN, FT_FADEOUT };
enum TimeBase { TB_BEATS, TB_SECONDS };

#include <FastLED.h>

extern struct CRGB leds[NUM_LEDS];
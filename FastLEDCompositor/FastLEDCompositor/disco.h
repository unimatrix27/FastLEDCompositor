#pragma once
#define NUM_LEDS_PER_STRIP 150
// how much LEDs per strip/segment (parallel output)
#define NUM_STRIPS 8				// how many strips or outputs in use.

#define LED_DT 14                       // Data pin to connect to the strip. (not used for OctoWS2811)
#define ledPin 13						// Teensy LED 

#define COLOR_ORDER GRB                 // Use BGR for APA102 and GRB for WS2812
#define LED_TYPE WS2812B                 // Or WS2812. Don't forget to change the FastLED.addLeds line as well.
#define NUM_LEDS 1132       // Total Number of LED's.




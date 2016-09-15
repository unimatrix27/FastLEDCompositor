/*
Testprogram for Composition - Framwwork
Author: Sebastian Stuecker

This code is heavily work in progress

Main File. does not do a lot except if used for testing stuff. 
Reads Midi data each loop and let the midi code (disco.cpp) handle it.

*/

#define USE_OCTOWS2811
#include "OctoWS2811.h"		 // Use OctoWS2811 Adaptor for parallel output
#include "FastLED.h"		 //	Include FastLED used for the fun part
#include "disco.h"			 // some defines etc.


#include "FastLED_Composition.h"		// Definitions for the main Compositor class




// Initialize changeable global variables.

uint8_t max_bright = 220;                   // Overall brightness definition. It can be changed on the fly, i.e. with a potentiometer.
struct CRGB leds[NUM_LEDS];					// this is the array for all LEDs



// Initialize our LED array.


// SETUP -------------------------------------------------------------------------------------------------------

// myComposition is going to be the main "canvas" where everything is put together
FastLED_Composition myComposition(leds, NUM_LEDS);

long autoOffTime= millis() + 5200000;
// functions for handling of MIDI events
void OnNoteOn(byte channel, byte note, byte velocity);
void OnNoteOff(byte channel, byte note, byte velocity);
void OnProgramChange(byte channel, byte program);
void OnControlChange(byte channel, byte control, byte value);
void RealTimeSystem(byte realtimebyte);
void setAutoPattern();
void allOff();


#define MIDI_CC_START 40 // we start to interpret MIDI CC commands with value 40.
#define NOTE_EFFECT_START 36  // Note values starting 36 are interpreted as effect number (26 equals the tone C2)

#define MIDI_START 250					// defines for MIDI commands (never change them)
#define MIDI_STOP 252
#define MIDI_CONTINUE 251



void OnControlChange(byte channel, byte control, byte value) {			// called whenever a MIDI CC is received
	static PaletteName lastPaletteName[NUM_CHANNELS] = { PN_RAINBOW };	// variable to prevent sending setTargetPalette again for values received more than once. 
	channel--;
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
		myComposition.getParams(channel)->vitality = 2 * value;
		break;
	case MIDI_CC_START + 4:
		myComposition.getParams(channel)->speed = value;
		break;
	case MIDI_CC_START + 5:
		myComposition.getParams(channel)->randomness = 2 * value;
		break;
	case MIDI_CC_START + 6:
		myComposition.getParams(channel)->hueVariability = 2 * value;
		break;
	case MIDI_CC_START + 7:
		myComposition.getParams(channel)->blurryness = 2 * value;
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
		myComposition.getParams(channel)->fadeTimeBase = TB_MILLIS;// (TimeBase)value;
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
	case MIDI_CC_START + 22:
		myComposition.getParams(channel)->startPos = NUM_LEDS * value / 127;
		break;
	case MIDI_CC_START + 23:
			myComposition.getParams(channel)->custom1 = value;
			break;
	case MIDI_CC_START + 24:
		myComposition.getParams(channel)->custom2 = value;
		break;
	case MIDI_CC_START + 25:
		myComposition.getParams(channel)->custom3 = value;
		break;
	case MIDI_CC_START + 26:
		myComposition.getParams(channel)->clonedistance = NUM_LEDS * value / 127;
		break;
	}
}


// Receive MIDI Clock information to create BPM
// not natively supported in TeensyDuino
// see blog post at
// http://little-scale.blogspot.de/2011/08/how-to-deal-with-real-time-midi-beat.html
// thanks for this great addition!

void RealTimeSystem(byte realtimebyte) {
	uint32_t now;
	static uint8_t g_bpm_cnt = 0;
	if (realtimebyte == 248) {
		now = millis();
		g_bpm_cnt++; if (g_bpm_cnt == 24) {
			g_bpm_cnt = 0;
			myComposition.setBeatnr(myComposition.getBeatnr() + 1);
			myComposition.setBpm(60000 / (now - myComposition.getTimebase()));
			myComposition.setTimebase(now);
		}
		myComposition.setBpmCnt(g_bpm_cnt);
	}
	if (realtimebyte == MIDI_START || realtimebyte == MIDI_CONTINUE) {
		g_bpm_cnt = 0;
		myComposition.setBeatnr(0);
		allOff();

	}
	if (realtimebyte == MIDI_STOP) { 
		g_bpm_cnt = 0;
		setAutoPattern();
	}

}

// for a new note, activate the channel. currently we do not care which note is played as long as it is above 36
// For notes below 36 we treat them the same way as MIDI CC
// this way it is the choice of the user if she wants to use MIDI CC or just Notes.
// depending on the sequencer software used
// the Author uses Renoise which perfectly supports MIDI CC in a great way
// https://www.renoise.com/

void OnNoteOn(byte channel, byte note, byte velocity) {
	channel--;
	if (channel >= NUM_CHANNELS) {
		return;
	}
	if (channel == 15) {
		myComposition.getParams(channel)->effectType = ET_BARMOVER;
		if (note == 127) {
			myComposition.setFade(channel, 2);
		}
		else {
			BarMover* mover;
			Channel* myChan;
			myChan = myComposition.getChannel(channel);
			if (myChan == NULL) {
				myComposition.addChannel(channel, velocity);
				myChan = myComposition.getChannel(channel);
			}
			mover = (BarMover*)myChan->getEffect();
			mover->noteOn(note, velocity);
		}
	}
	else {
		if (note < NOTE_EFFECT_START) {
			//OnControlChange(channel, MIDI_CC_START + note, velocity - 1);
			return;
		}
		myComposition.getParams(channel)->effectType = (EffectType)(note - NOTE_EFFECT_START);
		myComposition.addChannel(channel, velocity);
	}

}

// note off deactivates or fades out the channel
void OnNoteOff(byte channel, byte note, byte velocity) {
	channel--;
	if (channel >= NUM_CHANNELS) {
		return;
	}
	if (channel == 15) {
		BarMover* mover;
		Channel* myChan;
		myChan = myComposition.getChannel(channel);
		if (myChan != NULL) {
			mover = (BarMover*)myChan->getEffect();
			mover->noteOff(note);
		}
	}
	else {
		if (note < 36) {
			return;
		}
		myComposition.setFade(channel, 2);
	}

}

// we dont do anything for program changes right now. But maybe we should.
void OnProgramChange(byte channel, byte program) {

}



void readkeyboard() {
	byte inbyte;

	while (Serial.available() > 0) {
		inbyte = Serial.read();                                   // READ SINGLE BYTE COMMAND
		switch (inbyte) {
		case 'o':
			if (myComposition.getChannel(0)->isActive() == false) {
				myComposition.addChannel(0);
				myComposition.addChannel(1);
			}
			autoOffTime = millis() + 5200000;
			Serial.println("an");
			break;
		default:
			if (myComposition.getChannel(0)->isActive() == true) {
				myComposition.setFade(0, 2);
				myComposition.setFade(1, 2);
			}
			Serial.println("aus");
			break;
		}
	}
	if (millis() > autoOffTime) {
		myComposition.setFade(0, 2);
		myComposition.setFade(1, 2);
	}

}

void allOff() {
	for (int channel = 0; channel < NUM_CHANNELS; channel++) {
		myComposition.setFade(channel, 2);
	}
}


void setAutoPattern() {
	int channel = 12;
	ParameterSet* mp;
	mp = myComposition.getParams(channel);
	mp->effectType = ET_RAINBOW; mp->fadeTime = 1000; mp->fadeType = FT_FADE;
	mp->numLeds = NUM_LEDS;
	mp->brightness = 80;
	mp->channelSpeed = 5; mp->clonecount = 0;
	mp->fadeOutTime = 500;
	//mp->paletteBlendDuration = 20000;
	mp->setTargetPalette ((PaletteName) random8(0, PN_NUM_VALUES-3));
	mp->hueVariability = random8(0, 15);
	mp->hueVariability = 0;
	//myComposition.addChannel(channel);
//	myComposition.setFade(channel, 2);

	
	mp->fadeType = (FadeType) random8(FT_FADE,FT_WIPE+1);


	channel = 13;
	mp = myComposition.getParams(channel);
	mp->effectType = ET_MASKBEATSTROBE;
	mp->setTargetPalette(PN_BW, 0);
	mp->numLeds = NUM_LEDS;
	mp->brightness = 255;
	mp->custom1 = 60;
	mp->custom2 = 00;
	mp->custom3 = 0x04;
	mp->blendType = BT_NEGATIVE;
	
	//myComposition.addChannel(channel);

	channel = 14;
	mp = myComposition.getParams(channel);
	mp->effectType = ET_MASKBEATMOVER;
	mp->setTargetPalette(PN_BW, 0);
	mp->numLeds = NUM_LEDS;
	mp->brightness = 255;
	mp->custom1 = 15;
	mp->custom2 = 25;
	mp->custom3 = 0x15;
	mp->blendType = BT_NEGATIVE;

	//myComposition.addChannel(channel);


}



void setup() {
	//Serial.begin(115200);
	randomSeed(analogRead(0));
#ifdef USB_MIDI
	// Receive MIDI Clock information to create BPM
	// not natively supported in TeensyDuino
	// see blog post at
	// http://little-scale.blogspot.de/2011/08/how-to-deal-with-real-time-midi-beat.html
	usbMIDI.setHandleRealTimeSystem(RealTimeSystem); 
	// Give functions that handle the different MIDI receive events. 
	usbMIDI.setHandleControlChange(OnControlChange);  // handling of parameter changes through MIDI CC commands.
	usbMIDI.setHandleNoteOn(OnNoteOn);				  // called when a NOTE ON is received (and will thus enable a pattern)
	usbMIDI.setHandleNoteOff(OnNoteOff);			  // called when not off is received (disabling/fading out pattern)
#else
	Serial.begin(115200);
	Serial.setTimeout(5);
	delay(1000);
	Serial.print("serial ready\n");

#endif
	delay(500);
	FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip).setDither(max_bright < 255);
	FastLED.setBrightness(max_bright);                          // You can change the overall brightness on the fly, i.e. with a potentiometer.
	random16_add_entropy(random());


	ParameterSet* mp;
	setAutoPattern();
	//myComposition.addChannel(channel);

	//mover = (BarMover*)myChan->getEffect();
	//mover->addBar(2,3,0);
	//mover->addBar(2, 5, 0);
	//mover->addBar(2, 3, 0);





	/*
	mp = myComposition.getParams(0);
	mp->effectType = ET_NOISECANDLE; mp->fadeTime = 5000; mp->fadeType = FT_WIPE;
	mp->numLeds = 560;
	mp->channelSpeed = 0; mp->clonecount = 0;
	mp->fadeOutTime = 10000;
	//mp->paletteBlendDuration = 20000;
	//mp->setTargetPalette(PN_HEAT, 2000);
	//mp->hueVariability = 2;
	myComposition.addChannel(0);
	mp = myComposition.getParams(1);
	mp->effectType = ET_CONFETTI; mp->fadeTime = 1000; mp->fadeType = FT_WIPE;

	mp->velocity = 50;
	mp->vitality = 120;
	mp->numLeds = 230;
	mp->startPos = 165; mp->hueVariability = 0;
	mp->setTargetPalette(PN_HEAT, 2000);
	mp->brightness = 11;
	mp->blendType = BT_OVERLAY;
	myComposition.addChannel(1);
	*/
	/* myComposition.getParams(2);
	mp->effectType = ET_RAINBOW; mp->fadeTime = 5000; mp->fadeType = FT_WIPE;
	mp->numLeds = 300; mp->startPos = 1;
	mp->channelSpeed = 0; mp->clonecount = 0; mp->speed = 40;
	mp->paletteBlendDuration = 2000;
	mp->setTargetPalette(PN_OCEAN, 2000);
	mp->brightness = 20;
	mp->hueVariability = 20;
	if (mover->isFree(20, 20)) {
		mp->setTargetPalette(PN_FOREST, 20);
	}
	else {
		mp->setTargetPalette(PN_LAVA, 20);
	}
	//myComposition.addChannel(2);
	*/
} // setup()



void loop() {
#ifdef USB_MIDI
	while (usbMIDI.read());			// read MIDI each loop. MIDI could be faster than the FPS, therefore the while loop.
#else
	readkeyboard();
#endif
	/*if (mover->isFree(1, 1)) {
		mp->setTargetPalette(PN_FOREST, 0);
	}
	else {
		mp->setTargetPalette(PN_LAVA,0);
	}
	EVERY_N_MILLISECONDS(40) {
		mover->process();
	}
	EVERY_N_MILLISECONDS(5000) {

		i+=5;
		if (i % 2 == 0) {
			mover->noteOn(2, i);
		}
		else {
			mover->noteOff(2);
		}
	}
	*********************************************************************************************************************/


	myComposition.draw();	        // activate drawing function of compositio
	FastLED.show();					// output the array to the actual LED stripe
    //ChangeMe();		// doing some demo stuff, should be commented out in MIDI configuration
	 

} // loop()

// below just some code for experimenting and testing, should be removed in final configuration





void ChangeMe2() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
	uint16_t secondHand = (millis() / 1000) % 60;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
	static uint16_t lastSecond = 65000;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
	if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
		lastSecond = secondHand;
		switch (secondHand) {
		case  15: 
			myComposition.getParams(1)->channelSpeed = 2; break;
		case 20: myComposition.getParams(2)->speed = 20; break;
		case 35: myComposition.getParams(2)->setTargetPalette(PN_AUTO_RANDOM); 
			myComposition.getParams(2)->numLeds = 100;
			break;
		case 16: myComposition.getParams(2)->paletteBlendDuration = 5000; break;

		case 25: myComposition.getParams(2)->setTargetPalette(PN_AUTO_RANDOM); break;

		default:
			break;
		}
	}
}


void changePalette(PaletteName pn) {
	if (myComposition.getParams(1) != NULL) {
		myComposition.getParams(1)->setTargetPalette(pn,7000);
	}
}



	// 0 - 247
	// 248 - 496
	// 497 - 906
	// 907 - 1132
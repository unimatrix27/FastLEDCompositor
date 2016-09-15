// MovingBar.h

/* moving dot oder bar

Eigenschaften des GEsamtsystems

- custom1: 0 entspricht: länge so lange note on
1-127 entspricht: feste länge
- custom2: position der quelle (frct8)

Tone: Hue
Velocity: Signatur (ungerade nach links, gerade nach rechts)
custom2: source position
custom3: target position

bei der erzeugung sind source und target zu entrelativieren und beziehen sich auf startPos des Canvas.
so lange fire aktiv kann sich source verschieben und schiebt dann die Bar mit
collision detection nur bei gleicher signatur/Velocity

muss bei jedem neuen pixel und bei jedem movement gemacht werden
- bei pixel collision: wird nicht erzeugt
bei movement collision: wird nicht gemoved

- am ende rauslaufen
- em ende stehen bleiben (das ende wird dann immer groesser)


- Anzahl Pixel (1-255) (waechst so lange tone on gehalten wird oder feste anzahl)
- Zielposition in % der Größe
- Dim Down während Movement 

functions:

ist ein pixel belegt von einer bar? (pixelnum)

class movingbar
numLeds
position
hueVariability
destinationPos

functions
getNumLeds
getStartPos
getEndPos
getFreePixelsBehind
draw


draw funktion:
- execute DimDown
- wenn dim out, delete itself
- wenn speed nicht null
- how far to move now
- how much is free
- adjust far to move if needed, bsi hin yu 0
- execute move
- execute hueChange


how far to muve function
- check free pixels
- vergleiche getEndPos mit DestinationPos
- wenn gleich und wenn move out option gesetzt
- setze shrink flag
- return how far to move



*/


#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "deftypes.h"
#include "Effects.h"

class BarMover;

class MovingBar {
protected:
	uint8_t signature;			// only bars with same signature can collide, others just overlap. even signatures move "up". odd signatures move "down"
	uint16_t startPos;			// where does the bar currently start (at its current position)
	uint16_t size;				// what is the current size. for upmoving bars the end position is start+size, for downmoving its start-size. 
	uint16_t destinationPos;	// where will the bar stop moving or start disappearing (moving out)
	uint8_t hueVariability;		// how much will the hue change for each pixel
	uint8_t hue;				// what is the current hue of pixel 1
	uint32_t lastCall;			// when was the process function called the last time
	boolean growing;			// is the bar currently still growing?
	MovingBar* next;			// pointer to the next bar
	BarMover* parent;			// pointer to the BarMover Effect that created the bar in the first place.
public:
	MovingBar(BarMover *parent,uint8_t signature, uint16_t size, uint16_t end, uint16_t start=0, uint8_t startingHue = 0); // create a new bar and initialize some values
	boolean processFrame();     // returns false if the MovingBar wants to be deleted because its faded out or moved out of the canvas
	void finishGrowing();		// stop growing
	uint16_t getSize();		// get the size of the bar
	uint16_t getEndPos(boolean nextPos=false);		// get the end position, if nextPos = true then get predicted EndPos if moved 1 pixel
	uint16_t getPos(uint16_t index);			// return the absolute position based on the index position for a bar (to deal with overflowing bars)
	uint16_t getStartPos();		// get the start position
	uint8_t getSignature();				// get the signature
	uint16_t getDestination();			// get the destination
	uint8_t getCurrentHue(uint16_t position);	// get the current hue
	MovingBar* getNext();				// get the next pointer
	void setNext(MovingBar* next);		// set the next pointer
	boolean isInBar(uint16_t pixelnum); // check if specific pixel is part of current bar
};



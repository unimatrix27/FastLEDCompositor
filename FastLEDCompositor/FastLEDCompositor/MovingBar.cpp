// 
// 
// 

#include "MovingBar.h"
#include "Effects.h"

MovingBar::MovingBar(BarMover* parent, uint8_t signature, uint16_t size, uint16_t destination, uint16_t start, uint8_t startingHue) {
	Serial.println((uint32_t)this, HEX);
	this->parent = parent;
	this->signature = signature;
	this->hue = startingHue;
	this->next = nullptr;
	if (size == 0) {
		growing = true;
		this->size = 1;
	}
	else {
		growing = false;
		this->size = size;
	}
	this->destinationPos = destination;
	this->lastCall = millis();
	this->startPos = start;
}

uint16_t MovingBar::getDestination() {
	return destinationPos;
}


uint16_t MovingBar::getSize() {
	return this->size;
}
uint16_t MovingBar::getEndPos(boolean nextPos) {
	uint16_t pos;
	uint16_t s = size;
	if (nextPos == true) s++;
	pos = getPos(s);
	return pos;
}

uint16_t MovingBar::getPos(uint16_t index) {
	uint16_t pos;
	if (signature % 2 == 0) {
		pos = startPos + index;
		if (pos >= parent->getNumLeds()) {
			pos = pos - parent->getNumLeds();
		}
	}
	else {
		if (startPos < index) {
			pos = parent->getNumLeds() - (index - startPos);
		}
		else {
			pos = startPos - index;
		}
	}
	return pos;
}

uint16_t MovingBar::getStartPos() {
	return this->startPos;
}



uint8_t MovingBar::getSignature() {
	return signature;
}

boolean MovingBar::processFrame() {
	uint16_t nextPos = this->getEndPos(true);
	if (nextPos == this->destinationPos) {
		size--;
		if (size == 0) {
			return false;
		}
	}
	// check if destination is reached
	if (parent->isFree(nextPos, signature)) {   // is the next pixel free?
		if (this->growing==true) {
			this->size++;
		}
		else {
			startPos = getPos(1);
		}
	}
	return true;
}

MovingBar* MovingBar::getNext() {
	return this->next;
}

void MovingBar::finishGrowing() {
	this->growing = false;
}

void MovingBar::setNext(MovingBar* next) {
	this->next = next;
}

uint8_t MovingBar::getCurrentHue(uint16_t position) {
	return this->hue;
}

boolean MovingBar::isInBar(uint16_t pixelnum) {
	if (signature % 2 == 0) {
		if (startPos > getEndPos()) {
			if (pixelnum >= startPos || pixelnum <= getEndPos()) {
				return true;
			}
		}
		else {
			if (pixelnum >= startPos && pixelnum <= getEndPos()) {
				return true;
			}
		}
	}
	else {
		if (startPos > getEndPos()) {
			if (pixelnum <= startPos  &&  pixelnum >= getEndPos()) {
				return true;
			}
		}
		else {
			if (pixelnum <= startPos || pixelnum >= getEndPos()) {
				return true;
			}
		}
	}
	return false;
}

#include "FastLED.h"
#include "Effect.h"
#include "Effects.h"

Effect::Effect(){}
Effect::~Effect() {}

void Effect::setCanvas(LedRange* ledrange) {
	canvas = ledrange;
}

void Effect::setParameters(ParameterSet* myParams) {
	this->myParams = myParams;
}

uint16_t Effect::getNumLeds() {
	return canvas->getNumLeds();
}

void Effect::setComp(FastLED_Composition* comp) {
	myComp = comp;
}
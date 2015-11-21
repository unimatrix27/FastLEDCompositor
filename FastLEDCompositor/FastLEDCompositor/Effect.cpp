
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
#include "deftypes.h"
#include "Effects.h"
#include "FastLED_Composition.h"


void SolidEffect::draw() 
{
	fill_solid(canvas->getLeds(),
		canvas->getNumLeds(),
		myParams->getColor(myParams->hue));
}


void RainbowEffect::draw()
{
	uint16_t num = canvas->getNumLeds();
	//fill_rainbow(canvas->getLeds(), canvas->getNumLeds(), start, 255/ canvas->getNumLeds());
	for (int i = 0; i < num; i++) {
		uint16_t colorIndex = i*255-(2* myParams->custom1) / num;
		canvas->getLeds()[i] = myParams->getColor(colorIndex+myParams->hue);
	}
}


void MaskBeatStrobe::draw() {
	// custom1 + 1 = duty cycle in Prozent 0-127
	// custom2 = ramp down speed. 0 = immediate, 127 = full off time
	// custom3 last 3 bits only for speed 
	//  
	uint16_t num = canvas->getNumLeds();
	uint16_t BeatNr = myComp->getBeatnr();
	uint8_t bpmCnt = myComp->getBpmCnt();
	long actPercent;
	static uint16_t lastBeatNr = 0;
	static uint8_t lastBpmCnt = 0;
	static uint8_t nextBpmCnt = 0;
	if ((myParams->custom3 & 0x07) >> 4 > 2) {
		if (lastBpmCnt != bpmCnt) {
			lastBpmCnt = bpmCnt;
			if (bpmCnt == nextBpmCnt) {
				if ((myParams->custom3 & 0x07) >> 4 == 3) {
					nextBpmCnt += 12;
				}
				if ((myParams->custom3 & 0x07) >> 4 == 4) {
					nextBpmCnt += 8;
				}
				if ((myParams->custom3 & 0x07) >> 4 == 5) {
					nextBpmCnt += 6;
				}
				if ((myParams->custom3 & 0x07) >> 4 == 6) {
					nextBpmCnt += 4;
				}
				if ((myParams->custom3 & 0x07) >> 4 == 7) {
					nextBpmCnt += 3;
				}
				this->hit(nextBpmCnt-bpmCnt);
				if (nextBpmCnt > 23) { nextBpmCnt -= 24; }
			}
		}
	}
	else {
		if (lastBeatNr != BeatNr) {
			lastBeatNr = BeatNr;
			if ((myParams->custom3 & 0x07) >> 4 == 2) {
				if (BeatNr % 4 == 0) this->hit(96);
			}
			if ((myParams->custom3 & 0x07) >> 4 == 1) {
				if (BeatNr % 2 == 0) this->hit(48);
			}
			if ((myParams->custom3 & 0x07) >> 4 == 0) {
				this->hit(24);
			}
		}
	}
	if (millis()< nextHit) {
		actPercent = nextHit - millis();
		actPercent = actPercent * 127 / hitLength;
	}
	else actPercent = 127;
	if (actPercent < myParams->custom1) {
		fill_solid(canvas->getLeds(),num,myParams->getColor(myParams->hue));
	}
	else {
		fill_solid(canvas->getLeds(), num, myParams->getColor(128+myParams->hue));
	}
}

void MaskBeatStrobe::hit(uint8_t cnt) {
	// how long till next one
	hitLength = cnt*(60000 / (24 * myComp->getBpm()));
	nextHit = millis() + hitLength;

}

void MaskBeatMover::draw() {
	// custom1 + 1 = anzahl weiss
	// custom2 + 1 = anzahl schwarz
	// custom3 
	//   0x40-0x10 (3 bit 0-7): 0 = kein movement, 1 = every beat 1 movement,2 = every 2nd beat. 3 = every 4th beat
	//						   100 4 = 2 mal pro beat 0,12; 101 5 = 3 mal pro beat 0, 8, 16; ; 10 6 = 4 mal pro beat 0, 6, 12, 18; 111 7 = 6 mal pro beat 0, 4, 8, 12, 16, 20
	//   0x08-0x01 (4 bit 0-15): Anzahl versattz. 0=1; 100=-1, 1=2, 101=-2
	uint16_t num = canvas->getNumLeds();
	uint16_t i = 0;
	uint16_t BeatNr = myComp->getBeatnr();
	uint8_t bpmCnt = myComp->getBpmCnt();
	static uint16_t lastBeatNr = 0;
	static uint8_t lastBpmCnt = 0;
	static uint8_t nextBpmCnt = 0;
	uint8_t c1 = myParams->custom1;
	uint8_t c2 = myParams->custom2;
	if (myParams->custom3 & 0x40) {
		if (lastBpmCnt != bpmCnt) {
			lastBpmCnt = bpmCnt;
			if (bpmCnt == nextBpmCnt) {
				this->move();
				if ((myParams->custom3 & 0x30) >> 4 == 0) {
					nextBpmCnt += 12;
				}
				if ((myParams->custom3 & 0x30) >> 4 == 1) {
					nextBpmCnt += 8;
				}
				if ((myParams->custom3 & 0x30) >> 4 == 2) {
					nextBpmCnt += 6;
				}
				if ((myParams->custom3 & 0x30) >> 4 == 3) {
					nextBpmCnt += 4;
				}
				if (nextBpmCnt > 23) { nextBpmCnt -= 24; }
			}
		}
	}
	else {
		if (lastBeatNr != BeatNr) {
			lastBeatNr = BeatNr;
			if ((myParams->custom3 & 0x30) >> 4 == 1) {
				this->move();
			}
			if ((myParams->custom3 & 0x30) >> 4 == 2) {
				if (BeatNr % 2 == 0) this->move();
			}
			if ((myParams->custom3 & 0x30) >> 4 == 3) {
				if (BeatNr % 4 == 0) this->move();
			}
		}
	}


	
	while (i < num) {
		for (uint8_t w = 0; w < c1 + 1; w++){
			canvas->getLeds()[i] = myParams->getColor(myParams->hue);
			if (++i >= num) break; 
		}
		if (i >= num) break;
		for (uint8_t s = 0; s < c2 + 1; s++) {
			canvas->getLeds()[i] = myParams->getColor(128+myParams->hue);
			if (++i >= num) break;
		}
	}
}

void MaskBeatMover::move() {
	int8_t moveAmount;
	moveAmount = 1+(myParams->custom3 & 0x04);
	if (myParams->custom3 & 0x08) {
		myParams->startPos = myParams->startPos - moveAmount;
	}
	else {
		myParams->startPos = myParams->startPos + moveAmount;
	}
}

void ConfettiEffect::draw() {
	uint8_t  thisfade = 12;                                        // How quickly does it fade? Lower = slower fade rate.
	static int       thishue = 50;                                       // Starting hue.
	uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
	uint8_t   thissat = 100;                                      // The saturation, where 255 = brilliant colours.
	uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
	int       huediff = 240;                                      // Range of random #'s to use for hue
	uint8_t thisdelay = 15;                                        // We don't need much delay (if any)
	int pos;
	EVERY_N_MILLISECONDS(thisdelay) {
		fadeToBlackBy(canvas->getLeds(), canvas->getNumLeds(), thisfade);                    // Low values = slower fade.
		pos = random16(canvas->getNumLeds()-1);                               // Pick an LED at random.
		canvas->getLeds()[pos] += CHSV((thishue + random16(huediff)) / 4, thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
		thishue = thishue + thisinc;                                // It increments here.
	}
}
BaseBeatEffect::BaseBeatEffect() {
	timebase = myComp->getTimebase();
	beat = myComp->getBpm();
	firstRun = true;
}

void BaseBeatEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	uint16_t mid = n / 2;
	uint16_t val;
	if (firstRun == true && myParams->effectconf & 0x40) {						// if effect was just created, initialize timebase based on parameters in effectconf. 
		timebase = millis();
	}
	val = (0xff xor beat8(beat, timebase));
	val = map8(val, 0, 150);
	val = val << 1;

	uint8_t capVal;
	for (uint16_t i=0; i < mid; i++) {
		if (val > 240) { capVal = 240; } else { capVal = val; }
		l[mid + i] = myParams->getColor(capVal);
		l[mid - i] = myParams->getColor(capVal);
		if (val >  480 / n) { val -= 480 / n; }
		else { val = 0; }
	}
	firstRun = false;
}

void TwoColorsEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	static long tb = 0;
	static uint16_t b = 0;
	if (b == 0) {
		tb = myComp->getTimebase();
		b = myComp->getBpm();
	}
	uint16_t val = beat8(b, tb);
	fill_solid(l, n, CRGB::Black);
//	if ((g_bpm_beatnr & 1) xor (((myParams->vitality)&(0x01)) == 0)) {
//		val = (0xff ^ val);
//	}
	l[((n-1)*val / 256)] = myParams->getColor(myParams->hue);
	blur1d(l, n, myParams->blurryness);
}


SineloinEffect::SineloinEffect() {
	timebase = myComp->getTimebase();
	beat = myComp->getBpm();
	prevPos = 0;
	firstRun = true;
}
void SineloinEffect::draw() {
	// Effectconf Options:
	// Bits 3...0 ->  to influence the fade to black amount (00 = max fade, no slow fading tail, 11, maximum tail, slow fade)
	// Bit 5...4 -> phase offset 0,90,180,270 degrees		 (	
	// Bit 6 -> timebase on MIDI clock or effect start time
	// Channel Speed param effects movement speed of pixel
	// Vitality param affects number of pixels and their distance etc.

	uint16_t n = canvas->getNumLeds();					// how big is the pattern
	CRGB* l = canvas->getLeds();						// where to render the pattern onto
	uint16_t offset;									// used for phase offset of the sine wave
	uint16_t myBpm = (beat*(myParams->speed)) / 32;
	uint16_t pos;										// where to put the current pixel
	
	if (firstRun == true && myParams->effectconf & 0x40) {						// if effect was just created, initialize timebase based on parameters in effectconf. 
		timebase = millis();
	}
	fadeToBlackBy(l, n, (myParams->effectconf&0x0F)*8);		// fade canvas down based on parameter setting
	offset = 64 * (myParams->effectconf & 0x30)>>4;           // determine if offset needs to be applied based on parameter setting
	pos = beatsin8(myBpm, 0, 255, timebase, offset);		// determine the current position of the wave
	pos = pos*n / 256;
	if (firstRun) prevPos = pos;							// if the effect was just created, there is no tail to fill. 
	if (pos <= prevPos) {									// depending on direction of movement
		for (uint8_t i = 0; i <= (prevPos - pos); i++) {	// iterate through the pixel along the distance since the last frame
			l[pos+i]=myParams->getColor(myParams->hue);		// and fill those pixels with the current color
		}
	}
	else {													// same as above for other direction
		for (uint8_t i = 0; i <= (pos - prevPos); i++) {
			l[prevPos + i] = myParams->getColor(myParams->hue);
		}
	}
	prevPos = pos;											// memorize the current position for the next frame
	firstRun = false;										// set this to false as the first run is now over. 
}


// based on Fire 2012 https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
// from Mark Kriegsman
// COOLING = effectconf 20 - 100
// SPARKING = vitality 50 - 200

FireEffect::FireEffect() {
	firstRun = true;
}

FireEffect::~FireEffect(){
	delete heat;
}


void FireEffect::init() {
	firstRun = false;
	heat = new uint8_t[canvas->getNumLeds()];
	for (int i = 0; i < canvas->getNumLeds(); i++){
		heat[i] = 0;
	}
}

void FireEffect::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	if (firstRun == true) {
		init();
	}

	// Step 1.  Cool down every cell a little
	for (int i = 0; i < n; i++) {
		heat[i] = qsub8(heat[i], random8(0, ((myParams->effectconf * 20) / n) + 2));
	}
	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (int k = n - 1; k >= 2; k--) {
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
	}
	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if (random8() < myParams->vitality*2) {
		int y = random8(7);
		heat[y] = qadd8(heat[y], random8(160, 255));
	}   
	// Step 4.  Map from heat cells to LED colors
	for (int j = 0; j <n; j++) {
		// Scale the heat value from 0-255 down to 0-240
		// for best results with color palettes.
		byte colorindex = scale8(heat[j], 240);
		CRGB color = myParams->getColor(colorindex);
		l[j] = color;
	}
}


void CandleEffect::draw() {
#define N_ELEMENTS  20

	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	uint8_t BrightnessValues[] = { 7, 15, 31, 63, 127, 255, 25, 48, 77, 99, 190, 229, 150, 55, 255, 255 };
	uint8_t values[N_ELEMENTS];
	uint8_t paletteValues[4* N_ELEMENTS];
	CRGB pixel;
	CRGBPalette16 myPal;
	if (millis() - lastMillis > 100) {
		for (int i = 0; i < N_ELEMENTS; i++) {
			values[i] = BrightnessValues[random8(15)];
			pixel.r = 255;
			pixel.g = 147;
			pixel.b = 41;
			nscale8x3(pixel.r, pixel.g, pixel.b, dim8_raw(values[i]));
			paletteValues[(4 * i)] = (i*256/N_ELEMENTS);
			paletteValues[(4 *i )+1] = pixel.r;
			paletteValues[(4 * i)+2] = pixel.g;
			paletteValues[(4 * i)+3] = pixel.b;
			lastMillis = millis();
			
		}
		myPal.loadDynamicGradientPalette(paletteValues);
	}
	for (int i = 0; i < n; i++) {
		l[i] = ColorFromPalette(myPal, scale16by8(i, n), 255, LINEARBLEND);
	}
	

	// 255, 197, 143
	//nscale8x3(pixel.r, pixel.g, pixel.b, dim8_raw(5));


}

NoiseCandleEffect::NoiseCandleEffect() {
	x = random16();
	y = random16();
	z = random16();
	firstRun = true;
}

NoiseCandleEffect::~NoiseCandleEffect() {
	delete noise;
}

void NoiseCandleEffect::init() {
	firstRun = false;
	noise = new uint8_t[canvas->getNumLeds()];
}
void NoiseCandleEffect::draw() {
	if (firstRun== true) {
		this->init();
	}
	uint16_t speed = 4;
	uint32_t scale = 2000;
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	for (int i = 0; i < n; i++) {
		uint8_t data = inoise16(x, y + (i * scale), z) >> 8;
		noise[i] = data;
	}
	for (int i = 0; i < n; i++) {

		uint8_t index = (noise[i] >> 2);
		uint8_t bri = noise[i];

		CRGB color = CHSV(index, 200, bri);

		l[i] = color;
	}
	x += speed;

}


BarMover::BarMover() {
	firstBar = nullptr;
	lastBar = nullptr;
	currentBar = nullptr;
	lastMillis = millis();
}

BarMover::~BarMover(){
	MovingBar* iterator = lastBar;
	while (iterator != nullptr) {
		iterator = removeBar(iterator);
	}
}

MovingBar* BarMover::findPrev(MovingBar* bar) {
	MovingBar* iterator = firstBar;
	if (bar == iterator) return nullptr;
	if (iterator == nullptr) return iterator;
	while (iterator->getNext() != bar && iterator!=nullptr) {
		iterator= iterator->getNext();
	}
	return iterator;
}

MovingBar* BarMover::addBar(uint8_t signature, uint16_t size, uint8_t startingHue) {
	uint16_t start = scale16by8(canvas->getNumLeds(), 2 * myParams->custom2);
	MovingBar* newBar;
	newBar = new MovingBar(this, signature, size, scale16by8(canvas->getNumLeds(), 2 * myParams->custom3), start, startingHue);
		if (checkOverlap(newBar) == true) {
		delete newBar;
		return nullptr;
	}	
	if (firstBar == nullptr) {
		firstBar = newBar;
		lastBar = newBar;
		return newBar;
	}
	lastBar->setNext(newBar);
	lastBar = newBar;
	return newBar;
}



MovingBar* BarMover::removeBar(MovingBar* bar) {
	MovingBar* prev = findPrev(bar);
	if (bar == lastBar) {
		lastBar = prev;
	}
	if (bar == firstBar) {
		firstBar = firstBar->getNext();
	}
	if (prev != nullptr) {
		prev->setNext(bar->getNext());
	}
	delete bar;
	return prev;
}

void BarMover::process() {
	MovingBar* current = firstBar;
	MovingBar* next;
	while (current != nullptr) {
		if (current->processFrame() == false) {
			next = current->getNext();
			current = removeBar(current);
			current = next;
			if (current == nullptr) {
				break;
			}
		}
		else {
			current = current->getNext();
		}

	}
}

void BarMover::draw() {
	uint16_t n = canvas->getNumLeds();
	CRGB* l = canvas->getLeds();
	fill_solid(l, n, CRGB::Black);
	MovingBar* current = firstBar;
	int8_t  moveAmount;
	long timePassed;
	timePassed = 256 * (millis() - lastMillis);
	lastMillis = millis();
	speedBuffer += myComp->midiToInt(myParams->speed);
	if (speedBuffer > timePassed || (-1 * speedBuffer)>timePassed) {
		moveAmount = speedBuffer / timePassed;
		speedBuffer -= moveAmount*timePassed;
	}
	moveAmount = 0;
	while (current != nullptr) {
		for (int i = 0; i < moveAmount; i++) {
			if (current->processFrame() == false) {
				Serial.print("rem ");
				Serial.println((uint32_t)current,HEX);
				delay(200);
				current = removeBar(current);
				Serial.print("newcur ");
				Serial.println((uint32_t)current, HEX);
				delay(40);
				break;
			}
		}
		for (int p = 0; p < current->getSize(); p++) {
			uint16_t pos = current->getPos(p);
			l[pos] = myParams->getColor(current->getCurrentHue(p));
		}
		current = current->getNext();
	}
}

void BarMover::noteOn(uint8_t signature, uint8_t startHue) {
	if (signature == currentNote) {
		noteOff(signature);
	}
	currentBar=addBar(signature, myParams->custom1, 2*startHue);
	currentNote = signature;
}

void BarMover::noteOff(uint8_t signature) {
	if (currentBar != nullptr) {
		currentBar->finishGrowing();
	}
	currentNote = 0;
	currentBar = nullptr;
}

uint8_t BarMover::isFree(uint16_t pixelnum, uint8_t signature) {
	MovingBar* current = firstBar;
	while (current != nullptr) {
		if (current->getSignature() == signature) {
			if (current->isInBar(pixelnum)) return false;
		}
		current = current->getNext();
	}
	return true;
}

boolean BarMover::checkOverlap(MovingBar* bar) {
	MovingBar* current = firstBar;
	while (current != nullptr) {
		if (current->getSignature() == bar->getSignature()) {
			for (int p = 0; p < bar->getSize(); p++) {
				if (current->isInBar(bar->getPos(p))) return true;
			}
		}
		current = current->getNext();
	}
	return false;
}
#include "Transition.h"

Transition::Transition() {
}

void Transition::doTransition(Adafruit_NeoPixel_ZeroDMA& strip) {
	mBrightness = 0;
	mX1Low = 0;
	mX1High = 0;
	mX2Low = strip.numPixels() - 1;
	mX2High = strip.numPixels() - 1;
	mTransitionMode = TRANSITION_MODE_IN;
}

boolean Transition::doModeNext() {
	return mTransitionMode != TRANSITION_MODE_IN;
}

void Transition::loop(Adafruit_NeoPixel_ZeroDMA& strip) {
	switch (mTransitionMode) {
	case TRANSITION_MODE_IN:
		modeIn(strip);
		break;

	case TRANSITION_MODE_OUT:
		modeOut(strip);
		break;

	default:
		break;
	}
}

void Transition::modeIn(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mX1High < 295) mX1High++;
	if (mX2Low > 296) mX2Low--;
	if (mBrightness < 255) mBrightness++;

	if (mX1High == 295 && mX2Low == 296) mTransitionMode = TRANSITION_MODE_OUT;

	draw(strip);
}

void Transition::modeOut(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mX1Low < 295) mX1Low++;
	if (mX2High > 296) mX2High--;

	if (mX1Low == 295 && mX2High == 296) mTransitionMode = TRANSITION_MODE_DONE;

	draw(strip);
}

void Transition::draw(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (int16_t i = mX1Low; i <= mX1High; i++) {
		strip.setPixelColor(i, 0, 0, mBrightness);
	}
	for (int16_t i = mX2Low; i <= mX2High; i++) {
		strip.setPixelColor(i, 0, 0, mBrightness);
	}
}
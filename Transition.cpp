#include "Transition.h"

Transition::Transition() {
}

void Transition::doTransition(Adafruit_NeoPixel_ZeroDMA& strip) {
	mIntensity = 0;
	mX1Low = 210;
	mX1High = 210;
	mX2Low = 357;
	mX2High = 357;
	mTransitionMode = TRANSITION_MODE_IN;
}

boolean Transition::doModeNext() {
	return mTransitionMode == TRANSITION_MODE_OUT;
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
	if (mX1High < 283) mX1High++;
	if (mX2Low > 284) mX2Low--;

	mIntensity = 3.5 * (mX1High - mX1Low);
	//if (SerialUSB) {
	//	SerialUSB.print("modeIn mBrightness: ");
	//	SerialUSB.println(mBrightness);
	//}

	if (mX1High == 283 && mX2Low == 284) mTransitionMode = TRANSITION_MODE_OUT;

	draw(strip);
}

void Transition::modeOut(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mX1Low < 283) mX1Low++;
	if (mX2High > 284) mX2High--;

	mIntensity = 3.5 * (mX1High - mX1Low);
	//if (SerialUSB) {
	//	SerialUSB.print("modeOut mBrightness: ");
	//	SerialUSB.println(mBrightness);
	//}

	if (mX1Low == 283 && mX2High == 284) mTransitionMode = TRANSITION_MODE_DONE;

	draw(strip);
}

void Transition::draw(Adafruit_NeoPixel_ZeroDMA& strip) {
	uint32_t color = strip.Color(0, 0, mIntensity);

	// Sides
	for (int16_t i = 0; i < 210; i++) {
		//uint32_t color = strip.getPixelColor(i);
		//uint8_t r = color >> 16;
		//uint8_t g = color >> 8;
		//uint8_t b = color;
		strip.setPixelColor(i, color);
	}
	for (int16_t i = 358; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, color);
	}

	// Middle
	for (int16_t i = mX1Low; i <= mX1High; i++) {
		strip.setPixelColor(i, color);
	}
	for (int16_t i = mX2Low; i <= mX2High; i++) {
		strip.setPixelColor(i, color);
	}
}
#include "Booting.h"

Booting::Booting() {
}

void Booting::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	mBrightness = 0;
	mX1Low = 0;
	mX1High = 0;
	mX2Low = strip.numPixels() - 1;
	mX2High = strip.numPixels() - 1;
}

void Booting::loop(Adafruit_NeoPixel_ZeroDMA& strip) {
	mMillis = millis();
	if (mMillis > mMillisLast + 20) {
		mMillisLast = mMillis;

		if (mX1High < 295) mX1High++;
		if (mX2Low > 296) mX2Low--;

		if (mBrightness < 255) mBrightness++;

		if (mX1High == 295 && mX1Low < 295) mX1Low++;
		if (mX2Low == 296 && mX2High > 296) mX2High--;

		strip.clear();
		for (int16_t i = mX1Low; i <= mX1High; i++) {
			strip.setPixelColor(i, 0, 0, mBrightness);
		}
		for (int16_t i = mX2Low; i <= mX2High; i++) {
			strip.setPixelColor(i, 0, 0, mBrightness);
		}
		strip.show();
	}
}

boolean Booting::finished() {
	return mX1High == 295 && mX1Low == 295 && mX2Low == 296 && mX2High == 296;
}
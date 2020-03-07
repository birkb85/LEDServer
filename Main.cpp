#include "Main.h"

Main::Main() {
}

void Main::setup(Adafruit_NeoPixel_ZeroDMA& strip) {

}

void Main::loop(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (int16_t i = 0; i < strip.numPixels(); i++) {
		uint16_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
		strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
	}

	if (firstPixelHue < 65536)
		firstPixelHue += 256;
	else
		firstPixelHue = 0;
}

void Main::draw(Adafruit_NeoPixel_ZeroDMA& strip) {

}
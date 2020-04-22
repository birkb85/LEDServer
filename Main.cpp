#include "Main.h"

Main::Main() {
}

void Main::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	mColor = strip.Color(0, 0, 255);
}

void Main::handleData(String& data) {
	int dataTypeIndex = data.indexOf(":");
	if (dataTypeIndex > 0) {
		DataTypeMain dataTypeMain = (DataTypeMain)data.substring(0, dataTypeIndex).toInt();
		switch (dataTypeMain) {
		case DATA_MAIN_SET_MODE:
			mModeMain = (ModeMain)data.substring(dataTypeIndex + 1).toInt();
			break;

		case DATA_MAIN_COLOR:
			mColor = (uint32_t)data.substring(dataTypeIndex + 1).toInt();
			break;

		case DATA_MAIN_X:
			mX = (int16_t)data.substring(dataTypeIndex + 1).toInt();
			if (mX < 0 || mX >= 592) mX = 0;
			break;

		default:
			break;
		}
	}
}

void Main::loop(Adafruit_NeoPixel_ZeroDMA& strip) {
	switch (mModeMain) {
	case MODE_MAIN_BLINK:
		loopBlink(strip);
		break;

	case MODE_MAIN_WIPE:
		loopWipe(strip);
		break;

	case MODE_MAIN_THEATER_CHASE:
		loopTheaterChase(strip);
		break;

	case MODE_MAIN_RAINBOW:
		loopRainbow(strip);
		break;

	case MODE_MAIN_THEATER_CHASE_RAINBOW:
		loopTheaterChaseRainbow(strip);
		break;

	case MODE_MAIN_COMPASS:
		loopCompass(strip);
		break;

	default:
		break;
	}
}

//void Main::draw(Adafruit_NeoPixel_ZeroDMA& strip) {
//
//}

void Main::loopBlink(Adafruit_NeoPixel_ZeroDMA& strip) {
	strip.fill(strip.gamma32(mColor), 0, strip.numPixels());
}

void Main::loopWipe(Adafruit_NeoPixel_ZeroDMA& strip) {
	mX += mDirection;
	if (mX == 0 || mX == strip.numPixels() - 1) mDirection *= -1;
	strip.setPixelColor(mX, strip.gamma32(mColor));
}

void Main::loopTheaterChase(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (int16_t i = mTheaterChaseNumber; i < strip.numPixels(); i += 3) {
		strip.setPixelColor(i, strip.gamma32(mColor));
	}

	if (mTheaterChaseNumber < 2)
		mTheaterChaseNumber++;
	else
		mTheaterChaseNumber = 0;
}

void Main::loopRainbow(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (int16_t i = 0; i < strip.numPixels(); i++) {
		uint16_t pixelHue = mFirstPixelHue + (i * 65536L / strip.numPixels());
		strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
	}

	if (mFirstPixelHue < 65536)
		mFirstPixelHue += 256;
	else
		mFirstPixelHue = 0;
}

void Main::loopTheaterChaseRainbow(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (int16_t i = mTheaterChaseNumber; i < strip.numPixels(); i += 3) {
		uint16_t pixelHue = mFirstPixelHue + (i * 65536L / strip.numPixels());
		strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
	}

	if (mTheaterChaseNumber < 2)
		mTheaterChaseNumber++;
	else
		mTheaterChaseNumber = 0;

	if (mFirstPixelHue < 65536)
		mFirstPixelHue += 256;
	else
		mFirstPixelHue = 0;
}

void Main::loopCompass(Adafruit_NeoPixel_ZeroDMA& strip) {
	strip.setPixelColor(mX, strip.gamma32(mColor));
}
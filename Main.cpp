#include "Main.h"

Main::Main() {
}

void Main::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	mColor = strip.Color(0, 0, 255);
}

void Main::handleData(String data) {
	int dataTypeIndex = data.indexOf(":");
	if (dataTypeIndex > 0) {
		DataTypeMain dataTypeMain = DATA_MAIN_UNDEFINED;
		dataTypeMain = (DataTypeMain)data.substring(0, dataTypeIndex).toInt();

		switch (dataTypeMain) {
		case DATA_MAIN_UNDEFINED:
			break;

		case DATA_MAIN_SET_MODE:
			mModeMain = (ModeMain)data.substring(dataTypeIndex + 1).toInt();
			break;

		case DATA_MAIN_COLOR:
			mColor = (uint32_t)data.substring(dataTypeIndex + 1).toInt();
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

	default:
		break;
	}
}

void Main::draw(Adafruit_NeoPixel_ZeroDMA& strip) {

}

void Main::loopBlink(Adafruit_NeoPixel_ZeroDMA& strip) {
	strip.fill(strip.gamma32(mColor), 0, strip.numPixels());
}

void Main::loopWipe(Adafruit_NeoPixel_ZeroDMA& strip) {
	// TODO BB 2020-04-14. Testing mode.
	uint32_t color = strip.Color(0, 255, 0);
	for (int16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, strip.gamma32(color));
	}

	/*
	void colorWipe(uint32_t color, int wait) {
	  static uint8_t dir = 0;
	  if (dir == 0) {
		for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
		  strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
		  strip.show();                          //  Update strip to match
		  dir = 1;
		  delay(wait);                           //  Pause for a moment
		}
	  } else {
		for(int i=strip.numPixels()-1; i>=0; i--) { // For each pixel in strip...
		  strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
		  strip.show();                          //  Update strip to match
		  dir = 0;
		  delay(wait);                           //  Pause for a moment
		}
	  }
	}
	*/
}

void Main::loopTheaterChase(Adafruit_NeoPixel_ZeroDMA& strip) {
	// TODO BB 2020-04-14. Testing mode.
	uint32_t color = strip.Color(0, 0, 255);
	for (int16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, strip.gamma32(color));
	}

	/*
	void theaterChase(uint32_t color, int wait) {
	  static uint8_t dir = 0;
	  for(int a=0; a<10; a++) {  // Repeat 10 times...
		if (dir == 0) {
		  for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
			strip.clear();         //   Set all pixels in RAM to 0 (off)
			// 'c' counts up from 'b' to end of strip in steps of 3...
			for(int c=b; c<strip.numPixels(); c += 3) {
			  strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
			}
			strip.show(); // Update strip with new contents
			delay(wait);  // Pause for a moment
		  }
		} else {
		  for(int b=2; b>=0; b--) { //  'b' counts from 2 to 0...
			strip.clear();         //   Set all pixels in RAM to 0 (off)
			// 'c' counts up from 'b' to end of strip in steps of 3...
			for(int c=b; c<strip.numPixels(); c += 3) {
			  strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
			}
			strip.show(); // Update strip with new contents
			delay(wait);  // Pause for a moment
		  }
		}
	  }
	//  dir = !dir;
	}
	*/
}

void Main::loopRainbow(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (int16_t i = 0; i < strip.numPixels(); i++) {
		uint16_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
		strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
	}

	if (firstPixelHue < 65536)
		firstPixelHue += 256;
	else
		firstPixelHue = 0;
}

void Main::loopTheaterChaseRainbow(Adafruit_NeoPixel_ZeroDMA& strip) {
	// TODO BB 2020-04-14. Testing mode.
	uint32_t color = strip.Color(255, 255, 255);
	for (int16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, strip.gamma32(color));
	}

	/*
	void theaterChaseRainbow(int wait) {
	  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
	  for(int a=0; a<30; a++) {  // Repeat 30 times...
		for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
		  strip.clear();         //   Set all pixels in RAM to 0 (off)
		  // 'c' counts up from 'b' to end of strip in increments of 3...
		  for(int c=b; c<strip.numPixels(); c += 3) {
			// hue of pixel 'c' is offset by an amount to make one full
			// revolution of the color wheel (range 65536) along the length
			// of the strip (strip.numPixels() steps):
			int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
			uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
			strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
		  }
		  strip.show();                // Update strip with new contents
		  delay(wait);                 // Pause for a moment
		  firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
		}
	  }
	}
	*/
}
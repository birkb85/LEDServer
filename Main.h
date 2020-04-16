// Main.h

#ifndef _MAIN_h
#define _MAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>

class Main
{
public:
	Main();
	void setup(Adafruit_NeoPixel_ZeroDMA& strip);
	void handleData(String data);
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);
	void draw(Adafruit_NeoPixel_ZeroDMA& strip);

private:
	enum ModeMain {
		MODE_MAIN_BLINK,
		MODE_MAIN_WIPE,
		MODE_MAIN_THEATER_CHASE,
		MODE_MAIN_RAINBOW,
		MODE_MAIN_THEATER_CHASE_RAINBOW
	};
	ModeMain mModeMain = MODE_MAIN_RAINBOW;

	enum DataTypeMain {
		DATA_MAIN_UNDEFINED,
		DATA_MAIN_SET_MODE,
		DATA_MAIN_COLOR
	};

	long firstPixelHue = 0;
	uint32_t mColor = 0;

	void loopBlink(Adafruit_NeoPixel_ZeroDMA& strip);
	void loopWipe(Adafruit_NeoPixel_ZeroDMA& strip);
	void loopTheaterChase(Adafruit_NeoPixel_ZeroDMA& strip);
	void loopRainbow(Adafruit_NeoPixel_ZeroDMA& strip);
	void loopTheaterChaseRainbow(Adafruit_NeoPixel_ZeroDMA& strip);
};

#endif


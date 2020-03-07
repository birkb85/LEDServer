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
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);
	void draw(Adafruit_NeoPixel_ZeroDMA& strip);

private:
	long firstPixelHue = 0;
};

#endif


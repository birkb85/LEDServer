// Booting.h

#ifndef _BOOTING_h
#define _BOOTING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>

class Booting
{
public:
	Booting();
	void setup(Adafruit_NeoPixel_ZeroDMA& strip);
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);
	boolean finished();

private:
	unsigned long mMillis = 0;
	unsigned long mMillisLast = 0;

	uint8_t mBrightness = 0;
	int16_t mX1Low = 0;
	int16_t mX1High = 0;
	int16_t mX2Low = 0;
	int16_t mX2High = 0;
};

#endif


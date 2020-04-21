// Fireworks.h

#ifndef _FIREWORKS_h
#define _FIREWORKS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>

class Fireworks
{
public:
	Fireworks();
	void setup(Adafruit_NeoPixel_ZeroDMA& strip);
	void handleData(String& data);
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);
	void draw(Adafruit_NeoPixel_ZeroDMA& strip);

private:
	//enum DataTypeFireworks {
	//	DATA_FIREWORKS_UNDEFINED
	//};
};

#endif


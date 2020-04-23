// Firework.h

#ifndef _FIREWORK_h
#define _FIREWORK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "Firework_Rocket.h"

class Firework
{
public:
	Firework();
	void setup(Adafruit_NeoPixel_ZeroDMA& strip);
	void handleData(String& data);
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);
	//void draw(Adafruit_NeoPixel_ZeroDMA& strip);

private:
	//enum DataTypeFirework {
	//	DATA_FIREWORK_
	//};

	#define ROCKETS_SIZE 1
	Firework_Rocket mRockets[ROCKETS_SIZE];
};

#endif


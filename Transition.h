// Transition.h

#ifndef _TRANSITION_h
#define _TRANSITION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>

class Transition
{
public:
	Transition();
	void doTransition(Adafruit_NeoPixel_ZeroDMA& strip);
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);
	void draw(Adafruit_NeoPixel_ZeroDMA& strip);
	boolean doModeNext();

private:
	enum TransitionMode {
		TRANSITION_MODE_IN,
		TRANSITION_MODE_OUT,
		TRANSITION_MODE_DONE
	};
	TransitionMode mTransitionMode = TRANSITION_MODE_DONE;

	uint8_t mBrightness = 0;
	int16_t mX1Low = 0;
	int16_t mX1High = 0;
	int16_t mX2Low = 0;
	int16_t mX2High = 0;

	void modeIn(Adafruit_NeoPixel_ZeroDMA& strip);
	void modeOut(Adafruit_NeoPixel_ZeroDMA& strip);
};

#endif


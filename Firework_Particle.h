// Firework_Particle.h

#ifndef _FIREWORK_PARTICLE_h
#define _FIREWORK_PARTICLE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>

class Firework_Particle
{
public:
	Firework_Particle();
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);

private:
};

#endif


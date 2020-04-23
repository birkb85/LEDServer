// Firework_Rocket.h

#ifndef _FIREWORK_ROCKET_h
#define _FIREWORK_ROCKET_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "Firework_Particle.h"

class Firework_Rocket
{
public:
	Firework_Rocket();
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);

private:
	#define PARTICLES_SIZE 5
	Firework_Particle mParticles[PARTICLES_SIZE];
};

#endif


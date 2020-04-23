#include "Firework_Rocket.h"

Firework_Rocket::Firework_Rocket() {
}

void Firework_Rocket::loop(Adafruit_NeoPixel_ZeroDMA& strip) {
	for (uint8_t i = 0; i < PARTICLES_SIZE; i++) {
		mParticles[0].loop(strip);
	}
}
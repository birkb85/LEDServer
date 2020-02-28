// Pong_Bat.h

#ifndef _PONG_BAT_h
#define _PONG_BAT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Pong_Bat
{
public:
	Pong_Bat();
	int16_t x = 0;
	int8_t length = 0;
	uint32_t color = 0;

private:
};

#endif


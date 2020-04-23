// Pong_Ball.h

#ifndef _PONG_BALL_h
#define _PONG_BALL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Pong_Ball
{
public:
	Pong_Ball();
	int8_t mDir = 0;
	int16_t mX = 0;
	uint32_t mColor = 0;
	int8_t mSpd = 0;
	void update();

private:
};

#endif


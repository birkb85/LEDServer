// Pong.h

#ifndef _PONG_h
#define _PONG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "Pong_Ball.h"
#include "Pong_Bat.h"
#include "ClientData.h";

class Pong
{
public:
	Pong();
	void setup(Adafruit_NeoPixel_ZeroDMA& strip);
	void setButtonState(String data);
	void resetGame();
	void loop(Adafruit_NeoPixel_ZeroDMA& strip);

private:
	enum DataTypePong {
		DATA_PONG_P1_PRESSED,
		DATA_PONG_P1_RELEASED,
		DATA_PONG_P2_PRESSED,
		DATA_PONG_P2_RELEASED
	};

	unsigned long mMillis = 0;
	unsigned long mMillisLast = 0;

	boolean mButton1Pressed = false;
	boolean mButton1Released = false;
	boolean mButton2Pressed = false;
	boolean mButton2Released = false;

	const int16_t mGoalX = 284;
	const int16_t mBat1StartX = mGoalX - 100;
	const int16_t mBat2StartX = mGoalX + 100;

	Pong_Bat mBat1;
	Pong_Bat mBat2;
	Pong_Ball mBall;

	void updateGame(Adafruit_NeoPixel_ZeroDMA& strip);
	void updateStrip(Adafruit_NeoPixel_ZeroDMA& strip);
	void checkGameEnd(Adafruit_NeoPixel_ZeroDMA& strip);
	void gameEnd(Adafruit_NeoPixel_ZeroDMA& strip, Pong_Bat& bat);
	void resetButtonState();
};

#endif


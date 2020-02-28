// Pong.h

#ifndef _PONG_h
#define _PONG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//https://learn.adafruit.com/dma-driven-neopixels/overview
//#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "Pong_Ball.h"
#include "Pong_Bat.h"
#include "ClientData.h";

class Pong
{
public:
	Pong();
	void setup();
	void setButtonStateLast();
	void setButtonState(String data);
	void loop();

private:
	unsigned long mMillis = 0;
	unsigned long mMillisLast = 0;
	const int8_t mLedPin = 5; //8;
	const int16_t mLedCount = 592;
	//Adafruit_NeoPixel mStrip = Adafruit_NeoPixel(mLedCount, mLedPin, NEO_GRB + NEO_KHZ800);
	Adafruit_NeoPixel_ZeroDMA  mStrip = Adafruit_NeoPixel_ZeroDMA(mLedCount, mLedPin, NEO_GRB);

	int8_t mButton1StateLast = 0;
	int8_t mButton1State = 0;
	boolean mButton1Pressed = false;
	boolean mButton1Released = false;
	int8_t mButton2StateLast = 0;
	int8_t mButton2State = 0;
	boolean mButton2Pressed = false;
	boolean mButton2Released = false;

	Pong_Bat mBat1;
	Pong_Bat mBat2;
	Pong_Ball mBall;
	
	void resetButtonState();
	//void updateButtonState();
	void updateGame();
	void updateStrip();
	void checkGameEnd();
	void gameEnd(Pong_Bat bat);
};

#endif


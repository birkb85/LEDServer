#include "Pong.h"

Pong::Pong() {
}

void Pong::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	mBat1.x = 20;
	mBat1.length = 16;
	mBat1.color = strip.Color(255, 0, 0);

	mBat2.x = 420;
	mBat2.length = -16;
	mBat2.color = strip.Color(0, 255, 0);

	mBall.dir = -1;
	mBall.x = 220;
	mBall.color = strip.Color(0, 0, 255);
	mBall.spd = 1;
}

void Pong::setButtonState(String data) {
	DataTypePong dataTypePong = (DataTypePong)data.toInt();
	switch (dataTypePong)
	{
	case DATA_PONG_P1_PRESSED:
		mButton1Pressed = true;
		break;

	case DATA_PONG_P1_RELEASED:
		mButton1Released = true;
		break;

	case DATA_PONG_P2_PRESSED:
		mButton2Pressed = true;
		break;

	case DATA_PONG_P2_RELEASED:
		mButton2Released = true;
		break;

	default:
		break;
	}
}

void Pong::resetButtonState() {
	mButton1Pressed = false;
	mButton1Released = false;
	mButton2Pressed = false;
	mButton2Released = false;
}

void Pong::loop(Adafruit_NeoPixel_ZeroDMA& strip) {
	mMillis = millis();
	if (mMillis > mMillisLast + 20) {
		mMillisLast = mMillis;

		updateGame(strip);

		updateStrip(strip);

		checkGameEnd(strip);

		resetButtonState();
	}
}

void Pong::updateGame(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mButton1Pressed) {
		if (mBall.x >= mBat1.x && mBall.x <= mBat1.x + mBat1.length && mBall.dir == -1)
		{
			mBall.dir = 1;
			mBat1.x = mBall.x;
		}
	}

	if (mButton2Pressed) {
		if (mBall.x <= mBat2.x && mBall.x >= mBat2.x + mBat2.length && mBall.dir == 1)
		{
			mBall.dir = -1;
			mBat2.x = mBall.x;
		}
	}

	mBall.update();
	if (mBall.x < 0)
	{
		mBall.x += strip.numPixels();
		mBat2.x += mBat2.length;
	}
	if (mBall.x > strip.numPixels() - 1)
	{
		mBall.x -= strip.numPixels();
		mBat1.x += mBat1.length;
	}
}

void Pong::updateStrip(Adafruit_NeoPixel_ZeroDMA& strip) {
	strip.clear();

	if (mButton1Pressed) {
		for (int8_t i = 0; i <= mBat1.length; i++) {
			strip.setPixelColor(mBat1.x + i, mBat1.color);
		}
	}
	else if (mButton1Released) {
		strip.setPixelColor(mBat1.x + 1, mBat1.color);
		strip.setPixelColor(mBat1.x + mBat1.length - 1, mBat1.color);
	}
	else {
		strip.setPixelColor(mBat1.x, mBat1.color);
		strip.setPixelColor(mBat1.x + mBat1.length, mBat1.color);
	}

	if (mButton2Pressed) {
		for (int8_t i = 0; i >= mBat2.length; i--) {
			strip.setPixelColor(mBat2.x + mBat2.length - i, mBat2.color);
		}
	}
	else if (mButton2Released) {
		strip.setPixelColor(mBat2.x - 1, mBat2.color);
		strip.setPixelColor(mBat2.x + mBat2.length + 1, mBat2.color);
	}
	else {
		strip.setPixelColor(mBat2.x, mBat2.color);
		strip.setPixelColor(mBat2.x + mBat2.length, mBat2.color);
	}

	strip.setPixelColor(mBall.x, mBall.color);

	strip.setPixelColor(220, strip.Color(255, 255, 255));

	strip.show();
}

void Pong::checkGameEnd(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mBat1.x + mBat1.length >= 220)
		gameEnd(strip, mBat1);
	else if (mBat2.x + mBat2.length <= 220)
		gameEnd(strip, mBat2);
}

void Pong::gameEnd(Adafruit_NeoPixel_ZeroDMA& strip, Pong_Bat bat) {
	delay(500);
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, bat.color);
	}
	strip.show();
	delay(200);
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, 0);
	}
	strip.show();
	delay(200);
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, bat.color);
	}
	strip.show();
	delay(200);
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, 0);
	}
	strip.show();
	delay(200);

	mBat1.x = 20;
	mBat2.x = 420;
	mBall.x = 220;
}
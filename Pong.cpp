#include "Pong.h"

Pong::Pong() {
}

void Pong::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	mBat1.mLength = 16;
	mBat1.mColor = strip.Color(255, 0, 0);

	mBat2.mLength = 16;
	mBat2.mColor = strip.Color(0, 255, 0);

	mBall.mColor = strip.Color(0, 0, 255);

	resetGame();
}

void Pong::resetGame() {
	mGameRunning = false;

	mBat1.mX = mBat1StartX;

	mBat2.mX = mBat2StartX - (mBat2.mLength - 1);

	mBall.mDir = random(2) == 0 ? -1 : 1;
	mBall.mX = mGoalStartX;
	mBall.mSpd = 1;
}

void Pong::startGame() {
	mGameRunning = true;
}

void Pong::setButtonState(String& data) {
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
	if (!mGameRunning) {
		if (mButton1Pressed || mButton2Pressed) {
			startGame();
		}
	}
	else {
		updateGame(strip);
	}

	updateStrip(strip);

	checkGameEnd(strip);

	resetButtonState();
}

void Pong::updateGame(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mButton1Pressed) {
		if (mBall.mX >= mBat1.mX && mBall.mX < (mBat1.mX + mBat1.mLength) && mBall.mDir == -1)
		{
			mBall.mDir = 1;
			mBat1.mX = mBall.mX;
		}
	}

	if (mButton2Pressed) {
		if (mBall.mX >= mBat2.mX && mBall.mX < (mBat2.mX + mBat2.mLength) && mBall.mDir == 1)
		{
			mBall.mDir = -1;
			mBat2.mX = mBall.mX - (mBat2.mLength - 1);
		}
	}

	mBall.update();
	if ((mBall.mX < (mBat1StartX - 20)) || (mBall.mX > (mBat2StartX + (mBat2.mLength - 1) + 20))) mBall.update();
	if (mBall.mX < 0)
	{
		mBall.mX += strip.numPixels();
		mBat2.mX -= mBat2.mLength;
	}
	if (mBall.mX > strip.numPixels() - 1)
	{
		mBall.mX -= strip.numPixels();
		mBat1.mX += mBat1.mLength;
	}
}

void Pong::updateStrip(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mButton1Pressed) {
		for (int8_t i = 0; i < mBat1.mLength; i++) {
			strip.setPixelColor(mBat1.mX + i, mBat1.mColor);
		}
	}
	else if (mButton1Released) {
		strip.setPixelColor(mBat1.mX, mBat1.mColor);
		strip.setPixelColor(mBat1.mX + 1, mBat1.mColor);
		strip.setPixelColor(mBat1.mX + mBat1.mLength - 2, mBat1.mColor);
		strip.setPixelColor(mBat1.mX + mBat1.mLength - 1, mBat1.mColor);
	}
	else {
		strip.setPixelColor(mBat1.mX, mBat1.mColor);
		strip.setPixelColor(mBat1.mX + mBat1.mLength - 1, mBat1.mColor);
	}

	if (mButton2Pressed) {
		for (int8_t i = 0; i < mBat2.mLength; i++) {
			strip.setPixelColor(mBat2.mX + i, mBat2.mColor);
		}
	}
	else if (mButton2Released) {
		strip.setPixelColor(mBat2.mX, mBat2.mColor);
		strip.setPixelColor(mBat2.mX + 1, mBat2.mColor);
		strip.setPixelColor(mBat2.mX + mBat2.mLength - 2, mBat2.mColor);
		strip.setPixelColor(mBat2.mX + mBat2.mLength - 1, mBat2.mColor);
	}
	else {
		strip.setPixelColor(mBat2.mX, mBat2.mColor);
		strip.setPixelColor(mBat2.mX + mBat2.mLength - 1, mBat2.mColor);
	}

	strip.setPixelColor(mBall.mX, mBall.mColor);

	strip.setPixelColor(mGoalStartX, strip.Color(255, 255, 255));
	strip.setPixelColor(mGoalEndX, strip.Color(255, 255, 255));
}

void Pong::checkGameEnd(Adafruit_NeoPixel_ZeroDMA& strip) {
	if (mBat1.mX + mBat1.mLength - 1 >= mGoalStartX)
		gameEnd(strip, mBat1);
	else if (mBat2.mX <= mGoalEndX)
		gameEnd(strip, mBat2);
}

void Pong::gameEnd(Adafruit_NeoPixel_ZeroDMA& strip, Pong_Bat& bat) {
	delay(500);
	strip.fill(bat.mColor, 0, strip.numPixels());
	strip.show();
	delay(200);
	strip.clear();
	strip.show();
	delay(200);
	strip.fill(bat.mColor, 0, strip.numPixels());
	strip.show();
	delay(200);
	strip.clear();
	strip.show();
	delay(200);

	resetGame();
}
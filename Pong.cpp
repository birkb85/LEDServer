#include "Pong.h"

Pong::Pong() {
}

void Pong::setup() {
	mStrip.begin();
	mStrip.setBrightness(40);
	mStrip.show();

	mBat1.x = 20;
	mBat1.length = 16;
	//  bat1.color = strip.Color(random(256), random(256), random(256));
	mBat1.color = mStrip.Color(255, 0, 0);

	mBat2.x = 420;
	mBat2.length = -16;
	//  bat2.color = strip.Color(random(256), random(256), random(256));
	mBat2.color = mStrip.Color(0, 255, 0);

	mBall.dir = -1;
	mBall.x = 220;
	//  ball.color = strip.Color(random(256), random(256), random(256));
	mBall.color = mStrip.Color(0, 0, 255);
	mBall.spd = 1; //2.0f; // Lav så det vises flydende når hastigheden er større end 1.0f.
}

void Pong::setButtonStateLast() {
	mButton1StateLast = mButton1State;
	mButton2StateLast = mButton2State;
}

void Pong::setButtonState(String data) {
	//if (SerialUSB) {
	//	SerialUSB.print("Pong SetButtonState Client data, number: ");
	//	SerialUSB.print(clientData.mClientNumber);
	//	SerialUSB.print(", data: ");
	//	SerialUSB.println(clientData.mData);
	//}

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


	//if (data.startsWith("P1=")) {
	//	if (data.substring(3).toInt() == 1)
	//		mButton1Pressed = true;
	//	else
	//		mButton1Released = true;
	//	//mButton1State = data.substring(3).toInt();
	//}
	//else if (data.startsWith("P2=")) {
	//	if (data.substring(3).toInt() == 1)
	//		mButton2Pressed = true;
	//	else
	//		mButton2Released = true;
	//	//mButton2State = data.substring(3).toInt();
	//}

	//if (SerialUSB) {
	//	SerialUSB.print("1: pressed: ");
	//	SerialUSB.print(mButton1Pressed);
	//	SerialUSB.print(", released: ");
	//	SerialUSB.print(mButton1Released);
	//	SerialUSB.print(", 2: pressed: ");
	//	SerialUSB.print(mButton2Pressed);
	//	SerialUSB.print(", released: ");
	//	SerialUSB.println(mButton2Released);
	//}
}

void Pong::resetButtonState() {
	mButton1Pressed = false;
	mButton1Released = false;
	mButton2Pressed = false;
	mButton2Released = false;
}

void Pong::loop() {
	mMillis = millis();
	if (mMillis > mMillisLast + 20) {
		mMillisLast = mMillis;

		//updateButtonState();

		updateGame();

		updateStrip(); // TODO BB 2020-02-21. WifiServeren kan ikke modtage/sende beskeder når strip opdateres i hvert loop, find ud af hvorfor det sker...

		checkGameEnd(); // TODO BB 2020-02-21. For at teste hastighed. Se ovenstående.

		resetButtonState();

		//delay(50);
	}
}

//void Pong::updateButtonState() {
//	if (mButton1StateLast == LOW && mButton1State == HIGH)
//		mButton1Pressed = true;
//	else
//		mButton1Pressed = false;
//
//	if (mButton2StateLast == LOW && mButton2State == HIGH)
//		mButton2Pressed = true;
//	else
//		mButton2Pressed = false;
//
//	if (mButton1StateLast == HIGH && mButton1State == LOW)
//		mButton1Released = true;
//	else
//		mButton1Released = false;
//
//	if (mButton2StateLast == HIGH && mButton2State == LOW)
//		mButton2Released = true;
//	else
//		mButton2Released = false;
//
//	/*if (SerialUSB) {
//		if (mButton1Pressed) SerialUSB.println("Button 1 Pressed");
//		if (mButton1Released) SerialUSB.println("Button 1 Released");
//		if (mButton2Pressed) SerialUSB.println("Button 2 Pressed");
//		if (mButton2Released) SerialUSB.println("Button 2 Released");
//	}*/
//}

void Pong::updateGame() {
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
		mBall.x += mLedCount;
		mBat2.x += mBat2.length;
	}
	if (mBall.x > mLedCount - 1)
	{
		mBall.x -= mLedCount;
		mBat1.x += mBat1.length;
	}
}

void Pong::updateStrip() {
	mStrip.clear();

	if (mButton1Pressed) {
		for (int8_t i = 0; i <= mBat1.length; i++) {
			mStrip.setPixelColor(mBat1.x + i, mBat1.color);
		}
	}
	else if (mButton1Released) {
		mStrip.setPixelColor(mBat1.x + 1, mBat1.color);
		mStrip.setPixelColor(mBat1.x + mBat1.length - 1, mBat1.color);
	}
	else {
		mStrip.setPixelColor(mBat1.x, mBat1.color);
		mStrip.setPixelColor(mBat1.x + mBat1.length, mBat1.color);
	}

	if (mButton2Pressed) {
		for (int8_t i = 0; i >= mBat2.length; i--) {
			mStrip.setPixelColor(mBat2.x + mBat2.length - i, mBat2.color);
		}
	}
	else if (mButton2Released) {
		mStrip.setPixelColor(mBat2.x - 1, mBat2.color);
		mStrip.setPixelColor(mBat2.x + mBat2.length + 1, mBat2.color);
	}
	else {
		mStrip.setPixelColor(mBat2.x, mBat2.color);
		mStrip.setPixelColor(mBat2.x + mBat2.length, mBat2.color);
	}

	mStrip.setPixelColor(mBall.x, mBall.color);

	mStrip.setPixelColor(220, mStrip.Color(255, 255, 255));

	mStrip.show();
}

void Pong::checkGameEnd() {
	if (mBat1.x + mBat1.length >= 220)
		gameEnd(mBat1);
	else if (mBat2.x + mBat2.length <= 220)
		gameEnd(mBat2);
}

void Pong::gameEnd(Pong_Bat bat) {
	delay(500);
	for (int i = 0; i < mStrip.numPixels(); i++) {
		mStrip.setPixelColor(i, bat.color);
	}
	mStrip.show();
	delay(200);
	for (int i = 0; i < mStrip.numPixels(); i++) {
		mStrip.setPixelColor(i, 0);
	}
	mStrip.show();
	delay(200);
	for (int i = 0; i < mStrip.numPixels(); i++) {
		mStrip.setPixelColor(i, bat.color);
	}
	mStrip.show();
	delay(200);
	for (int i = 0; i < mStrip.numPixels(); i++) {
		mStrip.setPixelColor(i, 0);
	}
	mStrip.show();
	delay(200);

	mBat1.x = 20;
	mBat2.x = 420;
	mBall.x = 220;
}
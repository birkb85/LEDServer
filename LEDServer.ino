#include <Adafruit_NeoPixel_ZeroDMA.h> //https://learn.adafruit.com/dma-driven-neopixels/overview
#include "WifiServer.h";
#include "Transition.h"
#include "Main.h"
#include "Pong.h"
#include "Firework.h"

String mDataString = "";

const int8_t mStatusLedPin = 13;

const int8_t mLedStripPin = 5;
const int16_t mLedStripCount = 592;
Adafruit_NeoPixel_ZeroDMA  mStrip = Adafruit_NeoPixel_ZeroDMA(mLedStripCount, mLedStripPin, NEO_GRB);

// About 50-60mA per Led when full lit.
// About 20 A available.
// A brightness value of about 139, when all LEDs are full lit is about 20 A.
// From 10 to 139 is 130 values, client should send in a range of 0-129.
const uint8_t mBrightnessMin = 10;
const uint8_t mBrightnessMax = 139;

unsigned long mLoopMillis = 0;
unsigned long mLoopMillisLast = 0;
const uint8_t mLoopIntervalPong = 20;
const uint8_t mLoopIntervalFirework = 20;
// From 20 to 199 is 180 values, client should send in a range of 0-179.
const uint8_t mLoopIntervalMin = 20;
const uint8_t mLoopIntervalMax = 199;
uint8_t mLoopIntervalCurrent = 20;

boolean mStripClear = true;

WifiServer mWifiServer;
Transition mTransition;
Main mMain;
Pong mPong;
Firework mFirework;

enum Mode {
	MODE_BOOTING,
	MODE_MAIN,
	MODE_PONG,
	MODE_FIREWORK
};
Mode mMode = MODE_BOOTING;
Mode mModeNext = MODE_MAIN;

enum DataType {
	DATA_HEART_BEAT,
	DATA_MESSAGE,
	DATA_SET_BRIGHTNESS,
	DATA_SET_LOOP_INTERVAL,
	DATA_SET_STRIP_CLEAR,
	DATA_SET_MODE,
	DATA_MAIN,
	DATA_PONG,
	DATA_FIREWORK
};

void setup() {
	pinMode(mStatusLedPin, OUTPUT);
	digitalWrite(mStatusLedPin, HIGH);

	mDataString.reserve(100);

	mStrip.begin();
	mStrip.setBrightness(40);
	mStrip.setPixelColor(0, mStrip.Color(0, 0, 255));
	mStrip.show();

	mWifiServer.setup();
	mMain.setup(mStrip);
	mPong.setup(mStrip);
	mFirework.setup(mStrip);

	digitalWrite(mStatusLedPin, LOW);

	mStrip.setPixelColor(0, mStrip.Color(0, 0, 0));
	mStrip.show();

	mTransition.doTransition(mStrip);
}

void loop() {
	readSerial();

	mLoopMillis = millis();
	if (mLoopMillis > mLoopMillisLast + mLoopIntervalCurrent) {
		mLoopMillisLast = mLoopMillis;
		modeLoop();
	}
}

void readSerial() {
	int8_t clientNumber = mWifiServer.readSerial(mDataString);
	if (clientNumber >= 0 && mDataString.length() > 0) {
		while (mDataString.length() > 0) {
			//if (SerialUSB) SerialUSB.println(mDataString); // TODO BB Comment out after test.
			int dataTypeEndIndex = mDataString.indexOf(":");
			if (dataTypeEndIndex > 0) {
				int dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					DataType dataType = (DataType)mDataString.substring(0, dataTypeEndIndex).toInt();
					String data = mDataString.substring(dataTypeEndIndex + 1, dataEndIndex);
					mDataString = mDataString.substring(dataEndIndex + 1);
					switch (dataType) {
					case DATA_HEART_BEAT:
						mWifiServer.writeToClient(clientNumber, "HB:" + String(freeMemory()));
						break;

					case DATA_MESSAGE:
						mWifiServer.writeToClient(clientNumber, data);
						break;

					case DATA_SET_BRIGHTNESS:
						setBrightness(data.toInt());
						break;

					case DATA_SET_LOOP_INTERVAL:
						setLoopInterval(data.toInt());
						break;

					case DATA_SET_STRIP_CLEAR:
						setStripClear(data.toInt());
						break;

					case DATA_SET_MODE:
						mModeNext = (Mode)data.toInt();
						if (mModeNext != mMode)
							mTransition.doTransition(mStrip);
						break;

					case DATA_MAIN:
						mMain.handleData(data);
						break;

					case DATA_PONG:
						mPong.setButtonState(data);
						break;

					case DATA_FIREWORK:
						mFirework.handleData(data);
						break;

					default:
						break;
					}
				}
				else {
					mDataString = "";
				}
			}
			else {
				mDataString = "";
			}
		}
	}
	else {
		mDataString = "";
	}
}

void setBrightness(uint8_t brightness) {
	if (brightness + mBrightnessMin < mBrightnessMax) {
		mStrip.setBrightness(brightness + mBrightnessMin);
	}
	else {
		mStrip.setBrightness(mBrightnessMax);
	}
}

void setLoopInterval(uint8_t interval) {
	if (interval + mLoopIntervalMin < mLoopIntervalMax) {
		mLoopIntervalCurrent = interval + mLoopIntervalMin;
	}
	else {
		mLoopIntervalCurrent = mLoopIntervalMax;
	}
}

void setStripClear(uint8_t clear) {
	mStripClear = clear > 0;
}

void modeLoop() {
	if (mStripClear) mStrip.clear();

	switch (mMode) {
	case MODE_MAIN:
		mMain.loop(mStrip);
		break;

	case MODE_PONG:
		mPong.loop(mStrip);
		break;

	case MODE_FIREWORK:
		mFirework.loop(mStrip);
		break;

	default:
		break;
	}

	mTransition.loop(mStrip);
	if (mMode != mModeNext) {
		if (mTransition.doModeNext()) {
			mMode = mModeNext;
			switch (mMode) {
			case MODE_MAIN:
				break;

			case MODE_PONG:
				mLoopIntervalCurrent = mLoopIntervalPong;
				mStripClear = true;
				mPong.resetGame();
				break;

			case MODE_FIREWORK:
				mLoopIntervalCurrent = mLoopIntervalFirework;
				mStripClear = true;
				break;

			default:
				break;
			}
		}
	}

	mStrip.show();
}
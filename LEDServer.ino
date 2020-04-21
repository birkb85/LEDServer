#include <Adafruit_NeoPixel_ZeroDMA.h> //https://learn.adafruit.com/dma-driven-neopixels/overview
#include "WifiServer.h";
#include "Transition.h"
#include "Main.h"
#include "Pong.h"

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
// From 20 to 199 is 180 values, client should send in a range of 0-179.
const uint8_t mLoopIntervalMin = 20;
const uint8_t mLoopIntervalMax = 199;
uint8_t mLoopIntervalCurrent = 20;

boolean mStripClear = true;

WifiServer mWifiServer;
Transition mTransition;
Main mMain;
Pong mPong;

enum Mode {
	MODE_BOOTING,
	MODE_MAIN,
	MODE_PONG
};
Mode mMode = MODE_BOOTING;
Mode mModeNext = MODE_MAIN;

enum DataType {
	DATA_UNDEFINED,
	DATA_HEART_BEAT,
	DATA_MESSAGE,
	DATA_SET_BRIGHTNESS,
	DATA_SET_LOOP_INTERVAL,
	DATA_SET_STRIP_CLEAR,
	DATA_SET_MODE,
	DATA_MAIN,
	DATA_PONG
};

void setup() {
	pinMode(mStatusLedPin, OUTPUT);
	digitalWrite(mStatusLedPin, HIGH);

	mDataString.reserve(200);

	mStrip.begin();
	mStrip.setBrightness(40);
	mStrip.setPixelColor(0, mStrip.Color(0, 0, 255));
	mStrip.show();

	mWifiServer.setup();
	mMain.setup(mStrip);
	mPong.setup(mStrip);

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
			DataType dataType = DATA_UNDEFINED;

			int dataTypeEndIndex = mDataString.indexOf(":");
			if (dataTypeEndIndex > 0) {
				dataType = (DataType)mDataString.substring(0, dataTypeEndIndex).toInt();
			}

			int dataEndIndex = -1;
			boolean dataTypeHandled = false;
			switch (dataType) {
			case DATA_UNDEFINED:
				break;

			case DATA_HEART_BEAT:
				mWifiServer.writeToClient(clientNumber, "HB:" + String(freeMemory()));
				mDataString = mDataString.substring(4); //XX:;
				dataTypeHandled = true;
				break;

			case DATA_MESSAGE:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					mWifiServer.writeToClient(clientNumber, mDataString.substring(dataTypeEndIndex + 1, dataEndIndex));
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			case DATA_SET_BRIGHTNESS:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					setBrightness(mDataString.substring(dataTypeEndIndex + 1, dataEndIndex).toInt());
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			case DATA_SET_LOOP_INTERVAL:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					setLoopInterval(mDataString.substring(dataTypeEndIndex + 1, dataEndIndex).toInt());
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			case DATA_SET_STRIP_CLEAR:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					setStripClear(mDataString.substring(dataTypeEndIndex + 1, dataEndIndex).toInt());
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			case DATA_SET_MODE:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					mModeNext = (Mode)mDataString.substring(dataTypeEndIndex + 1, dataEndIndex).toInt();
					if (mModeNext != mMode)
						mTransition.doTransition(mStrip);
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			case DATA_MAIN:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					mMain.handleData(mDataString.substring(dataTypeEndIndex + 1, dataEndIndex));
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			case DATA_PONG:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					mPong.setButtonState(mDataString.substring(dataTypeEndIndex + 1, dataEndIndex));
					mDataString = mDataString.substring(dataEndIndex + 1);
					dataTypeHandled = true;
				}
				break;

			default:
				break;
			}
			if (dataTypeHandled) continue;

			mDataString = "";
			break;
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

			default:
				break;
			}
		}
	}

	mStrip.show();
}
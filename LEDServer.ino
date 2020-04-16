#include <Adafruit_NeoPixel_ZeroDMA.h> //https://learn.adafruit.com/dma-driven-neopixels/overview
#include "WifiServer.h";
#include "Transition.h"
#include "Main.h"
#include "Pong.h"

String mDataString = "";

const int8_t mStatusLedPin = 13;

unsigned long mMillis = 0;
unsigned long mMillisLast = 0;

const int8_t mLedStripPin = 5;
const int16_t mLedStripCount = 592;
Adafruit_NeoPixel_ZeroDMA  mStrip = Adafruit_NeoPixel_ZeroDMA(mLedStripCount, mLedStripPin, NEO_GRB);

const uint8_t mBrightnessMin = 20; // About 20 amp when full lit. (can be set to min + 127 = 147 max).

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

	mMillis = millis();
	if (mMillis > mMillisLast + 20) {
		mMillisLast = mMillis;
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
					mStrip.setBrightness(mBrightnessMin + (mDataString.substring(dataTypeEndIndex + 1, dataEndIndex).toInt() >> 1));
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

void modeLoop() {
	mStrip.clear();

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
				mPong.resetGame();
				break;

			default:
				break;
			}
		}
	}

	mStrip.show();
}
#include "Booting.h"
#include <Adafruit_NeoPixel_ZeroDMA.h> //https://learn.adafruit.com/dma-driven-neopixels/overview
#include "WifiServer.h";
#include "ClientData.h";
#include "Pong.h"

String mDataString = "";

const int8_t mStatusLedPin = 13;

const int8_t mLedStripPin = 5;
const int16_t mLedStripCount = 592;
Adafruit_NeoPixel_ZeroDMA  mStrip = Adafruit_NeoPixel_ZeroDMA(mLedStripCount, mLedStripPin, NEO_GRB);

WifiServer mWifiServer;
Pong mPong;

enum Mode {
	MODE_BOOTING,
	MODE_MAIN,
	MODE_SHOWCASE,
	MODE_PONG
};
Mode mMode = MODE_BOOTING;

enum DataType {
	DATA_UNDEFINED,
	DATA_HEART_BEAT,
	DATA_MESSAGE,
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
	mPong.setup(mStrip);

	digitalWrite(mStatusLedPin, LOW);

	mStrip.setPixelColor(0, mStrip.Color(0, 0, 0));
	mStrip.show();
}

void loop() {
	readSerial();
	modeLoop();
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

			case DATA_SET_MODE:
				break;

			case DATA_MAIN:
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
	switch (mMode) {
	case MODE_BOOTING:

		break;

	case MODE_MAIN:

		break;

	case MODE_SHOWCASE:

		break;

	case MODE_PONG:
		mPong.loop(mStrip);
		break;

	default:
		break;
	}
}
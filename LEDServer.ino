#include "WifiServer.h";
#include "ClientData.h";
#include "Pong.h"

/*
 * On first run, while debugging, set debugging to 'false' to reset and setup tcpserver. Then set debugging to 'true'.
 * When finished debugging, remember to set to false and upload.
 */
const bool mDebugging = false;
String mDataString = "";

const int8_t mLedPin = 13;

WifiServer mWifiServer;
Pong mPong;

void setup() {
	mDataString.reserve(200);

	pinMode(mLedPin, OUTPUT);
	digitalWrite(mLedPin, HIGH);

	mWifiServer.setup(mDebugging);
	mPong.setup();

	digitalWrite(mLedPin, LOW);
}

void loop() {

	//mPong.setButtonStateLast();

	int8_t clientNumber = mWifiServer.readSerial(&mDataString);
	if (clientNumber >= 0 && mDataString.length() > 0) {
		//if (SerialUSB) {
		//	SerialUSB.print("Client data received. Client number: ");
		//	SerialUSB.print(clientNumber);
		//	SerialUSB.print(", data: ");
		//	SerialUSB.println(mDataString);
		//}

		while (mDataString.length() > 0) {
			//if (SerialUSB) {
			//	SerialUSB.print("Data start: ");
			//	SerialUSB.println(mDataString);
			//}

			if (mDataString.startsWith("HB;")) {
				mWifiServer.writeToClient(clientNumber, "HB:" + String(freeMemory()));
				mDataString = mDataString.substring(3);
				//if (SerialUSB) SerialUSB.println("HB");
				continue;
			}
			else if (mDataString.startsWith("MSG:")) {
				int endIndex = mDataString.indexOf(";", 4);
				if (endIndex > 0) {
					mWifiServer.writeToClient(clientNumber, mDataString.substring(4, endIndex));
					mDataString = mDataString.substring(endIndex + 1);
					//if (SerialUSB) SerialUSB.println("MSG");
					continue;
				}
			}
			else if (mDataString.startsWith("PONG:")) {
				int endIndex = mDataString.indexOf(";", 5);
				if (endIndex > 0) {
					mPong.setButtonState(mDataString.substring(5, endIndex));
					mDataString = mDataString.substring(endIndex + 1);
					//if (SerialUSB) SerialUSB.println(dataPong);
					continue;
				}
			}

			//if (SerialUSB) {
			//	SerialUSB.print("Break on: [");
			//	SerialUSB.print(mDataString);
			//	SerialUSB.println("]");
			//}

			mDataString = "";
			break;
		}
	}
	else {
		mDataString = "";
	}

	//ClientData clientData = mWifiServer.readSerial();
	//if (!clientData.mClientNumber >= 0 && !clientData.mData.equals("")) {
	//	if (SerialUSB) {
	//		SerialUSB.print("Client data received, number: ");
	//		SerialUSB.print(clientData.mClientNumber);
	//		SerialUSB.print(", data: ");
	//		SerialUSB.println(clientData.mData);
	//	}

	//	//if (clientData.mData.startsWith("PONG:")) {
	//	//	clientData.mData = clientData.mData.substring(5);
	//	//	mPong.setButtonState(clientData);
	//	//	mPong.loop(); // TODO BB Testing..
	//	//}
	//}

	mPong.loop();
}

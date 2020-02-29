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

enum Mode {
	MODE_MAIN,
	MODE_SHOWCASE,
	MODE_PONG
};
Mode mMode = MODE_MAIN;

enum DataType {
	DATA_UNDEFINED,
	DATA_HEART_BEAT,
	DATA_MESSAGE,
	DATA_SET_MODE,
	DATA_MAIN,
	DATA_PONG
};

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
				//if (SerialUSB) SerialUSB.println("HB");
				dataTypeHandled = true;
				break;

			case DATA_MESSAGE:
				dataEndIndex = mDataString.indexOf(";", dataTypeEndIndex + 1);
				if (dataEndIndex > 0) {
					mWifiServer.writeToClient(clientNumber, mDataString.substring(dataTypeEndIndex + 1, dataEndIndex));
					mDataString = mDataString.substring(dataEndIndex + 1);
					//if (SerialUSB) SerialUSB.println("MSG");
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
					//if (SerialUSB) SerialUSB.println(dataPong);
					dataTypeHandled = true;
				}
				break;

			default:
				break;
			}
			if (dataTypeHandled) continue;

			//if (mDataString.startsWith("HB;")) {
			//	mWifiServer.writeToClient(clientNumber, "HB:" + String(freeMemory()));
			//	mDataString = mDataString.substring(3);
			//	//if (SerialUSB) SerialUSB.println("HB");
			//	continue;
			//}
			//else if (mDataString.startsWith("MSG:")) {
			//	int endIndex = mDataString.indexOf(";", 4);
			//	if (endIndex > 0) {
			//		mWifiServer.writeToClient(clientNumber, mDataString.substring(4, endIndex));
			//		mDataString = mDataString.substring(endIndex + 1);
			//		//if (SerialUSB) SerialUSB.println("MSG");
			//		continue;
			//	}
			//}
			//else if (mDataString.startsWith("PONG:")) {
			//	int endIndex = mDataString.indexOf(";", 5);
			//	if (endIndex > 0) {
			//		mPong.setButtonState(mDataString.substring(5, endIndex));
			//		mDataString = mDataString.substring(endIndex + 1);
			//		//if (SerialUSB) SerialUSB.println(dataPong);
			//		continue;
			//	}
			//}

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

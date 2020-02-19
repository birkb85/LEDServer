#include "WifiServer.h";
#include "ClientData.h";

/*
 * On first run, while debugging, set debugging to 'false' to reset and setup tcpserver. Then set debugging to 'true'.
 * When finished debugging, remember to set to false and upload.
 */
static bool mDebugging = false;

WifiServer mWifiServer;

void setup() {
	mWifiServer.init(mDebugging);
}

void loop() {
	ClientData clientData = mWifiServer.readSerial();
	if (!clientData.mClientNumber >= 0 && !clientData.mData.equals("")) {
		//    if (SerialUSB)
		//    {
		//      SerialUSB.print("Client data received, number: ");
		//      SerialUSB.print(clientData.mClientNumber);
		//      SerialUSB.print(", data: ");
		//      SerialUSB.println(clientData.mData);
		//    }
		mWifiServer.writeToClient(clientData);
	}
}

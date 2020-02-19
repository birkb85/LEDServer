#ifndef _WIFISERVER_H
#define _WIFISERVER_H

#include <Arduino.h>
#include <MemoryFree.h>;
#include "ClientData.h";

class WifiServer {
public:
	WifiServer();
	void init(bool isDebugging);
	ClientData readSerial();
	void writeToClient(ClientData clientData);

private:
	bool mDebugging;
	String mInputString;
	void initSerial();
	void initWIFIServer();
	void testReadWriteSerial();
	void printFreeMemory();
};

#endif // _WIFISERVER_H

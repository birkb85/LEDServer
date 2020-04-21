// WifiServer.h

#ifndef _WIFISERVER_h
#define _WIFISERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <MemoryFree.h>;

class WifiServer {
public:
	WifiServer();
	void setup();
	int8_t readSerial(String& dataString);
	void writeToClient(int8_t clientNumber, String& data);

private:
	const bool mDebugging = false;
	String mInputString = "";
	void initSerial();
	void initWIFIServer();
	void testReadWriteSerial();
	void printFreeMemory();
};

#endif

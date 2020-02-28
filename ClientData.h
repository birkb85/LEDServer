// ClientData.h

#ifndef _CLIENTDATA_h
#define _CLIENTDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class ClientData {
public:
	ClientData();
	int8_t mClientNumber = -1;
	String mData = "";

private:
};

#endif

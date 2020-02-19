#ifndef _CLIENTDATA_H
#define _CLIENTDATA_H

#include <Arduino.h>

class ClientData {
public:
	ClientData();
	int mClientNumber = -1;
	String mData;

private:
};

#endif // _CLIENTDATA_H

#include "WifiServer.h";

WifiServer::WifiServer() {
	mInputString.reserve(200);
}

void WifiServer::setup() {
	initSerial();
	initWIFIServer();
}

void WifiServer::initSerial() {
	if (mDebugging) {
		SerialUSB.begin(115200); // Communication with the host computer.
		while (!SerialUSB) { ; } // Not required. But have to wait while connected to PC.
	}

	Serial5.begin(115200); // Communication with ESP8266.
	while (!Serial5) { ; }
}

void WifiServer::initWIFIServer() {
	//  Kommando: AT+RST // Restart og vis firmware.
	//
	//  Sættes én gang:
	//  Kommando: AT+CWSAP="LED","12893467",3,3 // Set SSID, password, channel and encryption (3 = WPA2-PSK).
	//  Kommando: AT+CWMODE=3 // Set WiFi mode. SoftAP+station  mode.
	//
	//  Sættes hver gang enhed starter:
	//  Kommando: AT+CIPMUX=1 // Enable multiple connections. 
	//  Kommando: AT+CIPSERVER=1 // Create a TCP server. Default ip = 192.168.4.1, default port = 333.

	Serial5.println("AT+RST");
	while (!Serial5.available()) {}
	while (Serial5.available()) {
		char inChar = (char)Serial5.read();
		//    inputString += inChar;
		if (SerialUSB) SerialUSB.write(inChar);
		//delay(10);
	}

	//delay(100);

	Serial5.println("AT+CIPMUX=1");
	while (!Serial5.available()) {}
	while (Serial5.available()) {
		char inChar = (char)Serial5.read();
		//    inputString += inChar;
		if (SerialUSB) SerialUSB.write(inChar);
		//delay(10);
	}

	//delay(100);

	Serial5.println("AT+CIPSERVER=1");
	while (!Serial5.available()) {}
	while (Serial5.available()) {
		char inChar = (char)Serial5.read();
		//    inputString += inChar;
		if (SerialUSB) SerialUSB.write(inChar);
		//delay(10);
	}

	//delay(100);
}

void WifiServer::testReadWriteSerial() {
	// listen for user input and send it to the ESP8266
	while (SerialUSB.available()) {
		Serial5.write(SerialUSB.read());
	}

	// listen for communication from the ESP8266 and then write it to the serial monitor
	while (Serial5.available()) {
		SerialUSB.write(Serial5.read());
	}
}

int8_t WifiServer::readSerial(String& dataString) {
	while (Serial5.available()) {
		char inChar = (char)Serial5.read();
		mInputString += inChar;
	}

	if (mInputString.length() > 0) {
		// Strengen starter med "+" når serveren modtager en besked fra en klient.
		int plusIndex = mInputString.indexOf("+");
		if (plusIndex == -1) {
			mInputString = "";
			return -1;
		}
		else if (plusIndex > 0) {
			mInputString = mInputString.substring(plusIndex);
		}

		int messageIndex = mInputString.indexOf("+IPD,");
		if (messageIndex == -1) return -1;

		int playerIndex = mInputString.indexOf(",", messageIndex + 5);
		if (playerIndex == -1) return -1;

		int dataIndex = mInputString.indexOf(":", playerIndex + 1);
		if (dataIndex == -1) return -1;

		int dataLength = mInputString.substring(playerIndex + 1, dataIndex).toInt();
		if (dataLength == 0) return -1;
		if (mInputString.length() < dataIndex + 1 + dataLength) return -1;

		int8_t clientNumber = mInputString.substring(messageIndex + 5, playerIndex).toInt();
		dataString += mInputString.substring(dataIndex + 1, dataIndex + 1 + dataLength);

		mInputString = mInputString.substring(dataIndex + 1 + dataLength);

		return clientNumber;
	}

	return -1;
}

// Print out free memory for debugging.
void WifiServer::printFreeMemory() {
	if (SerialUSB) {
		SerialUSB.print("Free memory: ");
		SerialUSB.println(freeMemory());
	}
}

// Write message to client.
void WifiServer::writeToClient(int8_t clientNumber, String& data) {
	//  SerialUSB.print("writeToClient clientNumber: ");
	//  SerialUSB.print(clientNumber);
	//  SerialUSB.print(", data: [");
	//  SerialUSB.print(data);
	//  SerialUSB.print("], length: ");
	//  SerialUSB.print(data.length());
	//  SerialUSB.println();

	Serial5.print("AT+CIPSEND=");
	Serial5.print(clientNumber); // Client number
	Serial5.print(",");
	Serial5.println(data.length() + 2); // Data length + 2 bytes (CR and LF).
	delay(2); // 2 is minimum. Maybe larger is better...
	Serial5.println(data); // Data to be sent.
}

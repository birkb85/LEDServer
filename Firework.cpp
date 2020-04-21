#include "Firework.h"

Firework::Firework() {
}

void Firework::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	
}

void Firework::handleData(String& data) {
	int dataTypeIndex = data.indexOf(":");
	if (dataTypeIndex > 0) {
		//DataTypeFirework dataTypeFirework = (DataTypeFirework)data.substring(0, dataTypeIndex).toInt();
		//switch (dataTypeFirework) {
		//default:
		//	break;
		//}
	}
}

void Firework::loop(Adafruit_NeoPixel_ZeroDMA& strip) {

}

void Firework::draw(Adafruit_NeoPixel_ZeroDMA& strip) {

}
#include "Fireworks.h"

Fireworks::Fireworks() {
}

void Fireworks::setup(Adafruit_NeoPixel_ZeroDMA& strip) {
	
}

void Fireworks::handleData(String& data) {
	int dataTypeIndex = data.indexOf(":");
	if (dataTypeIndex > 0) {
		DataTypeFireworks dataTypeFireworks = DATA_FIREWORKS_UNDEFINED;
		dataTypeFireworks = (DataTypeFireworks)data.substring(0, dataTypeIndex).toInt();

		switch (dataTypeFireworks) {
		case DATA_FIREWORKS_UNDEFINED:
			break;

		default:
			break;
		}
	}
}

void Fireworks::loop(Adafruit_NeoPixel_ZeroDMA& strip) {

}

void Fireworks::draw(Adafruit_NeoPixel_ZeroDMA& strip) {

}
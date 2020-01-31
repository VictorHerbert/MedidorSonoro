#ifndef Thermistor_h
#define Thermistor_h

#include "Arduino.h"
#include "math.h"

class Thermistor {
	public:
		Thermistor(int pin, float A = 0.001129148, float B = 0.000234125, float C = 0.0000000876741);
		double getTemp();
	private:
    float A,B,C;
		int _pin;
};

#endif

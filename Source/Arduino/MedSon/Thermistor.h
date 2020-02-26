#ifndef Thermistor_h
#define Thermistor_h

#include "Arduino.h"
#include "math.h"

class Thermistor {
	public:
		Thermistor(int pin, float _A, float _B, float _C);
		double getTemp();
	private:
    float A,B,C;
		int _pin;
};

#endif

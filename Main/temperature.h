#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "Arduino.h"
class TemperatureSensor
{
private:    
  int temperature_pin;
  int byte[20];
  int getBinaryBit() const;
  int calcTemp() const;

public:
  TemperatureSensor(int);
  int getTemp();
};

#endif
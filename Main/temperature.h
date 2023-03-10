#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "Arduino.h"
class TemperatureSensor
{
private:    
  int temperature_pin;
  uint16_t byte[20];
  uint16_t getBinaryBit() const;
  int16_t calcTemp() const;

public:
  TemperatureSensor(int);
  int16_t getTemp();
};

#endif
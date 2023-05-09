#ifndef PRESSURE_H
#define PRESSURE_H
#include "Arduino.h"

#include <SPI.h>
class PressureSensor
{
private:
  int pressure_SS_pin;

  uint32_t press_counts = 0;     // digital pressure reading [counts]
  // double temp_counts = 0;      // digital temperature reading [counts]
  double pressure = 0;         // pressure reading [bar, psi, kPa, etc.]
  // double temperature = 0;      // temperature reading in deg C

  uint32_t outputmax = 15099494; // output at maximum pressure [counts]
  uint32_t outputmin = 1677722;  // output at minimum pressure [counts]
  uint8_t pmax = 15;            // maximum value of pressure range [bar, psi, kPa, etc.]
  uint8_t pmin = 0;             // minimum value of pressure range [bar, psi, kPa, etc.]

  double percentage = 0;       // holds percentage of full scale data

  char pBuff[20];

public:
  PressureSensor(int);
  void setup();
  uint16_t getPressure();
};

#endif
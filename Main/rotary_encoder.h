#ifndef R_ENCODER_H
#define R_ENCODER_H
#include "Arduino.h"

#include <AS5600.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#define SYS_VOL 3.3
#else
#define SERIAL Serial
#define SYS_VOL 5
#endif

class RotaryEncoder
{
private:
  AMS_5600 ams5600;

  float initial_angle;
  float angle_difference;

  uint8_t arm_length = 17;

  float convertRawAngleToDegrees(word);

public:
  void setup();
  void getAngleDifference();
  uint16_t getHeight();
};

#endif
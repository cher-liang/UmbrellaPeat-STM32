#include "rotary_encoder.h"

#define PI 3.14159265

float RotaryEncoder::convertRawAngleToDegrees(word newAngle) // returns angle at x100 (2 decimal precision)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  return newAngle * 0.087890625;
}

void RotaryEncoder::setup(float initial_angle)
{
  this->initial_angle=initial_angle;

  // Serial.printf("Initial angle: %d\r\n", (int) this->initial_angle);
}

float RotaryEncoder::calibrate()
{
  initial_angle = convertRawAngleToDegrees(ams5600.getRawAngle());

  Serial.printf("Initial angle: %d\r\n", (int) initial_angle);

  return initial_angle;
}

void RotaryEncoder::getAngleDifference()
{
  float current_angle = convertRawAngleToDegrees(ams5600.getRawAngle());
  angle_difference = 180 - abs(abs(initial_angle - current_angle) - 180);

  Serial.printf("Current Angle: %.2f\r\nAngle Difference: %.2f\r\n", current_angle, angle_difference);
}

uint16_t RotaryEncoder::getHeight() // return in mm
{
  if (angle_difference < 90)
  {
    return arm_length * cos(angle_difference * PI / 180.0) * 10;
  }else{
    Serial.println("Angle exceed 90 degrees!");
  }
}
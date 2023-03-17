#include "rotary_encoder.h"

#define PI 3.14159265

float RotaryEncoder::convertRawAngleToDegrees(word newAngle) // returns angle at x100 (2 decimal precision)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  return newAngle * 0.087890625;
}

void RotaryEncoder::setup()
{
  initial_angle = convertRawAngleToDegrees(ams5600.getRawAngle());

  Serial.printf("Initial Angle Set at %.2f\r\n", initial_angle);
}

void RotaryEncoder::getAngleDifference()
{
  float current_angle = convertRawAngleToDegrees(ams5600.getRawAngle());
  angle_difference = 180 - abs(abs(initial_angle - current_angle) - 180);

  Serial.printf("Current Angle: %.2f\r\nAngle Difference: %.2f\r\n", current_angle, angle_difference);
}

uint16_t RotaryEncoder::getHeight()
{
  if (angle_difference < 90)
  {
    return arm_length * cos(angle_difference * PI / 180.0) * 100;
  }else{
    Serial.println("Angle exceed 90 degrees!");
  }
}
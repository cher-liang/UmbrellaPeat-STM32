#include "temperature.h"

TemperatureSensor::TemperatureSensor(int temperature_pin)
{
  this->temperature_pin=temperature_pin;
  pinMode(temperature_pin, INPUT);
}
uint16_t TemperatureSensor::getBinaryBit() const
{
  uint16_t binary;
  unsigned long duration = pulseIn(temperature_pin, LOW); // Measure duration of the time low on data_pin

  if (duration < 40) // If time low < 40us, logic 1
  {
    binary = 1;
  }

  else if (duration > 40 && duration < 60) // If time low between 40 and 60us
  {
    binary = 2;
  } // Start bit

  else if (duration > 60) // If time low > 60us, logic 0
  {
    binary = 0;
  }

  return binary;
}

int16_t TemperatureSensor::calcTemp() const
{
  int rawTemp;
  int16_t temp;

  rawTemp = byte[6] * 1024 + byte[7] * 512 +
            byte[8] * 256 + byte[11] * 128 +
            byte[12] * 64 + byte[13] * 32 +
            byte[14] * 16 + byte[15] * 8 +
            byte[16] * 4 + byte[17] * 2 + byte[18] * 1;

  temp = ((rawTemp * 0.09770396 - 50)*100); // Calculate actual temperature in deg C * 100
                                    // 0.09770396 is 200/2047

  return temp;
}

int16_t TemperatureSensor::getTemp()
{
  for (int i=0 ; i<20 ; i++)
  { byte[i] = getBinaryBit();  }     // Store each bit in the array "byte[]"

  if (byte[0] == 2)
  {
    return calcTemp();
  }
}
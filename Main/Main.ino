#include <RH_ASK.h>
#include "temperature.h"
// #include "Temperature.ino"

#define RF_TRANSMIT_PIN PA9
#define TEMPERATURE_PIN 2

RH_ASK rf_driver(2000, PA10, RF_TRANSMIT_PIN); // STM32 G431KB
TemperatureSensor temp_sensor(TEMPERATURE_PIN);

void setup()
{
  rf_driver.init();
}

void loop()
{
  // put your main code here, to run repeatedly:

  int temp;
  char transmit_str[20];
  temp = temp_sensor.getTemp();
  if (temp){
    sprintf(transmit_str, "Temp = %d \x2E %d", (temp/100),(temp%100));
  }
  else{
    sprintf(transmit_str, "Start bit not detected");
  }
  rf_driver.send((uint8_t *)transmit_str, strlen(transmit_str));
  rf_driver.waitPacketSent();

  delay(200); // Wait for 50ms until the next measurement
}

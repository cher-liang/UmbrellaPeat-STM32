#include <Wire.h>

#include "STM32LowPower.h"
#include <FlashStorage_STM32.h>

#include <RH_ASK.h>
#include <AS5600.h>

#include "flash.h"
#include "temperature.h"
#include "pressure.h"
#include "rotary_encoder.h"

#define RF_TRANSMIT_PIN PA9
#define TEMPERATURE_PIN 2
#define PRESSURE_SS_PIN 10
#define PUSH_BUTTON_1 PC13

RH_ASK rf_driver(2000, PA10, RF_TRANSMIT_PIN); // STM32 G431KB
FlashStorage flash_storage;
TemperatureSensor temp_sensor(TEMPERATURE_PIN);
PressureSensor pressure_sensor(PRESSURE_SS_PIN);
RotaryEncoder rotary_encoder;

unsigned long start_time;

void setup()
{
  start_time = millis();

  pinMode(PUSH_BUTTON_1, INPUT);

  Serial.begin(9600);
  // LowPower.begin();
  // Wire.begin();

  delay(200);
  if (!digitalRead(PUSH_BUTTON_1))
  {
    reset();
  }
  else
  {
    rotary_encoder.setup(flash_storage.getRotaryInitialAngle());
  }

  pressure_sensor.setup();

  uint16_t count = flash_storage.setup();

  uint16_t elevation = rotary_encoder.getHeight();
  int16_t temperature = temp_sensor.getTemp();
  uint16_t pressure = pressure_sensor.getPressure();

  PeatData pData = {
      elevation,
      temperature,
      pressure};

  flash_storage.writeFlashData(pData);

  if (count + 1 % 336 == 0)
  {
    rf_driver.init();

    PeatData temporary;
    char rf_transmit_str[60];

    for (int i = 0; i < 336; i++)
    {
      temporary = flash_storage.getFlashData();

      char dataBuf[20];
      sprintf(dataBuf,
              "%d,%d,%d,%d;",
              i,
              temporary.peat_height,
              temporary.temperature,
              temporary.pressure);

      strcat(rf_transmit_str, dataBuf);
      if (i % 3 == 0)
      {
        rf_driver.send((uint8_t *)rf_transmit_str, strlen(rf_transmit_str));
        rf_driver.waitPacketSent();

        memset(rf_transmit_str, '\0', sizeof(char) * 60);
      }
    }

    flash_storage.clearFlash();

  }

  LowPower.shutdown(5000 - (millis() - start_time));
}

void loop()
{
  // put your main code here, to run repeatedly:

  delay(1000);
  // pressure_sensor.getPressure();

  // uint16_t elevation = rotary_encoder.getHeight();
  // Serial.printf("%d mm\r\n",elevation);

  // int16_t temp;
  // char transmit_str[20];
  // temp = temp_sensor.getTemp();
  // if (temp){
  //   sprintf(transmit_str, "Temp = %d.%02d", (temp/100),(temp%100));
  // }
  // else{
  //   sprintf(transmit_str, "Start bit not detected");
  // }
  // rf_driver.send((uint8_t *)transmit_str, strlen(transmit_str));
  // rf_driver.waitPacketSent();
}

void reset()
{
  Serial.println("Resetting");
  flash_storage.clearFlash();

  // // Commented out temporarily
  // Serial.println("Wait 5 seconds, setup initial angle, make sure device is at 90 degrees");
  // delay(5000);
  // float initial_angle = rotary_encoder.calibrate();
  // flash_storage.writeRotaryInitialAngle(initial_angle);
}
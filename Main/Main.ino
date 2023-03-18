#include <Wire.h>

#include "STM32LowPower.h"
#include <FlashStorage_STM32.h>

#include <RH_ASK.h>
#include <AS5600.h>

#include "temperature.h"
#include "flash.h"
#include "rotary_encoder.h"

#define RF_TRANSMIT_PIN PA9
#define TEMPERATURE_PIN 2
#define PUSH_BUTTON_1 PC13

RH_ASK rf_driver(2000, PA10, RF_TRANSMIT_PIN);  // STM32 G431KB
TemperatureSensor temp_sensor(TEMPERATURE_PIN);
FlashStorage flash_storage;
RotaryEncoder rotary_encoder;

int pushButtonState = 0;

void setup() {
  pinMode(PUSH_BUTTON_1, INPUT);

  Serial.begin(9600);
  // Wire.begin();

  delay(200);
  if (!digitalRead(PUSH_BUTTON_1)) {
    reset();
  } else {
    rotary_encoder.setup(flash_storage.getRotaryInitialAngle());
  }

  PeatData temp;

  flash_storage.setup();


  // rf_driver.init();


  // FlashStorage Testing
  PeatData pData = {
    150,
    -150,
    1023
  };
  flash_storage.writeFlashData(pData);

  // LowPower.deepSleep(1000);

  // pData.peat_height = 148;
  // pData.temperature = -90;
  // flash_storage.writeFlashData(pData);

  // LowPower.deepSleep(1000);

  // readData = flash_storage.getFlashData();
  // Serial.println(readData.peat_height);
  // Serial.println(readData.temperature);
  // Serial.println(readData.pressure);
}

void loop() {
  // put your main code here, to run repeatedly:

  // uint16_t elevation = rotary_encoder.getHeight();
  // Serial.printf("%d.%d\r\n",elevation/100,elevation%100);
  // int buttonState=digitalRead(PUSH_BUTTON_1);
  // Serial.print
  // val = digitalRead(PUSH_BUTTON_1);
  // Serial.printf("Button state: %d\r\n", val);
  delay(1000);

  // // FlashStorage Testing

  // PeatData readData = flash_storage.getFlashData();
  // Serial.println(readData.peat_height);
  // Serial.println(readData.temperature);
  // Serial.println(readData.pressure);

  // delay(1000);

  // int16_t temp;
  // char transmit_str[20];
  // temp = temp_sensor.getTemp();
  // if (temp){
  //   sprintf(transmit_str, "Temp = %d \x2E %d", (temp/100),(temp%100));
  // }
  // else{
  //   sprintf(transmit_str, "Start bit not detected");
  // }
  // rf_driver.send((uint8_t *)transmit_str, strlen(transmit_str));
  // rf_driver.waitPacketSent();

  // Serial.println("Entering Sleep mode");
  // delay(100);

  // LowPower.shutdown(10000);
  // // LowPower.deepSleep(360000);

  // Serial.println("Exit Sleep mode");

  // delay(10000);
}

void reset() {
  Serial.println("Resetting");
  flash_storage.clearFlash();


  // // Commented out temporarily
  // Serial.println("Wait 5 seconds, setup initial angle, make sure device is at 90 degrees");
  // delay(5000);
  // float initial_angle = rotary_encoder.calibrate();
  // flash_storage.writeRotaryInitialAngle(initial_angle);
}
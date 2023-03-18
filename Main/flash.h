#ifndef FLASH_H
#define FLASH_H
#include "Arduino.h"

#include "FlashStorage_STM32.hpp"
struct PeatData
{
  int16_t peat_height;
  int16_t temperature;
  uint16_t pressure;
};

class FlashStorage
{
private:
  uint16_t eeAddress = 0;
  uint16_t eeReadAddress = 292; //temp

  uint16_t eeAddressMax;

public:
  FlashStorage();
  uint16_t setup();
  void getFlashStorageInfo();
  void writeRotaryInitialAngle(float);
  float getRotaryInitialAngle();
  void writeFlashData(PeatData);
  PeatData getFlashData();
  void clearFlash();
};  

#endif
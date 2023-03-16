#ifndef FLASH_H
#define FLASH_H
#include "Arduino.h"

#include "FlashStorage_STM32.hpp"
struct PeatData{
  int16_t peat_height;
  int16_t temperature;
  uint16_t pressure; 
};

class FlashStorage
{
private:    
  uint16_t eeAddress=0;
  uint16_t eeReadAddress = 0;
  
  uint16_t eeAddressMax;

public:
  FlashStorage(int);
  // void writeFlashTimestamp(uint32_t);
  void writeFlashData(PeatData);
  void clearFlash();
  // uint32_t getFlashInit();
  PeatData getFlashData();
  // uint32_t getFlashEnd();

};

#endif
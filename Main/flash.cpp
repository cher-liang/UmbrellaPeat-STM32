#include "flash.h"

FlashStorage::FlashStorage() {
  eeAddressMax = EEPROM.length();
}

void FlashStorage::setup() {
  PeatData temp;
  for (int i = 4; i < EEPROM.length(); i += 6) {
    EEPROM.get(i, temp);

    if (temp.peat_height == 0xffffffff && temp.temperature == 0xffffffff && temp.pressure == 0xffff) {  // Check for empty PeatData
      eeAddress = i;
      break;
    }
  }
  Serial.print("eeAddress: ");
  Serial.println(eeAddress);
}

void FlashStorage::getFlashStorageInfo() {
  Serial.print(F("\nStart EEPROM_put on "));
  Serial.println(BOARD_NAME);
  Serial.println(FLASH_STORAGE_STM32_VERSION);

  Serial.print("EEPROM length: ");
  Serial.println(EEPROM.length());
}

void FlashStorage::writeRotaryInitialAngle(float initial_angle) {
  EEPROM.put(0, initial_angle);
  EEPROM.commit();
}

float FlashStorage::getRotaryInitialAngle() {
  float initial_angle;
  EEPROM.get(0, initial_angle);

  if (initial_angle != initial_angle) {  // when float is NaN
    Serial.println("Rotary encoder initial angle not set!");
    Serial.println("Hold Button B1 (Blue) then press Reset Button (Black)");
    return 0;
  } else {
    return initial_angle;
  }
}

void FlashStorage::writeFlashData(PeatData pData) {  // Write PeatData to flash storage and increment the eeAddress
  Serial.print("Write data at ");
  Serial.println(eeAddress);

  if (eeAddress < (eeAddressMax - sizeof(PeatData))) {
    EEPROM.put(eeAddress, pData);
    eeAddress += sizeof(PeatData);

    EEPROM.commit();
  }
}

PeatData FlashStorage::getFlashData() {  // Read PeatData from flash storage and increment eeReadAddress
  Serial.print("Read data at ");
  Serial.println(eeReadAddress);

  PeatData pData;
  EEPROM.get(eeReadAddress, pData);
  eeReadAddress += sizeof(PeatData);

  return pData;
}

void FlashStorage::clearFlash() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, -1);
  }

  EEPROM.commit();
}
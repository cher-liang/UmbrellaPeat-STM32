#include "flash.h"

FlashStorage::FlashStorage(int _) {
  // Serial.print(F("\nStart EEPROM_put on "));
  // Serial.println(BOARD_NAME);
  // Serial.println(FLASH_STORAGE_STM32_VERSION);

  // Serial.print("EEPROM length: ");
  // Serial.println(EEPROM.length());

  eeAddressMax = EEPROM.length();
}

// void FlashStorage::writeFlashTimestamp(uint32_t start_timestamp)
// {
//     EEPROM.put(eeAddress, start_timestamp);
//     eeAddress += sizeof(uint32_t);

//     EEPROM.commit();
// }

void FlashStorage::writeFlashData(PeatData pData) { // Write PeatData to flash storage and increment the eeAddress
  Serial.print("Write data at ");
  Serial.println(eeAddress);

  if (eeAddress < (eeAddressMax - sizeof(PeatData))) {
    EEPROM.put(eeAddress, pData);
    eeAddress += sizeof(PeatData);

    EEPROM.commit();
  }
}

// uint32_t FlashStorage::getFlashInit()
// {
//     uint32_t start_timestamp = 0;
//     EEPROM.get(0, start_timestamp);

//     return start_timestamp;
// }

PeatData FlashStorage::getFlashData() { // Read PeatData from flash storage and increment eeReadAddress
  Serial.print("Read data at ");
  Serial.println(eeReadAddress);

  if (eeReadAddress < eeAddress) {
    PeatData pData;
    EEPROM.get(eeReadAddress, pData);

    eeReadAddress += sizeof(PeatData);

    return pData;
  } else {
    Serial.println("Read address error");
    Serial.print("eeAddress = ");
    Serial.println(eeAddress);
    return {};
  }
}

// PeatData FlashStorage::getFlashEnd()
// {
//     uint32_t end_timestamp = 0;
//     EEPROM.get(eeReadAddress, end_timestamp);

//     return end_timestamp;
// }
void FlashStorage::clearFlash() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }

  EEPROM.commit();
}
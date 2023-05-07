#include "pressure.h"

PressureSensor::PressureSensor(int pressure_SS_pin)
{
    this->pressure_SS_pin = pressure_SS_pin;
    pinMode(pressure_SS_pin, OUTPUT);
}
void PressureSensor::setup()
{
    SPI.begin();
    digitalWrite(pressure_SS_pin, HIGH); // set SS High
}

uint16_t PressureSensor::getPressure() // return in PSI x100
{
    uint8_t data[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // holds output data
    uint8_t cmd[3] = {0xAA, 0x00, 0x00};                          // command to be sent

    SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0)); // SPI at 200kHz
    digitalWrite(pressure_SS_pin, LOW);                             // set SS Low

    SPI.transfer(cmd, 3);                // send Read Command
    digitalWrite(pressure_SS_pin, HIGH); // set SS High
    delay(10);                           // wait for conversion
    digitalWrite(pressure_SS_pin, LOW);

    SPI.transfer(data, 7);
    digitalWrite(pressure_SS_pin, HIGH);
    SPI.endTransaction();

    press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
    percentage = (press_counts / 16777215) * 100;             // calculate pressure as percentage of full scale
    
    // calculation of pressure value according to equation 2 of datasheet
    pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
    
     // calculate pressure in kilopascals
    return (uint16_t)pressure* 6894.757 * 100000;
}

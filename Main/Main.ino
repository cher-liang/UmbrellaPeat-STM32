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
// #define VOLTAGE_REGULATOR_PIN 10
#define TEMPERATURE_PIN 2
#define PRESSURE_SS_PIN 8
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
  // pinMode(VOLTAGE_REGULATOR_PIN, OUTPUT);

  // digitalWrite(VOLTAGE_REGULATOR_PIN, LOW);

  Serial.begin(9600);
  LowPower.begin();
  
  Wire.begin();

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

  Serial.printf("Measurements taken: %d\r\n", count);

  measure_peat();

  if ((count + 1) >= 336)
  {
    transmit_data();
  }
  // digitalWrite(VOLTAGE_REGULATOR_PIN, HIGH);

  
}

void loop()
{
  // put your main code here, to run repeatedly:
  LowPower.shutdown((60000 - (millis() - start_time)));
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
  Serial.println("Wait 5 seconds, setup initial angle, make sure device is at 90 degrees");
  delay(5000);
  float initial_angle = rotary_encoder.calibrate();
  flash_storage.writeRotaryInitialAngle(initial_angle);
}

void measure_peat() // take measurement and save to flash
{
  uint16_t elevation = rotary_encoder.getHeight();
  Serial.printf("Peat Height: %dmm\r\n",elevation);
  
  int16_t temperature = temp_sensor.getTemp();
  Serial.printf("Temperature: %d.%02d\r\n", temperature / 100, temperature % 100);
  uint16_t pressure = pressure_sensor.getPressure();
  Serial.printf("Pressure: %d.%02dkPa\r\n",pressure / 100, pressure % 100);

  PeatData pData = {
      elevation,
      temperature,
      pressure};

  flash_storage.writeFlashData(pData);

  // rf_driver.init();
  // char dataBuf[20];
  // sprintf(dataBuf,
  //         "%d,%d,%d;",
  //         elevation,
  //         temperature,
  //         pressure);
  // rf_driver.send((uint8_t *)dataBuf, strlen(dataBuf));
  // rf_driver.waitPacketSent();
}

void transmit_data() // read flash and transmit data
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

extern "C" void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV16;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}
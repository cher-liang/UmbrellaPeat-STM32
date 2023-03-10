#include <Wire.h>
#include <AS5600.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL   3.3
#else
  #define SERIAL Serial
  #define SYS_VOL   5
#endif

AMS_5600 ams5600;

int ang, lang = 0;
#define DATA_PIN D2

int createBinaryBit()
{
  int binary;
  unsigned long duration = pulseIn(DATA_PIN,LOW);     // Measure duration of the time low on data_pin
  
    if (duration < 40)                                // If time low < 40us, logic 1
    { binary = 1; }
    
    else if (duration > 40 && duration < 60)          // If time low between 40 and 60us
    { binary = 2; }    // Start bit
    
    else if (duration > 60)                           // If time low > 60us, logic 0
    { binary = 0; }
    
    return binary;
}

double calcTemp(int byte[])
{
  int rawTemp;
  double temp;

  rawTemp = byte[6]*1024 + byte[7]*512 + 
            byte[8]*256 + byte[11]*128 + 
            byte[12]*64 + byte[13]*32 + 
            byte[14]*16 + byte[15]*8 + 
            byte[16]*4 + byte[17]*2 + byte[18]*1;

  temp = rawTemp*0.09770396 - 50;               // Calculate actual temperature in deg C
                                                // 0.09770396 is 200/2047  
  
  return temp;
}
void setup()
{
  SERIAL.begin(9600);
    pinMode(DATA_PIN, INPUT);
  Wire.begin();
  SERIAL.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
  if(ams5600.detectMagnet() == 0 ){
    while(1){
        if(ams5600.detectMagnet() == 1 ){
            SERIAL.print("Current Magnitude: ");
            SERIAL.println(ams5600.getMagnitude());
            break;
        }
        else{
            SERIAL.println("Can not detect magnet");
        }
        delay(1000);
    }
  }
}
/*******************************************************
/* Function: convertRawAngleToDegrees
/* In: angle data from AMS_5600::getRawAngle
/* Out: human readable degrees as float
/* Description: takes the raw angle and calculates
/* float value in degrees.
/*******************************************************/
float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  float retVal = newAngle * 0.087890625;
  return retVal;
}
void loop()
{
delay(2000);
  Serial.print("Angle = ");
    SERIAL.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));
    int byte[20];                         // Array to store binary number
  double temp;

  for (int i=0 ; i<20 ; i++)
  { byte[i] = createBinaryBit();  }     // Store each bit in the array "byte[]"
  
  temp = calcTemp(byte);

  /* To display on Serial Monitor: */
  Serial.print("Temperature = ");
  Serial.println(temp,1);               // Display Temp in Serial Monitor to 1 dp

                   
}

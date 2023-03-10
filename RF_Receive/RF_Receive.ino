
#include <RH_ASK.h>    
RH_ASK rf(2000, PA10, PA9); // rf as object for RG_ASK

String str_receive;  

void setup()

{
  Serial.begin(9600);
  rf.init(); // Initialize rf Object
}

void loop()

{

  uint8_t buf[20];
  uint8_t buflen = sizeof(buf);

  if (rf.recv(buf, &buflen))
  {

    str_receive = String((char *)buf); // Receive String from the Transmitter
    Serial.println(str_receive);

    delay(1500);
  }
}
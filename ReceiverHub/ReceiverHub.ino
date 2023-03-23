#include <RH_ASK.h>
#include <string.h>
#include <SPI.h>
#include <SD.h>

RH_ASK rf(2000, PA10, PA9); // rf as object for RH_ASK

char *token;
int i = 0;
int j = 0;
int measurementCount[3];
int peatHeight[3];
int temperature[3];
int pressure[3];

File peatDataFile;

void setup()
{
    Serial.begin(9600);
    rf.init(); // Initialize rf Object

    pinMode(10, OUTPUT);

    Serial.print("Initializing SD card...");

    if (!SD.begin(10))
    {
        Serial.println("initialization failed!");
        while (1)
            ;
    }
    Serial.println("initialization done.");
}
void loop()
{

    uint8_t buf[60];
    uint8_t buflen = sizeof(buf);

    if (rf.recv(buf, &buflen))
    {
        // Serial.println((char *)buf);
        processRfData((char *)buf);
        appendToFileSD();

        delay(50);
    }
}

void processRfData(char *str)
{
    token = strtok(str, ";,\n");
    while (token != NULL)
    {
        if (i % 4 == 0)
        {
            measurementCount[j] = atoi(token);
        }
        else if (i % 4 == 1)
        {
            peatHeight[j] = atoi(token);
        }
        else if (i % 4 == 2)
        {
            temperature[j] = atoi(token);
        }
        else if (i % 4 == 3)
        {
            pressure[j] = atoi(token);
        }
        i++;
        if (i % 4 == 0)
        {
            j++;
        }
        token = strtok(NULL, ";,\n");
    }
    i = 0;
    j = 0;

    Serial.printf("measurementCount: {%d,%d,%d}\r\n", measurementCount[0], measurementCount[1], measurementCount[2]);
    Serial.printf("peatHeight: {%d,%d,%d}\r\n", peatHeight[0], peatHeight[1], peatHeight[2]);
    Serial.printf("temperature: {%d,%d,%d}\r\n", temperature[0], temperature[1], temperature[2]);
    Serial.printf("pressure: {%d,%d,%d}\r\n", pressure[0], pressure[1], pressure[2]);
}

void appendToFileSD() 
{
    peatDataFile = SD.open("peatData1.csv", FILE_WRITE);
    char line[20];

    // if the file opened okay, write to it:
    if (peatDataFile)
    {
        // Serial.print("Writing to peatData1.csv...");
        for (int i = 0; i < 3; i++)
        {
            sprintf(line, "%d, %d, %d.%02d, %d.%02d",
                    measurementCount[i],
                    peatHeight[i],
                    temperature[i] / 100, temperature[i] % 100,
                    pressure[i] / 100, pressure[i] % 100);
            peatDataFile.println(line);
        }
        // close the file:
        peatDataFile.close();
        // Serial.println("done.");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening peatData1.csv");
    }
}
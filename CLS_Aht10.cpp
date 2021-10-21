#include <Arduino.h>
#include <Wire.h>
//#include <Adafruit_AHT10.h>
#include "CLS_Aht10.h"

//#define DEBUG_AHT10

Adafruit_AHT10 aht;
extern bool athReady;

CLS_Aht10::CLS_Aht10()
{
}

void CLS_Aht10::init()
{
    if (!aht.begin())
    {
        Serial.println("Could not find AHT? Check wiring");
        while (1)
            delay(10);
    }
    Serial.println("AHT10 or AHT20 found");
}

void CLS_Aht10::doRead()
{
    unsigned long time = millis();
    if (lastReadTime > time)
        lastReadTime = 0;
    if (time - lastReadTime >= 1000)
    {
        lastReadTime = time;
        readAht10();
    }
}

void CLS_Aht10::readAht10()
{
    aht.getEvent(&humidity, &temperature); // populate temp and humidity objects with fresh data
#ifdef DEBUG_AHT10
    Serial.print("Temperature: ");
    Serial.print(temperature.temperature);
    Serial.println(" degrees C");
    Serial.print("Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
#endif
}

String CLS_Aht10::toTextPlain()
{
    String value = "Cannot read AHT-10\n";
    if (athReady)
    {
        readAht10();
        value = "Temperature (C)=";
        value += String(temperature.temperature);
        value += "\n";
        value += "Relative humidity (%)=";
        value += String(humidity.relative_humidity);
        value += "\n";
    }
    return value;
}

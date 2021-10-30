#include <Arduino.h>
#include <Wire.h>
//#include <Adafruit_AHT10.h>
#include "CLS_I2C_Scan.h"
#include "CLS_Aht10.h"

//#define DEBUG_AHT10

Adafruit_AHT10 aht;
extern bool athReady;
extern CLS_I2C_Scan i2cScan;

extern void trace(String trace);

CLS_Aht10::CLS_Aht10()
{
}

void CLS_Aht10::init()
{
    trace("ath-10 initialization");
    trace("I2C ath-10 presence test");
    athReady = isI2CReadable();
    trace("I2C ath-10 ready = " + String(athReady));
    if (athReady)
    {
        Serial.println("AHT10 or AHT20 found");
        if (!aht.begin())
        {
            Serial.println("AHT begin() default ! Check wiring");
            while (1) // TODO: Trouver une solution meilleure
                delay(10);
        }
    }
    else
        Serial.println("AHT IC2 access default ! Check wiring");
}

bool CLS_Aht10::isI2CReadable()
{
    bool value = false;
    value = i2cScan.waitFor(ATH_10_I2C_ADDR, 1000); // test i2C to know if AHT-10 (temperature and hygrometrie) is readable
    return value;
}

bool CLS_Aht10::hasValidRead()
{
    bool value = false;
    if (hasRaed)
    {
        unsigned long time = millis();
        unsigned long gap;

        if (lastReadTime > time) // time overflow
            lastReadTime = 0;
        gap = time - lastReadTime;
        if (gap <= 10000)
            value = true;
    }
    return value;
}

void CLS_Aht10::doRead()
{
    if (athReady)
    {
        unsigned long time = millis();
        if (lastReadTime > time) // time overflow
            lastReadTime = 0;
        if (time - lastReadTime >= AHT_READ_INTERVAL_TIME)
        {
            lastReadTime = time;
            readAht10();
        }
    }
}

void CLS_Aht10::readAht10()
{
    if (athReady)
    {
        aht.getEvent(&humidity, &temperature); // populate temp and humidity objects with fresh data
        hasRaed = true;
#ifdef DEBUG_AHT10
        Serial.print("Temperature: ");
        Serial.print(temperature.temperature);
        Serial.println(" degrees C");
        Serial.print("Humidity: ");
        Serial.print(humidity.relative_humidity);
        Serial.println("% rH");
#endif
    }
}

String CLS_Aht10::toTextPlain()
{
    String value = "AHT-10 Values\n";
    value += "-------------\n";
    if (athReady)
    {
        //readAht10();
        value += "Temperature (C)=";
        value += String(temperature.temperature);
        value += "\n";
        value += "Relative humidity (%)=";
        value += String(humidity.relative_humidity);
        value += "\n";
    }
    else
        value += "AHT-10 no valid read available\n";
    return value;
}

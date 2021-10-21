#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "CLS_Bmp180.h"

//#define DEBUG_BMP180

Adafruit_BMP085 bmp;

CLS_Bmp180::CLS_Bmp180()
{
}

void CLS_Bmp180::init()
{
    //    Wire.begin();
    if (!bmp.begin())
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
}

void CLS_Bmp180::doRead()
{
    unsigned long time = millis();
    if (lastReadTime > time)
        lastReadTime = 0;
    if (time - lastReadTime >= 1000)
    {
        lastReadTime = time;
        readBmp180();
    }
}

void CLS_Bmp180::readBmp180()
{
    temperature = bmp.readTemperature();
    pressure = bmp.readPressure();
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    altitude = bmp.readAltitude();

#ifdef DEBUG_BMP180
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.print(" *C");

    Serial.print(" --- Pressure = ");
    Serial.print(pressure);
    Serial.print(" Pa");

    Serial.print(" --- Altitude = ");
    Serial.print(altitude);
    Serial.println(" meters");
#endif
}

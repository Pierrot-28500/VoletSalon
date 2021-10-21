
#include <Arduino.h>
#include "CLS_LDR.h"

//#define DEBUG_LDR

const float voltToLuminosite = 100.0f / 0.94f;
const int analog_pin = A0;

CLS_LDR::CLS_LDR()
{
}

float CLS_LDR::getLuminosity()
{
    float a_val = analogRead(analog_pin);
    float volt = (a_val / 1024.0f) * 3.3f;
    float luminosity = volt * voltToLuminosite;

#ifdef DEBUG_LDR
    unsigned long time = millis();
    if (time - lastDebugTime >= 1000)
    {
        Serial.print("Volt: ");
        Serial.print(volt);
        Serial.print(" --- luminosité: ");
        Serial.println(luminosity);
        lastDebugTime = time;
    }
#endif

    return luminosity;
}

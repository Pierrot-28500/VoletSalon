
//#include <stdc++.h>

#ifndef _CLS_AHT_10_H
#define _CLS_AHT_10_H

#include <Adafruit_AHT10.h>

#define ATH_10_I2C_ADDR 0x38

class CLS_Aht10
{
public:
    bool hasRaed = false;
    sensors_event_t humidity;
    sensors_event_t temperature;

    CLS_Aht10();
    void init();
    bool isI2CReadable();
    bool hasValidRead();
    void doRead();
    String toTextPlain();

protected:
    const unsigned long AHT_READ_INTERVAL_TIME = 1000;
    unsigned long lastReadTime = 0;

    void readAht10();
};

#endif

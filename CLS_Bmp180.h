#ifndef _CLS_BMP_180_H
#define _CLS_BMP_180_H

class CLS_Bmp180
{
public:
    const float READ_TEMPERATURE_ERROR = 255.0f;
    const int32_t READ_PRESSURE_ERROR = 255;
    const float READ_ALTITUDE_ERROR = -1.0f;

    float temperature = READ_TEMPERATURE_ERROR;
    int32_t pressure = READ_PRESSURE_ERROR;
    float altitude = READ_ALTITUDE_ERROR;

    CLS_Bmp180();
    void init();
    void doRead();

protected:
    const unsigned long BMP_180_READ_INTERVAL_TIME = 1000;
    unsigned long lastReadTime = 0;

    void readBmp180();
};

#endif
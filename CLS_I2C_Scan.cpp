#include <arduino.h>
#include <Wire.h>
#include "CLS_I2C_Scan.h"

#define DEBUG_SCAN_I2C_LOW_LEVEL
#define DEBUG_SCAN_I2C_HIGH_LEVEL

CLS_I2C_Scan::CLS_I2C_Scan()
{
}

void CLS_I2C_Scan::init()
{
}

byte CLS_I2C_Scan::testAddress(byte address)
{
    byte error = 0;
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    return error;
}

void CLS_I2C_Scan::waitFor(byte address)
{
    while (testAddress(address) != 0)
        delay(1000);
}

bool CLS_I2C_Scan::waitFor(byte address, unsigned long timeout)
{
    bool findedFlag = false;
    bool timeoutFlag = false;
    byte error;
    unsigned long time = millis();
    while (findedFlag == false && timeoutFlag == false)
    {
        if (testAddress(address) == 0)
            findedFlag = true;
        else
        {
            unsigned long elapsedTime = millis() - time;
            if (elapsedTime >= timeout)
                timeoutFlag = true;
        }
    }
    return findedFlag;
}

void CLS_I2C_Scan::doScan()
{
    byte error, address;
    int nDevices;

#ifdef DEBUG_SCAN_I2C_HIGH_LEVEL
    Serial.println("\n I2C Scanner ");
    Serial.println(" Scanning… ");
    nDevices = 0;
#endif
    for (address = 1; address < 127; address++)
    {
        /*
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
*/
        error = testAddress(address);
        if (error == 0)
        {
#ifdef DEBUG_SCAN_I2C_HIGH_LEVEL
            Serial.print(" I2C device found at address 0x ");
            if (address < 16)
                Serial.print(" 0 ");
            Serial.print(address, HEX);
            Serial.println(" ! ");
#endif
            nDevices++;
        }
        else if (error == 4)
        {
#ifdef DEBUG_SCAN_I2C_LOW_LEVEL
            Serial.print(" Unknown error at address 0x ");
            if (address < 16)
                Serial.print(" 0 ");
            Serial.println(address, HEX);
#endif
        }
    }
#ifdef DEBUG_SCAN_I2C_HIGH_LEVEL
    if (nDevices == 0)
        Serial.println(" No I2C devices found\n ");
    else
        Serial.println(" I2C scan done\n ");
#endif
}

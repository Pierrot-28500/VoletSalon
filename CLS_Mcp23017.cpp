#include <Arduino.h>
#include "CLS_Mcp23017.h"
#include "CLS_Shutter.h"

CLS_Mcp23017::CLS_Mcp23017()
{
}

void CLS_Mcp23017::init()
{
    mcp.begin();

        mcp.pullUp(15, 1);
    mcp.pullUp(14, 1);
    mcp.pullUp(13, 1);
    mcp.pullUp(12, 1);
    mcp.pullUp(11, 1);
    mcp.pullUp(10, 1);
    mcp.pullUp(9, 1);
    mcp.pullUp(8, 1);

    mcp.pullUp(7, 1);
    mcp.pullUp(6, 1);
    mcp.pullUp(5, 1);
    mcp.pullUp(4, 1);

    mcp.pullUp(3, 1); // Shutter btn up request
    mcp.pullUp(2, 1); // Shutter btn down request
    /*
    mcp.pullUp(1, 1);
    mcp.pullUp(0, 1);
*/
}

/*
void CLS_Mcp23017::initTest()
{
    init();
    mcp.pinMode(0, OUTPUT);
    mcp.digitalWrite(0, LOW);
    mcp.pinMode(1, OUTPUT);
    mcp.digitalWrite(1, LOW);
}
*/

void CLS_Mcp23017::readTest()
{
    unsigned long time = millis();
    if (time - lastReadTime >= 1000)
    {
        lastReadTime = time;
        Serial.print("MCP Gpios=");
        uint16_t gpIoValues = mcp.readGPIOAB();
        Serial.println(gpIoValues, BIN);
    }
}

void CLS_Mcp23017::setUnConnected(uint8_t pin)
{
    mcp.pinMode(pin, INPUT);
    mcp.pullUp(pin, 1);
}

void CLS_Mcp23017::pinMode(uint8_t pin, uint8_t mode)
{
    mcp.pinMode(pin, mode);
}

void CLS_Mcp23017::pullUp(uint8_t pin, int poolUp)
{
    mcp.pullUp(pin, poolUp);
}

uint8_t CLS_Mcp23017::digitalRead(uint8_t pin)
{
    return mcp.digitalRead(pin);
}

void CLS_Mcp23017::digitalWrite(uint8_t pin, uint8_t pValue)
{
    mcp.digitalWrite(pin, pValue);
}

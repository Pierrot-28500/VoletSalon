#include <Arduino.h>
#include "CLS_Mcp23017.h"
#include "CLS_Shutter.h"

CLS_Mcp23017::CLS_Mcp23017()
{
}

void CLS_Mcp23017::init()
{
    mcp.begin_I2C();

    mcp.pinMode(15, INPUT_PULLUP);
    mcp.pinMode(14, INPUT_PULLUP);
    mcp.pinMode(13, INPUT_PULLUP);
    mcp.pinMode(12, INPUT_PULLUP);
    mcp.pinMode(11, INPUT_PULLUP);
    mcp.pinMode(10, INPUT_PULLUP);
    mcp.pinMode(9, INPUT_PULLUP);
    mcp.pinMode(8, INPUT_PULLUP);

    mcp.pinMode(7, INPUT_PULLUP);
    mcp.pinMode(6, INPUT_PULLUP);
    mcp.pinMode(5, INPUT_PULLUP);
    mcp.pinMode(4, INPUT_PULLUP);

    mcp.pinMode(3, 1); // Shutter btn up request
    mcp.pinMode(2, 1); // Shutter btn down request
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
    mcp.pinMode(pin, INPUT_PULLUP);
    //mcp.pinMode(pin, INPUT);
    //mcp.pullUp(pin, 1);
}

void CLS_Mcp23017::pinMode(uint8_t pin, uint8_t mode)
{
    mcp.pinMode(pin, mode);
}

void CLS_Mcp23017::pullUp(uint8_t pin, int poolUp)
{
    mcp.pinMode(pin, poolUp);
}

uint8_t CLS_Mcp23017::digitalRead(uint8_t pin)
{
    return mcp.digitalRead(pin);
}

void CLS_Mcp23017::digitalWrite(uint8_t pin, uint8_t pValue)
{
    mcp.digitalWrite(pin, pValue);
}

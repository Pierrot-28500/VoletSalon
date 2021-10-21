#ifndef _CLS_MCP_23017
#define _CLS_MCP_23017

#include <Adafruit_MCP23017.h>

#define MCP_I2C_ADDR 0x20

class CLS_Mcp23017
{
public:
    CLS_Mcp23017();
    void init();
    //void initTest();
    void readTest();

    void setUnConnected(uint8_t pin);
    void pinMode(uint8_t pin, uint8_t mode);
    void pullUp(uint8_t pin, int poolUp);
    uint8_t digitalRead(uint8_t pin);
    void digitalWrite(uint8_t pin, uint8_t pValue);

protected:
    Adafruit_MCP23017 mcp;
    unsigned long lastReadTime = 0;
};

#endif

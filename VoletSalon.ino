
#include <Arduino.h>
#include <Wire.h>

#include "CLS_PushButton.h"
#include "CLS_Shutter.h"
#include "CLS_LDR.h"
#include "CLS_I2C_Scan.h"
#include "CLS_Bmp180.h"
#include "CLS_Aht10.h"
#include "CLS_Mcp23017.h"
#include "CLS_IrReceiver.h"
#include "CLS_WiFiServer.h"
#include "CLS_WebServer.h"

#define CMD_DOWN_PIN 0 // A0 de MCP-23017
#define CMD_UP_PIN 1   // A1 de MCP-23017
#define BTN_DOWN_PIN 2 // D5, GPIO_14
#define BTN_UP_PIN 3   // D6, GPIO_12

CLS_Shutter shutter;
CLS_LDR ldr;
CLS_I2C_Scan i2cScan;
CLS_Bmp180 bmp180;
CLS_Aht10 aht10;
CLS_Mcp23017 mcp23017;
CLS_IrReceiver irReceiver;
CLS_WiFiServer *pWiFiServer;
CLS_WebServer *pWebServer;

#define DEBUG_SETUP

bool athReady = false;

void trace(String trace)
{
#ifdef DEBUG_SETUP
    Serial.println(trace);
#endif
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(100);
    trace("Starting");
    trace("waiting 1 second for peripherals init");
    delay(1000); // I2C peripherals wait ready
    trace("Wire initialization");
    Wire.begin();
    trace("i2C initialization");
    i2cScan.init();
    trace("waiting for MCP23017");
    i2cScan.waitFor(MCP23017_ADDRESS); // wait for I2C shutters GPIOs
    trace("Mcp23017 GPIOs initialization");
    mcp23017.init();
    trace("shutter initialization");
    shutter.init(&mcp23017, BTN_UP_PIN, BTN_DOWN_PIN, CMD_UP_PIN, CMD_DOWN_PIN);
    trace("I2C ath-10 presence test");
    athReady = i2cScan.waitFor(ATH_10_I2C_ADDR, 1000); // scan to know witch others devices are connected
    trace("I2C ath-10 ready = " + String(athReady));
    i2cScan.doScan(); // scan to know witch others devices are connected
    //bmp180.init();
    if (athReady)
    {
        trace("ath-10 initialization");
        aht10.init();
    }
    trace("irReceiver initialization");
    irReceiver.init();
    trace("WiFi server initialization");
    pWebServer = new CLS_WebServer();
    pWiFiServer = new CLS_WiFiServer();
}

void loop()
{
    //doLdrJob();
    //bmp180.doRead();
    if (athReady)
        aht10.doRead();
    uint32_t irRequest = irReceiver.getIrRequest();
    doIrCommand(irRequest);
    shutter.doJob();
    pWiFiServer->handleClients();
}

void doLdrJob()
{
    ldr.getLuminosity();
}

void doIrCommand(uint32_t irRequest)
{
    if (irRequest != ENUM_IR_CMD_REQUEST::IR_NONE)
        shutter.onIrCommand(irRequest);
}

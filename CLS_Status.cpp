
#include <Arduino.h>
#include <Wire.h>
#include "CLS_Status.h"
#include "CLS_Aht10.h"
//#define DEBUG_STATUS

extern CLS_Aht10 aht10;

CLS_Status::CLS_Status()
{
#ifdef DEBUG_STATUS
    Serial.println("CLS_WebServer::CLS_Status::CLS_Status()  ---- Instance created");
#endif
}

String CLS_Status::toTextPlain()
{
    String value = aht10.toTextPlain();

    return value;
}

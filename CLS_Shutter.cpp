
#include <Arduino.h>
#include "CLS_Shutter.h"
#include "CLS_PushButton.h"

#define DEBUG_SHUTTLE_HIGH_LEVEL
//#define DEBUG_SHUTTLE_LOW_LEVEL

/*
#define BTN_DOWN_PIN 14 // D5, GPIO_14
#define BTN_UP_PIN 12   // D6, GPIO_12

#define CMD_DOWN_PIN 0 // A0 de MCP-23017
#define CMD_UP_PIN 1   // A1 de MCP-23017
*/

#define STAT_ACTIVE_BTN LOW
#define STAT_ACTIVE_CMD LOW
#define STAT_INACTIVE_CMD HIGH

CLS_PushButton btnDownRequest;
CLS_PushButton btnUpRequest;

CLS_Shutter::CLS_Shutter()
{
}

void CLS_Shutter::init(CLS_Mcp23017 *pMcp, int openGpIn_num, int closeGpIn_num, int openGpOut_num, int closeGpOut_num)
{
    this->pMcp23017 = pMcp;
    this->openGpInNum = openGpIn_num;
    this->openGpOutNum = openGpOut_num;
    this->closeGpInNum = closeGpIn_num;
    this->closeGpInNum = closeGpOut_num;
    this->actualPosition = 0;
    this->requiredPosition = this->actualPosition;
    this->currentIrCommand = ENUM_IR_CMD_REQUEST::IR_NONE;

    if (pMcp23017 == nullptr)
    {
        pinMode(openGpOutNum, OUTPUT);
        pinMode(closeGpOutNum, OUTPUT);
    }
    else
    {
        pMcp23017->pinMode(openGpOutNum, OUTPUT);
        pMcp23017->pinMode(closeGpOutNum, OUTPUT);
    }
    outputCommand(SHUTTER_OUTPUTS_CMDS::OUTPUTS_NONE);
    //outputCommand(openGpOut_num, STAT_INACTIVE_CMD);
    //outputCommand(closeGpOut_num, STAT_INACTIVE_CMD);

    btnUpRequest.init(pMcp, openGpIn_num, STAT_ACTIVE_BTN);
    btnDownRequest.init(pMcp, closeGpIn_num, STAT_ACTIVE_BTN);
}

void CLS_Shutter::outputCommand(uint8_t gpio_num, uint8_t value)
{
    if (pMcp23017 == nullptr)
        digitalWrite(gpio_num, value);
    else
        pMcp23017->digitalWrite(gpio_num, value);
}

void CLS_Shutter::outputCommand(SHUTTER_OUTPUTS_CMDS command)
{
    switch (command)
    {
    case SHUTTER_OUTPUTS_CMDS::OUTPUTS_NONE:
        outputCommand(openGpOutNum, STAT_INACTIVE_CMD);
        outputCommand(closeGpOutNum, STAT_INACTIVE_CMD);
        break;
    case SHUTTER_OUTPUTS_CMDS::OUTPUTS_CLOSE:
        outputCommand(openGpOutNum, STAT_INACTIVE_CMD);
        delay(500);
        outputCommand(closeGpOutNum, STAT_ACTIVE_CMD);
        break;
    case SHUTTER_OUTPUTS_CMDS::OUTPUTS_OPEN:
        outputCommand(closeGpOutNum, STAT_INACTIVE_CMD);
        delay(500);
        outputCommand(openGpOutNum, STAT_ACTIVE_CMD);
        break;
    default:
        break;
    }
}

long CLS_Shutter::evaluatePosition()
{
    long value = actualPosition;
    unsigned long time = millis();
    unsigned long timeGap = time - startCmdTime;

#ifdef DEBUG_SHUTTLE_LOW_LEVEL
    Serial.println("long CLS_Shutter::evaluatePosition()");
#endif
    switch (actualCmd)
    {
    case SHUTTER_ACTUAL_CMD::SHUTTER_SET_HOME:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.print("time = ");
        Serial.print(time);
        Serial.print(" --- startCmdTime = ");
        Serial.print(startCmdTime);
        Serial.print(" --- timeGap = ");
        Serial.print(timeGap);
#endif
        value = SHUTTER_FULL_ACTION_TIME - timeGap;
        if (value > SHUTTER_FULL_ACTION_TIME)
            value = SHUTTER_FULL_ACTION_TIME;
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.print(" --- set home new evalvalue = ");
        Serial.println(value);
#endif
        break;
    case SHUTTER_ACTUAL_CMD::SHUTTER_CLOSING:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.print("time = ");
        Serial.print(time);
        Serial.print(" --- startCmdTime = ");
        Serial.print(startCmdTime);
        Serial.print(" --- timeGap = ");
        Serial.print(timeGap);
#endif
        value += timeGap;
        if (value > SHUTTER_FULL_ACTION_TIME)
            value = SHUTTER_FULL_ACTION_TIME;
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.print(" --- closing new evalvalue = ");
        Serial.println(value);
#endif
        break;
    case SHUTTER_ACTUAL_CMD::SHUTTER_OPENNING:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.print("time = ");
        Serial.print(time);
        Serial.print(" --- startCmdTime = ");
        Serial.print(startCmdTime);
        Serial.print(" --- timeGap = ");
        Serial.print(timeGap);
#endif
        value -= timeGap;
        if (value < 0)
            value = 0;
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.print(" --- openning new evalvalue = ");
        Serial.println(value);
#endif
        break;
    }
    return value;
}

void CLS_Shutter::updatePosition()
{
    actualPosition = evaluatePosition();
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
    Serial.print("actualPosition = ");
    Serial.println(actualPosition);
#endif
}

void CLS_Shutter::stop()
{
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
    Serial.println("CLS_Shutter::stop()");
#endif
    //outputCommand(openGpOutNum, STAT_INACTIVE_CMD);
    //outputCommand(closeGpOutNum, STAT_INACTIVE_CMD);
    outputCommand(SHUTTER_OUTPUTS_CMDS::OUTPUTS_NONE);
    updatePosition();
    actualCmd = SHUTTER_ACTUAL_CMD::SHUTTER_NO_CMD;
    currentIrCommand = ENUM_IR_CMD_REQUEST::IR_NONE;
}

void CLS_Shutter::setHomePosition()
{
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
    Serial.println("CLS_Shutter::setHomePosition(): ");
#endif
    {
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
        Serial.println("openning");
#endif
        requiredPosition = 0;
        actualPosition = SHUTTER_FULL_ACTION_TIME;
        outputCommand(SHUTTER_OUTPUTS_CMDS::OUTPUTS_OPEN);
        //outputCommand(openGpOutNum, STAT_ACTIVE_CMD);
        //outputCommand(closeGpOutNum, STAT_INACTIVE_CMD);
        startCmdTime = millis();
        actualCmd = SHUTTER_ACTUAL_CMD::SHUTTER_SET_HOME;
    }
}

void CLS_Shutter::setPosition(unsigned long newPosition)
{
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
    Serial.print("CLS_Shutter::setPosition(): ");
    Serial.println(newPosition);
#endif
    if (requiredPosition != actualPosition)
        stop();
    if (newPosition > actualPosition)
    {
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
        Serial.println("closing");
#endif
        outputCommand(SHUTTER_OUTPUTS_CMDS::OUTPUTS_CLOSE);
        //outputCommand(openGpOutNum, STAT_INACTIVE_CMD);
        //outputCommand(closeGpOutNum, STAT_ACTIVE_CMD);
        requiredPosition = newPosition;
        startCmdTime = millis();
        actualCmd = SHUTTER_ACTUAL_CMD::SHUTTER_CLOSING;
    }
    else if (newPosition < actualPosition)
    {
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
        Serial.println("openning");
#endif
        outputCommand(SHUTTER_OUTPUTS_CMDS::OUTPUTS_OPEN);
        //outputCommand(closeGpOutNum, STAT_INACTIVE_CMD);
        //outputCommand(openGpOutNum, STAT_ACTIVE_CMD);
        requiredPosition = newPosition;
        startCmdTime = millis();
        actualCmd = SHUTTER_ACTUAL_CMD::SHUTTER_OPENNING;
    }
    else
    {
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
        Serial.println("no cmd to do");
#endif
        outputCommand(SHUTTER_OUTPUTS_CMDS::OUTPUTS_NONE);
        //outputCommand(closeGpOutNum, STAT_INACTIVE_CMD);
        //outputCommand(openGpOutNum, STAT_INACTIVE_CMD);
        actualCmd = SHUTTER_ACTUAL_CMD::SHUTTER_NO_CMD;
        currentIrCommand = ENUM_IR_CMD_REQUEST::IR_NONE;
    }
}

void CLS_Shutter::setRatioPosition(int ratio)
{
    unsigned long stepTime = (unsigned long)SHUTTER_FULL_ACTION_TIME / 10.0L;
    setPosition((unsigned long)ratio * stepTime);
}

void CLS_Shutter::onIrCommand(uint32_t command)
{
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
    Serial.print("onIrCommand: ");
    Serial.println(command, HEX);
#endif
    if (command != currentIrCommand)
    {
        switch (command)
        {
        case ENUM_IR_CMD_REQUEST::IR_HOME:
            currentIrCommand = command;
            setHomePosition();
            break;
        case ENUM_IR_CMD_REQUEST::IR_OK:
            currentIrCommand = command;
            stop();
            break;
        case ENUM_IR_CMD_REQUEST::IR_TOP:
            Serial.println("IR_TOP");
            setPosition(0);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_BOTTOM:
            Serial.println("IR_BOTTOM");
            setPosition(SHUTTER_FULL_ACTION_TIME);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_ONE:
            Serial.println("IR_ONE");
            setRatioPosition(1);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_TWO:
            Serial.println("IR_TWO");
            setRatioPosition(2);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_THREE:
            Serial.println("IR_THREE");
            setRatioPosition(3);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_FOUR:
            Serial.println("IR_FOUR");
            setRatioPosition(4);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_FIVE:
            Serial.println("IR_FIVE");
            setRatioPosition(5);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_SIX:
            Serial.println("IR_SIX");
            setRatioPosition(6);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_SEVEN:
            Serial.println("IR_SEVEN");
            setRatioPosition(7);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_EIGHT:
            Serial.println("IR_EIGHT");
            setRatioPosition(8);
            currentIrCommand = command;
            break;
        case ENUM_IR_CMD_REQUEST::IR_NINE:
            Serial.println("IR_NINE");
            setRatioPosition(9);
            currentIrCommand = command;
            break;
        }
    }
}

SHUTTER_CMDS CLS_Shutter::getRequest()
{
    SHUTTER_CMDS value = SHUTTER_CMDS::NO_SHUTTER_CMD;

    PUSH_BUTTON_ACTION downBtnAction = btnDownRequest.getAction();
    PUSH_BUTTON_ACTION upBtnAction = btnUpRequest.getAction();

    switch (upBtnAction)
    {
    case PUSH_BUTTON_ACTION::LONG_PUSH:
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
        Serial.println("upBtnAction LONG_PUSH");
#endif
        value = SHUTTER_CMDS::HOME_SHUTTER;
        break;
    case PUSH_BUTTON_ACTION::SHORT_PUSH:
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
        Serial.println("upBtnAction SHORT_PUSH");
#endif
        value = SHUTTER_CMDS::OPEN_SHUTTER;
        break;
    default:
        switch (downBtnAction)
        {
        case PUSH_BUTTON_ACTION::LONG_PUSH:
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
            Serial.println("upBtnAction LONG_PUSH");
#endif
            value = SHUTTER_CMDS::CLOSE_SHUTTER;
            break;
        case PUSH_BUTTON_ACTION::SHORT_PUSH:
#ifdef DEBUG_SHUTTLE_HIGH_LEVEL
            Serial.println("upBtnAction SHORT_PUSH");
#endif
            value = SHUTTER_CMDS::CLOSE_SHUTTER;
            break;
        default:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
            Serial.println("No pushButton action");
#endif
            break;
        }
        break;
    }
    return value;
}

void CLS_Shutter::shutterCmd(SHUTTER_CMDS cmd)
{
    switch (cmd)
    {
    case SHUTTER_CMDS::HOME_SHUTTER:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.println("HomePosition");
#endif
        setHomePosition();
        break;
    case SHUTTER_CMDS::CLOSE_SHUTTER:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.println("Close");
#endif
        setPosition(SHUTTER_FULL_ACTION_TIME);
        break;
    case SHUTTER_CMDS::OPEN_SHUTTER:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.println("Open");
#endif
        setPosition(0);
        break;
    case SHUTTER_CMDS::STOP_SHUTTER:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.println("Stop");
#endif
        stop();
        break;
    case SHUTTER_CMDS::NO_SHUTTER_CMD:
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
        Serial.println("No shutter cmd");
#endif
        stop();
        break;
    }
}

void CLS_Shutter::StopIfNeed()
{
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
    Serial.println("CLS_Shutter::StopIfNeed()");
#endif
    long evaluatedPosition;
    switch (actualCmd)
    {
    case SHUTTER_ACTUAL_CMD::SHUTTER_SET_HOME:
        evaluatedPosition = evaluatePosition();
        if (evaluatedPosition <= 0)
            stop();
        break;
    case SHUTTER_ACTUAL_CMD::SHUTTER_CLOSING:
        evaluatedPosition = evaluatePosition();
        if (evaluatedPosition >= requiredPosition)
            stop();
        break;
    case SHUTTER_ACTUAL_CMD::SHUTTER_OPENNING:
        evaluatedPosition = evaluatePosition();
        if (evaluatedPosition <= requiredPosition)
            stop();
        break;
    default:
        break;
    }
}

void CLS_Shutter::doJob()
{
#ifdef DEBUG_SHUTTLE_LOW_LEVEL
    Serial.println("CLS_Shutter::doJob()");
#endif
    StopIfNeed();
    SHUTTER_CMDS cmd = getRequest();
    if (cmd != SHUTTER_CMDS::NO_SHUTTER_CMD)
    {
        if (actualCmd != SHUTTER_ACTUAL_CMD::SHUTTER_NO_CMD)
            stop();
        else
            shutterCmd(cmd);
    }
}

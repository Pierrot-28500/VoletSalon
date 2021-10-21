

#include <Arduino.h>
#include "CLS_PushButton.h"

const int ANTI_BOUNCE_TIME = 100; // ms
const int LONG_PUSH_TIME = 2000;  // ms

CLS_PushButton::CLS_PushButton()
{
}

void CLS_PushButton::init(CLS_Mcp23017 *pMcp, int gpio_num, int activeState)
{
    this->pMcp23017 = pMcp;
    this->gpioNum = gpio_num;
    this->activeState = activeState;
    this->lastChangeTime = 0;
    this->prevState = PUSH_BUTTON_STATE::UNKNOW_STATE;
    if (pMcp23017 == nullptr)
        pinMode(gpio_num, INPUT);
    else
    {
        pMcp23017->pinMode(gpio_num, INPUT);
        pMcp23017->pullUp(gpio_num, false);
    }
}

PUSH_BUTTON_ACTION CLS_PushButton::getAction()
{
    PUSH_BUTTON_ACTION value = PUSH_BUTTON_ACTION::NO_PUSH_ACTION;
    PUSH_BUTTON_STATE logicalState = PUSH_BUTTON_STATE::UNKNOW_STATE;

    int phyState;
    if (pMcp23017 == nullptr)
        phyState = digitalRead(gpioNum);
    else
        phyState = pMcp23017->digitalRead(gpioNum);
    unsigned long time = millis();
    if (phyState == activeState)
        logicalState = PUSH_BUTTON_STATE::ACTIVE_STATE;
    else
        logicalState = PUSH_BUTTON_STATE::INACTIVE_STATE;
    if (prevState == PUSH_BUTTON_STATE::UNKNOW_STATE)
    {
        prevState = logicalState;
        lastChangeTime = time;
    }
    else
    {
        if (logicalState != prevState)
        {
            unsigned long timeGap = time - lastChangeTime;
            if (timeGap >= ANTI_BOUNCE_TIME) // plus de rebond
            {
                if (logicalState == PUSH_BUTTON_STATE::INACTIVE_STATE) // released
                {
                    if (timeGap >= LONG_PUSH_TIME)
                        value = PUSH_BUTTON_ACTION::LONG_PUSH;
                    else
                        value = PUSH_BUTTON_ACTION::SHORT_PUSH;
                }
            }
            prevState = logicalState;
            lastChangeTime = time;
        }
    }
    return value;
}

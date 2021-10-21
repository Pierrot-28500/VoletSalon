
#ifndef _CLS_PUSH_BUTTON_H_
#define _CLS_PUSH_BUTTON_H_

#include "CLS_Mcp23017.h"

enum PUSH_BUTTON_ACTION
{
    NO_PUSH_ACTION,
    SHORT_PUSH,
    LONG_PUSH
};

enum PUSH_BUTTON_STATE
{
    UNKNOW_STATE,
    ACTIVE_STATE,
    INACTIVE_STATE
};

class CLS_PushButton
{
public:
    CLS_PushButton();
    void init(CLS_Mcp23017 *pMcp, int gpio_num, int activeState);
    PUSH_BUTTON_ACTION getAction();

protected:
    CLS_Mcp23017 *pMcp23017;
    int gpioNum = -1;
    int activeState = 0;

    PUSH_BUTTON_STATE prevState = PUSH_BUTTON_STATE::UNKNOW_STATE;
    unsigned long lastChangeTime = 0;
};

#endif

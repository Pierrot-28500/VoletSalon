
//#include <stdc++.h>

#ifndef _CLS_SHUTTER_H_
#define _CLS_SHUTTER_H_

#include "CLS_Mcp23017.h"
#include "CLS_IrReceiver.h"

#define SHUTTER_FULL_ACTION_TIME 25000 // 25 seconds

enum SHUTTER_ACTUAL_CMD
{
    SHUTTER_OPENNING,
    SHUTTER_CLOSING,
    SHUTTER_SET_HOME,
    SHUTTER_NO_CMD
};

enum SHUTTER_CMDS
{
    NO_SHUTTER_CMD = 0,
    OPEN_SHUTTER = 1,
    CLOSE_SHUTTER = 2,
    STOP_SHUTTER = 3,
    HOME_SHUTTER = 4,
};

enum SHUTTER_OUTPUTS_CMDS
{
    OUTPUTS_NONE,
    OUTPUTS_OPEN,
    OUTPUTS_CLOSE
};

class CLS_Shutter
{
protected:
    long evaluatePosition();
    void updatePosition();
    void StopIfNeed();
    void shutterCmd(SHUTTER_CMDS cmd);
    SHUTTER_CMDS getRequest();

    CLS_Mcp23017 *pMcp23017;
    int openGpInNum;
    int openGpOutNum;
    int closeGpInNum;
    int closeGpOutNum;

    int requiredPosition;
    int actualPosition;
    unsigned long startCmdTime = 0L;

    uint32_t currentIrCommand = ENUM_IR_CMD_REQUEST::IR_NONE;

public:
    SHUTTER_ACTUAL_CMD actualCmd = SHUTTER_ACTUAL_CMD::SHUTTER_NO_CMD;

    CLS_Shutter();
    void init(CLS_Mcp23017 *pMcp, int openGpIn_num, int closeGpIn_num, int openGpOut_num, int closeGpOut_num);
    void stop();
    void setHomePosition();
    void setPosition(unsigned long newPosition);
    void setRatioPosition(int ratio);
    void onIrCommand(uint32_t command);
    void doJob();

protected:
    void outputCommand(uint8_t gpio_num, uint8_t value);
    void outputCommand(SHUTTER_OUTPUTS_CMDS command);
};

#endif

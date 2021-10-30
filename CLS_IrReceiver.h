
//#include <stdc++.h>

#ifndef _CLS_IR_RECEIVER_H
#define _CLS_IR_RECEIVER_H

enum ENUM_IR_CMD_REQUEST
{
    IR_NONE = 0x0000,
    IR_ON_OFF_PWR = 0x46C7,
    IR_RED = 0x466D,
    IR_GREEN = 0x466E,
    IR_YELLOW = 0X466F,
    IR_BLUE = 0x4670,
    IR_HOME = 0x4692,
    IR_DISK_POPUP = 0x469A,
    IR_OPTIONS = 0x46C9,
    IR_TOP = 0x4658,
    IR_RIGHT = 0x465B,
    IR_BOTTOM = 0x4659,
    IR_LEFT = 0x465A,
    IR_OK = 0x465C,
    IR_BACK = 0x14683,
    IR_MENU = 0x469C,
    IR_START_STREAM_POS = 0x4621,
    IR_PLAY_STREAM = 0x462C,
    IR_PAUSE_STREAM = 0x4630,
    IR_END_STREAM_POS = 0x4620,
    IR_QUICK_READ_BACK_STREAM = 0x4629,
    IR_STOP_STREAM = 0x4631,
    IR_QUICK_READ_STRAM = 0x1428,
    IR_ONE = 0x4601,
    IR_TWO = 0x4602,
    IR_THREE = 0x4603,
    IR_FOUR = 0x4604,
    IR_FIVE = 0x4605,
    IR_SIX = 0x4606,
    IR_SEVEN = 0x4607,
    IR_EIGHT = 0x4608,
    IR_NINE = 0x4609,
    IR_SUBTILE = 0x464B,
    IR_ZERO = 0x4600,
    IR_AUDIO = 0x464E
};

class CLS_IrReceiver
{
public:
    CLS_IrReceiver();
    void init();
    uint32_t getIrRequest();

protected:
    void debug();
};

#endif

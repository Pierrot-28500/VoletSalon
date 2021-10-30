
//#include <stdc++.h>

#ifndef _CLS_I2C_SCAN_H
#define _CLS_I2C_SCAN_H

class CLS_I2C_Scan
{
public:
    CLS_I2C_Scan();
    void init();
    byte testAddress(byte address);
    void waitFor(byte address);
    bool waitFor(byte address, unsigned long timeOut);
    void doScan();
};

#endif

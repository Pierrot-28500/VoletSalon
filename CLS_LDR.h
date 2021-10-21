
#ifndef _CLS_LDR_H_
#define _CLS_LDR_H_

class CLS_LDR
{
public:
    CLS_LDR();
    float getLuminosity();

protected:
    unsigned long lastDebugTime = 0;
};

#endif

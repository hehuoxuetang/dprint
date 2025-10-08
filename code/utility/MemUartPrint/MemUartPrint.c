#include "printd.h"
#include "memd.h"
#include "tickd.h"
#include "uartd.h"

#include "MemUartPrint.h"


CDbgUartd s_tUpHd = {0};


int UP_Init(int vPort, int vSize)
{
    s_tUpHd = Uartd_Init(vPort, vSize);
    // 默认支持tick功能
    Tickd_Init(&s_tUpHd);
    // 支持内存监测
    Memd_Init(&s_tUpHd);

    UP_Printf(1, "mem uart port: %d", vPort);

    return 0;
}



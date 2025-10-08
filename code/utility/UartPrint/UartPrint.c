#include "printd.h"
#include "uartd.h"
#include "Tickd.h"

#include "UartPrint.h"


CDbgUartd s_tUpHd = {0};


int UP_Init(int vPort, int vSize)
{
    s_tUpHd = Uartd_Init(vPort, vSize);
    // 默认支持tick功能
    Tickd_Init(&s_tUpHd);

    UP_Printf(1, "uart port: %d", vPort);
    
    return 0;
}



#include "dutil.h"
#include "ctrl_uart.h"

#include "printd.h"
#include "filed.h"
#include "Tickd.h"

#include "FilePrint.h"


CDbgFiled s_tUpHd = {0};


int UP_Init(const char *strName, int vMaxSize)
{
    s_tUpHd = Filed_Init(strName, vMaxSize);
    // 默认支持tick功能
    Tickd_Init(&s_tUpHd);

    UP_Printf(1, "max size: %d", vMaxSize);

    return 0;
}



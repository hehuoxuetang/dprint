#include "printd.h"
#include "dispd.h"
#include "filed.h"
#include "uartd.h"
#include "Tickd.h"

#include "PrintDFU.h"


CDbgDispd s_tDispHd = {0};
CDbgFiled s_tFileHd = {0};
CDbgUartd s_tUartHd = {0};


int DispP_Init(void)
{
    s_tDispHd = Dispd_Init();

    DispP_Printf(1, "dispd");

    return 0;
}

int FileP_Init(const char *strName, int vMaxSize)
{

    s_tFileHd = Filed_Init(strName, vMaxSize);
    // 默认支持tick功能
    Tickd_Init(&s_tFileHd);

    FileP_Printf(1, "file max: %d", vMaxSize);

    return 0;
}

int UartP_Init(int vPort, int vSize)
{
    s_tUartHd = Uartd_Init(vPort, vSize);

    UartP_Printf(1, "port: %d", vPort);

    return 0;
}

// 默认的初始化
int UP_Init(int vPort)
{
	DispP_Init();
	FileP_Init("log.txt", 512*1024);
	if(vPort >= 0)
		UartP_Init(vPort, 1024);

	return 0;
}


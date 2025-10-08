#include "ConsoleIn.h"
#include "Tickd.h"
#include "uartd.h"

#include "UartPrintC.h"


CDbgUartd s_tUpHd = {0};

/**********************************************************************
* 函数: UP_Init
* 功能: WinPrintC模块初始化
* 参数: vId--
*		vSize--。
* 返回: 0值--成功
*		非0--错误类型
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建
**********************************************************************/
int UP_Init(int vPort, int vSize)
{
    s_tUpHd = Uartd_Init(vPort, vSize);
	// 默认支持tick功能
    Tickd_Init(&s_tUpHd);
	// 增加控制台
    Console_Init(&s_tUpHd);
    UP_Printf(1, "port console: %d", vPort, vSize);

    return 0;
}



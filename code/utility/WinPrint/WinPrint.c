#include "printd.h"
#include "wind.h"
#include "Tickd.h"

#include "WinPrint.h"


CDbgHd s_tUpHd = {{0},{{0},{0},0}};

/**********************************************************************
* 函数: UP_Init
* 功能: WinPrint模块初始化
* 参数: vId--控制台ID。仅预留，暂不支持。建议传入0值
*		vSize--日志缓存大小。有待支持。建议传入0值。
* 返回: 0值--成功
*		非0--错误类型
* 说明: 
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建
**********************************************************************/
int UP_Init(int vId, int vSize)
{
    HDbgDev hDev;

    hDev.vUartId = vId;
    s_tUpHd = Wind_Init(hDev, vSize);
    // 默认支持tick功能
    Tickd_Init(&s_tUpHd);

    UP_Printf(1, "win bufsize: %d", vSize);
    
    return 0;
}



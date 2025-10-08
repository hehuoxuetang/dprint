#include "MemIn.h"
#include "heap.h"
#include "stack.h"
#include "memd.h"


// 记录模块的基础信息
static CMdVar	s_tMdVar = {NULL};


/**********************************************************************
* 函数: Memd_Init
* 功能: 模块初始化
* 参数: ptHd--调试输出的句柄
*		vStackSize--纳入监控的栈大小。取0值代表不监控栈
* 返回: 0值--成功
*		非0--错误类型
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-06-15	CCQ		创建
**********************************************************************/
int Memd_Init(void *ptHd)
{
    s_tMdVar.pvHd = ptHd;
	// 堆：首页64页，后续每页16项
	Hpd_Init();

    return 0;
}

// 调试信息输出宏
#if 1
int MdPrintf(const char *strFmt, ...)
{
	va_list args;
	char szLog[1024];
	int printed = 0;

	if(s_tMdVar.pvHd != NULL){
		va_start(args, strFmt);
		printed = vsnprintf(szLog, sizeof(szLog)-1, strFmt, args);
		printed = DbgWrite(s_tMdVar.pvHd, szLog, 0);
		printed+=DbgFeed(s_tMdVar.pvHd, 1);
		va_end(args);
	}

	return printed;
}
#else
int MdPrintf(const char *strFmt, ...)
{
    va_list args;
	int printed = 0;

	if(s_tMdVar.pvHd != NULL){
        va_start(args, strFmt);
        DbgPrintf(s_tMdVar.pvHd, 1, strFmt, args);
        va_end(args);
	}

    return printed;
}
#endif
// ---------------用于匹配使用函数指针的情况-----------------

void* Memd_Malloc4Ptr(int vSize)
{
    return Memd_Malloc(vSize);
}

void Memd_Free4Ptr(void* pvPtr)
{
    Memd_Free(pvPtr);
}

void* Memd_Realloc4Ptr(void* pvPtr, int vSize)
{
    return Memd_Realloc(pvPtr, vSize);
}



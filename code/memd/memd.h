#ifndef _MEMD_H
#define _MEMD_H


typedef struct
{
	void		*pvHd;
}CMdVar;


int MdPrintf(const char *strFmt, ...);

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// 初始化：调试句柄配置
int Memd_Init(void *ptHd);

// mem替代接口
#define Memd_Malloc(uSize)            Hpd_MallocDeal(uSize, __FUNCTION__, __LINE__)
#define Memd_Free(pvBuf)              Hpd_FreeDeal(pvBuf, __FUNCTION__, __LINE__)
#define Memd_Realloc(pvBuf, uSize)    Hpd_ReallocDeal(pvBuf, uSize, __FUNCTION__, __LINE__)

// 解决以函数指针方式调用的问题
void* Memd_Malloc4Ptr(int uSize);
void  Memd_Free4Ptr(void* pvBuf);
void* Memd_Realloc4Ptr(void* pvBuf, int uSize);


// 方便用户直接打印相关信息
int Memd_PrInfo(void);


//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

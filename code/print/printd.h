#ifndef _DBG_PRINT_H
#define	_DBG_PRINT_H


// 默认格式化缓存大小
#define DBG_FMT_SIZE            1024

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// 最大超时时间
#define DBG_TO_MAX		(0x7fffffff)

// 各函数指针类型
typedef int (*FDbgClear)(void *ptDev);
typedef int (*FDbgRead)(void *ptDev, void *szBuf, int vLen, int vToMs);
typedef int (*FDbgWrite)(void *ptDev, const void *szBuf, int vLen);
typedef int (*FDbgClose)(void *ptDev);

// 配置结构
typedef struct
{
	// 句柄
	union
	{
	    int		vHd;
		void	*pvHd;
	};
	// 函数指针
	FDbgClear	fnClear;
	FDbgRead    fnRead;
    FDbgWrite   fnWrite;
	FDbgClose   fnClose;
    // 配置项
    int         vFmtMax  : 28;	//log格式化缓存上限
    int         vTimeFlg : 1;
    int         vFileFlg : 1;
    int         vFuncFlg : 1;
    int         vLineFlg : 1;
}CDbgInfo;


int DbgClear(void *ptDev);
int DbgWrite(void *ptDev, const void *szBuf, int vLen);
int DbgRead(void *ptDev, void *szBuf, int vLen, int vToMs);
int DbgClose(void *ptDev);

// 格式化输出
int DbgPrint(void *ptDev, const char *strFmt, ...);

// 打印文件名，函数名，行号等信息
int DbgPrintInfo(void *ptDev, const char *strFile, const char *strFunc, const int vLine);
// 输出hex
int DbgPrintHex(void *ptDev, unsigned int vLf, const char *strTip, void* szAddr, unsigned int vLen, unsigned int vAlign);
// 换行
int DbgFeed(void *ptDev, unsigned int vLf);

// 方便使用的函数宏
#define DbgPrintf(pvDev, vLf, ...)   \
            ({ \
                DbgPrintInfo(pvDev, __FILE__, __FUNCTION__, __LINE__); \
                DbgPrint(pvDev, ##__VA_ARGS__);  \
                DbgFeed(pvDev, vLf);  \
            })

#define DbgPrinta(pvDev, vLf, szBuf, vLen)	\
            ({ \
                DbgPrintInfo(pvDev, __FILE__, __FUNCTION__, __LINE__); \
                DbgWrite(pvDev, szBuf, vLen);  \
                DbgFeed(pvDev, vLf);  \
            })

#define DbgPrinth(pvDev, vLf, strTip, szAddr, vLen, vAlign)	\
            ({ \
                DbgPrintInfo(pvDev, __FILE__, __FUNCTION__, __LINE__); \
                DbgPrintHex(pvDev, vLf, strTip, szAddr, vLen, vAlign);  \
            })
			
//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

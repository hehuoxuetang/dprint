#ifndef _HEAPD_H
#define _HEAPD_H


// 一般采用链表方式
// 用数组方式的话，为避免干扰动态内存，要采用全局数组
//#define HP_ARRAY_CNT	100

// 内存块的记录方式
#define HP_STORE_LINK	0	// 链表方式
#define HP_STORE_ARRAY	1	// 数组方式


// 异常处理函数
typedef void (*FMemdErr)(void);

typedef struct _tagHpBlock
{
	// 申请到的内存地址
    void    	*pvAddr;
	// 申请的内存大小
    int			vSize;
	// 申请内存的函数
    const char  *strName;
	// 申请内存函数的行号
    int			vLineNo;
	// 下一个内存块
	struct _tagHpBlock	*ptNext;
}CHpBlock;

typedef struct
{
	// 区间申请项：申请项都是追加的方式，因此只要记录首项地址即可
    CHpBlock	*ptNew;
	// 动态内存的变化：负值代表释放的比申请的多
	int			vCurSize;		// 动态内存的当前值
	int			vMaxSize;		// 动态内存的最大值
	// 启动标志
	int			vEnable;
}CHpSec;

typedef struct
{
    // 记录全局堆内存使用的峰值
    int		vHeapMax;
    // 记录全局堆内存使用的当前值
    int		vHeapCur;
	// 链表最大项数
	int		vCntMax;
	// 链表当前项数
	int		vCntCur;
}CHpCalc;

typedef struct
{
	// 配置：错误回调函数
	FMemdErr	fnMemdErr;
	// 申请但未释放内存块
	CHpBlock	*ptHead;
	// 错误内存块
	CHpBlock	*ptErr;
	// 统计值
	CHpCalc		tCalc;
	// 记录类型
//	int			vFlag;
}CHpVar;

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// 初始化：调试句柄配置
int Hpd_Init(void);

// 基础接口
void* Hpd_MallocDeal(int uSize, const char* pcFuncName, int uLineNo);
void  Hpd_FreeDeal(void* pvBuf, const char* pcFuncName, int uLineNo);
void* Hpd_ReallocDeal(void* pvBuf, int uSize, const char* pcFuncName, int uLineNo);

int Hpd_PrInfo(void);


int Hpd_StartCount(void);
int Hpd_EndCount(void);

//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

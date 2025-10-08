#include "MemIn.h"
#include "memd.h"
#include "heap.h"


// 调试信息输出宏
#define HpPrintf(...)		MdPrintf(__VA_ARGS__)

// 记录模块的基础信息
static CHpVar	s_tHpVar = {NULL, NULL, NULL, {0}};
// 记录区间信息
static CHpSec	s_tHpSec = {0};


// 检查链表中是否存在相同块：只判断地址就够了
static CHpBlock* Hpd_CheckExist(CHpBlock *ptLink, void *pvAddr)
{
	CHpBlock *ptCur = ptLink;

	// 识别重复块，避免多次记录相同的错误块信息
    while(ptCur != NULL){
		if(ptCur->pvAddr == pvAddr){
			// 已经存在相同地址
			break;
		}
		ptCur = ptCur->ptNext;
	}
	// 存在相同块，则返回块地址；不存在则返回NULL
	return ptCur;
}

/**********************************************************************
* 函数: MdError
* 功能: 本模块的错误处理
* 参数: strName--错误所在的函数
*		vLineNo--错误所在的行号
* 返回: 无
* 说明: 如果用户有注册错误处理则调用用户的错误处理，否则采用默认的错误处理
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-06-15	CCQ		创建
**********************************************************************/
static void HpdError(void)
{
	if(s_tHpVar.fnMemdErr != NULL){
		s_tHpVar.fnMemdErr();
	}
	else{
		while(1){
			Sys_SleepMs(5*1000);
			HpPrintf("heap debug error!");
		}
	}
}


// 是否数组方式
#ifdef HP_ARRAY_CNT

// 记录申请项的数组
static CHpBlock s_taHpBlock[HP_ARRAY_CNT];


// 申请一个记录块：数组
static CHpBlock* Hpd_NewBlock(void)
{
	void *pvPtr = NULL;
	int i;

	// 查询数组是否有空闲空间
	for(i=0; i<HP_ARRAY_CNT; i++){
		if(s_taHpBlock[i].pvAddr == NULL){
			pvPtr = &s_taHpBlock[i];
			break;
		}
	}
	// 空间消耗完，直接报错
	if(pvPtr == NULL){
		HpPrintf("%s %d", __FUNCTION__, __LINE__);
		HpdError();
	}

	return pvPtr;
}

// 释放一个记录块：数组
static void Hpd_DelBlock(CHpBlock *ptBlock)
{
	memset(ptBlock, 0, sizeof(CHpBlock));
}

#else

// 申请一个记录块：动态内存
static CHpBlock* Hpd_NewBlock(void)
{
	void *pvPtr = NULL;
	int vLen;

	vLen = sizeof(CHpBlock);
	pvPtr = Sys_Malloc(vLen);
	if(pvPtr == NULL){
		HpPrintf("%s %d", __FUNCTION__, __LINE__);
		HpdError();
	}
	memset(pvPtr, 0, vLen);

	return pvPtr;
}

// 释放一个记录块：动态内存
static void Hpd_DelBlock(CHpBlock *ptBlock)
{
	Sys_Free(ptBlock);
}

#endif


/**********************************************************************
* 函数: Memd_Init
* 功能: 模块初始化
* 参数: vHeadSize--首页开辟的项空间
*		vNextSize--后续新增页的项空间
* 返回: 0值--成功
*		非0--错误类型
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-06-15	CCQ		创建
**********************************************************************/
int Hpd_Init(void)
{
	s_tHpVar.fnMemdErr = NULL;
	// 堆占用及堆错误信息链表
	s_tHpVar.ptHead = NULL;
	s_tHpVar.ptErr = NULL;
	// 统计信息
	memset(&s_tHpVar.tCalc, 0, sizeof(CHpCalc));

#ifdef HP_ARRAY_CNT
	memset(s_taHpBlock, 0, sizeof(CHpBlock)*HP_ARRAY_CNT);
#endif

    return 0;
}

// 追加一个堆错误信息块
static void Hpd_AddErr(void *pvAddr, int vSize, const char* strName, int vLineNo)
{
    CHpBlock *ptBlock = NULL;

	// 识别重复块，避免多次记录相同的错误块信息
    ptBlock = Hpd_CheckExist(s_tHpVar.ptErr, pvAddr);
	if(ptBlock == NULL){
		// 不存在相同块，则加入新块
		ptBlock = Hpd_NewBlock();
		ptBlock->pvAddr = pvAddr;
		ptBlock->strName = strName;
		ptBlock->vLineNo = vLineNo;
		ptBlock->vSize = vSize;
		ptBlock->ptNext = s_tHpVar.ptErr;
		s_tHpVar.ptErr = ptBlock;
	}
}

// 追加一个堆信息块
static void Hpd_AddBlock(void *pvAddr, int vSize, const char* strName, int vLineNo)
{
    CHpBlock *ptBlock = NULL;

	// 出现重复的地址
	ptBlock = Hpd_CheckExist(s_tHpVar.ptHead, pvAddr);
	if(ptBlock != NULL){
		HpPrintf("%s %d -- 0x%x %d", strName, vLineNo, pvAddr, vSize);
		HpdError();
		goto LB_END;
	}

	// 加入链表
	ptBlock = Hpd_NewBlock();
	ptBlock->pvAddr = pvAddr;
	ptBlock->strName = strName;
	ptBlock->vLineNo = vLineNo;
	ptBlock->vSize = vSize;
	ptBlock->ptNext = s_tHpVar.ptHead;
	s_tHpVar.ptHead = ptBlock;

	// 更新统计值
	s_tHpVar.tCalc.vHeapCur += vSize;
	if(s_tHpVar.tCalc.vHeapCur > s_tHpVar.tCalc.vHeapMax){
		s_tHpVar.tCalc.vHeapMax = s_tHpVar.tCalc.vHeapCur;
	}
	s_tHpVar.tCalc.vCntCur += 1;
	if(s_tHpVar.tCalc.vCntCur > s_tHpVar.tCalc.vCntMax){
		s_tHpVar.tCalc.vCntMax = s_tHpVar.tCalc.vCntCur;
	}

	// 区间统计更新
	if(s_tHpSec.vEnable != 0){
		s_tHpSec.vCurSize += vSize;
		if(s_tHpSec.vCurSize > s_tHpSec.vMaxSize){
			s_tHpSec.vMaxSize = s_tHpSec.vCurSize;
		}
	}

LB_END:
	return;
}

// 删除一个堆信息块
static void Hpd_SubBlock(void* pvPtr, const char* strName, int vLineNo)
{
	CHpBlock *ptBlock = NULL;
	CHpBlock *ptPre = NULL;

	// 是否存在对应块
	ptBlock = Hpd_CheckExist(s_tHpVar.ptHead, pvPtr);
	if(ptBlock == NULL){
		// 没找到，则添加错误信息
		Hpd_AddErr(pvPtr, 0, strName, vLineNo);
		goto LB_END;
	}

	// 区间统计更新
	if(s_tHpSec.vEnable != 0){
		if(s_tHpSec.ptNew == ptBlock){
			s_tHpSec.ptNew = ptBlock->ptNext;
		}
		s_tHpSec.vCurSize -= ptBlock->vSize;
	}

	// 链表中删除对应项
	if(s_tHpVar.ptHead == ptBlock){
		s_tHpVar.ptHead = ptBlock->ptNext;
	}
	else{
		ptPre = s_tHpVar.ptHead;
		while(ptPre != NULL){
			if(ptPre->ptNext == ptBlock){
				ptPre->ptNext = ptBlock->ptNext;
				break;
			}
			else{
                ptPre = ptPre->ptNext;
			}
		}
	}
	// 更新统计值
	s_tHpVar.tCalc.vHeapCur -= ptBlock->vSize;
	s_tHpVar.tCalc.vCntCur -= 1;
	// 清对应项
	Hpd_DelBlock(ptBlock);

LB_END:
	return;
}

// 申请一个堆内存块
void* Hpd_MallocDeal(int vSize, const char* strName, int vLineNo)
{
    void* pvBuf = NULL;

//	HpPrintf("%s: %d", strName, vSize);
    pvBuf = Sys_Malloc(vSize);
	if(pvBuf == NULL){
		// 申请不到内存，进入错误处理
		HpPrintf("[malloc error]--%s %d %d", strName, vLineNo, vSize);
		HpdError();
	}
    Hpd_AddBlock(pvBuf, vSize, strName, vLineNo);

    return pvBuf;
}

// 释放一个堆内存块
void Hpd_FreeDeal(void* pvPtr, const char* strName, int vLineNo)
{
//	HpPrintf("%s: %d", strName, vLineNo);
	if(pvPtr == NULL){
		// 请求释放空内存
		// 实际应用代码中存在这种情况，直接过滤处理
		HpPrintf("[free error]--%s %d", strName, vLineNo);
	}
	else{
		Hpd_SubBlock(pvPtr, strName, vLineNo);
    	Sys_Free(pvPtr);
	}
}

// realloc一个堆内存块
void* Hpd_ReallocDeal(void* pvPtr, int vSize, const char* strName, int vLineNo)
{
    void* pvNew;

	if(pvPtr != NULL){
		Hpd_SubBlock(pvPtr, strName, vLineNo);
	}
    pvNew = Sys_Realloc(pvPtr, vSize);
	if(pvNew == NULL){
		// 申请不到内存，进入错误处理
		HpPrintf("[realloc error]--%s %d %d", strName, vLineNo, vSize);
		HpdError();
	}
	else{
		Hpd_AddBlock(pvNew, vSize, strName, vLineNo);
	}

    return pvNew;
}

// 打印基础信息
int Hpd_PrInfo(void)
{
	CHpBlock *ptBlock = NULL;

	HpPrintf("cur: %d, max: %d", s_tHpVar.tCalc.vHeapCur, s_tHpVar.tCalc.vHeapMax);
	// 打印err tree
    if(s_tHpVar.ptErr != NULL)
    {
    	ptBlock = s_tHpVar.ptErr;
		HpPrintf("error:");
        HpPrintf("func\tline\taddr\t\tsize");
        while(ptBlock != NULL)
        {
            HpPrintf("%s\t%d\t0x%x\t%d", ptBlock->strName, ptBlock->vLineNo,
            		ptBlock->pvAddr, ptBlock->vSize);
			ptBlock = ptBlock->ptNext;
        }
    }
    // 打印block tree
    HpPrintf("block:");
	HpPrintf("func\t\tline\taddr\t\tsize");
	ptBlock = s_tHpVar.ptHead;
    while(ptBlock != NULL)
    {
		HpPrintf("%s\t%d\t0x%x\t%d", ptBlock->strName, ptBlock->vLineNo,
            	ptBlock->pvAddr, ptBlock->vSize);
		ptBlock = ptBlock->ptNext;
    }

    return 0;
}

int Hpd_StartCount(void)
{
	memset(&s_tHpSec, 0, sizeof(CHpSec));
	s_tHpSec.ptNew = s_tHpVar.ptHead;
	s_tHpSec.vEnable = 1;

	return 0;
}

int Hpd_EndCount(void)
{
	CHpBlock *ptBlock = NULL;

	s_tHpSec.vEnable = 0;

	HpPrintf("sec--cur: %d, max: %d", s_tHpSec.vCurSize, s_tHpSec.vMaxSize);
    // 打印block tree
    HpPrintf("block:");
	HpPrintf("func\t\tline\taddr\t\tsize");
	ptBlock = s_tHpVar.ptHead;
    while((ptBlock != NULL) && (ptBlock != s_tHpSec.ptNew))
    {
		HpPrintf("%s\t%d\t0x%x\t%d", ptBlock->strName, ptBlock->vLineNo,
            	ptBlock->pvAddr, ptBlock->vSize);
		ptBlock = ptBlock->ptNext;
    }

    return 0;
}


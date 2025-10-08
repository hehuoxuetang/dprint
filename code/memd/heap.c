#include "MemIn.h"
#include "memd.h"
#include "heap.h"


// ������Ϣ�����
#define HpPrintf(...)		MdPrintf(__VA_ARGS__)

// ��¼ģ��Ļ�����Ϣ
static CHpVar	s_tHpVar = {NULL, NULL, NULL, {0}};
// ��¼������Ϣ
static CHpSec	s_tHpSec = {0};


// ����������Ƿ������ͬ�飺ֻ�жϵ�ַ�͹���
static CHpBlock* Hpd_CheckExist(CHpBlock *ptLink, void *pvAddr)
{
	CHpBlock *ptCur = ptLink;

	// ʶ���ظ��飬�����μ�¼��ͬ�Ĵ������Ϣ
    while(ptCur != NULL){
		if(ptCur->pvAddr == pvAddr){
			// �Ѿ�������ͬ��ַ
			break;
		}
		ptCur = ptCur->ptNext;
	}
	// ������ͬ�飬�򷵻ؿ��ַ���������򷵻�NULL
	return ptCur;
}

/**********************************************************************
* ����: MdError
* ����: ��ģ��Ĵ�����
* ����: strName--�������ڵĺ���
*		vLineNo--�������ڵ��к�
* ����: ��
* ˵��: ����û���ע�������������û��Ĵ������������Ĭ�ϵĴ�����
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-06-15	CCQ		����
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


// �Ƿ����鷽ʽ
#ifdef HP_ARRAY_CNT

// ��¼�����������
static CHpBlock s_taHpBlock[HP_ARRAY_CNT];


// ����һ����¼�飺����
static CHpBlock* Hpd_NewBlock(void)
{
	void *pvPtr = NULL;
	int i;

	// ��ѯ�����Ƿ��п��пռ�
	for(i=0; i<HP_ARRAY_CNT; i++){
		if(s_taHpBlock[i].pvAddr == NULL){
			pvPtr = &s_taHpBlock[i];
			break;
		}
	}
	// �ռ������ֱ꣬�ӱ���
	if(pvPtr == NULL){
		HpPrintf("%s %d", __FUNCTION__, __LINE__);
		HpdError();
	}

	return pvPtr;
}

// �ͷ�һ����¼�飺����
static void Hpd_DelBlock(CHpBlock *ptBlock)
{
	memset(ptBlock, 0, sizeof(CHpBlock));
}

#else

// ����һ����¼�飺��̬�ڴ�
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

// �ͷ�һ����¼�飺��̬�ڴ�
static void Hpd_DelBlock(CHpBlock *ptBlock)
{
	Sys_Free(ptBlock);
}

#endif


/**********************************************************************
* ����: Memd_Init
* ����: ģ���ʼ��
* ����: vHeadSize--��ҳ���ٵ���ռ�
*		vNextSize--��������ҳ����ռ�
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-06-15	CCQ		����
**********************************************************************/
int Hpd_Init(void)
{
	s_tHpVar.fnMemdErr = NULL;
	// ��ռ�ü��Ѵ�����Ϣ����
	s_tHpVar.ptHead = NULL;
	s_tHpVar.ptErr = NULL;
	// ͳ����Ϣ
	memset(&s_tHpVar.tCalc, 0, sizeof(CHpCalc));

#ifdef HP_ARRAY_CNT
	memset(s_taHpBlock, 0, sizeof(CHpBlock)*HP_ARRAY_CNT);
#endif

    return 0;
}

// ׷��һ���Ѵ�����Ϣ��
static void Hpd_AddErr(void *pvAddr, int vSize, const char* strName, int vLineNo)
{
    CHpBlock *ptBlock = NULL;

	// ʶ���ظ��飬�����μ�¼��ͬ�Ĵ������Ϣ
    ptBlock = Hpd_CheckExist(s_tHpVar.ptErr, pvAddr);
	if(ptBlock == NULL){
		// ��������ͬ�飬������¿�
		ptBlock = Hpd_NewBlock();
		ptBlock->pvAddr = pvAddr;
		ptBlock->strName = strName;
		ptBlock->vLineNo = vLineNo;
		ptBlock->vSize = vSize;
		ptBlock->ptNext = s_tHpVar.ptErr;
		s_tHpVar.ptErr = ptBlock;
	}
}

// ׷��һ������Ϣ��
static void Hpd_AddBlock(void *pvAddr, int vSize, const char* strName, int vLineNo)
{
    CHpBlock *ptBlock = NULL;

	// �����ظ��ĵ�ַ
	ptBlock = Hpd_CheckExist(s_tHpVar.ptHead, pvAddr);
	if(ptBlock != NULL){
		HpPrintf("%s %d -- 0x%x %d", strName, vLineNo, pvAddr, vSize);
		HpdError();
		goto LB_END;
	}

	// ��������
	ptBlock = Hpd_NewBlock();
	ptBlock->pvAddr = pvAddr;
	ptBlock->strName = strName;
	ptBlock->vLineNo = vLineNo;
	ptBlock->vSize = vSize;
	ptBlock->ptNext = s_tHpVar.ptHead;
	s_tHpVar.ptHead = ptBlock;

	// ����ͳ��ֵ
	s_tHpVar.tCalc.vHeapCur += vSize;
	if(s_tHpVar.tCalc.vHeapCur > s_tHpVar.tCalc.vHeapMax){
		s_tHpVar.tCalc.vHeapMax = s_tHpVar.tCalc.vHeapCur;
	}
	s_tHpVar.tCalc.vCntCur += 1;
	if(s_tHpVar.tCalc.vCntCur > s_tHpVar.tCalc.vCntMax){
		s_tHpVar.tCalc.vCntMax = s_tHpVar.tCalc.vCntCur;
	}

	// ����ͳ�Ƹ���
	if(s_tHpSec.vEnable != 0){
		s_tHpSec.vCurSize += vSize;
		if(s_tHpSec.vCurSize > s_tHpSec.vMaxSize){
			s_tHpSec.vMaxSize = s_tHpSec.vCurSize;
		}
	}

LB_END:
	return;
}

// ɾ��һ������Ϣ��
static void Hpd_SubBlock(void* pvPtr, const char* strName, int vLineNo)
{
	CHpBlock *ptBlock = NULL;
	CHpBlock *ptPre = NULL;

	// �Ƿ���ڶ�Ӧ��
	ptBlock = Hpd_CheckExist(s_tHpVar.ptHead, pvPtr);
	if(ptBlock == NULL){
		// û�ҵ�������Ӵ�����Ϣ
		Hpd_AddErr(pvPtr, 0, strName, vLineNo);
		goto LB_END;
	}

	// ����ͳ�Ƹ���
	if(s_tHpSec.vEnable != 0){
		if(s_tHpSec.ptNew == ptBlock){
			s_tHpSec.ptNew = ptBlock->ptNext;
		}
		s_tHpSec.vCurSize -= ptBlock->vSize;
	}

	// ������ɾ����Ӧ��
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
	// ����ͳ��ֵ
	s_tHpVar.tCalc.vHeapCur -= ptBlock->vSize;
	s_tHpVar.tCalc.vCntCur -= 1;
	// ���Ӧ��
	Hpd_DelBlock(ptBlock);

LB_END:
	return;
}

// ����һ�����ڴ��
void* Hpd_MallocDeal(int vSize, const char* strName, int vLineNo)
{
    void* pvBuf = NULL;

//	HpPrintf("%s: %d", strName, vSize);
    pvBuf = Sys_Malloc(vSize);
	if(pvBuf == NULL){
		// ���벻���ڴ棬���������
		HpPrintf("[malloc error]--%s %d %d", strName, vLineNo, vSize);
		HpdError();
	}
    Hpd_AddBlock(pvBuf, vSize, strName, vLineNo);

    return pvBuf;
}

// �ͷ�һ�����ڴ��
void Hpd_FreeDeal(void* pvPtr, const char* strName, int vLineNo)
{
//	HpPrintf("%s: %d", strName, vLineNo);
	if(pvPtr == NULL){
		// �����ͷſ��ڴ�
		// ʵ��Ӧ�ô����д������������ֱ�ӹ��˴���
		HpPrintf("[free error]--%s %d", strName, vLineNo);
	}
	else{
		Hpd_SubBlock(pvPtr, strName, vLineNo);
    	Sys_Free(pvPtr);
	}
}

// reallocһ�����ڴ��
void* Hpd_ReallocDeal(void* pvPtr, int vSize, const char* strName, int vLineNo)
{
    void* pvNew;

	if(pvPtr != NULL){
		Hpd_SubBlock(pvPtr, strName, vLineNo);
	}
    pvNew = Sys_Realloc(pvPtr, vSize);
	if(pvNew == NULL){
		// ���벻���ڴ棬���������
		HpPrintf("[realloc error]--%s %d %d", strName, vLineNo, vSize);
		HpdError();
	}
	else{
		Hpd_AddBlock(pvNew, vSize, strName, vLineNo);
	}

    return pvNew;
}

// ��ӡ������Ϣ
int Hpd_PrInfo(void)
{
	CHpBlock *ptBlock = NULL;

	HpPrintf("cur: %d, max: %d", s_tHpVar.tCalc.vHeapCur, s_tHpVar.tCalc.vHeapMax);
	// ��ӡerr tree
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
    // ��ӡblock tree
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
    // ��ӡblock tree
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


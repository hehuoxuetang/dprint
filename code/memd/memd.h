#ifndef _MEMD_H
#define _MEMD_H


typedef struct
{
	void		*pvHd;
}CMdVar;


int MdPrintf(const char *strFmt, ...);

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// ��ʼ�������Ծ������
int Memd_Init(void *ptHd);

// mem����ӿ�
#define Memd_Malloc(uSize)            Hpd_MallocDeal(uSize, __FUNCTION__, __LINE__)
#define Memd_Free(pvBuf)              Hpd_FreeDeal(pvBuf, __FUNCTION__, __LINE__)
#define Memd_Realloc(pvBuf, uSize)    Hpd_ReallocDeal(pvBuf, uSize, __FUNCTION__, __LINE__)

// ����Ժ���ָ�뷽ʽ���õ�����
void* Memd_Malloc4Ptr(int uSize);
void  Memd_Free4Ptr(void* pvBuf);
void* Memd_Realloc4Ptr(void* pvBuf, int uSize);


// �����û�ֱ�Ӵ�ӡ�����Ϣ
int Memd_PrInfo(void);


//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

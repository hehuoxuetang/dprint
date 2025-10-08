#include "MemIn.h"
#include "heap.h"
#include "stack.h"
#include "memd.h"


// ��¼ģ��Ļ�����Ϣ
static CMdVar	s_tMdVar = {NULL};


/**********************************************************************
* ����: Memd_Init
* ����: ģ���ʼ��
* ����: ptHd--��������ľ��
*		vStackSize--�����ص�ջ��С��ȡ0ֵ�������ջ
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-06-15	CCQ		����
**********************************************************************/
int Memd_Init(void *ptHd)
{
    s_tMdVar.pvHd = ptHd;
	// �ѣ���ҳ64ҳ������ÿҳ16��
	Hpd_Init();

    return 0;
}

// ������Ϣ�����
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
// ---------------����ƥ��ʹ�ú���ָ������-----------------

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



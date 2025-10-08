#include <time.h>
#include "RelyBase.h"
#include "printd.h"


int DbgClear(void *ptDev)
{
	int vRet;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

    vRet = ptInfo->fnClear(ptDev);

    return vRet;
}

int DbgWrite(void *ptDev, const void *szBuf, int vLen)
{
	int vRet;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

    vRet = ptInfo->fnWrite(ptDev, szBuf, vLen);

    return vRet;
}

int DbgRead(void *ptDev, void *szBuf, int vLen, int vToMs)
{
	int vRet;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

    vRet = ptInfo->fnRead(ptDev, szBuf, vLen, vToMs);

    return vRet;
}

int DbgClose(void *ptDev)
{
    int vRet;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

    vRet = ptInfo->fnClose(ptDev);

    return vRet;
}

int DbgPrint(void *ptDev, const char *strFmt, ...)
{
	va_list args;
	int printed;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;
	char szLog[ptInfo->vFmtMax];

	va_start(args, strFmt);
	printed = vsnprintf(szLog, sizeof(szLog)-1, strFmt, args);
	va_end(args);

	printed = ptInfo->fnWrite(ptDev, szLog, 0);

	return printed;
}

// �Զ��������ô�ӡ��Ϣͷ
int DbgPrintInfo(void *ptDev, const char *strFile, const char *strFunc, const int vLine)
{
    int vRet = 0;
    time_t vTime;
    struct tm* ptTime = NULL;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

    if(ptInfo->vTimeFlg)
    {
        time(&vTime);
        ptTime = localtime(&vTime);
        vRet += DbgPrint(ptDev, "[%d:%d:%d]", ptTime->tm_hour, ptTime->tm_min, ptTime->tm_sec);
    }
    if(ptInfo->vFileFlg || ptInfo->vFuncFlg || ptInfo->vLineFlg)
    {
        if(ptInfo->vLineFlg == 0)
            vRet += DbgPrint(ptDev, "[%s,%s,]",
                (ptInfo->vFileFlg == 0) ? "" : strFile,
                (ptInfo->vFuncFlg == 0) ? "" : strFunc);
        else
            vRet += DbgPrint(ptDev, "[%s,%s,%d]",
                (ptInfo->vFileFlg == 0) ? "" : strFile,
                (ptInfo->vFuncFlg == 0) ? "" : strFunc,
                vLine);
    }
    // ��ͷ����Ϣ������>>��ʾ��
    if(vRet > 0)
    {
        vRet += ptInfo->fnWrite(ptDev, ">> ", 0);
    }

    return vRet;
}

// ����
int DbgFeed(void *ptDev, unsigned int vLf)
{
    int i;
    int vRet = 0;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

    for(i=0; i<vLf; i++)
    {
        vRet += ptInfo->fnWrite(ptDev, _LF_STR_, 0);
    }

    return vRet;
}

/**********************************************************************
* Procedure: DW_Printh
* Function:  ��ʮ��������������е���������
* Parameter: vLf--������
			szTip--��ʾ��Ϣ
			szAddr--HEX���ݵ�ַ
			vLen--HEX���ݳ��ȣ���λΪ1,2,4��
			vAlign--������뷽ʽ(��ѡֵΪ1,2,4��
* Result:   ������Ϣ����
* Description: ���ݳ���Ҫ����뷽ʽƥ��
***********************************************************************
* LOG DATE      AUTHOR  ACTION
***********************************************************************
* <20211027>    <ccq>   <����>
**********************************************************************/
int DbgPrintHex(void *ptDev, unsigned int vLf, const char *szTip, void* szAddr, unsigned int vLen, unsigned int vAlign)
{
	unsigned int iCnt = 0;
	int iRet = 0;
	CDbgInfo *ptInfo = (CDbgInfo*)ptDev;

	if((szAddr == NULL) || (vLen == 0) || ((vAlign != 1) && (vAlign != 2) && (vAlign != 4)))
		return 0;

    // tip
    if(szTip != NULL){
        DbgPrint(ptDev, "[%s]", szTip);
    }

	// hex
	for(iCnt=0; iCnt<vLen; iCnt++)
	{
		if(vAlign == 2)
		{
			if(iCnt % 12 == 0 && iCnt != 0)
				iRet += ptInfo->fnWrite(ptDev, _LF_TAB_, 0);
			iRet += DbgPrint(ptDev, "%04x ", *((unsigned short*)szAddr+iCnt));
		}
		else if(vAlign == 4)
		{
			if(iCnt % 8 == 0 && iCnt != 0)
				iRet += ptInfo->fnWrite(ptDev, _LF_TAB_, 0);
			iRet += DbgPrint(ptDev, "%08x ", *((unsigned int*)szAddr+iCnt));
		}
		else
		{
			if(iCnt % 16 == 0 && iCnt != 0)
				iRet += ptInfo->fnWrite(ptDev, _LF_TAB_, 0);
			iRet += DbgPrint(ptDev, "%02x ", *((unsigned char*)szAddr+iCnt));
		}
	}
    // ����
    iRet += DbgFeed(ptDev, vLf);

	return iRet;
}


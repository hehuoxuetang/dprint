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

// 自动根据配置打印信息头
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
    // 有头部信息则增加>>提示符
    if(vRet > 0)
    {
        vRet += ptInfo->fnWrite(ptDev, ">> ", 0);
    }

    return vRet;
}

// 换行
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
* Function:  按十六进制输出缓存中的数据内容
* Parameter: vLf--换行数
			szTip--提示信息
			szAddr--HEX数据地址
			vLen--HEX数据长度（单位为1,2,4）
			vAlign--输出对齐方式(可选值为1,2,4）
* Result:   调试信息长度
* Description: 数据长度要与对齐方式匹配
***********************************************************************
* LOG DATE      AUTHOR  ACTION
***********************************************************************
* <20211027>    <ccq>   <新增>
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
    // 换行
    iRet += DbgFeed(ptDev, vLf);

	return iRet;
}


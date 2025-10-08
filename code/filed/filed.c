#include "FiledIn.h"

#include "printd.h"
#include "filed.h"


int Filed_Clear(void *pvDev)
{
	int vRet;
	CDbgFiled *ptFiled = (CDbgFiled*)pvDev;

	vRet = FS_Truncate(ptFiled->tInfo.vHd, 0);

    return vRet;
}

int Filed_Close(void *pvDev)
{
	int vRet;
	CDbgFiled *ptFiled = (CDbgFiled*)pvDev;

	vRet = FS_Close(ptFiled->tInfo.vHd);

    return vRet;
}

// 用于字符串输出
int Filed_Write(void *pvDev, const void *szBuf, int vLen)
{
    RET_START
    int size, index;
    int vRes;
	CDbgFiled *ptFiled = (CDbgFiled*)pvDev;

    size = (vLen == 0) ? strlen(szBuf) : vLen;

    index = FS_Seek(ptFiled->tInfo.vHd, 0, FS_SEEK_END);
    if(index + size > ptFiled->vFileMax){
        FS_Truncate(ptFiled->tInfo.vHd, 0);
		rset(-1);
    }
    vRes = FS_Write(ptFiled->tInfo.vHd, (void*)szBuf, size);
	rjudge(vRes != size, -1);

	RD_LAB
    return vRes;
}

// iToMs：作为文件偏移使用
int Filed_Read(void *pvDev, void *szBuf, int vLen, int vToMs)
{
    RET_START
    int index;
    int vRes = 0;
	CDbgFiled *ptFiled = (CDbgFiled*)pvDev;

    index = FS_Seek(ptFiled->tInfo.vHd, vToMs, FS_SEEK_SET);
    rjudge(index != vToMs, -1);
    vRes = FS_Read(ptFiled->tInfo.vHd, szBuf, vLen);
	rjudge(vRes != vLen, -2);

	RD_LAB
    return vRes;
}

CDbgFiled Filed_Init(const char *strName, int vSize)
{
    RET_START
    CDbgFiled tDev = {0};
	int vFd;

	vFd = FS_Open(strName, FS_MODE_WRITE);
	rjudge(vFd <= 0, -1);
	// 设备信息初始化
	tDev.tInfo.vHd = vFd;
	tDev.vFileMax = vSize;
	// 初始化函数指针
    tDev.tInfo.fnClose = Filed_Close;
    tDev.tInfo.fnWrite = Filed_Write;
    tDev.tInfo.fnRead = Filed_Read;
	tDev.tInfo.fnClear = Filed_Clear;
	// 配置项初始化
	tDev.tInfo.vFmtMax = DBG_FMT_SIZE;
    tDev.tInfo.vTimeFlg = 0;
    tDev.tInfo.vFileFlg = 0;
    tDev.tInfo.vFuncFlg = 0;
    tDev.tInfo.vLineFlg = 0;

    RD_LAB
    return tDev;
}



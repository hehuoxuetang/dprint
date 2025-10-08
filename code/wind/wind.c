#include <conio.h>
#include "WindIn.h"

#include "wind.h"


int Wind_Close(void *ptDev)
{
    return 0;
}

// �����ַ������
int Wind_Write(void *ptDev, const void *szData, int vLen)
{
    int i, vSize;
    char *szBuf = (char*)szData;

	vSize = vLen == 0 ? strlen(szBuf) : vLen;

    for(i=0; i<vSize; i++)
        putchar(szBuf[i]);

    return vSize;
}

int Wind_Read(void *ptDev, void *szData, int vLen, int vToMs)
{
    int i, vSize;
    char *szBuf = (char*)szData;

	vSize = vLen == 0 ? strlen(szBuf) : vLen;

    for(i=0; i<vSize; i++)
        szBuf[i] = getch();

    return vSize;
}

CDbgWind Wind_Init(void)
{
    CDbgWind tDev = {0};

	// �豸��Ϣ��ʼ��
	tDev.tInfo.vHd = 0;
	// ��ʼ������ָ��
    tDev.tInfo.fnClose = Wind_Close;
    tDev.tInfo.fnWrite = Wind_Write;
    tDev.tInfo.fnRead = Wind_Read;
	// �������ʼ��
	tDev.tInfo.vFmtMax = DBG_FMT_SIZE;
    tDev.tInfo.vTimeFlg = 0;
    tDev.tInfo.vFileFlg = 0;
    tDev.tInfo.vFuncFlg = 1;
    tDev.tInfo.vLineFlg = 0;

    return tDev;
}



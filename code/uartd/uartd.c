#include "UartdIn.h"

#include "uartd.h"

// ������������ʾ���ڳ�ʼ���Ľ��
static int UartdBeep(int vCnt)
{
	int i;
	// ��beep
    for(i=0; i<vCnt; i++)
    {
        dsleep(500);
        dbeep();
    }

	return 0;
}

/**********************************************************************
* ����: UartdOpen
* ����: ���ڳ�ʼ��
* ����: vPort--����ID
*		vBufSize--���ջ����С
* ����: ��С��0--�ɹ���0���򿪳ɹ���1���Ѿ��Ǵ�״̬��
*		С��0ֵ--ʧ�ܡ�-1����ʧ�ܡ�
* ˵��: �Եײ�Ĵ���open�ӿڽ��м򻯣��Է���ʹ�á�
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-20	CCQ		����
**********************************************************************/
int UartdOpen(int vPort, int vBufSize)
{
	RET_START
    uart_t tAttr;
    int vRes;

    rjudge(Uart_IsOpen(vPort), 1);
    // ����Ĭ�����ô�
    memset(&tAttr,0,sizeof(uart_t));
    tAttr.uBaudRateIndex=BAUDRATE_115200;   //������
    tAttr.uParityIndex=PARITY_NO;           //��żУ��λ
    tAttr.uRxBufSize=vBufSize;       //���ջ�������С����λBYTE
    tAttr.uStopBit=1;
    tAttr.uDataBits=8;
    vRes = Uart_Open(vPort, &tAttr);
    rdjudge(vRes != 0, -1);

    R_END
}

int Uartd_Open(int vPort, int vSize)
{
	RET_START
    int vRes, vCnt;

    vRes = UartdOpen(vPort, vSize);
	// ��������ʾ
	vCnt = vRes == 1 ? 1 : (vRes == 0 ? 2 : 3);
	UartdBeep(vCnt);

	rjudge(vRes < 0, -1);
	// ���ڴ�ӡ��ʾ: ���㿴����ʼ���Ľ��
    dsleep(5*1000);
    Uart_StrOut(vPort, "uartd open!\r\n");

	RD_END
}

int Uartd_Close(void *ptDev)
{
	CDbgUartd *ptUartd = (CDbgUartd*)ptDev;

	if(Uart_IsOpen(ptUartd->tInfo.vHd))
	{
		Uart_Close(ptUartd->tInfo.vHd);
	}

    return 0;
}

// �����ַ������
int Uartd_Write(void *ptDev, const void *szBuf, int iLen)
{
	RET_START
    int vRes;
    int vInLen;
	int vOutlen = 0;
	CDbgUartd *ptUartd = (CDbgUartd*)ptDev;

	// �豸�򿪼��
	vRes = Uart_IsOpen(ptUartd->tInfo.vHd);
    rjudge(vRes == 0, -1);

    vInLen = (iLen == 0) ? strlen(szBuf) : iLen;
    vOutlen = Uart_Write(ptUartd->tInfo.vHd, (unsigned char*)szBuf, vInLen);
    rjudge(vInLen != vOutlen, -2);

	RD_LAB
    return vOutlen;
}

int Uartd_Read(void *ptDev, void *szBuf, int iLen, int iToMs)
{
	RET_START
    int vRes;
	int vReadLen = 0;
	CDbgUartd *ptUartd = (CDbgUartd*)ptDev;

	// �豸�򿪼��
	vRes = Uart_IsOpen(ptUartd->tInfo.vHd);
    rjudge(vRes == 0, -1);

	vReadLen = Uart_Read(ptUartd->tInfo.vHd, (unsigned char*)szBuf, iLen, iToMs);
    rjudge(iLen != vReadLen, -2);

	RD_LAB
    return vReadLen;
}

CDbgUartd Uartd_Init(int vPort, int vSize)
{
	RET_START
    CDbgUartd tDev = {0};
	int vRes;

	vRes = Uartd_Open(vPort, vSize);
	rjudge(vRes != 0, -1);
	// �豸��Ϣ��ʼ��
	tDev.tInfo.vHd = vPort;
	tDev.vBufSize = vSize;
	// ��ʼ������ָ��
    tDev.tInfo.fnClose = Uartd_Close;
    tDev.tInfo.fnWrite = Uartd_Write;
    tDev.tInfo.fnRead = Uartd_Read;
	// �������ʼ��
	tDev.tInfo.vFmtMax = DBG_FMT_SIZE;
    tDev.tInfo.vTimeFlg = 0;
    tDev.tInfo.vFileFlg = 0;
    tDev.tInfo.vFuncFlg = 1;
    tDev.tInfo.vLineFlg = 0;

    RD_LAB
    return tDev;
}



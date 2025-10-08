#include "UartdIn.h"

#include "uartd.h"

// 蜂鸣，用于提示串口初始化的结果
static int UartdBeep(int vCnt)
{
	int i;
	// 响beep
    for(i=0; i<vCnt; i++)
    {
        dsleep(500);
        dbeep();
    }

	return 0;
}

/**********************************************************************
* 函数: UartdOpen
* 功能: 串口初始化
* 参数: vPort--串口ID
*		vBufSize--接收缓存大小
* 返回: 不小于0--成功。0，打开成功；1，已经是打开状态。
*		小于0值--失败。-1，打开失败。
* 说明: 对底层的串口open接口进行简化，以方便使用。
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-20	CCQ		创建
**********************************************************************/
int UartdOpen(int vPort, int vBufSize)
{
	RET_START
    uart_t tAttr;
    int vRes;

    rjudge(Uart_IsOpen(vPort), 1);
    // 采用默认设置打开
    memset(&tAttr,0,sizeof(uart_t));
    tAttr.uBaudRateIndex=BAUDRATE_115200;   //波特率
    tAttr.uParityIndex=PARITY_NO;           //奇偶校验位
    tAttr.uRxBufSize=vBufSize;       //接收缓冲区大小，单位BYTE
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
	// 蜂鸣器提示
	vCnt = vRes == 1 ? 1 : (vRes == 0 ? 2 : 3);
	UartdBeep(vCnt);

	rjudge(vRes < 0, -1);
	// 串口打印提示: 方便看到初始化的结果
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

// 用于字符串输出
int Uartd_Write(void *ptDev, const void *szBuf, int iLen)
{
	RET_START
    int vRes;
    int vInLen;
	int vOutlen = 0;
	CDbgUartd *ptUartd = (CDbgUartd*)ptDev;

	// 设备打开检查
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

	// 设备打开检查
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
	// 设备信息初始化
	tDev.tInfo.vHd = vPort;
	tDev.vBufSize = vSize;
	// 初始化函数指针
    tDev.tInfo.fnClose = Uartd_Close;
    tDev.tInfo.fnWrite = Uartd_Write;
    tDev.tInfo.fnRead = Uartd_Read;
	// 配置项初始化
	tDev.tInfo.vFmtMax = DBG_FMT_SIZE;
    tDev.tInfo.vTimeFlg = 0;
    tDev.tInfo.vFileFlg = 0;
    tDev.tInfo.vFuncFlg = 1;
    tDev.tInfo.vLineFlg = 0;

    RD_LAB
    return tDev;
}



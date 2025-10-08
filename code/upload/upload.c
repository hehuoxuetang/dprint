#include "RelyBase.h"
#include "libapi/CtrlUart.h"

#include "printd.h"


static int Upload_UartOpen(int vPort, int vBufSize)
{
	RET_START
    uart_t tAttr;
    int vRes;

    rjudge(Uart_IsOpen(vPort), 1);
    // 采用默认设置打开
    memset(&tAttr,0,sizeof(uart_t));
    tAttr.uBaudRateIndex=BAUDRATE_115200;   //波特率
    tAttr.uParityIndex=PARITY_NO;           //奇偶校验位
    tAttr.uRxBufSize=vBufSize;       		//接收缓冲区大小，单位BYTE
    tAttr.uStopBit=1;
    tAttr.uDataBits=8;
    vRes = Uart_Open(vPort, &tAttr);
    rdjudge(vRes != 0, -1);

    R_END
}

// 通过串口将日志文件发送出去
int Upload_ByUart(void *pvDev, int vUartId)
{
	RET_START
	// 一次最大发送128个字节
	unsigned char szSend[128];
	int vReaded, vIndex;
	int vRes;

	// 打开串口
	vRes = Upload_UartOpen(vUartId, 64);
	rjudge(vRes < 0, -1);
	// 开始上传
	vIndex = 0;
	while(1){
		vReaded = DbgRead(pvDev, szSend, sizeof(szSend), vIndex);
		vIndex += vReaded;
		if(vReaded != sizeof(szSend)){
			if(vReaded > 0){
				Uart_Write(vUartId, szSend, vReaded);
			}
			break;
		}
		else{
			Uart_Write(vUartId, szSend, vReaded);
		}
	}
	// 串口为本函数打开的才做关闭处理
	if(vRes == 0)
		Uart_Close(vUartId);

	RD_LAB
	return vIndex;
}


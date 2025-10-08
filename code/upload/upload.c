#include "RelyBase.h"
#include "libapi/CtrlUart.h"

#include "printd.h"


static int Upload_UartOpen(int vPort, int vBufSize)
{
	RET_START
    uart_t tAttr;
    int vRes;

    rjudge(Uart_IsOpen(vPort), 1);
    // ����Ĭ�����ô�
    memset(&tAttr,0,sizeof(uart_t));
    tAttr.uBaudRateIndex=BAUDRATE_115200;   //������
    tAttr.uParityIndex=PARITY_NO;           //��żУ��λ
    tAttr.uRxBufSize=vBufSize;       		//���ջ�������С����λBYTE
    tAttr.uStopBit=1;
    tAttr.uDataBits=8;
    vRes = Uart_Open(vPort, &tAttr);
    rdjudge(vRes != 0, -1);

    R_END
}

// ͨ�����ڽ���־�ļ����ͳ�ȥ
int Upload_ByUart(void *pvDev, int vUartId)
{
	RET_START
	// һ�������128���ֽ�
	unsigned char szSend[128];
	int vReaded, vIndex;
	int vRes;

	// �򿪴���
	vRes = Upload_UartOpen(vUartId, 64);
	rjudge(vRes < 0, -1);
	// ��ʼ�ϴ�
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
	// ����Ϊ�������򿪵Ĳ����رմ���
	if(vRes == 0)
		Uart_Close(vUartId);

	RD_LAB
	return vIndex;
}


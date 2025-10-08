#ifndef  __UARTD_H__
#define  __UARTD_H__

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// ���ڵ��Ծ��
typedef struct
{
    CDbgInfo	tInfo;
	// ���ջ����С
	int			vBufSize;
}CDbgUartd;

//------------------------------EXTERN_PARAMETER_END--------------------------------//
	
// �Ժ�����ķ�ʽ�򻯵���
#define Uart_StrOut(vId, pStr)      		Uart_Write(vId, (unsigned char*)(pStr), strlen(pStr))
#define Uart_ReadForever(vId, szBuf, uLen)  Uartd_Read(vId, szBuf, uLen, DBG_TO_MAX)


int Uartd_Close(void *ptDev);
int Uartd_Write(void *ptDev, const void *szBuf, int iLen);
int Uartd_Read(void *ptDev, void *szBuf, int iLen, int iToMs);


//------------------------------EXTERN_FUNCTION_START--------------------------------//

CDbgUartd Uartd_Init(int vPort, int vSize);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

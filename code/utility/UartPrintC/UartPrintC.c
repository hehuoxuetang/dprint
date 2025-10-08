#include "ConsoleIn.h"
#include "Tickd.h"
#include "uartd.h"

#include "UartPrintC.h"


CDbgUartd s_tUpHd = {0};

/**********************************************************************
* ����: UP_Init
* ����: WinPrintCģ���ʼ��
* ����: vId--
*		vSize--��
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����
**********************************************************************/
int UP_Init(int vPort, int vSize)
{
    s_tUpHd = Uartd_Init(vPort, vSize);
	// Ĭ��֧��tick����
    Tickd_Init(&s_tUpHd);
	// ���ӿ���̨
    Console_Init(&s_tUpHd);
    UP_Printf(1, "port console: %d", vPort, vSize);

    return 0;
}



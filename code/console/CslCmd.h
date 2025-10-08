#ifndef _CONSOLE_CMD_H_
#define _CONSOLE_CMD_H_


// ָ����С
#define CMD_BUF_SIZE              256

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// ָ�������ظ�
#define CMD_FLAG_REPEAT		0x01
// ָ������������
#define CMD_CFG_MAXARGS		16

// ������ʵ��
typedef	int (*FCmdEntry) (void *pvThis, int vFlag, int argc, char *argv[]);

//------------------------------EXTERN_PARAMETER_END--------------------------------//

// ָ����
typedef struct {
	const char		*strName;      	// ָ����
	int             vMaxArgs;		// ָ�������������ֵ
	int				vRepeatFlag;	// �ظ���־
	FCmdEntry		fnCmdDeal;		// ָ��ִ�к���
	const char		*strHelp;		// ������Ϣ
	int				vEnableFlag;	// ʹ�ܱ�־
}CCmdItem;

// ָ���б�
typedef struct _tagCmdList
{
    struct _tagCmdList	*ptNext;
    CCmdItem			tCmdItem;
}CCmdList;


int CslCmd_Init(void);

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// ���Կ���
void Console_Echo(int vFlag);
// ע��ͷ���ָ��
int Console_RegisterCmd(const char *strName, int vMaxArgs, int vRepeatFlag, FCmdEntry fnCmdDeal, char *strHelp);
int Console_UnregisterCmd(const char *strName);
// ָ������
int Console_Loop(void);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

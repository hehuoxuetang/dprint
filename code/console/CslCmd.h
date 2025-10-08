#ifndef _CONSOLE_CMD_H_
#define _CONSOLE_CMD_H_


// 指令缓存大小
#define CMD_BUF_SIZE              256

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// 指令允许重复
#define CMD_FLAG_REPEAT		0x01
// 指令最大参数个数
#define CMD_CFG_MAXARGS		16

// 处理函数实例
typedef	int (*FCmdEntry) (void *pvThis, int vFlag, int argc, char *argv[]);

//------------------------------EXTERN_PARAMETER_END--------------------------------//

// 指令项
typedef struct {
	const char		*strName;      	// 指令名
	int             vMaxArgs;		// 指令参数允许的最大值
	int				vRepeatFlag;	// 重复标志
	FCmdEntry		fnCmdDeal;		// 指令执行函数
	const char		*strHelp;		// 帮助信息
	int				vEnableFlag;	// 使能标志
}CCmdItem;

// 指令列表
typedef struct _tagCmdList
{
    struct _tagCmdList	*ptNext;
    CCmdItem			tCmdItem;
}CCmdList;


int CslCmd_Init(void);

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// 回显开关
void Console_Echo(int vFlag);
// 注册和放弃指令
int Console_RegisterCmd(const char *strName, int vMaxArgs, int vRepeatFlag, FCmdEntry fnCmdDeal, char *strHelp);
int Console_UnregisterCmd(const char *strName);
// 指令流程
int Console_Loop(void);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

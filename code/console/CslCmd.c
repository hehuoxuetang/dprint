#include "ConsoleIn.h"

#include "CslCmd.h"


// erase sequence
const char erase_seq[] = "\b \b";
// used to expand TABs
const char tab_seq[] = "        ";


// 把所有全局变量归集
typedef struct
{
	char		strBuf[CMD_BUF_SIZE];	// 指令缓存
	char		strLast[CMD_BUF_SIZE];	// 用于缓存最后一条指令
	CCmdList	*ptList;				// 指令列表
	int			vEchoFlag;				// 回显标志：0关闭，1打开
	int			vOpenFlag;				// 打开标志：0关闭，1打开(用于多线程环境)
}CCmdVar;
static CCmdVar s_tCmdVar = {{0}, {0}, NULL, 0, 0};


// 回显接口
#define Console_EchoPrintf(...)		do{if(s_tCmdVar.vEchoFlag != 0) Console_Printf(__VA_ARGS__);}while(0)


/**********************************************************************
* 函数: Console_Echo
* 功能: 回显设置
* 参数: vFlag--0关闭，1打开
* 返回: 无
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建
**********************************************************************/
void Console_Echo(int vFlag)
{
	s_tCmdVar.vEchoFlag = vFlag;
}

/**********************************************************************
* 函数: Console_Echo
* 功能: 回显设置
* 参数: strName--指令名
* 返回: 指令项指针。NULL表示失败
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建
**********************************************************************/
static CCmdItem* Console_FindCmdItem (char *strName)
{
	CCmdList *ptList = s_tCmdVar.ptList;
	CCmdItem *ptItem = NULL;

	while(ptList != NULL)
	{
		if (strcmp (strName, ptList->tCmdItem.strName) == 0)
		{
			ptItem = &ptList->tCmdItem;
			break;
		}
		ptList = ptList->ptNext;
	}

	return ptItem;
}

static char * delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if (*np == 0) {
		return (p);
	}

	if (*(--p) == '\t') {			/* will retype the whole line	*/
		while (*colp > plen) {
			Console_Printf (erase_seq);
			(*colp)--;
		}
		for (s=buffer; s<p; ++s) {
			if (*s == '\t') {
				Console_Printf (tab_seq+((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
			} else {
				++(*colp);
				Console_Printf ("%c",*s);
			}
		}
	} else {
		Console_Printf (erase_seq);
		(*colp)--;
	}
	(*np)--;
	*p = '\0';
	return (p);
}

static int parse_line (char *line, char *argv[])
{
	int nargs = 0;

	while (nargs < CMD_CFG_MAXARGS) {

		/* skip any white space */
		while ((*line == ' ') || (*line == '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;

			return (nargs);
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && (*line != ' ') && (*line != '\t')) {
			++line;
		}

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;

			return (nargs);
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	return (nargs);
}

/**********************************************************************
* 函数: Console_ReadLine
* 功能: 读取一行
* 参数: 无
* 返回: 读到的一行长度。大于0表示成功，其它失败。
* 说明: 从console读取到buffer
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建LOG
**********************************************************************/
static int Console_ReadLine(void)
{
	char *p = s_tCmdVar.strBuf;
	int	n = 0;				/* buffer index		*/
	int	col =0;				/* output column cnt	*/
	int plen =0;
	char c;

	//获取buffer相关信息
	while((*p) !='\0')
	{
		if (*p == '\t')
		{
			col += 8 - ((col) & 07);
		}
		else
		{
			++(col);
		}
		n++;
		p++;
	}

	while(1)
	{
		if(Console_Read(&c,1) != 1)
			break;

		switch (c)
		{
			case '\r':				/* Enter		*/
			case '\n':
				*p = '\0';
				Console_EchoPrintf("\r\n");
				return (p - s_tCmdVar.strBuf);

			case '\0':				/* nul			*/
				continue;

			case 0x03:				/* ^C - break		*/
				s_tCmdVar.strBuf[0] = '\0';	/* discard input */
				return (-1);

			case 0x15:				/* ^U - erase line	*/
				while (col > plen) {
					Console_EchoPrintf(erase_seq);
					--col;
				}
				p = s_tCmdVar.strBuf;
				n = 0;
				continue;

			case 0x17:				/* ^W - erase word 	*/
				p=delete_char(s_tCmdVar.strBuf, p, &col, &n, plen);
				while ((n > 0) && (*p != ' ')) {
					p=delete_char(s_tCmdVar.strBuf, p, &col, &n, plen);
				}
				continue;

			case 0x08:				/* ^H  - backspace	*/
			case 0x7F:				/* DEL - backspace	*/
				p=delete_char(s_tCmdVar.strBuf, p, &col, &n, plen);
				continue;

			default:
				/*
				 * Must be a normal character then
				 */
				if (n < CMD_BUF_SIZE-2) {
					if (c == '\t') {	/* expand TABs		*/
						Console_EchoPrintf(tab_seq+(col&07));
						col += 8 - (col&07);
					} else {
						++col;		/* echo input		*/
						Console_EchoPrintf("%c",c);
					}
					*p++ = c;
					++n;
				}
				else
				{			/* Buffer full		*/
					Console_EchoPrintf("%c",'\a');
				}
		}
	}
	return -1;
}

static int Console_CmdRun(char *cmd, int flag)
{
	CCmdItem *cmdtp;
	char cmdbuf[CMD_BUF_SIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char *str = cmdbuf;
	char *argv[CMD_CFG_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CMD_BUF_SIZE) {
		Console_Printf("## Command too long!\r\n");
		return -1;
	}

	strcpy (cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
			    (*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
			    (*sep == ';') &&	/* separator		*/
			    ( sep != str) &&	/* past string start	*/
			    (*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */

		/* find macros in this token and replace them */
//		process_macros (token, finaltoken);

		/* Extract arguments */
		if ((argc = parse_line (token, argv)) == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		/* Look up command in command table */
		if ((cmdtp = Console_FindCmdItem(argv[0])) == NULL) {
			Console_Printf ("## Unknown command '%s' - try 'help': -1\r\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
		}

		/* found - check max args */
		if (argc > cmdtp->vMaxArgs) {
			Console_Printf ("## The Maxargs is %d!\r\n", cmdtp->vMaxArgs);
			rc = -1;
			continue;
		}

		/* OK - call function to do the command */
		if(cmdtp->fnCmdDeal != NULL)
		{
			if ((cmdtp->fnCmdDeal) (cmdtp, flag, argc, argv) != 0) {
				rc = -1;
			}
			//ConsolePrintf ("\r\n");
			Console_EchoPrintf("\r\n");
		}

		repeatable &= cmdtp->vRepeatFlag;

		/* Did the user stop this? */
//		if (had_ctrlc ())
//			return 0;	/* if stopped then not repeatable */
	}

	return rc ? rc : repeatable;

}

static int Console_CmdProc(void)
{
	int iLen;
	int flag=0;
	int rc;
	iLen = Console_ReadLine();
	if (iLen > 0)
	{
		strcpy (s_tCmdVar.strLast, s_tCmdVar.strBuf);
		memset(s_tCmdVar.strBuf, 0, CMD_BUF_SIZE);
	}
	else if (iLen == 0)
	{
		flag |= CMD_FLAG_REPEAT;
	}
	if(iLen >= 0)
	{
		rc = Console_CmdRun(s_tCmdVar.strLast,flag);
		if (rc <= 0) {
			// invalid command or not repeatable, forget it
			s_tCmdVar.strLast[0] = 0;
		}
	}
	return 0;
}


static int Console_CmdRegister(CCmdItem *ptItem)
{
	CCmdList *ptCmdList;

	ptCmdList = malloc(sizeof(CCmdList));
	if(ptCmdList != NULL)
	{
		memcpy(&ptCmdList->tCmdItem, ptItem, sizeof(CCmdItem));
		ptCmdList->ptNext = s_tCmdVar.ptList;
		s_tCmdVar.ptList = ptCmdList;
	}

	return (ptCmdList != NULL)? 0 : -1;
}



////////////////////////////////////////////////////////////////////////////////
// 内置指令实现
///////////////////////////////////////////////////////////////////////////////

/**********************************************************************
* 函数: Console_HelpEntry
* 功能: 内置“help”指令处理函数
* 参数: pvThis--输出句柄
*		vFlag--指令缓存大小
*		argc--参数项个数
*		argv--参数项内容
* 返回: 0值--成功
*		非0--错误类型
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建LOG
**********************************************************************/
static int Console_HelpEntry (void *pvThis, int vFlag, int argc, char *argv[])
{
	int i;
	int iRet = 0;
	CCmdItem *ptCmdItem = (CCmdItem*)pvThis;
	CCmdList *ptCmdList = s_tCmdVar.ptList;

	if (argc == 1)
	{
		while(ptCmdList != NULL)
		{
			Console_Printf("%s -- ",ptCmdList->tCmdItem.strName);
			Console_Printf("%s\r\n",ptCmdList->tCmdItem.strHelp);
			ptCmdList = ptCmdList->ptNext;
		}

		return -1;
	}
	// command help (long version)
	for (i = 1; i < argc; ++i) {
		if ((ptCmdItem = Console_FindCmdItem(argv[i])) != NULL) {
			if (ptCmdItem->strHelp)
				Console_Printf ("%s \r\n", ptCmdItem->strHelp);
		}
		else {
			Console_Printf ("Unknown command '%s' - try 'help'"
							" without arguments for list of all"
							" known commands\r\n", argv[i]
						);
			iRet = -1;
		}
	}

	return iRet;
}

/**********************************************************************
* 函数: Console_HelpEntry
* 功能: 内置“echo”指令处理函数
* 参数: pvThis--输出句柄
*		vFlag--指令缓存大小
*		argc--参数项个数
*		argv--参数项内容
* 返回: 0值--成功
*		非0--错误类型
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建LOG
**********************************************************************/
int Console_EchoEntry (void *pvThis, int vFlag, int argc, char *argv[])
{
    int iRet = 0;

    if (argc == 2)
    {
        if(strcmp (argv[1], "on") == 0)
        {
            Console_Echo(1);
            Console_Printf("echo on: ok!\r\n");
        }
        else if(strcmp (argv[1], "off") == 0)
        {
            Console_Echo(0);
            Console_Printf("echo off: ok!\r\n");
        }
        else
        {
            Console_Printf("not support param: %s\r\n", argv[1]);
        }
    }
    else
    {
        iRet = -1;
        Console_Printf("this cmd must have 2 argc\r\n");
    }

    return iRet;
}

/**********************************************************************
* 函数: Console_ExitEntry
* 功能: 退出指令模式
* 参数: pvThis--输出句柄
*		vFlag--指令缓存大小
*		argc--参数项个数
*		argv--参数项内容
* 返回: 0值--成功
*		非0--错误类型
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建LOG
**********************************************************************/
int Console_ExitEntry (void *pvThis, int vFlag, int argc, char *argv[])
{
    int iRet = 0;

    if (argc == 1)
    {
        s_tCmdVar.vOpenFlag = 0;
    }
    else
    {
        iRet = -1;
        Console_Printf ("this cmd have not argc\r\n");
    }

    return iRet;
}


////////////////////////////////////////////////////////////////////////////////
// 主要对外接口
///////////////////////////////////////////////////////////////////////////////

int Console_RegisterCmd(const char *strName, int vMaxArgs, int vRepeatFlag, FCmdEntry fnCmdDeal, char *strHelp)
{
	CCmdItem tCmdItem;

	tCmdItem.strName = strName;
	tCmdItem.vMaxArgs = vMaxArgs;
	tCmdItem.vRepeatFlag = vRepeatFlag;
	tCmdItem.fnCmdDeal = fnCmdDeal;
	tCmdItem.strHelp = strHelp;
	tCmdItem.vEnableFlag = 0;

	return Console_CmdRegister(&tCmdItem);
}

int Console_UnregisterCmd(const char *strName)
{
	CCmdList *ptListItemScanPre = NULL;
	CCmdList *ptListItemScan = NULL;
	CCmdList *ptListItem = NULL;
	int iRet = -1;

	if(strcmp(strName,  s_tCmdVar.ptList->tCmdItem.strName) == 0)
	{
		// 是第一个
		// 修正指针
		ptListItem = s_tCmdVar.ptList;
		s_tCmdVar.ptList = s_tCmdVar.ptList->ptNext;

		// 释放掉空间
		free(ptListItem);

		iRet = 0;
	}
	else
	{
		// 不是第一个
		ptListItemScanPre = s_tCmdVar.ptList;
		ptListItemScan = s_tCmdVar.ptList->ptNext;

		// 查找
		while(ptListItemScan != NULL)
		{
			if (strcmp (strName, ptListItemScan->tCmdItem.strName) == 0)
			{
				ptListItem = ptListItemScan;
				break;
			}
			ptListItemScanPre = ptListItemScan;
			ptListItemScan = ptListItemScan->ptNext;
		}

		if(ptListItem != NULL)
		{
			// 修正指针
			ptListItemScanPre->ptNext = ptListItem->ptNext;
			// 释放掉空间
			free(ptListItem);

			iRet = 0;
		}
	}

	return iRet;
}

/**********************************************************************
* 函数: Console_Loop
* 功能: 控制台指令循环
* 参数: 无
* 返回: 0值
* 说明:
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建
**********************************************************************/
int Console_Loop(void)
{
    s_tCmdVar.vOpenFlag = 1;
	while(s_tCmdVar.vOpenFlag != 0)
	{
		Console_CmdProc();
	}

	return 0;
}

/**********************************************************************
* 函数: Console_Init
* 功能: 控制台初始化
* 参数: ptHd--输出句柄
*		vSize--指令缓存大小
* 返回: 0值--成功
*		非0--错误类型
* 说明: 暂不考虑重复调用的情况（会引起内存泄漏）
*		暂不考虑多线程的情况
***********************************************************************
* 日期			作者		动作
***********************************************************************
* 2022-05-09	CCQ		创建
**********************************************************************/
int CslCmd_Init(void)
{
	memset(&s_tCmdVar, 0, sizeof(CCmdVar));

	// 注册内置指令
	Console_RegisterCmd("help",	CMD_CFG_MAXARGS, CMD_FLAG_REPEAT, Console_HelpEntry, "print help\r\n");
    Console_RegisterCmd("echo", CMD_CFG_MAXARGS, CMD_FLAG_REPEAT, Console_EchoEntry, "echo on/off, echo off\r\n");
    Console_RegisterCmd("exit", CMD_CFG_MAXARGS, CMD_FLAG_REPEAT, Console_ExitEntry, "exit cmd func\r\n");

    return 0;
}


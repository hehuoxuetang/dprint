#include "ConsoleIn.h"

#include "CslCmd.h"


// erase sequence
const char erase_seq[] = "\b \b";
// used to expand TABs
const char tab_seq[] = "        ";


// ������ȫ�ֱ����鼯
typedef struct
{
	char		strBuf[CMD_BUF_SIZE];	// ָ���
	char		strLast[CMD_BUF_SIZE];	// ���ڻ������һ��ָ��
	CCmdList	*ptList;				// ָ���б�
	int			vEchoFlag;				// ���Ա�־��0�رգ�1��
	int			vOpenFlag;				// �򿪱�־��0�رգ�1��(���ڶ��̻߳���)
}CCmdVar;
static CCmdVar s_tCmdVar = {{0}, {0}, NULL, 0, 0};


// ���Խӿ�
#define Console_EchoPrintf(...)		do{if(s_tCmdVar.vEchoFlag != 0) Console_Printf(__VA_ARGS__);}while(0)


/**********************************************************************
* ����: Console_Echo
* ����: ��������
* ����: vFlag--0�رգ�1��
* ����: ��
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����
**********************************************************************/
void Console_Echo(int vFlag)
{
	s_tCmdVar.vEchoFlag = vFlag;
}

/**********************************************************************
* ����: Console_Echo
* ����: ��������
* ����: strName--ָ����
* ����: ָ����ָ�롣NULL��ʾʧ��
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����
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
* ����: Console_ReadLine
* ����: ��ȡһ��
* ����: ��
* ����: ������һ�г��ȡ�����0��ʾ�ɹ�������ʧ�ܡ�
* ˵��: ��console��ȡ��buffer
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����LOG
**********************************************************************/
static int Console_ReadLine(void)
{
	char *p = s_tCmdVar.strBuf;
	int	n = 0;				/* buffer index		*/
	int	col =0;				/* output column cnt	*/
	int plen =0;
	char c;

	//��ȡbuffer�����Ϣ
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
// ����ָ��ʵ��
///////////////////////////////////////////////////////////////////////////////

/**********************************************************************
* ����: Console_HelpEntry
* ����: ���á�help��ָ�����
* ����: pvThis--������
*		vFlag--ָ����С
*		argc--���������
*		argv--����������
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����LOG
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
* ����: Console_HelpEntry
* ����: ���á�echo��ָ�����
* ����: pvThis--������
*		vFlag--ָ����С
*		argc--���������
*		argv--����������
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����LOG
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
* ����: Console_ExitEntry
* ����: �˳�ָ��ģʽ
* ����: pvThis--������
*		vFlag--ָ����С
*		argc--���������
*		argv--����������
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����LOG
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
// ��Ҫ����ӿ�
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
		// �ǵ�һ��
		// ����ָ��
		ptListItem = s_tCmdVar.ptList;
		s_tCmdVar.ptList = s_tCmdVar.ptList->ptNext;

		// �ͷŵ��ռ�
		free(ptListItem);

		iRet = 0;
	}
	else
	{
		// ���ǵ�һ��
		ptListItemScanPre = s_tCmdVar.ptList;
		ptListItemScan = s_tCmdVar.ptList->ptNext;

		// ����
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
			// ����ָ��
			ptListItemScanPre->ptNext = ptListItem->ptNext;
			// �ͷŵ��ռ�
			free(ptListItem);

			iRet = 0;
		}
	}

	return iRet;
}

/**********************************************************************
* ����: Console_Loop
* ����: ����ָ̨��ѭ��
* ����: ��
* ����: 0ֵ
* ˵��:
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����
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
* ����: Console_Init
* ����: ����̨��ʼ��
* ����: ptHd--������
*		vSize--ָ����С
* ����: 0ֵ--�ɹ�
*		��0--��������
* ˵��: �ݲ������ظ����õ�������������ڴ�й©��
*		�ݲ����Ƕ��̵߳����
***********************************************************************
* ����			����		����
***********************************************************************
* 2022-05-09	CCQ		����
**********************************************************************/
int CslCmd_Init(void)
{
	memset(&s_tCmdVar, 0, sizeof(CCmdVar));

	// ע������ָ��
	Console_RegisterCmd("help",	CMD_CFG_MAXARGS, CMD_FLAG_REPEAT, Console_HelpEntry, "print help\r\n");
    Console_RegisterCmd("echo", CMD_CFG_MAXARGS, CMD_FLAG_REPEAT, Console_EchoEntry, "echo on/off, echo off\r\n");
    Console_RegisterCmd("exit", CMD_CFG_MAXARGS, CMD_FLAG_REPEAT, Console_ExitEntry, "exit cmd func\r\n");

    return 0;
}


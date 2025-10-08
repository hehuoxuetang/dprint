#include "DispdIn.h"

#include "dispd.h"


static CDispdVar s_tDispdVar = {0, 0, NULL, NULL};


static int DispdOpen(void)
{
    unsigned int uLineSpace, uFontWidth, uFontHeight;
    unsigned int uViewWidth, uViewHeight;
    unsigned int uSize, i;

    uLineSpace = Disp_FontGet(FONT_TYPE_LINE_SPACE);
    uFontWidth = Disp_FontGet(FONT_TYPE_SIZE_WIDTH);
    uFontHeight = Disp_FontGet(FONT_TYPE_SIZE_HEIGHT);
    uViewWidth = Disp_GetGoPara( GO_VIEW_WIDTH);
    uViewHeight = Disp_GetGoPara( GO_VIEW_HEIGHT);
    // 每行容纳字符数
    s_tDispdVar.vMaxChar = uViewWidth / uFontWidth;
    // 行数: 最后一行不占行间距空间
    s_tDispdVar.vMaxLine = (uViewHeight + uLineSpace) / (uFontHeight + uLineSpace);

    // 申请显示缓存:保留字串结束符的空间
    uSize = (s_tDispdVar.vMaxChar+1)*(s_tDispdVar.vMaxLine+1);
    s_tDispdVar.pTextHead = malloc(uSize);
    memset(s_tDispdVar.pTextHead, 0, uSize);
	// 申请行文本指针
    uSize = s_tDispdVar.vMaxLine*sizeof(unsigned int);
    s_tDispdVar.pLineText = malloc(uSize);
    for(i=0; i<s_tDispdVar.vMaxLine; i++)
    {
        s_tDispdVar.pLineText[i] = s_tDispdVar.pTextHead+i*(s_tDispdVar.vMaxChar+1);
    }
	// 当前行缓存
	s_tDispdVar.pText = s_tDispdVar.pTextHead+i*(s_tDispdVar.vMaxChar+1);
	s_tDispdVar.vIndex = 0;

    return 0;
}

// 一次追加一行
static int DispdWriteLine(const char *pBuf, int vLen)
{
    int size, i;
    char *pText = NULL;

    // 更新显示内容
    size = (vLen == 0) ? strlen(pBuf) : vLen;
    pText = s_tDispdVar.pLineText[0];
    for(i=0; i<s_tDispdVar.vMaxLine-1; i++)
    {
        s_tDispdVar.pLineText[i] = s_tDispdVar.pLineText[i+1];
    }
    if(size > s_tDispdVar.vMaxChar)
    {
        size = s_tDispdVar.vMaxChar;
    }
    memcpy(pText, pBuf, size);
    pText[size] = '\0';
    s_tDispdVar.pLineText[s_tDispdVar.vMaxLine-1] = pText;
    // 显示debug信息
    Disp_ClearScreen();
    for(i=0; i<s_tDispdVar.vMaxLine; i++)
    {
        Disp_WinTextOut(i, s_tDispdVar.pLineText[i]);
    }

    return 0;
}

int Dispd_Write(void *pvDev, const void *szBuf, int vLen)
{
	const char *szData = szBuf;
	int i, vSize;

    vSize = (vLen == 0) ? strlen(szData) : vLen;
	for(i=0; i<vSize; i++){
		if(szData[i] == '\r'){
			if(szData[i+1] == '\n'){
				i += 1;
			}
			DispdWriteLine(s_tDispdVar.pText, s_tDispdVar.vIndex);
			s_tDispdVar.vIndex = 0;
		}
		else if(szData[i] == '\n'){
			DispdWriteLine(s_tDispdVar.pText, s_tDispdVar.vIndex);
			s_tDispdVar.vIndex = 0;
		}
		else{
			if(s_tDispdVar.vIndex < s_tDispdVar.vMaxChar){
				s_tDispdVar.pText[s_tDispdVar.vIndex++] = szData[i];
			}
		}
	}

    return 0;
}

int Dispd_Read(void *pvDev, void *szBuf, int vLen, int vToMs)
{
	return 0;
}

int Dispd_Close(void *pvDev)
{
    if(s_tDispdVar.pLineText != NULL)
    {
        free(s_tDispdVar.pLineText);
        s_tDispdVar.pLineText = NULL;
    }
    if(s_tDispdVar.pTextHead != NULL)
    {
        free(s_tDispdVar.pTextHead);
        s_tDispdVar.pTextHead = NULL;
    }

    return 0;
}

CDbgDispd Dispd_Init(void)
{
    RET_START
	int vRes;
    CDbgDispd tDev = {0};

	vRes = DispdOpen();
    rjudge(vRes != 0, -1);
	// 设备信息初始化
	tDev.tInfo.vHd = 0;
	// 初始化函数指针
    tDev.tInfo.fnClose = Dispd_Close;
    tDev.tInfo.fnWrite = Dispd_Write;
    tDev.tInfo.fnRead = Dispd_Read;
	// 配置项初始化
	tDev.tInfo.vFmtMax = DBG_FMT_SIZE;
    tDev.tInfo.vTimeFlg = 0;
    tDev.tInfo.vFileFlg = 0;
    tDev.tInfo.vFuncFlg = 0;
    tDev.tInfo.vLineFlg = 0;

    RD_LAB
    return tDev;
}


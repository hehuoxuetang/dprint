#ifndef _DISP_H
#define _DISP_H


// 先只支持单行显示,后续考虑支持跨行
typedef struct
{
    // 一行支持的字符数
    unsigned int    vMaxChar;
    // 一屏最多支持几行
    unsigned int    vMaxLine;
    // 全屏文本缓存
    char            *pTextHead;
    // 行文本指针：分行指向 全屏文本缓存
    char            **pLineText;
	// 当前行文本缓存
	char			*pText;
	// 当前行文本索引
	unsigned int	vIndex;
}CDispdVar;

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// 显示调试句柄
typedef struct
{
    CDbgInfo	tInfo;
}CDbgDispd;

//------------------------------EXTERN_PARAMETER_END--------------------------------//

//------------------------------EXTERN_FUNCTION_START--------------------------------//

CDbgDispd Dispd_Init(void);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

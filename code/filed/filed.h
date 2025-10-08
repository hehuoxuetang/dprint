#ifndef _FILED_H
#define _FILED_H

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// 文件调试句柄
typedef struct
{
    CDbgInfo	tInfo;
	// 文件允许的最大值
	int			vFileMax;
}CDbgFiled;

//------------------------------EXTERN_PARAMETER_END--------------------------------//

//------------------------------EXTERN_FUNCTION_START--------------------------------//

CDbgFiled Filed_Init(const char *strName, int vSize);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

#ifndef _FILED_H
#define _FILED_H

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// �ļ����Ծ��
typedef struct
{
    CDbgInfo	tInfo;
	// �ļ���������ֵ
	int			vFileMax;
}CDbgFiled;

//------------------------------EXTERN_PARAMETER_END--------------------------------//

//------------------------------EXTERN_FUNCTION_START--------------------------------//

CDbgFiled Filed_Init(const char *strName, int vSize);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

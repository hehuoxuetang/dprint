#ifndef _DISP_H
#define _DISP_H


// ��ֻ֧�ֵ�����ʾ,��������֧�ֿ���
typedef struct
{
    // һ��֧�ֵ��ַ���
    unsigned int    vMaxChar;
    // һ�����֧�ּ���
    unsigned int    vMaxLine;
    // ȫ���ı�����
    char            *pTextHead;
    // ���ı�ָ�룺����ָ�� ȫ���ı�����
    char            **pLineText;
	// ��ǰ���ı�����
	char			*pText;
	// ��ǰ���ı�����
	unsigned int	vIndex;
}CDispdVar;

//------------------------------EXTERN_PARAMETER_START--------------------------------//

// ��ʾ���Ծ��
typedef struct
{
    CDbgInfo	tInfo;
}CDbgDispd;

//------------------------------EXTERN_PARAMETER_END--------------------------------//

//------------------------------EXTERN_FUNCTION_START--------------------------------//

CDbgDispd Dispd_Init(void);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

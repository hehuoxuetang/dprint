#ifndef _DBG_PRINT_H
#define	_DBG_PRINT_H


// Ĭ�ϸ�ʽ�������С
#define DBG_FMT_SIZE            1024

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// ���ʱʱ��
#define DBG_TO_MAX		(0x7fffffff)

// ������ָ������
typedef int (*FDbgClear)(void *ptDev);
typedef int (*FDbgRead)(void *ptDev, void *szBuf, int vLen, int vToMs);
typedef int (*FDbgWrite)(void *ptDev, const void *szBuf, int vLen);
typedef int (*FDbgClose)(void *ptDev);

// ���ýṹ
typedef struct
{
	// ���
	union
	{
	    int		vHd;
		void	*pvHd;
	};
	// ����ָ��
	FDbgClear	fnClear;
	FDbgRead    fnRead;
    FDbgWrite   fnWrite;
	FDbgClose   fnClose;
    // ������
    int         vFmtMax  : 28;	//log��ʽ����������
    int         vTimeFlg : 1;
    int         vFileFlg : 1;
    int         vFuncFlg : 1;
    int         vLineFlg : 1;
}CDbgInfo;


int DbgClear(void *ptDev);
int DbgWrite(void *ptDev, const void *szBuf, int vLen);
int DbgRead(void *ptDev, void *szBuf, int vLen, int vToMs);
int DbgClose(void *ptDev);

// ��ʽ�����
int DbgPrint(void *ptDev, const char *strFmt, ...);

// ��ӡ�ļ��������������кŵ���Ϣ
int DbgPrintInfo(void *ptDev, const char *strFile, const char *strFunc, const int vLine);
// ���hex
int DbgPrintHex(void *ptDev, unsigned int vLf, const char *strTip, void* szAddr, unsigned int vLen, unsigned int vAlign);
// ����
int DbgFeed(void *ptDev, unsigned int vLf);

// ����ʹ�õĺ�����
#define DbgPrintf(pvDev, vLf, ...)   \
            ({ \
                DbgPrintInfo(pvDev, __FILE__, __FUNCTION__, __LINE__); \
                DbgPrint(pvDev, ##__VA_ARGS__);  \
                DbgFeed(pvDev, vLf);  \
            })

#define DbgPrinta(pvDev, vLf, szBuf, vLen)	\
            ({ \
                DbgPrintInfo(pvDev, __FILE__, __FUNCTION__, __LINE__); \
                DbgWrite(pvDev, szBuf, vLen);  \
                DbgFeed(pvDev, vLf);  \
            })

#define DbgPrinth(pvDev, vLf, strTip, szAddr, vLen, vAlign)	\
            ({ \
                DbgPrintInfo(pvDev, __FILE__, __FUNCTION__, __LINE__); \
                DbgPrintHex(pvDev, vLf, strTip, szAddr, vLen, vAlign);  \
            })
			
//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

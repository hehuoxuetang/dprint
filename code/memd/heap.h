#ifndef _HEAPD_H
#define _HEAPD_H


// һ���������ʽ
// �����鷽ʽ�Ļ���Ϊ������Ŷ�̬�ڴ棬Ҫ����ȫ������
//#define HP_ARRAY_CNT	100

// �ڴ��ļ�¼��ʽ
#define HP_STORE_LINK	0	// ����ʽ
#define HP_STORE_ARRAY	1	// ���鷽ʽ


// �쳣������
typedef void (*FMemdErr)(void);

typedef struct _tagHpBlock
{
	// ���뵽���ڴ��ַ
    void    	*pvAddr;
	// ������ڴ��С
    int			vSize;
	// �����ڴ�ĺ���
    const char  *strName;
	// �����ڴ溯�����к�
    int			vLineNo;
	// ��һ���ڴ��
	struct _tagHpBlock	*ptNext;
}CHpBlock;

typedef struct
{
	// ����������������׷�ӵķ�ʽ�����ֻҪ��¼�����ַ����
    CHpBlock	*ptNew;
	// ��̬�ڴ�ı仯����ֵ�����ͷŵı�����Ķ�
	int			vCurSize;		// ��̬�ڴ�ĵ�ǰֵ
	int			vMaxSize;		// ��̬�ڴ�����ֵ
	// ������־
	int			vEnable;
}CHpSec;

typedef struct
{
    // ��¼ȫ�ֶ��ڴ�ʹ�õķ�ֵ
    int		vHeapMax;
    // ��¼ȫ�ֶ��ڴ�ʹ�õĵ�ǰֵ
    int		vHeapCur;
	// �����������
	int		vCntMax;
	// ����ǰ����
	int		vCntCur;
}CHpCalc;

typedef struct
{
	// ���ã�����ص�����
	FMemdErr	fnMemdErr;
	// ���뵫δ�ͷ��ڴ��
	CHpBlock	*ptHead;
	// �����ڴ��
	CHpBlock	*ptErr;
	// ͳ��ֵ
	CHpCalc		tCalc;
	// ��¼����
//	int			vFlag;
}CHpVar;

//------------------------------EXTERN_FUNCTION_START--------------------------------//

// ��ʼ�������Ծ������
int Hpd_Init(void);

// �����ӿ�
void* Hpd_MallocDeal(int uSize, const char* pcFuncName, int uLineNo);
void  Hpd_FreeDeal(void* pvBuf, const char* pcFuncName, int uLineNo);
void* Hpd_ReallocDeal(void* pvBuf, int uSize, const char* pcFuncName, int uLineNo);

int Hpd_PrInfo(void);


int Hpd_StartCount(void);
int Hpd_EndCount(void);

//------------------------------EXTERN_FUNCTION_END--------------------------------//


#endif

#ifndef __TICKD_H
#define __TICKD_H

// ������
#if defined(__ECLIPSE__)

unsigned int Sys_GetTickCount(void);
#define GetTickCount()  Sys_GetTickCount()

#elif defined(WIN32) 

__declspec (dllimport) unsigned long __stdcall GetTickCount(void);

#else

unsigned long GetTickCount(void);

#endif

// ����
typedef struct 
{
    unsigned int	vCount;
    void          	*pvDev;
}CTickdVar;


//------------------------------EXTERN_FUNCTION_START--------------------------------//

int Tickd_Init(void *pvDev);
void* Tickd_GetDev(void);
unsigned int Tickd_GetCount(void);

void Tickd_Start(void);
void Tickd_Step(void);
void Tickd_End(void);

// ----------����Ƕ��------------//

#define TICKD_START      Tickd_Start();
#define TICKD_STEP       do{DbgPrintInfo(Tickd_GetDev(), __FILE__, __FUNCTION__, __LINE__);Tickd_Step();}while(0);
#define TICKD_END        do{DbgPrintInfo(Tickd_GetDev(), __FILE__, __FUNCTION__, __LINE__);Tickd_End();}while(0);

// ----------����Ƕ��------------//

#define TICK_START      unsigned int vTickCnt = Tickd_GetCount();
// ���¿�ʼ��ʱ
#define TICK_RESTART    vTickCnt = Tickd_GetCount();
// ��ӡ��ǰ��ʱֵ�����¿�ʼ��ʱ
#define TICK_STEP       do{DbgPrintf(Tickd_GetDev(), 1, "tick ms: %lu", Tickd_GetCount()-vTickCnt);vTickCnt=Tickd_GetCount();}while(0);
#define TICK_END        DbgPrintf(Tickd_GetDev(), 1, "tick ms: %lu", Tickd_GetCount()-vTickCnt);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

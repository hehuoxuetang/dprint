#include "printd.h"
#include "tickd.h"


// ����
static CTickdVar s_tTickdVar = {0};


int Tickd_Init(void *pvDev)
{
    s_tTickdVar.pvDev = pvDev;
    s_tTickdVar.vCount = GetTickCount();

    return 0;
}

// ������Ϣ���
#define TickdPrintf(fmt, ...)   \
			({ \
                DbgPrint(s_tTickdVar.pvDev, fmt, ##__VA_ARGS__);  \
                DbgFeed(s_tTickdVar.pvDev, 1);  \
            })
// ����tick��
#define TickdCount()            (GetTickCount()-s_tTickdVar.vCount)


void* Tickd_GetDev(void)
{
    return s_tTickdVar.pvDev;
}

unsigned int Tickd_GetCount(void)
{
    return (unsigned int)GetTickCount();
}

void Tickd_Start(void)
{
    s_tTickdVar.vCount = Tickd_GetCount();
}

void Tickd_Step(void)
{
    TickdPrintf("tick: %d", TickdCount());
    s_tTickdVar.vCount = Tickd_GetCount();
}

void Tickd_End(void)
{
    TickdPrintf("tick: %d", TickdCount());
}


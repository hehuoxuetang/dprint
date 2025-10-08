#include <string.h>
#include "memd.h"
#include "stack.h"


typedef struct
{
    unsigned char	*szStack;
	int				vMaxSize;
}CStackd;
static CStackd s_tStackd = {NULL, 0};


int Stack_StartCount(int vMaxSize)
{
    unsigned char szStack[vMaxSize];

    s_tStackd.szStack = szStack;
    s_tStackd.vMaxSize = vMaxSize;
    memset(szStack, STACK_INIT_VAL, vMaxSize);

    return 0;
}

int Stack_EndCount(void)
{
    int i;
	int vUsedCnt;

	for(i=0; i<s_tStackd.vMaxSize; i++)
	{
		if(s_tStackd.szStack[i] != STACK_INIT_VAL){
			break;
		}
	}
	vUsedCnt = s_tStackd.vMaxSize - i;
	MdPrintf("stack used: %d", vUsedCnt);

    return vUsedCnt;
}

int Stack_Check(unsigned char *szStack, int vSize)
{
	int i;
	int vRet = 0;

	for(i=0; i<vSize; i++)
	{
		if(szStack[i] != STACK_INIT_VAL){
			MdPrintf("stack error!");
			vRet = -1;
			break;
		}
	}

	return vRet;
}


#include "input.h"


typedef struct
{
    int  BufX;
    int  BufSize;
    char *dbuf[2];
    unsigned short dind[2];
}CRamdVar;
static CRamdVar s_tDramInfo = {0, 0, {NULL, NULL}, {0, 0}};

#define s_dbuf  (s_tDramInfo.dbuf[s_tDramInfo.BufX])
#define s_dind  (s_tDramInfo.dind[s_tDramInfo.BufX])

#define DramReverseX(x)     ((x) == 0 ? 1 : 0)

int Ramd_Init(int size)
{
    RET_START
    char *ptr;

    memset(&s_tDramInfo, 0, sizeof(CRamdVar));

    ptr = malloc(size*2);
    rjudge(ptr == NULL, -1);
    memset(ptr, 0, size*2);

    s_tDramInfo.dbuf[0] = ptr;
    s_tDramInfo.dbuf[1] = ptr+size;
    s_tDramInfo.BufSize = size;

    RD_END
}

int Ramd_Write(const char *buf, int len)
{
    RET_START

    rjudge(len >= s_tDramInfo.BufSize, -1);

    if(s_dind + len >= s_tDramInfo.BufSize)
    {
        s_tDramInfo.BufX = DramReverseX(s_tDramInfo.BufX);
        memset(s_dbuf, 0, s_tDramInfo.BufSize);
        s_dind = 0;
    }
    memcpy(s_dbuf + s_dind, buf, len);
    s_dind += len;

    RD_END;
}

int Ramd_Read(int index, CData *ptInfo)
{
    int pre;

    pre = DramReverseX(s_tDramInfo.BufX);
    if(index == 0)
    {
        ptInfo->ptr = (unsigned char*)(s_tDramInfo.dbuf[pre]);
        ptInfo->len = s_tDramInfo.dind[pre];
    }
    else
    {
        ptInfo->ptr = (unsigned char*)s_dbuf;
        ptInfo->len = s_dind;
    }

    return 0;
}

int Ramd_Deinit(void)
{
	if(s_tDramInfo.dbuf[0] != NULL)
		free(s_tDramInfo.dbuf[0]);

	return 0;
}


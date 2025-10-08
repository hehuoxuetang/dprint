#ifndef _RELY_MEM_H
#define _RELY_MEM_H


void* Sys_Malloc(unsigned int uSize);
void  Sys_Free(void* pBuf);
void* Sys_Realloc(void* pBuf, unsigned int uSize);

unsigned int Sys_SleepMs(unsigned int uMs);

#endif

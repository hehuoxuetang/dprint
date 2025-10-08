#ifndef _CONSOLE_INPUT_H
#define _CONSOLE_INPUT_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "dutil.h"

#include "printd.h"

// IO¾ä±ú
extern void *s_ptCslHd;

// »ù´¡¶ÁÐ´½Ó¿Ú
#define Console_Printf(...)			DbgPrint(s_ptCslHd, __VA_ARGS__)
#define Console_Printl(...)			do{DbgPrint(s_ptCslHd, __VA_ARGS__);DbgFeed(s_ptCslHd, 1);}while(0)
#define Console_Feed(vLf)			DbgFeed(s_ptCslHd, vLf)

#define Console_Read(szBuf, vLen)	DbgRead(s_ptCslHd, szBuf, vLen, DBG_TO_MAX)

//------------------------------EXTERN_FUNCTION_START--------------------------------//

int Console_Init(void *ptHd);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

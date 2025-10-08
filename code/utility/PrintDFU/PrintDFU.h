#ifndef __PRINT_DFU_H
#define __PRINT_DFU_H


//------------------------------EXTERN_FUNCTION_START--------------------------------//

extern CDbgDispd s_tDispHd;
extern CDbgFiled s_tFileHd;
extern CDbgUartd s_tUartHd;

int DispP_Init(void);
int FileP_Init(const char *strName, int vMaxSize);
int UartP_Init(int vPort, int vSize);

int UP_Init(int vPort);

#define DispP_Printf(vLf, ...)			DbgPrintf(&s_tDispHd, vLf, __VA_ARGS__)

#define FileP_Feed(vLf)                 DbgFeed(&s_tFileHd, vLf)
#define FileP_Printf(vLf, ...)          DbgPrintf(&s_tFileHd, vLf, __VA_ARGS__)
#define FileP_Printh(vLf, strTip, szAddr, vLen, vAlign)    DbgPrinth(&s_tFileHd, vLf, strTip, szAddr, vLen, vAlign)
#define FileP_UpByUart(vPort)			Upload_ByUart(&s_tFileHd, vPort)
#define FileP_Clear()					DbgClear(&s_tFileHd)

#define UartP_Printf(vLf, ...)             DbgPrintf(&s_tUartHd, vLf, __VA_ARGS__)

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

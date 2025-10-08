#ifndef __FILE_PRINT_H
#define __FILE_PRINT_H


//------------------------------EXTERN_FUNCTION_START--------------------------------//

extern CDbgFiled s_tUpHd;

int UP_Init(const char *strName, int vMaxSize);
int UP_FileU(int vPort);


#define UP_Feed(vLf)                    DbgFeed(&s_tUpHd, vLf)

#define UP_Printf(vLf, ...)             DbgPrintf(&s_tUpHd, vLf, __VA_ARGS__)
#define UP_Printa(vLf, szBuf, vLen)     DbgPrinta(&s_tUpHd, vLf, szBuf, vLen)
#define UP_Printh(vLf, strTip, szAddr, vLen, vAlign)    DbgPrinth(&s_tUpHd, vLf, strTip, szAddr, vLen, vAlign)

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

#ifndef __WIN_PRINT_C_H
#define __WIN_PRINT_C_H


//------------------------------EXTERN_FUNCTION_START--------------------------------//

extern CDbgWind s_tUpHd;

int UP_Init(void);

#define UP_Feed(vLf)                    DbgFeed(&s_tUpHd, vLf)

#define UP_Printf(vLf, ...)             DbgPrintf(&s_tUpHd, vLf, __VA_ARGS__)
#define UP_Printa(vLf, szBuf, vLen)     DbgPrinta(&s_tUpHd, vLf, szBuf, vLen)
#define UP_Printh(vLf, strTip, szAddr, vLen, vAlign)    DbgPrinth(&s_tUpHd, vLf, strTip, szAddr, vLen, vAlign)

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

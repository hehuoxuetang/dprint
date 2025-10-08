#ifndef _RAMD_H
#define _RAMD_H



int Ramd_Init(int size);
int Ramd_Write(const char *buf, int len);
int Ramd_Read(int index, CData *ptInfo);
int Ramd_Deinit(void);


#endif

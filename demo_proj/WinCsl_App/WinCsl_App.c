#include <stdio.h>
#include "dWinCsl.h"


int Sc_Open(int *pvPara)
{
	UP_Printf(1, "open");
	return 0;
}

int Sc_CheckIn(int *pvPara)
{
	UP_Printf(1, "check in");
	return 0;
}

int Sc_Reset(int *pvPara)
{
	UP_Printf(1, "reset");
	return 0;
}

int Sc_Comm(int *pvPara)
{
	UP_Printf(1, "comm");
	return 0;
}

int Sc_Close(int *pvPara)
{
	UP_Printf(1, "close");
	return 0;
}


const static CCmItem s_taScMenu[] = {
    {"SmartCard接口", {CM_STYLE_TITLE|5}, NULL},
    {"open接口",      {CM_STYLE_ITEM}, Sc_Open},
    {"CheckIn接口",   {CM_STYLE_ITEM}, Sc_CheckIn},
    {"Reset接口",     {CM_STYLE_ITEM}, Sc_Reset},
    {"Comm接口",      {CM_STYLE_ITEM}, Sc_Comm},
    {"Close接口",     {CM_STYLE_ITEM}, Sc_Close},
};

int main(int argc,char *argv[])
{
    UP_Init();

    //Console_Loop();
    Console_PopupMenu(s_taScMenu);

    return 0;
}


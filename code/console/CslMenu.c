#include "ConsoleIn.h"

#include "CslMenu.h"

#define CM_NUM_MAX      11
// key type
#define CM_KEY_NUM      0
#define CM_KEY_FUN      1

typedef struct
{
    char    strNum[CM_NUM_MAX+1];
    int     vNumLen;
}CCmVar;
static CCmVar   s_tCmVar = {{0}, 0};

int Console_ShowMenu(CCmItem *ptMenu)
{
	RET_START
	int i;
	CCmMenu *ptCur = (CCmMenu*)ptMenu;

	rjudge(ptCur->tTitle.tAttr.vType != Cm_GetStyle(CM_STYLE_TITLE), -1);

	Console_Feed(1);
	// 显示标题栏
	if(ptCur->tTitle.strText != NULL){
		Console_Printl("\t%s", ptCur->tTitle.strText);
	}
	// 显示菜单栏
	for(i=0; i<ptCur->tTitle.tAttr.vPara; i++){
		Console_Printl("%d.%s", i+1, ptCur->taItem[i].strText);
	}
	Console_Feed(1);

	RD_END
}

static int Console_Num(char vNum, int vAddFlag)
{
    int vRet = -1;

    if(vAddFlag != 0){
        if(s_tCmVar.vNumLen < CM_NUM_MAX){
            s_tCmVar.strNum[s_tCmVar.vNumLen++] = vNum;
            s_tCmVar.strNum[s_tCmVar.vNumLen] = '\0';
            vRet = 0;
        }
    }
    else{
        if(s_tCmVar.vNumLen > 0){
            s_tCmVar.vNumLen -= 1;
            s_tCmVar.strNum[s_tCmVar.vNumLen] = '\0';
            vRet = 0;
        }
    }

    return vRet;
}

int Console_MenuKey(int *pvKey, int *pvType)
{
	RET_START
	char vKey;
	int vRes;

    s_tCmVar.vNumLen = 0;

    Console_Printf("please input: ");
	while(1)
	{
		// Read key
		vRes = Console_Read(&vKey, 1);
		rjudge(vRes != 1, -1);

		// key deal
		if(vKey >= '0' && vKey <= '9'){
            Console_Num(vKey, 1);
            Console_Printf("%c", vKey);
		}
        else if(vKey == '\b'){
            Console_Num(0, 0);
            Console_Printf("\b \b");
        }
        else if(vKey == '\r' || vKey == '\n'){
            *pvKey = atoi(s_tCmVar.strNum);
            *pvType = CM_KEY_NUM;
            break;
        }
        else if(vKey == 'u' || vKey == 'e'){
            *pvKey = vKey;
            *pvType = CM_KEY_FUN;
            break;
        }
	}
    Console_Feed(1);

	RD_END
}

int Console_PopupMenu(const CCmItem *ptMenu)
{
	RET_START
	CCmMenu *ptCur = (CCmMenu*)ptMenu;
	CCmItem *ptItem = NULL;
	FCmFunc fnDeal = NULL;
	int vKey, vType;
	int vIndex, vRes;

	while(1){
        // show menu
		Console_ShowMenu((CCmItem*)ptCur);
		// get key value
		vRes = Console_MenuKey(&vKey, &vType);
		rjudge(vRes != 0, -1);
        // key deal
        if(vType == CM_KEY_NUM){
            vIndex = vKey > 0 ? vKey - 1 : ptCur->tTitle.tAttr.vPara;
            if(vIndex < ptCur->tTitle.tAttr.vPara){
                ptItem = &ptCur->taItem[vIndex];
                if(ptItem->tAttr.vType == Cm_GetStyle(CM_STYLE_GROUP)){
                    // 进入下一级目录
                    ptCur = ptItem->pvPara;
                }
                else if(ptItem->tAttr.vType == Cm_GetStyle(CM_STYLE_ITEM)){
                    // 进入处理函数
                    fnDeal = ptItem->pvPara;
                    fnDeal(&vKey);
                }
                else{
                    rend(-2);
                }
            }
        }
        else if(vType == CM_KEY_FUN){
            if(vKey == 'u'){
                if(ptCur->tTitle.pvPara != NULL){
                    if(ptCur->tTitle.pvPara != ptMenu){
                        ptCur = ptCur->tTitle.pvPara;
                    }
                }
            }
            else if(vKey == 'e'){
                rend(1);
            }
            else{
                rend(-3);
            }
        }
	}

	RD_END
}


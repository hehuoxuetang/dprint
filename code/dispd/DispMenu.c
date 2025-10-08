#include "DispdIn.h"

#include "DispMenu.h"


int Dispd_ShowMenu(CCmItem *ptMenu)
{
	RET_START
	int i;
	int vItemNum = 1;	// 菜单项的编号
	int vLineNum = 0;	// 菜单项所在行号
	char strText[64];
	CCmMenu *ptCur = (CCmMenu*)ptMenu;

	// 第一行是标题信息
	rjudge(ptCur->tTitle.tAttr.vType != Cm_GetStyle(CM_STYLE_TITLE), -1);

	// 显示标题栏
	if(ptCur->tTitle.strText != NULL){
		Disp_WinTextOutEx(0, ptCur->tTitle.strText, FONT_STYLE_MID);
		vLineNum = 1;
	}
	// 显示菜单栏
	for(i=0; i<ptCur->tTitle.tAttr.vPara; i++){
		sprintf(strText, "%d.%s", vItemNum++, ptCur->taItem[i].strText);
		Disp_WinTextOut(vLineNum++, strText);
	}

	RD_END
}

int Dispd_MenuKey(void)
{
	int vKey;

	// Read key
	vKey = GUI_WaitKeypad(0x7fffffff);

	return vKey;
}

int Dispd_PopupMenu(const CCmItem *ptMenu)
{
	RET_START
	CCmMenu *ptCur = (CCmMenu*)ptMenu;
	CCmItem *ptItem = NULL;
	FCmFunc fnDeal = NULL;
	int vKey;
	int vIndex;

	while(1){
        // show menu
		Dispd_ShowMenu((CCmItem*)ptCur);
		// get key value
		vKey = Dispd_MenuKey();
        // key deal
        if(vKey >= '0' && vKey <= '9'){
            vIndex = vKey == '0' ? 9 : vKey - '1';
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
                    rend(-1);
                }
            }
        }
        else {
            if(vKey == VK_STAR){
                if(ptCur->tTitle.pvPara != NULL){
                    if(ptCur->tTitle.pvPara != ptMenu){
                        ptCur = ptCur->tTitle.pvPara;
                    }
                }
            }
            else if(vKey == VK_CANCEL){
                rend(1);
            }
            else{
                rend(-2);
            }
        }
	}

	RD_END
}


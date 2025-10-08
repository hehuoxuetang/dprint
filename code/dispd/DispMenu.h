#ifndef _DISP_MENU_H_
#define _DISP_MENU_H_


//------------------------------EXTERN_PARAMETER_START--------------------------------//

// 参数
#define CM_PARA_MASK		(0xFF)
#define CM_PARA_SHIFT		(0)

// 类型
#define CM_STYLE_MASK		(0xFF00)
#define CM_STYLE_SHIFT		(8)

#define CM_STYLE_ITEM       (0x0000)      // 普通菜单项
#define CM_STYLE_GROUP      (0x0100)      // 普通菜单组
#define CM_STYLE_TITLE		(0x0200)      // 普通菜单标题

// 取参数值
#define Cm_GetPara(vPara)	((vPara)>>CM_PARA_SHIFT)
// 取风格类型
#define Cm_GetStyle(vStyle)	((vStyle)>>CM_STYLE_SHIFT)

// 菜单响应函数指针
typedef int (*FCmFunc)(int *pvPara);


typedef struct
{
	unsigned char	vPara;
	unsigned char	vType;
	unsigned short	vRsv;
}CCmAttr;

typedef struct
{
    char	*strText;
    union
    {
        unsigned int	vAttr;
        CCmAttr			tAttr;
    };
    void	*pvPara;
}CCmItem;

//------------------------------EXTERN_PARAMETER_END--------------------------------//

typedef struct
{
	CCmItem tTitle;
	CCmItem taItem[1];
}CCmMenu;

//------------------------------EXTERN_FUNCTION_START--------------------------------//

int Dispd_PopupMenu(const CCmItem *ptMenu);

//------------------------------EXTERN_FUNCTION_END--------------------------------//

#endif

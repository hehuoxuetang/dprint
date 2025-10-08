#ifndef _DISP_MENU_H_
#define _DISP_MENU_H_


//------------------------------EXTERN_PARAMETER_START--------------------------------//

// ����
#define CM_PARA_MASK		(0xFF)
#define CM_PARA_SHIFT		(0)

// ����
#define CM_STYLE_MASK		(0xFF00)
#define CM_STYLE_SHIFT		(8)

#define CM_STYLE_ITEM       (0x0000)      // ��ͨ�˵���
#define CM_STYLE_GROUP      (0x0100)      // ��ͨ�˵���
#define CM_STYLE_TITLE		(0x0200)      // ��ͨ�˵�����

// ȡ����ֵ
#define Cm_GetPara(vPara)	((vPara)>>CM_PARA_SHIFT)
// ȡ�������
#define Cm_GetStyle(vStyle)	((vStyle)>>CM_STYLE_SHIFT)

// �˵���Ӧ����ָ��
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

#ifndef _RELY_HCI_H_
#define _RELY_HCI_H_


//��ֵ
#define VK_INVALID              0xFF
#define VK_POWER                0x01        // ��Դ��
#define VK_CANCEL               0x02        // ȡ����
#define VK_DEL                  0x03        // �����
#define VK_OK                   0x04        // ȷ�ϼ�
#define VK_LEFT                 0x25        // �����
#define VK_UP                   0x26        // ���ϼ�
#define VK_RIGHT                0x27        // ���Ҽ�
#define VK_DOWN                 0x28        // ���¼�
#define VK_F3                   0x72        // ���ܼ�
#define VK_PAY                  0x87        // �տ��
#define VK_0                    ('0')       // 0x30 ~ 0x39
#define VK_1                    ('1')
#define VK_2                    ('2')
#define VK_3                    ('3')
#define VK_4                    ('4')
#define VK_5                    ('5')
#define VK_6                    ('6')
#define VK_7                    ('7')
#define VK_8                    ('8')
#define VK_9                    ('9')
#define VK_STAR                 ('*')       // 0x2a ���ϼ�
#define VK_SHARP                ('#')       // 0x23 ���¼�
#define VK_AT                   ('@')       // 0x40
#define VK_MENU                 0x12
#define KEY_INVALID             VK_INVALID



// CGdiObj�ṹ�Ĳ���ֵ
typedef enum{
    // λ���
    GO_CLR_BITS = 0,	// Һ��λ��
    GO_BMP_BITS,		// �Դ�λ��
    // �����ȡ�������
    GO_VIEW_LEFT,
    GO_VIEW_TOP,
    GO_VIEW_WIDTH,
    GO_VIEW_HEIGHT,
}EGdiObj;

// ������������
enum
{
    FONT_TYPE_FORE = 0,
    FONT_TYPE_BACK,
    FONT_TYPE_MODE,
    FONT_TYPE_SIZE,
    FONT_TYPE_ORDER,
    FONT_TYPE_LINE_TOP,
    FONT_TYPE_LINE_SPACE,
    FONT_TYPE_LINE_LEFT,
    FONT_TYPE_SIZE_WIDTH,
    FONT_TYPE_SIZE_HEIGHT
};


// ������: ����ԭ�����λ����
#define FONT_STYLE_COPY             0x0000
#define FONT_STYLE_INVERT           0x8000
#define FONT_STYLE_INVERT_MASK      0x8000
// ������
#define FONT_STYLE_SEQORDER         (0x0000)    // ����Ĭ�ϣ������ϵ���
#define FONT_STYLE_INVORDER         (0x4000)    // ���򣺴��µ���
#define FONT_STYLE_INVORDER_MASK    (0x4000)
// ���뷽ʽ
#define FONT_STYLE_LEFT             0x0000
#define FONT_STYLE_MID              0x1000
#define FONT_STYLE_RIGHT            0x2000
#define FONT_STYLE_ALIGN_MASK       0x3000
#define FONT_STYLE_ALIGN_SHIFT      12
// �����С
#define FONT_STYLE_DEF          	0x0000
#define FONT_STYLE_S                0x0100
#define FONT_STYLE_M                0x0200
#define FONT_STYLE_L                0x0300
#define FONT_STYLE_64               0x0400
#define FONT_STYLE_12               0x0500
#define FONT_STYLE_8                0x0600
#define FONT_STYLE_SIZE_MASK        0x0700
#define FONT_STYLE_SIZE_SHIFT       8
// �Ƿ�����
#define FONT_STYLE_LINE_COVER       0x0000
#define FONT_STYLE_LINE_CLEAR       0x0800
#define FONT_STYLE_CLEAR_MASK       0x0800


//ȷ��
#define MB_OK          0x0001
//ȡ��
#define MB_CANCEL      0x0002


int Disp_GetGoPara(int vItem);
int Disp_FontGet(unsigned int uType);
void Disp_ClearScreen(void);
int Disp_WinTextOut(unsigned int uLineNum, char *pcStr);
int Disp_WinTextOutEx(unsigned int uLineNum, char* pcStr, unsigned int uOpStyle);

int GUI_TipBox(char* pcTitle, char* pcText, char* pcTip, unsigned int uStyle, unsigned int uTimeOut);
unsigned int GUI_WaitKeypad(unsigned int uTimeout);


#endif

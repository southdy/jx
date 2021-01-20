/*****************************************************************************************
//	����ѡ��˵�
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-2-21
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

struct KPopupMenuItem
{
	char			szData[64];
	unsigned int	uDataLen;
	unsigned int	uBgColor;
	unsigned short	uID;
};

#define MENU_DEFAULT_POS				-607
#define	MENU_ITEM_DEFAULT_WIDTH			0
#define	MENU_ITEM_DEFAULT_HEIGHT		0
#define	MENU_ITEM_DEFAULT_RIGHT_WIDTH	-1
#define MENU_ITEM_DEFAULT_INDENT		255
#define MENU_ITEM_DEFAULT_TITLEUPSPACE	1

enum POPUPMENU_FLAG
{
	PM_F_HAVE_HEAD_TAIL_IMG		= 0x0001,		//�ڲ˵����ϵ�����ӱ߽�ͼ��
	PM_F_ZOOM_SELECTED_ITEM		= 0x0002,		//�Ŵ�ǰѡ�еĲ˵���
	PM_F_TAB_SPLIT_ITEM_TEXT	= 0x0004,		//tab�ַ��ָ�Ĳ˵���
	PM_F_HAVE_ITEM_SEPARATOR	= 0x0008,		//�˵���֮���зָ���
	PM_F_AUTO_DEL_WHEN_HIDE		= 0x0010,		//�˵���ʧ��ʱ���Զ�ɾ���˵�����
	PM_F_CANCEL_BY_CALLER		= 0x0020,		//�ɵ������������˵�����ʧ/�ر�
};

struct KPopupMenuData
{
	short			nX;					//�˵�����λ�õ�x����
	short			nY;					//�˵�����λ�õ�y����
	short			nXX;				//ԭʼ�Ĳ˵�����λ�õ�x����
	short			nYY;				//ԭʼ�Ĳ˵�����λ�õ�y����

	short			nItemWidth;			//�˵���Ŀ�ȣ���������Ŀ�ȣ�
	short			nItemRightWidth;	//�˵����Ҳ�����������Ŀ��
	short			nItemHeight;		//�˵���ĸ߶�
	short			nNumItem;			//�˵������Ŀ
	short			nSelectedItem;		//��ǰѡ�еĲ˵���
	unsigned short	usMenuFlag;			//�˵����Ե�һЩ��ǣ�Ϊö������POPUPMENU_FLAGȡֵ�����

	unsigned char	byFontSize;			//�˵�������Ĵ�С
	unsigned char	byItemTitleUpSpace;	//�˵�����������˵ļ������λ�����ص㣩
	short			nItemTitleIndent;	//�˵�����������

//	bool			bZoomSelectedItem;	//�Ƿ�Ŵ�ǰѡ�еĲ˵���
//	bool			bHaveHeadTailImg;	//�Ƿ��ڲ˵����ϵ�����ӱ߽�ͼ��
//	bool			bHaveSeparator;		//�˵����Ƿ��зָ���
//	bool			bAutoDelete;		//�Ƿ�˵���ʧ��ʱ���Զ�ɾ���˵�����
//	bool			bManualCancel;		//�Ƿ��ɵ������������Ƿ�˵���ʧ

	unsigned int	uBorderLineColor;	//�˵��ı߿���ɫ��Ϊ0����
	unsigned int	uSelItemBgColor;	//��ѡ�в˵���ı���ɫ
	unsigned int	uTextColor;			//�˵��������ɫ
	unsigned int	uTextBorderColor;	//�˵�������ֱ�Եɫ
	unsigned int	uSelTextColor;		//��ѡ�в˵��������ɫ
	unsigned int	uSelTextBorderColor;//��ѡ�в˵�������ֱ�Եɫ

	KPopupMenuItem	Items[1];			//�˵�������
};

#define	MENU_DATA_SIZE(nNumItem)	(sizeof(KPopupMenuData) + (nNumItem - 1) * sizeof(KPopupMenuItem))

class KWndWindow;
class KPopupMenu
{
public:
	static void	Popup(KPopupMenuData* pMenu, KWndWindow* pCaller, unsigned int uParam);
	static void Cancel();
	static void	PaintMenu();
	static void	LoadTheme(const char* pScheme);		//������淽��
	static int	HandleInput(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	static void OnWndDelete(KWndWindow* pWnd);
	static void	InitMenuData(KPopupMenuData* pMenu, int nNumItem);
	static KPopupMenuData*	GetMenuData() {	return m_pMenu;}
private:
	static bool SelectItem(int x, int y, bool bApply);
	static bool SelectItem(int nIndex, bool bApply);
private:
	static KPopupMenuData*  m_pMenu;
	static KWndWindow*		m_pCaller;
	static unsigned int		m_uCallerParam;
	
	static short			m_nImgWidth;	//ͼ��ˮƽ����
	static short			m_nImgHeight;	//ͼ�δ�ֱ�߶�
	static short			m_nIndent;
    static short			m_nFrame;
	static int				m_nMenuHeight;
	static char				m_szImage[128];	//ͼ���ļ���

	static unsigned int	m_uDefBorderLineColor;	//�˵��ı����ɫ��Ϊ0����
	static unsigned int	m_uDefItemBgColor;		//�˵���ı���ɫ
	static unsigned int	m_uDefSelItemBgColor;	//��ѡ�в˵���ı���ɫ
	static unsigned int	m_uDefTextColor;		//�˵��������ɫ
	static unsigned int	m_uDefTextBorderColor;	//�˵�������ֱ�Եɫ
	static unsigned int	m_uDefSelTextColor;		//��ѡ�в˵��������ɫ
	static unsigned int	m_uDefSelTextBorderColor;//��ѡ�в˵�������ֱ�Եɫ
};

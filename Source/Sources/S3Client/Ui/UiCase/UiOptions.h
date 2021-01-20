/*****************************************************************************************
//	����--ѡ�����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"
//#include "../../Engine/Src/LinkStruct.h"

struct KPopupMenuData;

enum	SWORD_ONLINE_OPTION_INDEX
{
	OPTION_I_START = 0,
	OPTION_I_DYNALIGHT = OPTION_I_START,	//��̬��Ӱ
	OPTION_I_WEATHER,						//��������
	OPTION_I_PERSPECTIVE,					//͸��ģʽ

	OPTION_INDEX_COUNT,
};

struct KToggleOptionItem
{
	char	szName[32];	//����
	bool	bInvalid;	//�����ã���Ч��
	short	bEnable;	//��ѡ���Ƿ�ѡ��
};

class KUiOptions : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiOptions*	OpenWindow(KWndWindow* pReturn = NULL);//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiOptions*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow();					//�رմ���
	static void			LoadScheme(const char* pScheme);//������淽��
	
	static void			LoadSetting(bool bReload, bool bUpdate);

//	void				SetPerspective(int);
//	void				SetDynaLight(int);
	void				SetMusicValue(int);
	void				SetSoundValue(int);
	void				SetBrightness(int);
//	void                SwitchWeather();
	void				ToggleOption(int nIndex);	//�л�������ѡ��

private:
	KUiOptions();
	~KUiOptions() {}
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	OnScrollBarPosChanged(KWndWindow* pWnd, int nPos);	//��Ӧ���������϶�
//	void	PopupSkinMenu();
	void	CancelMenu();
	void	StoreSetting();
	void	UpdateSettingSet(int eSet, bool bOnlyUpdateUi = false);
	void	Initialize();					// ��ʼ��
	void    PopupSeleteSetMenu(int nX, int nY);	//����ѡ�����÷����Ĳ˵�
	void	LoadScheme(KIniFile* pIni);	//������淽��
	void	UpdateAllToggleBtn();
	void	UpdateAllStatusImg();

private:
	static KUiOptions* m_pSelf;
private:
	KWndWindow* m_pReturn;

	// ��ť
	KWndButton		m_ShortcutKeyBtn;	//�򿪿�ݼ��趨����
	KWndButton		m_CloseBtn;			//�رհ�ť
//	KWndLabeledButton	m_SkinBtn;		//���淽����ť

	KWndScrollBar	m_BrightnessScroll;	//���ȵ��ڻ���
	KWndScrollBar	m_BGMValue;			//������������
	KWndScrollBar	m_SoundValue;		//��Ч��������
	KPopupMenuData*	m_pSkinMenu;

	KWndLabeledButton m_ShortcutSetView;//e...�����ʾ��ǰ��ѡ��Ŀ�ݼ�����

	int	m_nBrightness, m_nSoundValue, m_nMusicValue;
	int m_nShortcutSet;

#define	MAX_TOGGLE_BTN_COUNT	4
	KWndScrollBar		m_Scroll;
	KWndLabeledButton	m_ToggleBtn[MAX_TOGGLE_BTN_COUNT];
	KWndImage		m_StatusImage[MAX_TOGGLE_BTN_COUNT];
	unsigned int	m_uEnableTextColor;		//m_ToggleBtn�������ֵ���ɫ
	unsigned int	m_uDisableTextColor;	//m_ToggleBtn�������ֵ���ɫ
	unsigned int	m_uInvalidTextColor;	//m_ToggleBtn�������ֵ���ɫ
	int				m_nStatusEnableFrame;
	int				m_nStatusDisableFrame;
	int				m_nStatusInvalidFrame;

	KToggleOptionItem	m_ToggleItemList[OPTION_INDEX_COUNT];
	int					m_nFirstControlableIndex;
	int					m_nToggleBtnValidCount;
	int					m_nToggleItemCount;
};

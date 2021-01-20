/*****************************************************************************************
//	����--����������Ϣ����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-11
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"
#include "../Elem/PopupMenu.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Core/src/GameDataDef.h"

class KUiFastInputMsg : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiFastInputMsg* OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiFastInputMsg* GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void				CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				LoadScheme(const char* pScheme);//������淽��
	static void				Clear();
	static void				UpdateCurrentChannel();
private:
	KUiFastInputMsg();
	~KUiFastInputMsg();
	void	Show();									//��ʾ����
	void	Hide();									//���ش���
	void	OnSend();
	void	Initialize();							//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	SwitchLockMode();
	void	PaintWindow();
	void	PopupChannelMenu();
	void	PopupColorMenu();
	void	CancelMenu();
	void	SelectedColor(int x, int y);
	void	LoadScheme(class KIniFile* pIni);	//������淽��
	void	InputRecentMsg(bool bPrior);
private:
	static KUiFastInputMsg*	m_pSelf;
private:
	enum { UI_INPUT_MSG_MAX_COLOR = 12 };
	KWndButton	m_ChannelBtn;
	KWndButton	m_ColorBtn;
	KWndButton	m_SendBtn;
	KWndEdit512	m_InputEdit;
	KWndText32	m_ChannelName;
	unsigned int m_ColorList[UI_INPUT_MSG_MAX_COLOR];
	KRColor		m_CurColor;
	bool		m_bLocked;
	char		m_cNumColor;
	char		m_cPreMsgCounter;
	char		m_cLatestMsgIndex;
	bool		m_bChannelMenu;
	bool		m_bColorMenu;
#define	MAX_RECENT_MSG_COUNT	8
	char		m_RecentMsg[MAX_RECENT_MSG_COUNT][512];
	KPopupMenuData* m_pMenuData;
	KUiChatChannel* m_pChannelData;
};

/*****************************************************************************************
//	����--ѡ�񸽽���ĳ�����
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-2-26
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndEdit.h"

struct KPlayerInterAction
{
	char	szActionName[32];	//��Ϊ����
	int		nId;				//��Ϊid
};

struct KUiPlayerItem;

class KUiSelPlayerNearby : protected KWndShowAnimate
{
public:
	static KUiSelPlayerNearby*	OpenWindow();				//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static int					DoesHaveVisibleWnd();
	static void					CloseWindow(bool bDestroy);				//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void					LoadScheme(const char* pScheme);		//������淽��
private:
	KUiSelPlayerNearby();
	~KUiSelPlayerNearby() {}
	void	Initialize();					//��ʼ��
	void	LoadScheme(class KIniFile* pIni);//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	OnClickXButton(int nAction);
	void	UpdateData();
	void	Clear();
	void	LoadActionList(KIniFile* pSetting);
private:
	static KUiSelPlayerNearby*	m_pSelf;
	KUiPlayerItem*			m_pDataList;
	int						m_nPlayerCount;
	KWndList				m_PlayerList;
	KWndScrollBar			m_ListScroll;

#define	MAX_NUM_BUTTON		9
	KWndPureTextBtn			m_ActionBtns[MAX_NUM_BUTTON];
	KPlayerInterAction		m_ActionList[MAX_NUM_BUTTON];
	int						m_nActionCount;
		
	KWndButton				m_CancelBtn, m_RefreshBtn;
	KWndEdit32	m_InputEdit;
};
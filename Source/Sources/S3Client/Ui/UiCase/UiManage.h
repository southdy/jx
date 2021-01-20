/*****************************************************************************************
//	����--�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndEdit.h"
#include "../Elem/WndImagePart.h"
#include "UiTeamManage.h"

// -------------------------------------------------------------------------
// ---> ���ɹ���
class KUiCliqueManage : public KWndPage
{
public:
	int		Init();									//��ʼ��
	void	LoadScheme(const char* pScheme);		//������淽��	
protected:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
};

// -------------------------------------------------------------------------
// ---> ������
class KUiConfraternityManage : public KWndPage
{
public:
	int		Init();									//��ʼ��
	void	LoadScheme(const char* pScheme);		//������淽��	
protected:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ---> ������
class KUiManage : protected KWndPageSet
{
public:
	static KUiManage*	OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiManage*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void			LoadScheme(const char* pScheme);		//������淽��

	void	UpdateLeaderData();
	void	UpdateTeamData(int nID);
	void	UpdateTeamChange(const KUiPlayerItem* playerItem,int bAdd);
	void	UpdateToJoinChange(const KUiPlayerItem* playerItem,int bAdd);

private:
	KUiManage(){}
	~KUiManage(){ m_pSelf = 0;}
	void	Initialize();								//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	
private:
	static KUiManage* m_pSelf;
private:
	// ��������ҳ
	KUiTeamManage			m_TeamPad;
	KUiCliqueManage			m_CliquePad;
	KUiConfraternityManage	m_ConfraternityPad;
	// ������ҳ������İ�ť
	KWndButton				m_TeamPadBtn;
	KWndButton				m_CliquePadBtn;
	KWndButton				m_ConfraternityPadBtn;
	// �����Լ������ϵĿؼ�
	KWndButton				m_Close;
	KWndText32				m_LeaderShipLevel;
	KWndImagePart			m_LeaderShipExperience;
};

// -------------------------------------------------------------------------

//extern KUiManage		g_UiManage;

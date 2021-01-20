/*****************************************************************************************
//	����--��ӹ������
//	Copyright : Kingsoft 2002-2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndText.h"
#include "../elem/wndbutton.h"
#include "../elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../../../core/src/gamedatadef.h"

class KUiTeamManage : public KWndShowAnimate
{
public:
	static KUiTeamManage* OpenWindow();		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiTeamManage* GetIfVisible();	//�������������ʾ���򷵻�ʵ��ָ��
	static void			  CloseWindow();	//�رմ���
	static void			  LoadScheme(const char* pScheme);		//������淽��
//	static void			  UpdateNearbyTeams(KUiTeamItem* pList, int nCount); //������Χ������б�
	void				  UpdateData(KUiPlayerTeam* pInfo);

private:
	KUiTeamManage();
	~KUiTeamManage() {}
	void	Initialize();							//��ʼ��
	void	LoadScheme(class KIniFile* pSetting);	//������淽��
protected:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	//void	OnApplyJoin();					//�������
	void	OnKick();						//�߳���Ա
	void	OnAppoint();					//�����ӳ�
	void	OnInvite();						//�������
	void	OnLeave();						//�뿪����
	void	OnDismiss();					//��ɢ����
	void	OnRefresh();					//ˢ��
	void	OnSelTeamMember(int nItem);
	void	OnSelNearby(int nItem);
	void	Clear();
	//void	SwitchNearbyMode(bool bNearbyPlayer);
private:
	static KUiTeamManage* m_pSelf;

	KWndText32		m_LeaderAbility;		// ͳ˧��	
	KWndList		m_TeamMember;			// ��Ա�б�
	KWndList		m_NearbyList;			// ��������б�
	KWndScrollBar	m_MemberScroll;			// ��Ա�б������
	KWndScrollBar	m_NearbyScroll;			// ��������б������

//	KWndButton		m_NearbyPlayerTab;		// �г���Χ��ҵİ�ť
//	KWndButton		m_NearbyTeamTab;		// �г���Χ��ҵİ�ť

	KWndButton		m_btnInvite;			// ����
	KWndButton		m_btnKick;				// �߳�
//	KWndButton		m_btnApplyJoin;			// �������

	KWndButton		m_btnAppoint;			// ����
	KWndButton		m_btnLeave, m_btnDismiss;// ��ӡ���ɢ
	KWndButton		m_btnCloseTeam;			// �ر����
	KWndButton		m_btnRefresh;			// ˢ��
	KWndButton		m_btnCancel;			// cancel button

	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersList;
	KUiPlayerItem*	m_pNearbyPlayersList;
//	KUiTeamItem*	m_pNearbyTeamsList;
};

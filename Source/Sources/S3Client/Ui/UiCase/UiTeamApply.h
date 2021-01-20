/*****************************************************************************************
//	����--����������
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-2-26
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"

struct KUiTeamItem;

class KUiTeamApply : protected KWndShowAnimate
{
public:
	static KUiTeamApply* OpenWindow();			//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiTeamApply* GetIfVisible();		//�������������ʾ���򷵻�ʵ��ָ��
	static void			 CloseWindow();			//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void			 LoadScheme(const char* pScheme);//������淽��
	static void			 UpdateData(KUiTeamItem* pList, int nCount);
private:
	KUiTeamApply();
	~KUiTeamApply();
	void	Initialize();						//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	OnApply();
	void	Clear();
	void	OnNewTeam();						//�Լ��½�������
private:
	static KUiTeamApply*	m_pSelf;
	KWndButton				m_RefuseBtn, m_ApplyBtn, m_NewBtn, m_CloseBtn, m_RefreshBtn;
	KWndList				m_TeamList;
	KWndScrollBar			m_ListScroll;
	KUiTeamItem*			m_pDataList;
	int						m_nCount;
};
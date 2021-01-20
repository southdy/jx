/*****************************************************************************************
//	����--�½��������
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-2-26
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"

class KUiTeamNew : protected KWndShowAnimate
{
public:
	static KUiTeamNew*	OpenWindow();		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiTeamNew*	GetIfVisible();		//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow();		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
private:
	void	Initialize();					//��ʼ��
	void	LoadScheme(const char* pScheme);//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	OnOk();
	bool	CheckInput(char* pName);
private:
	static KUiTeamNew*		m_pSelf;
	KWndEdit32				m_TeamName;
	KWndButton				m_OkBtn, m_CancelBtn;
};
/*****************************************************************************************
//	����--ѡ����Ϸ����������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndShowAnimate.h"

class KUiSelServer : protected KWndShowAnimate
{
public:
	static KUiSelServer* OpenWindow();				//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void			 CloseWindow(bool bDestroy);//�رմ���
private:
	KUiSelServer();
	~KUiSelServer();
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	OnLogin();
	void	OnCancel();
	int		OnKeyDown(unsigned int uKey);
	void	OnClickButton(KWndButton* pBtn);
	void	GetList();

private:
	static KUiSelServer* m_pSelf;
private:
	struct KLoginServer* m_pServList;

	KWndList		m_List;
	KWndScrollBar	m_ScrollBar;
	KWndButton		m_Login;
	KWndButton		m_Cancel;
	char			m_szLoginBg[32];

	bool			m_bSelRegion;
	int				m_nRegionIndex;
};

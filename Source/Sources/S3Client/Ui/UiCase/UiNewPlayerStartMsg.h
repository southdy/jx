/*****************************************************************************************
//	����--���ֽ������ʾ����
//	Copyright : Kingsoft 2003
//	Author	:   Freeway Chen
//	CreateTime:	2003-7-8
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"

class KUiNewPlayerStartMsg : protected KWndShowAnimate
{
public:
	static KUiNewPlayerStartMsg* OpenWindow();		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void	CloseWindow(bool bDestroy);         //�رմ���

private:
	KUiNewPlayerStartMsg();
	~KUiNewPlayerStartMsg();

	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	OnOk();
	int		OnKeyDown(unsigned int uKey);
	void	OnClickButton(KWndButton* pBtn);
private:
	void	LoadList();
	static  KUiNewPlayerStartMsg* m_pSelf;
private:
	KScrollMessageListBox	m_List;
	KScrollMessageListBox	m_AdvanceList;
	KWndButton				m_NewerBtn;
	KWndButton				m_AdvanceBtn;
	KWndButton		        m_Ok;

};

/*****************************************************************************************
//	����--��Ļ�����Ʋ�����
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-22
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndToolBar.h"
#include "../Elem/WndButton.h"

class KUiHeaderControlBar : public KWndToolBar
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiHeaderControlBar* OpenWindow();	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void				CloseWindow();		//�رմ���
	static void				LoadScheme(const char* pScheme);//������淽��
	static void				DefaultScheme(const char* pScheme);//���³�ʼ������
	static KUiHeaderControlBar* GetSelf()	{return m_pSelf;}
private:
	~KUiHeaderControlBar() {}
	void	Initialize();							//��ʼ��
	void	Breathe();
private:
	static KUiHeaderControlBar*	m_pSelf;
};
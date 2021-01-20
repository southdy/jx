/*****************************************************************************************
//	����--������´���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndImage.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndImagePart.h"
#include "../Elem/WndShowAnimate.h"

//PatchShell Inclued
#include "../../../SUpdate/SUpdateShell.h"


//====================================
//	����--������½���
//====================================
class KUiUpdataPatch : protected KWndShowAnimate
{
public:
	static KUiUpdataPatch* OpenWindow();	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void			   CloseWindow();	//�رմ���

private:
	KUiUpdataPatch();
	~KUiUpdataPatch();
	int		Initialize();					//��ʼ��
	void	Breathe();
	void	LoadScheme(const char* pScheme);//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���

	void	OnFinished(bool bRestart);
	void	SetCloseParam(const char* pInfo, bool bQuit, bool bOk);
private:
	static KUiUpdataPatch*	m_pSelf;
private:
	KWndMessageListBox	m_MsgList;
	KWndScrollBar		m_MsgScroll;
	KWndImagePart		m_Progress;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	KWndText256			m_StatusText;
	int					m_bQuit;
	HMODULE				m_PatchShell;
	fnSwordUpdateGetResult	m_fnSwordUpdateGetResult;
};

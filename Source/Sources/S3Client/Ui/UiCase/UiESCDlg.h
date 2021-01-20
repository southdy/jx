// -------------------------------------------------------------------------
//	�ļ���		��	UiESCDlg.h
//	������		��	����
//	����ʱ��	��	2002-9-16 10:32:22
//	��������	��	
//
// -------------------------------------------------------------------------
#ifndef __UIESCDLG_H__
#define __UIESCDLG_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiESCDlg : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiESCDlg*	OpenWindow();		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiESCDlg*	GetIfVisible();
	static void			CloseWindow(bool bDestroy);		//�رմ���
private:
	KUiESCDlg() {}
	~KUiESCDlg();
	void	Show();
	void	Hide();
	int		Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	OnClickButton(KWndWindow* pWnd);	//��Ӧ�����ť
	int		OnKeyDown(unsigned int uKey);
	KWndButton*	GetActiveBtn();
private:
	static KUiESCDlg*	m_pSelf;
	KWndButton	m_ExitGameBtn;			// �˳���Ϸ
	//KWndButton	m_ExitBtn;				// �˳�����
	KWndButton	m_HelpBtn;
	//KWndButton	m_TaskBtn;
	KWndButton	m_OptionsBtn;
	KWndButton	m_ContinueGameBtn;		// ������Ϸ
};

#endif // __UIESCDLG_H__
// -------------------------------------------------------------------------
//	�ļ���		��	UiInit.h
//	������		��	����
//	����ʱ��	��	2002-9-10 11:25:36
//	��������	��	��ʼ����	
//
// -------------------------------------------------------------------------
#ifndef __UIINIT_H__
#define __UIINIT_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"

class KUiInit : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiInit*		OpenWindow(bool bStartMusic = true, bool bJustLaunched = false);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void			CloseWindow();							//�رմ���
	static void			PlayTitleMusic();
	static void			StopTitleMusic();

private:
	KUiInit() {}
	~KUiInit() {}
    void    ShowCompleted();
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndButton* pWnd);	//��Ӧ�����ť
	KWndButton*	GetActiveBtn();
	void	OnAutoLogin();
	int		OnKeyDown(unsigned int uKey);
	void	PlayStartMovie();
private:
	static KUiInit* m_pSelf;
private:
	char		m_szLoginBg[32];
	KWndButton m_EnterGame;					// ������Ϸ
	KWndButton m_GameConfig;				// ��Ϸ����
	KWndButton m_DesignerList;				// ������Ա����
//	KWndButton m_AutoLogin;					//�Զ���½
	KWndButton m_ExitGame;					// �˳���Ϸ

	int			m_nCurrentMovieIndex;		//��ǰ���ŵĶ����ı��
};

#endif // __UIINIT_H__
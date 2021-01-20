/*****************************************************************************************
//	����--login����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-12
------------------------------------------------------------------------------------------
	�������и�ѡ�ť����ѡ�񣬼�¼��½�˺ţ����´γ��ֵ�½����ʱ�Զ���д�˺š�
	�˺Ų��á�һ��һ�����ұ�(one-time pad)���ı��巨���ܴ洢��
*****************************************************************************************/

#pragma once
#include "../Elem/WndEdit.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndShowAnimate.h"

#define	MSG_ACCOUNT_PWD_ERROR			"��������ʺŻ�����������������롣"


class KUiLogin : protected KWndShowAnimate
{
public:
	static KUiLogin* OpenWindow();				//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void		 CloseWindow(bool bDestroy);//�رմ���
	static bool		 IsValidPassword(const char* pszPassword, int nLen);
private:
	KUiLogin();
	~KUiLogin();
	void	Show();
	void	Initialize();							//��ʼ��
	void	LoadScheme(const char* pScheme);		//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	int		GetInputInfo(char* pszAccount, char* pszPassword);
	void	OnLogin();								//��½��Ϸ���������
	void	OnCancel();								//
	void	OnKeyDown(unsigned int uKey);
	void	OnEnableAutoLogin();
private:
	static KUiLogin* m_pSelf;
private:
	enum UILOGIN_STATUS
	{
		UILOGIN_S_IDLE,
		UILOGIN_S_LOGINING,
	}			m_Status;//	��½�����״̬
	KWndEdit32	m_Account;
	KWndEdit32	m_PassWord;
	KWndButton	m_Login, m_Cancel;
	KWndButton	m_RememberAccount;
	char		m_szLoginBg[32];
};

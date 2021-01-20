// -------------------------------------------------------------------------
//	�ļ���		��	UiSelPlayer.h
//	������		��	����
//	����ʱ��	��	2002-9-10 14:24:08
//	��������	��	
//	---> ��ɫѡ�����,�������½���Ԫ��:
//		1) ������ѡ��Ľ�ɫ
//		2) �ĸ���ť��ȷ�����½���ɾ����ȡ��
// -------------------------------------------------------------------------
#pragma once 

#include "../elem/WndShowAnimate.h"
#include "../elem/wndtext.h"
#include "../elem/wndbutton.h"

#ifndef MAX_PLAYER_PER_ACCOUNT
	#define	MAX_PLAYER_PER_ACCOUNT	3
#endif
// -------------------------------------------------------------------------

class KUiSelPlayer : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiSelPlayer* OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void			 CloseWindow();					//�رմ���

	static void	GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);

private:
	KUiSelPlayer();
	~KUiSelPlayer() {}
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	void	PaintWindow();						//�������
	void	UpdateData();						//����

	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//��Ϣ����
	void	OnClickButton(KWndButton* pWnd, bool bDoubleClick);	//��Ӧ�����ť
	void	OnNew();
	void	OnDel();
	void	OnCancel();
	void	OnSelectPlayer(int nSel, bool bPlaySound = true);
	void	OnEnterGame();
	int		OnKeyDown(unsigned int uKey);
	KWndButton*	GetActiveBtn();
private:
	static KUiSelPlayer* m_pSelf;
private:
	KWndText80	m_LifeTimeText;
	// ����Ԫ��
	KWndText32	m_Name[MAX_PLAYER_PER_ACCOUNT];
	KWndText32	m_Level[MAX_PLAYER_PER_ACCOUNT];
	KWndButton	m_player[MAX_PLAYER_PER_ACCOUNT];	// ������ɫ
	KWndImage	m_PlayerInfoBg[MAX_PLAYER_PER_ACCOUNT];
	KWndButton	m_btnOk;		// ȷ��
	KWndButton	m_btnCancel;	// ȡ��
	KWndButton	m_btnNew;		// �½���ɫ
	KWndButton	m_btnDel;		// ɾ����ɫ

	unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char			m_szSelPlayerSound[MAX_PLAYER_PER_ACCOUNT][128];

	KWndImage	m_ButterflyWnd;

	char		m_szLoginBg[32];

	char		m_szPlayerImgPrefix[128];

	int			m_ChildPos[4];
	int			m_ChildWndXOffset[4];
	int			m_ChildWndYOffset;

private:
	int			m_bJustClicked;
	short		m_nNumPlayer;
	short		m_nSelPlayer;	// ��ǰѡ�е��ǵڼ�����ɫ
						//  -1	-- ��ѡ�У�    0	-- ѡ�е�һ����
						//  1	-- ѡ�еڶ�����2	-- ѡ�е�������
};

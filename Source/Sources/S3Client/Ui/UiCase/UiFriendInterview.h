/*****************************************************************************************
//	����--���촰��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-27
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndImage.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndMessageListBox.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"

struct KUiPlayerItem;
struct KUiChatMessage;

class KUiFriendInterview : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiFriendInterview*	OpenWindow(KUiPlayerItem* pFriend);		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static int					DoesHaveVisibleWnd();
	static void					CloseWindow();							//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void					LoadScheme(const char* pScheme);		//������淽��
private:
	KUiFriendInterview();
	~KUiFriendInterview();
	int		Initialize();								//��ʼ��
	void	CloseSelf();
	void	CloneTheme(KUiFriendInterview* pCopy);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	PaintWindow();
	int		IsRecyclable();
	void	OnSend();
	void	PopupFaceMenu();
	void	SetFriendData(KUiPlayerItem* pFriend);
	static KUiFriendInterview*	FindWindow(KUiPlayerItem* pFriend, bool bCreate);
	static void					SetSelfName(const char* pszSelfName);
private:
	static KUiFriendInterview* m_pFirstSelf;
	KUiFriendInterview*		m_pNextSelf;
private:
	KWndMessageListBox	m_MsgList;
	KWndScrollBar		m_MsgScroll;
	KWndEdit512			m_InputWnd;
	KWndText32			m_FriendName;
	KWndButton			m_SendBtn, m_CloseBtn, m_ColorBtn, m_FaceBtn;

	KRColor				m_SelfTextColor;	//�˴��ڵ��Լ�˵�Ļ����ı���ɫ
	KUiPlayerItem		m_FriendData;		//���ѵı������

	static char			m_szSelfName[32];	//�Լ�������
	static int			m_nSelfNameLen;		//�Լ����ֵĳ���
	static unsigned int	m_uLastTextColor;	//���ѡ����ı���ɫ
};

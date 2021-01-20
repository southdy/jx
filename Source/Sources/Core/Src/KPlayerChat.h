//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerChat.h
// Date:	2002.10.05
// Code:	�߳�����
// Desc:	PlayerChat Class
//---------------------------------------------------------------------------

#ifndef KPLAYERCHAT_H
#define KPLAYERCHAT_H

#include	"GameDataDef.h"
#ifndef _SERVER
#include	"KNode.h"
#include	"KList.h"
#endif

#ifdef _SERVER
#define		MAX_FRIEND_TEAM				1	// �����ѷ�����
#define		CHAT_SPEED					20	// ��Ҷ೤ʱ���ܷ�һ�仰
#else
#define		MAX_FRIEND_TEAM				10	// �����ѷ�����
#endif
#define		CHAT_APPLY_ADD_FRIEND_LIST	10

enum	// ��ǰ����Ƶ��
{
	CHAT_CUR_CHANNEL_NONE = 0,				// ��ǰû�з���Ƶ��
	CHAT_CUR_CHANNEL_ALL,					// ����ͬһ������
	CHAT_CUR_CHANNEL_SCREEN,				// ��ǰ����������
	CHAT_CUR_CHANNEL_SINGLE,				// ����
	CHAT_CUR_CHANNEL_TEAM,					// ���ж���
	CHAT_CUR_CHANNEL_FACTION,				// ͬ����
	CHAT_CUR_CHANNEL_TONG,					// ͬ����
	CHAT_CUR_CHANNEL_SCREENSINGLE,			// ���������ڵ���
	CHAT_CUR_CHANNEL_SYSTEM,				// ϵͳƵ����ר����ϵͳ����ҷ���Ϣ�����붩��
	CHAT_CUR_CHANNEL_NUM,	// Ƶ������
};

extern	DWORD	g_dwChannelAnd[CHAT_CUR_CHANNEL_NUM];
extern	DWORD	g_dwChannelOr[CHAT_CUR_CHANNEL_NUM];

#ifndef _SERVER
class KCHAT_RECORDER
{
private:
	char	m_szDay[11];
	char	m_szTime[9];

private:
	void		SetTime();

public:
	void		SaveSentenceSingle(char *lpszSelfName, char *lpszTarget, char *lpszTalkerName, char *lpszSentence);
	void		SaveSentenceChannel(char *lpszSelfName, char *lpszTalkerName, char *lpszSentence);

};
#endif

// ÿ��������ѵ�����
class CChatFriend : public KNode
{
public:
	DWORD			m_dwID;					// ����ID
	int				m_nPlayerIdx;			// ������ player �����е�λ��
	char			m_szName[32];			// ��������

public:
	CChatFriend() {Release();};
	void			Release() {m_dwID = 0; memset(m_szName, 0, 32);};
};

// ���ѷ�������
class CChatFriendTeam
{
public:
	int				m_nFriendNo;			// ��������
	char			m_szTeamName[32];		// ����
	KList			m_cEveryOne;			// ����ÿһ�����ѵ�����
public:
	CChatFriendTeam();
	void			Release();
#ifdef _SERVER
	void			DataBaseAddOne(int nSelfIdx, DWORD dwID, char *lpszName);		// ��ҵ�½��ʱ������ݿ�������ʱר��
#endif
};

#ifndef _SERVER
class CChatApplyListNode : public KNode
{
public:
	int		m_nPlayerIdx;
	int		m_nAccessFlag;
	char	m_szName[32];
	char	m_szInfo[MAX_SENTENCE_LENGTH];

public:
	CChatApplyListNode() { m_nPlayerIdx = 0; m_nAccessFlag = 0; m_szName[0] = 0; m_szInfo[0] = 0; };
};
#endif

#ifdef _SERVER
#include	<list>
#endif

class KPlayerChat
{
public:

#ifdef _SERVER
	int				m_nAddFriendList[CHAT_APPLY_ADD_FRIEND_LIST];	// ��ǰ������ӵĺ��� npc id �б�
	int				m_nListPos;										// m_nAddFriendList �ĵ�ǰλ��
	int				m_nSyncState;									// ��ҵ�½ʱ���ͺ������ݵĹ���
	CChatFriend		*m_pStateSendPos;								// ��ҵ�½ʱ���ͺ�������ʱλ��ָ��
	int				m_nTimer;										// ��������ˢ���ٶ�
#endif

#ifndef _SERVER
	KList			m_cApplyAddList;					// �ͻ����������б�(����)
	int				m_nCurChannel;						// ��ǰ����Ƶ��
	BOOL			m_btIniLoadFlag;					// m_cTeamInfo �Ƿ�����
	KIniFile		m_cTeamInfo;						// �ͻ��˺��ѷ����ļ�
	int				m_nLoginGetFirstOneFriendFlag;		// ��ҵ�¼ʱ���յ���һ��������Ϣ
	KCHAT_RECORDER	m_cRecorder;
#endif

	DWORD			m_dwTakeChannel;					// ��ǰ����Ƶ��
	DWORD			m_dwTargetID;						// ��ǰ�������ID �ڲ�ͬ��Ƶ���»��в�ͬ�ĺ���
	int				m_nTargetIdx;						// ��ǰ��������ڷ������˵�index
	CChatFriendTeam	m_cFriendTeam[MAX_FRIEND_TEAM];		// ���ѷ����б�

public:
	void			Release();
	BOOL			CheckExist(DWORD dwID);
	BOOL			CheckExist(char *lpszName);
	BOOL			GetName(DWORD dwID, char *lpszName);
	CChatFriend*	GetFriendNode(DWORD dwID);
	int				GetTeamNo(DWORD dwID);
	void			SetTakeChannel(DWORD dwChannel);			// ��������Ƶ��
	BOOL			CheckTalkChannel(int nChannel);

#ifdef _SERVER
	void			ServerSendChat(int nPlayerIdx, BYTE* pProtocol);// �������յ��ͻ��˷�����������Ϣ
	void			DataBaseAddOne(int nSelfIdx, DWORD dwID, char *lpszName);// ��ҵ�½��ʱ������ݿ�������ʱר��
	void			AddFriendData(int nSelfIdx, int nDestIdx);		// ��Ӻ�������
	void			ResendOneFriendData(DWORD dwID, int nPlayerIdx);// ���·���ĳ����������
	BOOL			SyncFriendData(int nPlayerIdx, unsigned int &nParam);// ��ҵ�¼ʱ�������ݿ���Һ���������ɺ��Զ����η��ͺ������ݸ��ͻ���
	void			StartSendFriendData();							// ��ҵ�¼ʱ�������ݿ���Һ���������ɺ���ôη�����ʼ���͸��ͻ��˺�������
	void			DeleteFriend(DWORD dwID, DWORD dwSelfID);		// ɾ��ĳ���������ݣ���֪ͨ�Է���Ӧ����
	void			DeleteFriendData(DWORD dwID, int nSelfIndex);	// ɾ��ĳ���������ݣ���֪ͨ�Լ��ͻ��ˣ�
	void			RedeleteFriend(DWORD dwID, int nSelfIndex);		// �ٴ�ɾ��ĳ����
	void			OffLine(DWORD dwSelfID);						// ���������
	void			GetMsgOffLine(DWORD dwID, int nSelfIdx);		// �õ�֪ͨ�к�������
	void			TimerAdd();										// ����ʱ�������һ
	// ����ϵͳ��Ϣ nType = 0 ��ȫ����ҷ��� nType == 1 ��ĳ���ض���ҷ���
	static	void	SendSystemInfo(int nType, int nTargetIdx, char *lpszSendName, char *lpszSentence, int nSentenceLength);
	static	void	SendGlobalSystemInfo(char *lpszSendName, char *lpszSentence, int nSentenceLength);
	typedef std::list<std::string>	STRINGLIST;
	static	void	MakeBrother(const STRINGLIST& brothers);
	static	void	MakeEnemy(char* szPlayer, char* szEnemy);
	static	void	SendInfoToGM(char *lpszAccName, char *lpszRoleName, char *lpszSentence, int nSentenceLength);
	static	void	SendInfoToIP(DWORD nIP, DWORD nID, char *lpszAccName, char *lpszRoleName, char *lpszSentence, int nSentenceLength);
#endif

#ifndef _SERVER
	void			SetCurChannel(int nChannelNo, DWORD dwID = 0, int nIdx = 0);// �趨��ǰ����Ƶ��
	void			AddChannel(int nChannelNo);						// ���Ӷ���Ƶ��
	void			SubChannel(int nChannelNo);						// ȡ������Ƶ��
	void			SendTakeChannel();								// �ͻ��˷��Ͷ���Ƶ����������

	void			SendSentence(KUiMsgParam *pMsg, char *lpszSentence);// �ͻ��˷�����������������
	void			QQSendSentence(DWORD dwID, int nIdx, KUiMsgParam *pMsg, char *lpszSentence);// �ͻ��˷�����������������(QQר��)

	void			GetChat(PLAYER_SEND_CHAT_SYNC *pChat);			// �ͻ����յ��������

	void			ApplyAddFriend(DWORD dwNpcID, char *lpszSentence = NULL);// ��������������
	void			ApplyDeleteFriend(DWORD dwID, int nTeamNo, char *lpszSelfName);// �ͻ�������ɾ��ĳ����

	BOOL			GetApplyOneInfo(int *lpnPlayerIdx, char *lpszName, char *lpszInfo);// �����ȡ������������������Ϣ

	void			SaveTeamInfo(char *lpszName);					// �������к��ѷ�����Ϣ
	int				CheckTeamNo(char *lpszName, char *lpszGetName);	// �ж�ĳ��ID�ĺ������ĸ�����
	void			LoadTeamName(char *lpszPlayerName);				// ��������(��ʼ����ʱ����ã�����ʱ���������)

	void			AddOne(DWORD dwID, int nPlayerIdx, char *lpszName, int nTeamNo);// ���һ����Ա
	BOOL			CheckIsFriend(int nPlayerIdx);					// �ж�ĳ��playeridx�Ƿ����
	int				AddTeam(char *lpszPlayerName, char *lpszTeamName);// ����飬���صڼ��飬if < 0 ʧ��
	BOOL			RenameTeam(int nTeamNo, char *lpszTeamName, char *lpszPlayerName);// �����
	BOOL			DeleteTeam(int nTeamNo, char *lpszPlayerName);	// ɾ���飬���سɹ����
	BOOL			OneFriendChangeTeam(DWORD dwID, int nSrcTeam, int nDestTeam);// ��һ�����Ѵ�һ�����ƶ�����һ����

	void			GetMsgOffLine(DWORD dwID);						// �õ�֪ͨ�к�������

	BOOL			CheckTakeChannel(int nChannelNo);				// �ж��Ƿ�����ĳ��Ƶ��
	void			SaveTakeChannelInfo(char *lpszName);			// ����Ƶ��������Ϣ
	void			LoadTakeChannelInfo(char *lpszName);			// ����Ƶ��������Ϣ

	// ���ڽ���
	int				SendSelectChannelList(KUiChatChannel *pList, int nCount);	// ���Ϳ�ѡ������Ƶ���б������
	int				SendTakeChannelList(KUiChatChannel *pList, int nCount);		// ���Ϳɶ�������Ƶ���б������
	BOOL			GetCurChannel(KUiChatChannel *pList);			// ��õ�ǰʹ������Ƶ����Ϣ
	int				IGetTeamInfo(int nTeamNo, KUiChatGroupInfo *psTeamInfo);	// ���ĳ��������Ϣ��������һ����Ч����(��������һ����Ч�飬����-1)
	int				IGetTeamFriendInfo(int nTeamNo, KUiPlayerItem *pList);	// ���ĳ����������г�Ա����Ϣ
#endif

};

#endif


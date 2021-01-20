/********************************************************************
	created:	2003/05/15
	file base:	Player
	file ext:	h
	author:		liupeng
	
	purpose:	
*********************************************************************/
#ifndef __INCLUDE_PLAYER_H__
#define __INCLUDE_PLAYER_H__

// There comes a fucking request that to use zclient instead of RainBow
// as the network-lib came.
// This macro is the switch.
#include "tstring.h"
#include "Thread.h"
#include "Event.h"
#include "Buffer.h"
#include "Library.h"

#if defined _USE_RAINBOW
#include "IClient.h"
#include "RainbowInterface.h"
#else
#include "..\..\LINUX\IClient.h"
#endif
#include "CriticalSection.h"

// move from .cpp to here.
#include "KProtocol.h"
// flying add the math lib
#include "math.h"

// GM Command list
#define		MAX_GM_COUNT	256
#define		MAX_GM_SIZE		128

enum NPCCMD
{
	do_none,		// ʲôҲ����
	do_stand,		// վ��
	do_walk,		// ����
	do_run,			// �ܶ�
	do_jump,		// ��Ծ
	do_skill,		// �����ܵ�����
	do_magic,		// ʩ��
	do_attack,		// ����
	do_sit,			// ����
	do_hurt,		// ����
	do_death,		// ����
	do_defense,		// ��
	do_idle,		// ����
	do_specialskill,// ���ܿ��ƶ���
	do_special1,	// ����1
	do_special2,	// ����2
	do_special3,	// ����3
	do_special4,	// ����4
	do_runattack,
	do_manyattack,
	do_jumpattack,
	do_revive,
};

typedef struct 
{
	int nCamp; //��Ӫ
} PLAYER_INFO;

typedef struct
{
	NPCCMD		CmdKind;		// ����C
	int			Param_X;		// ����X
	int			Param_Y;		// ����Y
	int			Param_Z;		// ����Y
} NPC_COMMAND;

#define		LOCAL_REVIVE_TYPE			1

class IPlayer
{
public:
	virtual bool ConnectToGateway() = 0;
	// flying add the interfaces
	virtual int SetPos(int nX, int nY) = 0;
	virtual int SetAttack() = 0;
	virtual int SetSilence(int nSilence) = 0;
	virtual int SetChatTimer(int nTimer) = 0;
	virtual ~IPlayer(){};
};

typedef struct _ITEMS
{
} ITEMS;

class CPlayer : public IPlayer, protected OnlineGameLib::Win32::CThread
{
public:

	explicit CPlayer( const char *pSrvIP, unsigned short nPort, const char *pAccName );
	virtual ~CPlayer();

	virtual bool ConnectToGateway();

	using CThread::Start;

protected:
	void Free();
	void LoginGateway();

	void ProcessGateway();
	void ProcessGatewayMsg();
	void ProcessGatewayLoop();

	void ProcessGameSvr();
	void ProcessGameSvrMsg();
	void ProcessGameSvrLoop();
	
	bool ConnectToGameSvr( const char *pIPAddr, unsigned short nPort, GUID &guid );
	void ShutdownGateway();
	void ShutdownGameSvr();
	bool OnAccountLoginNotify(const char* pMsg);
	bool OnPlayerLoginNotify(const char* pMsg);
	int  OnGetRoleList(const char* pMsg);
    //bool OnAccountLogoutNotify(const char* pMsg);
	void SyncEnd();

	void SpeakWord( const char *pText, size_t size /*0*/ );
	void CastSkill();
	void SendGMCommand();
	void QueryDefaultChannel(char* szName, int nSize);
	void WalkTo();

	virtual int Run();
#if defined _USE_RAINBOW
	static void __stdcall ClientEventNotify( LPVOID lpParam, const unsigned long &ulnEventType );
#else
	static void ClientEventNotify( LPVOID lpParam, const unsigned long &ulnEventType );
#endif
	void _ClientEventNotify( const unsigned long &ulnEventType );

#if defined _USE_RAINBOW
	static void __stdcall GameServerToClientEventNotify( LPVOID lpParam, const unsigned long &ulnEventType );
#else
	static void GameServerToClientEventNotify( LPVOID lpParam, const unsigned long &ulnEventType );
#endif
	void _GameServerToClientEventNotify( const unsigned long &ulnEventType );

	typedef HRESULT ( __stdcall * pfnCreateClientInterface )( REFIID riid, void **ppv );
#if defined _USE_RAINBOW
	static OnlineGameLib::Win32::CLibrary	s_theRainbowLib;
#endif
	// �����̵߳��˳����¼�
	OnlineGameLib::Win32::CEvent			m_hQuit;

private:

	IClient		*m_pClientToGateWay;
	IClient		*m_pClientToGameSvr;

	const OnlineGameLib::Win32::_tstring	m_sGatewayIP;
	const unsigned short					m_nGatewayPort;
	const OnlineGameLib::Win32::_tstring	m_sAccName;		// �ʺŵ����֣���������֣��ͽ�ɫ��������һ�µ�

	const LONG	m_nStatus;

	// �������������Player ID
	static DWORD	m_dwTargetID;
	// �������Լ���ID
	DWORD			m_dwID;
	// flying add this
	// ��ǰս��NPC ID	
	DWORD			m_dwNpcID;
	POINT			m_ptCurPos;
	POINT			m_ptTagPos;
	
	// GMָ��
	unsigned char   szGMCommand[128];

    unsigned        m_RandValue;

    unsigned        m_uGameServerLoopCount;

    unsigned char  *m_pbyRoleMsg;

    int             m_nShutDownGatewayFlag;
	
	// flying add this member, the range that robot can move.
	unsigned int	m_nMoveRange;
	// Target Player's coordinate
	POINT			m_ptTarget;
	int				m_nCurChannel;
	int				m_nGMChannelID;
	int DoSetPos();
	//int GetPlayerInfo();
	// While the command line option include "SetPos", use this member.
	POINT			m_ptBirth;
	// flag
	int				m_nFlgSetPos;
	int				m_nFlgSetAttack;
	int				m_nFlgAI;
	int				m_nFlgSilence;
	int				m_nFlgChatTimer;
	int				m_nFlgGMDone;

	int				m_nDoing;
	int				m_nTeamLeader;
	int				m_nChatTimer;	
	// �ƶ�����ƫ��
	POINT			m_ptOffset;
	// ��core�Ǳ߰����һ�㶫����ʵ�����ƵĹ���
	PLAYER_INFO		m_player;
	int				ProcessNpcSync(NPC_NORMAL_SYNC* pPS);
	int				ProcessPlayerSync(NPC_PLAYER_TYPE_NORMAL_SYNC* pPS);
	int				ProcessSwitchGameServer(void* lpBuffer);
	// ����Ping����
	void			SendPing(DWORD dTime);
	// ������չЭ��
	int				ProcessExtProtocol(char* pMsg);
	// װ����
	ITEMS			m_OldItems;
	ITEMS			m_NewItons;
public:

	enum
	{
		enumSleep			= 0x10,			// ��δ��������
		enumStartup,						// ��������	
		enumConnectToGateway,				// ��ʼ��������
		enumConnectToGatewaySucess,			// �������سɹ�
		enumGetRoleList,					// �����صõ���ɫ�б�
		enumConnectToGameSvr,				// �����صõ�������Ϸ��������ͨ��
		enumPlayGame,						// ��ʼͬ����������ʼ��Ϸ
        enumExitGame                        // �˳���Ϸ
	};

	void SetStatus( LONG nStatus ) { ::InterlockedExchange( const_cast< LONG * >( &m_nStatus ), nStatus ); }
	LONG GetStatus() { return ::InterlockedExchange( const_cast< LONG * >( &m_nStatus ), m_nStatus ); }

    int GetAccountName(OnlineGameLib::Win32::_tstring &sRetAccountName)
    {
        sRetAccountName = m_sAccName;
        return true;
    }
	// flying add this interface to accept GM command
	int SetPos(int nX, int nY);
	int SetAttack();
	int SetSilence(int nSilence);
	int SetChatTimer(int nTimer);
private:
	int KSG_StringToMD5String(char szDestMD5String[64], const char cszSrcString[]);
};


using OnlineGameLib::Win32::CCriticalSection;
extern CCriticalSection		g_csPlayerList;
#endif // __INCLUDE_PLAYER_H__
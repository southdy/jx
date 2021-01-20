//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerMenuState.h
// Date:	2002.12.10
// Code:	�߳�����
// Desc:	PlayerMenuState Class
//---------------------------------------------------------------------------

#ifndef KPLAYERMENUSTATE_H
#define KPLAYERMENUSTATE_H

#include "GameDataDef.h"
enum
{
	PLAYER_MENU_STATE_NORMAL = 0,
	PLAYER_MENU_STATE_TEAMOPEN,
	PLAYER_MENU_STATE_TRADEOPEN,
	PLAYER_MENU_STATE_TRADING,
	PLAYER_MENU_STATE_IDLE,
	PLAYER_MENU_STATE_NUM,
};

class KPlayerMenuState
{
public:
	int			m_nState;
	int			m_nTradeDest;						// �������˼ǵ��� player index �ͻ��˼ǵ��� npc id
	int			m_nTradeState;						// �Ƿ��Ѿ�����ok 0 û�� 1 ����
	char		m_szSentence[MAX_SENTENCE_LENGTH];

#ifdef _SERVER
	int			m_nBackState;
	char		m_szBackSentence[MAX_SENTENCE_LENGTH];
#endif

#ifndef _SERVER
	int			m_nTradeDestState;
#endif

public:
	KPlayerMenuState();

#ifdef _SERVER
	void		Release();
	void		BackupState();
	void		RestoreState();
	void		SetState(int nPlayerIdx, int nState, char *lpszSentence = NULL, int nSentenceLength = 0, DWORD dwDestNpcID = 0);
	void		RestoreBackupState(int nPlayerIdx);
#endif

#ifndef _SERVER
	void		Release();
	void		SetState(int nState);
#endif

};

class KPlayerMenuStateGraph
{
public:
	char		m_szName[PLAYER_MENU_STATE_NUM][80];

public:
	BOOL		Init();
	void		GetStateSpr(int nState, char *lpszGetName);
};

#endif

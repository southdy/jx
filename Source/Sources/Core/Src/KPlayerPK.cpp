//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerPK.cpp
// Date:	2003.07.15
// Code:	�߳�����
// Desc:	PlayerPK Class
//---------------------------------------------------------------------------


#include	"KCore.h"
#include	"KPlayer.h"
#include	"KPlayerDef.h"
#include	"KPlayerPK.h"

#ifndef _SERVER
#include "CoreShell.h"
#endif

#ifdef _SERVER

//-------------------------------------------------------------------------
//	���ܣ����캯��
//-------------------------------------------------------------------------
KPlayerPK::KPlayerPK()
{
	m_nPlayerIndex = 0;
}

//-------------------------------------------------------------------------
//	���ܣ���ʼ��
//-------------------------------------------------------------------------
void	KPlayerPK::Init(int nPlayerIdx)
{
	this->m_nPlayerIndex = nPlayerIdx;

	m_nNormalPKFlag			= 1;
	m_nEnmityPKState		= enumPK_ENMITY_STATE_CLOSE;
	m_nEnmityPKAim			= 0;
	m_nEnmityPKTime			= 0;
	m_nEnmityPKLaunchFlag	= 0;
	m_nExercisePKFlag		= 0;
	m_nExercisePKAim		= 0;
	m_nPKValue				= 0;
}

void	KPlayerPK::Active()
{
	EnmityPKCountDown();
}

//-------------------------------------------------------------------------
//	���ܣ��趨����PK״̬ TRUE �򿪣����Կ���  FALSE �رգ������Կ���
//-------------------------------------------------------------------------
void	KPlayerPK::SetNormalPKState(BOOL bFlag)
{
	m_nNormalPKFlag = bFlag;

	PK_NORMAL_FLAG_SYNC	sFlag;
	sFlag.ProtocolType = s2c_pksyncnormalflag;
	sFlag.m_btFlag = m_nNormalPKFlag;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sFlag, sizeof(PK_NORMAL_FLAG_SYNC));
}

//-------------------------------------------------------------------------
//	���ܣ��������PK״̬ TRUE �򿪣����Կ���  FALSE �رգ������Կ���
//-------------------------------------------------------------------------
BOOL	KPlayerPK::GetNormalPKState()
{
	return this->m_nNormalPKFlag;
}

//-------------------------------------------------------------------------
//	���ܣ��رճ�ɱPK
//-------------------------------------------------------------------------
void	KPlayerPK::EnmityPKClose()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_CLOSE)
		return;

	PK_ENMITY_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncenmitystate;
	sState.m_btState = enumPK_ENMITY_STATE_CLOSE;
	sState.m_dwNpcID = 0;
	sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	if (m_nEnmityPKAim > 0 && m_nEnmityPKAim < MAX_PLAYER)
	{
		g_pServer->PackDataToClient(Player[m_nEnmityPKAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

		Player[m_nEnmityPKAim].m_cPK.m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		Player[m_nEnmityPKAim].m_cPK.m_nEnmityPKAim = 0;
		Player[m_nEnmityPKAim].m_cPK.m_nEnmityPKTime = 0;
	}
	this->m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
	this->m_nEnmityPKAim = 0;
	this->m_nEnmityPKTime = 0;
	m_nEnmityPKLaunchFlag = 0;
}

//-------------------------------------------------------------------------
//	���ܣ���ɱPK��ʼ
//-------------------------------------------------------------------------
BOOL	KPlayerPK::EnmityPKOpen(int nAim)
{
	// ����
	if (nAim <= 0 || nAim >= MAX_PLAYER || Player[nAim].m_nIndex < 1)
		return FALSE;
	// ��ս��ģʽ
	if (!Npc[Player[m_nPlayerIndex].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_1;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	if (!Npc[Player[nAim].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_6;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// �Լ�������
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp == camp_begin)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_2;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// �����д�
	if (m_nExercisePKFlag == TRUE)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_3;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// �Է������д�
	if (Player[nAim].m_cPK.GetExercisePKState())
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_7;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// ���ڳ�ɱ
	if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_4;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// �Է�������
	if (Npc[Player[nAim].m_nIndex].m_Camp == camp_begin)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_5;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}

	Player[nAim].m_cPK.EnmityPKClose();
	Player[nAim].m_cPK.m_nEnmityPKState = enumPK_ENMITY_STATE_TIME;
	Player[nAim].m_cPK.m_nEnmityPKAim = m_nPlayerIndex;
	Player[nAim].m_cPK.m_nEnmityPKTime = PK_ANMITY_TIME;
	Player[nAim].m_cPK.m_nEnmityPKLaunchFlag = 0;

	m_nEnmityPKState = enumPK_ENMITY_STATE_TIME;
	m_nEnmityPKAim = nAim;
	m_nEnmityPKTime = PK_ANMITY_TIME;
	m_nEnmityPKLaunchFlag = 1;

	PK_ENMITY_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncenmitystate;
	sState.m_btState = enumPK_ENMITY_STATE_TIME;

	sState.m_dwNpcID = Npc[Player[m_nPlayerIndex].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[nAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	sState.m_dwNpcID = Npc[Player[nAim].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[nAim].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ���ó�ɱ״̬����Ӧ enumPK_ANMITY_STATE
//-------------------------------------------------------------------------
int		KPlayerPK::GetEnmityPKState()
{
	return this->m_nEnmityPKState;
}

//-------------------------------------------------------------------------
//	���ܣ���ó�ɱĿ�� if Ŀ��==0 ���ڳ�ɱ״̬ else �õ���ɱĿ��
//-------------------------------------------------------------------------
int		KPlayerPK::GetEnmityPKAim()
{
	return this->m_nEnmityPKAim;
}

//-------------------------------------------------------------------------
//	���ܣ��ر��д�PK
//-------------------------------------------------------------------------
void	KPlayerPK::ExercisePKClose()
{
	if (m_nExercisePKFlag == FALSE)
		return;

	PK_EXERCISE_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncexercisestate;
	sState.m_btState = 0;
	sState.m_dwNpcID = 0;
	sState.m_wLength = sizeof(PK_EXERCISE_STATE_SYNC) - 1 - sizeof(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	if (m_nExercisePKAim > 0 && m_nExercisePKAim < MAX_PLAYER)
	{
		g_pServer->PackDataToClient(Player[m_nExercisePKAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

		Player[m_nExercisePKAim].m_cPK.m_nExercisePKFlag = FALSE;
		Player[m_nExercisePKAim].m_cPK.m_nExercisePKAim = 0;
	}
	this->m_nExercisePKFlag = FALSE;
	this->m_nExercisePKAim = 0;
}

//-------------------------------------------------------------------------
//	���ܣ����д�PK
//-------------------------------------------------------------------------
BOOL	KPlayerPK::ExercisePKOpen(int nAim)
{
	// �Լ�������
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp == camp_begin)
		return FALSE;
	// �����д�
	if (m_nExercisePKFlag == TRUE)
		return FALSE;
	// ����
	if (nAim <= 0 || nAim >= MAX_PLAYER || Player[nAim].m_nIndex < 1)
		return FALSE;
	// �Է�������
	if (Npc[Player[nAim].m_nIndex].m_Camp == camp_begin)
		return FALSE;

	this->EnmityPKClose();
	Player[nAim].m_cPK.EnmityPKClose();
	this->m_nExercisePKFlag = TRUE;
	this->m_nExercisePKAim = nAim;
	Player[nAim].m_cPK.m_nExercisePKFlag = TRUE;
	Player[nAim].m_cPK.m_nExercisePKAim = this->m_nPlayerIndex;

	PK_EXERCISE_STATE_SYNC	sState;
	sState.ProtocolType = s2c_pksyncexercisestate;
	sState.m_btState = 1;

	sState.m_dwNpcID = Npc[Player[m_nPlayerIndex].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_EXERCISE_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[nAim].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	sState.m_dwNpcID = Npc[Player[nAim].m_nIndex].m_dwID;
	strcpy(sState.m_szName, Npc[Player[nAim].m_nIndex].Name);
	sState.m_wLength = sizeof(PK_EXERCISE_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ�����д�Ŀ�� if Ŀ��==0 �����д�״̬ else �õ��д�Ŀ��
//-------------------------------------------------------------------------
int		KPlayerPK::GetExercisePKAim()
{
	return this->m_nExercisePKAim;
}

//-------------------------------------------------------------------------
//	���ܣ��趨PKֵ
//-------------------------------------------------------------------------
void	KPlayerPK::SetPKValue(int nValue)
{
	this->m_nPKValue = (nValue < 0 ? 0 : nValue);
	if (m_nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	PK_VALUE_SYNC	sValue;
	sValue.ProtocolType = s2c_pksyncpkvalue;
	sValue.m_nPKValue = m_nPKValue;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sValue, sizeof(PK_VALUE_SYNC));
}

//-------------------------------------------------------------------------
//	���ܣ����PKֵ
//-------------------------------------------------------------------------
int		KPlayerPK::GetPKValue()
{
	return m_nPKValue;
}

//-------------------------------------------------------------------------
//	���ܣ�����(�����)PKֵ��PKֵ�����ٵ�0
//-------------------------------------------------------------------------
void	KPlayerPK::AddPKValue(int nAdd)
{
	m_nPKValue += nAdd;
	if (m_nPKValue < 0)
		m_nPKValue = 0;
	if (m_nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	PK_VALUE_SYNC	sValue;
	sValue.ProtocolType = s2c_pksyncpkvalue;
	sValue.m_nPKValue = m_nPKValue;
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sValue, sizeof(PK_VALUE_SYNC));
}

//-------------------------------------------------------------------------
//	���ܣ��رճ�ɱPK���д�PK
//-------------------------------------------------------------------------
void	KPlayerPK::CloseAll()
{
	ExercisePKClose();
	EnmityPKClose();
}

//-------------------------------------------------------------------------
//	���ܣ���ɱ����ʱ
//-------------------------------------------------------------------------
void	KPlayerPK::EnmityPKCountDown()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime > 0)
			return;
		m_nEnmityPKTime = PK_ANMITY_CLOSE_TIME;
		m_nEnmityPKState = enumPK_ENMITY_STATE_PKING;

		if (Player[m_nEnmityPKAim].m_nIndex <= 0)
			return;
		PK_ENMITY_STATE_SYNC	sState;
		sState.ProtocolType = s2c_pksyncenmitystate;
		sState.m_btState = enumPK_ENMITY_STATE_PKING;
		sState.m_dwNpcID = Npc[Player[m_nEnmityPKAim].m_nIndex].m_dwID;
		strcpy(sState.m_szName, Npc[Player[m_nEnmityPKAim].m_nIndex].Name);
		sState.m_wLength = sizeof(PK_ENMITY_STATE_SYNC) - 1 - sizeof(sState.m_szName) + strlen(sState.m_szName);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, (BYTE*)&sState, sState.m_wLength + 1);
	}
	else if (m_nEnmityPKState == enumPK_ENMITY_STATE_PKING)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime > 0)
			return;
		m_nEnmityPKTime = 0;
		ExercisePKClose();
	}
}

#endif

#ifndef _SERVER

//-------------------------------------------------------------------------
//	���ܣ���ʼ��
//-------------------------------------------------------------------------
void	KPlayerPK::Init()
{
//	m_nNormalPKFlag			= 1;
	m_nEnmityPKState		= enumPK_ENMITY_STATE_CLOSE;
	m_nEnmityPKAim			= 0;
	m_nEnmityPKTime			= 0;
	m_szEnmityAimName[0]	= 0;
	m_nExercisePKFlag		= 0;
	m_nExercisePKAim		= 0;
	m_szExerciseAimName[0]	= 0;
//	m_nPKValue				= 0;
}

void	KPlayerPK::Active()
{
	EnmityPKCountDown();
}

//-------------------------------------------------------------------------
//	���ܣ��趨����PK״̬ TRUE �򿪣����Կ���  FALSE �رգ������Կ���
//-------------------------------------------------------------------------
void	KPlayerPK::SetNormalPKState(BOOL bFlag, BOOL bShowMsg/* = TRUE*/)
{
	if (m_nNormalPKFlag == bFlag)
		return;

	m_nNormalPKFlag = bFlag;
	CoreDataChanged(GDCNI_PK_SETTING, 0, bFlag);

	if (bShowMsg)
	{
		KSystemMessage	sMsg;
		if (bFlag)
			sprintf(sMsg.szMessage, MSG_PK_NORMAL_FLAG_OPEN);
		else
			sprintf(sMsg.szMessage, MSG_PK_NORMAL_FLAG_CLOSE);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ������������򿪡��ر�����PK״̬
//-------------------------------------------------------------------------
void	KPlayerPK::ApplySetNormalPKState(BOOL bFlag)
{
//	SetNormalPKState(bFlag, FALSE);
	PK_APPLY_NORMAL_FLAG_COMMAND	sApply;
	sApply.ProtocolType = c2s_pkapplychangenormalflag;
	sApply.m_btFlag = bFlag;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(PK_APPLY_NORMAL_FLAG_COMMAND));

}

//-------------------------------------------------------------------------
//	���ܣ��������PK״̬ TRUE �򿪣����Կ���  FALSE �رգ������Կ���
//-------------------------------------------------------------------------
BOOL	KPlayerPK::GetNormalPKState()
{
	return this->m_nNormalPKFlag;
}

//-------------------------------------------------------------------------
//	���ܣ�������������ɱĳ��
//-------------------------------------------------------------------------
void	KPlayerPK::ApplyEnmityPK(char *lpszName)
{
	if (!lpszName || !lpszName[0])
		return;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchName(lpszName);
	if (nNpcIdx <= 0)
		return;
	ApplyEnmityPK(Npc[nNpcIdx].m_dwID);
}

//-------------------------------------------------------------------------
//	���ܣ�������������ɱĳ��
//-------------------------------------------------------------------------
void	KPlayerPK::ApplyEnmityPK(int nNpcID)
{
	if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_PK_ERROR_4);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return;
	}
	if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_PK_ERROR_1);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return;
	}
	int		nIdx = NpcSet.SearchID(nNpcID);
	if (nIdx == 0 || Npc[nIdx].m_Kind != kind_player)
		return;

	PK_APPLY_ENMITY_COMMAND	sApply;
	sApply.ProtocolType = c2s_pkapplyenmity;
	sApply.m_dwNpcID = nNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(PK_APPLY_ENMITY_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ��趨��ɱPK״̬
//-------------------------------------------------------------------------
void	KPlayerPK::SetEnmityPKState(int nState, int nNpcID/* = 0*/, char *lpszName/* = NULL*/)
{
	if (nState == enumPK_ENMITY_STATE_CLOSE)
	{
		if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ENMITY_CLOSE);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}

		m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		m_nEnmityPKAim = 0;
		m_nEnmityPKTime = 0;
		m_szEnmityAimName[0] = 0;
	}
	else if (nState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		m_nEnmityPKAim = nNpcID;
		m_nEnmityPKTime = PK_ANMITY_TIME;
		m_szEnmityAimName[0] = 0;
		if (lpszName)
			strcpy(m_szEnmityAimName, lpszName);

		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sprintf(sMsg.szMessage, MSG_PK_ENMITY_SUCCESS_1, m_szEnmityAimName);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		sprintf(sMsg.szMessage, MSG_PK_ENMITY_SUCCESS_2);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	else	// if (nState == enumPK_ENMITY_STATE_PKING)
	{
		m_nEnmityPKState = enumPK_ENMITY_STATE_PKING;
		m_nEnmityPKTime = 0;
		if (nNpcID > 0)
			m_nEnmityPKAim = nNpcID;
		if (lpszName)
			strcpy(m_szEnmityAimName, lpszName);

		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sprintf(sMsg.szMessage, MSG_PK_ENMITY_OPEN);
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ���ɱ����ʱ
//-------------------------------------------------------------------------
void	KPlayerPK::EnmityPKCountDown()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime < 0)
			m_nEnmityPKTime = 0;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��趨�д�״̬
//-------------------------------------------------------------------------
void	KPlayerPK::SetExercisePKState(int nState, int nNpcID/* = 0*/, char *lpszName/* = NULL*/)
{
	SetEnmityPKState(enumPK_ENMITY_STATE_CLOSE);

	if (nState)
	{
		m_nExercisePKFlag = 1;
		m_nExercisePKAim = nNpcID;
		m_szExerciseAimName[0] = 0;
		if (lpszName)
			strcpy(m_szExerciseAimName, lpszName);
	}
	else
	{
		m_nExercisePKFlag = 0;
		m_nExercisePKAim = 0;
		m_szExerciseAimName[0] = 0;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��趨PKֵ
//-------------------------------------------------------------------------
void	KPlayerPK::SetPKValue(int nValue)
{
	if (m_nPKValue == nValue)
		return;

	this->m_nPKValue = (nValue < 0 ? 0 : nValue);
	if (m_nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_PK_VALUE, m_nPKValue);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

}

#endif














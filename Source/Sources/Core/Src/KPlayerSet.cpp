//#include <objbase.h>
//#include <crtdbg.h>
#include "KCore.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "GameDataDef.h"
#include "KProtocolProcess.h"
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#include "KSubWorldSet.h"
#include "CoreServerShell.h"
#endif
#include "KPlayerSet.h"
#include "Text.h"
#include "CoreUseNameDef.h"

#define		PLAYER_FIRST_LUCKY				0

KPlayerSet PlayerSet;

KPlayerSet::KPlayerSet()
{
#ifdef _SERVER
	m_pWelcomeMsg = NULL;
	m_ulNextSaveTime = 0;
	m_ulDelayTimePerSave = 0;
	m_ulMaxSaveTimePerPlayer = 0;
#endif
}

BOOL	KPlayerSet::Init()
{
	int i;
	
#ifdef _SERVER
	m_nNumPlayer = 0;
	m_ulNextSaveTime = 0;
	m_ulMaxSaveTimePerPlayer = 60 * 20 * 30;
	m_ulDelayTimePerSave = m_ulMaxSaveTimePerPlayer / MAX_PLAYER;
#endif

	// �Ż����ұ�
	m_FreeIdx.Init(MAX_PLAYER);
	m_UseIdx.Init(MAX_PLAYER);

	// ��ʼʱ���е�����Ԫ�ض�Ϊ��
	for (i = MAX_PLAYER - 1; i > 0; i--)
	{
		m_FreeIdx.Insert(i);
	}

	if ( !m_cLevelAdd.Init() )
		return FALSE;
	if ( !m_cLeadExp.Init() )
		return FALSE;
	for (i = 0; i < MAX_PLAYER; i++)
	{
		Player[i].Release();
		Player[i].SetPlayerIndex(i);
#ifdef _SERVER
		Player[i].m_cPK.Init(i);
#endif
		Player[i].m_cTong.Init(i);
		Player[i].m_ItemList.Init(i);
		Player[i].m_Node.m_nIndex = i;
	}

	if ( !m_cNewPlayerAttribute.Init() )
		return FALSE;

#ifdef _SERVER

	KTabFile	cPKParam;

	memset(m_sPKPunishParam, 0, sizeof(m_sPKPunishParam));
//	g_SetFilePath("\\");
	if (!cPKParam.Load(defPK_PUNISH_FILE))
		return FALSE;
	for (i = 0; i < MAX_DEATH_PUNISH_PK_VALUE + 1; i++)
	{
		cPKParam.GetInteger(i + 2, 2, 1, &m_sPKPunishParam[i].m_nExp);
		cPKParam.GetInteger(i + 2, 3, 1, &m_sPKPunishParam[i].m_nMoney);
		cPKParam.GetInteger(i + 2, 4, 1, &m_sPKPunishParam[i].m_nItem);
		cPKParam.GetInteger(i + 2, 5, 1, &m_sPKPunishParam[i].m_nEquip);
	}
#endif

	// ������
	KIniFile	cTongFile;
	if (cTongFile.Load(defPLAYER_TONG_PARAM_FILE))
	{
		cTongFile.GetInteger("TongCreate", "Level", 60, &m_sTongParam.m_nLevel);
		cTongFile.GetInteger("TongCreate", "LeadLevel", 10, &m_sTongParam.m_nLeadLevel);
		cTongFile.GetInteger("TongCreate", "Money", 500000, &m_sTongParam.m_nMoney);
	}
	else
	{
		m_sTongParam.m_nLevel		= 60;
		m_sTongParam.m_nLeadLevel	= 10;
		m_sTongParam.m_nMoney		= 500000;
	}

	return TRUE;
}

int	KPlayerSet::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

int KPlayerSet::FindSame(DWORD dwID)
{
	int nUseIdx = 0;

	nUseIdx = m_UseIdx.GetNext(0);
	while(nUseIdx)
	{
		if (Player[nUseIdx].m_dwID == dwID)
			return nUseIdx;
		nUseIdx = m_UseIdx.GetNext(nUseIdx);
	}
	return 0;
}

int		KPlayerSet::GetFirstPlayer()
{
	m_nListCurIdx = m_UseIdx.GetNext(0);
	return m_nListCurIdx;
}

int		KPlayerSet::GetNextPlayer()
{
	if ( !m_nListCurIdx )
		return 0;
	m_nListCurIdx = m_UseIdx.GetNext(m_nListCurIdx);
	return m_nListCurIdx;
}

#ifdef _SERVER
int KPlayerSet::Add(LPSTR szPlayerID, void* pGuid)
{
	if (!pGuid || !szPlayerID || !szPlayerID[0])
		return 0;

	int i;
/*
	DWORD dwID = g_FileName2Id(szPlayerID);
	i = FindSame(dwID);
	if (i)
		return 0;
*/
	DWORD dwID = g_FileName2Id(szPlayerID);

	i = FindFree();

	if (i)
	{
		Player[i].m_dwID = dwID;
		Player[i].m_nNetConnectIdx = -1;
		Player[i].m_dwLoginTime = g_SubWorldSet.GetGameTime();
		memcpy(&Player[i].m_Guid, pGuid, sizeof(GUID));
		Player[i].SetPlayerIndex(i);
		m_FreeIdx.Remove(i);
		m_UseIdx.Insert(i);
		m_nNumPlayer ++;
		return i;
	}
	return 0;
}

int	KPlayerSet::Broadcasting(char* pMessage, int nLen)
{
	if ( !pMessage || nLen <= 0 || nLen >= MAX_SENTENCE_LENGTH)
		return 0;
	if (!g_pServer)
		return 0;
	g_pServer->PreparePackSink();
	KPlayerChat::SendSystemInfo(0, 0, MESSAGE_SYSTEM_ANNOUCE_HEAD, pMessage, nLen);
	g_pServer->SendPackToClient(-1);
	return 1;
}


void KPlayerSet::SendWelcomeMsg(int nPlayerIndex)
{
	//====֪ͨ���˼���====
	char	szBuffer[MAX_SENTENCE_LENGTH];
	int		nLength;
	static int nMsg = 0;
	const char* Msgs[3] =
	{	"��<color=green>%s<color=restore>��Ҳ����ȿ������ݡ�",
		"<color=green>%s<color=restore>��̤�貨΢����ƮȻ������",
		"һ�����⣬<color=green>%s<color=restore>���������" };

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, Msgs[nMsg], Player[nPlayerIndex].m_PlayerName);
	nMsg++;
	nMsg %= 3;
	nLength = TEncodeText(szBuffer, strlen(szBuffer));
	KPlayerChat::SendSystemInfo(0, 0, MESSAGE_SYSTEM_ANNOUCE_HEAD, szBuffer, nLength);

	//====���ͻ�ӭ��Ϣ
	if (m_pWelcomeMsg)
	{
		int	*pAMsg = (int*)m_pWelcomeMsg;
		int nCount =  *pAMsg;
		pAMsg ++;
		for (int i = 0; i < nCount; i++)
		{
			int	nLen = *pAMsg;
			KPlayerChat::SendSystemInfo(1, nPlayerIndex, "�⸮�ܼ�", (((char*)pAMsg) + sizeof(int)), nLen);
			pAMsg = (int*)(((char*)pAMsg) + nLen + sizeof(int));
		}
	}
}

void KPlayerSet::ReloadWelcomeMsg()
{
	return;
	/*    ��ӭ��Ϣ�ı��ļ���ʽҪ��
	ÿ��Ϊһ����Ϣ��  Msg:��Ϣ����	
	������Ϣ���ݵĳ��ȣ�ʹÿ����Ϣ���������᳤�Ȳ�����MAX_SENTENCE_LENGTH
	*/

	if (m_pWelcomeMsg)
	{
		free (m_pWelcomeMsg);
		m_pWelcomeMsg = NULL;
	}
	KFile		File;
	char		Head[8];
	if (File.Open("\\Msg\\WelcomeMsg.txt") == FALSE)
		return;

	int nLen = File.Size();
	m_pWelcomeMsg = malloc(nLen + sizeof(int));
	if (m_pWelcomeMsg == NULL)
		return;

	*(int*)m_pWelcomeMsg = 0;
	char* pSearchBegin = (char*)m_pWelcomeMsg + sizeof(int);
	int* pLineHeader = (int*)pSearchBegin;
	File.Read(pLineHeader, nLen);
	File.Close();

	char* pMsgHeader;
	while(pMsgHeader = (char*)memchr(pSearchBegin, ':', nLen))
	{
		pMsgHeader++;	//�����Ǹ�ð��
		nLen -= pMsgHeader -pSearchBegin; //ð�ź��ʣ�೤��
		if (nLen <= 0)
			break;
		int	nSkipLen = pMsgHeader - (char*)pLineHeader - sizeof(int);
		pSearchBegin = pMsgHeader;
		if (nSkipLen < 0)
			continue;
		if (nSkipLen > 0)
			memset(&pLineHeader[1], 0, nSkipLen);
		char* pMsgTail = (char*)memchr(pSearchBegin, 0x0d, nLen);	//�����н�β
		int	nMsgLen;
		if (pMsgTail)
		{
			*pMsgTail = 0;
			pMsgTail++;
			if ((pMsgTail < pSearchBegin + nLen) && *pMsgTail == 0x0a)
			{
				*pMsgTail = 0;
				pMsgTail++;
			}
			nMsgLen = pMsgTail - pSearchBegin;
			pSearchBegin = pMsgTail;
		}
		else
		{
			nMsgLen = nLen;
		}
		nLen -= nMsgLen;
		nMsgLen = TEncodeText((char*)&pLineHeader[1], nMsgLen + nSkipLen);		
		if (nMsgLen > 0)
		{
			*pLineHeader = nMsgLen;
			pLineHeader = (int*)(((char*)pLineHeader) + sizeof(int) + (*pLineHeader));
			(*(int*)m_pWelcomeMsg) ++;
		}
		if (pMsgTail == NULL || nLen <= 0)
			break;
	}
	if ((int*)m_pWelcomeMsg == 0)
	{
		free(m_pWelcomeMsg);
		m_pWelcomeMsg = NULL;
	}
}

void KPlayerSet::PrepareRemove(int nIndex)
{
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	g_DebugLog("SERVER:Player[%s] has been removed!", Npc[Player[nIndex].m_nIndex].Name);
	// ֪ͨ��������Լ�������
	Player[nIndex].m_cChat.OffLine(Player[nIndex].m_dwID);
	// �����ӣ��뿪����
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	Player[nIndex].LeaveTeam((BYTE*)&sLeaveTeam);
	// ����������ڽ��У�ȡ������
	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	Player[nIndex].TradeDecision((BYTE*)&sTrade);
	// PK״̬ȡ��
	Player[nIndex].m_cPK.CloseAll();

//	Player[nIndex].Save();
	Player[nIndex].WaitForRemove();
}

void KPlayerSet::PrepareLoginFailed(int nIndex)
{
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	g_DebugLog("SERVER:Player[%s] has been removed!", Npc[Player[nIndex].m_nIndex].Name);
	// ֪ͨ��������Լ�������
	Player[nIndex].m_cChat.OffLine(Player[nIndex].m_dwID);
	// �����ӣ��뿪����
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	Player[nIndex].LeaveTeam((BYTE*)&sLeaveTeam);
	// ����������ڽ��У�ȡ������
	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	Player[nIndex].TradeDecision((BYTE*)&sTrade);
	// PK״̬ȡ��
	Player[nIndex].m_cPK.CloseAll();
	// ���״̬����
	Player[nIndex].m_cTong.Clear();

	// ����TimeOut����Player
	Player[nIndex].m_nNetConnectIdx = -1;
	Player[nIndex].m_dwLoginTime = 0;
}

void KPlayerSet::PrepareExchange(int i)
{
	if (!Player[i].m_bExchangeServer || !Npc[Player[i].m_nIndex].m_bExchangeServer)
		return;

	// ���PK״̬
	Player[i].m_cPK.CloseAll();

	// ���
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	Player[i].LeaveTeam((BYTE*)&sLeaveTeam);

	// ����������ڽ��У�ȡ������
	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	Player[i].TradeDecision((BYTE*)&sTrade);

	Player[i].Save();
//	Player[i].UpdateEnterGamePos(Player[i].m_sExchangePos.m_dwMapID,
//		Player[i].m_sExchangePos.m_nX,
//		Player[i].m_sExchangePos.m_nY,
//		Npc[Player[i].m_nIndex].m_FightMode);
}


void KPlayerSet::RemoveQuiting(int nIndex)
{
	if (Player[nIndex].m_nNetConnectIdx == -1 || Player[nIndex].m_dwID == 0)
		return;

	if (Player[nIndex].IsWaitingRemove())
	{
		if (Player[nIndex].m_nIndex > 0)	// have npc
		{
			int nRegion = Npc[Player[nIndex].m_nIndex].m_RegionIndex;
			int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
			
			if (nSubWorld >= 0 && nRegion >= 0)
			{
				SubWorld[nSubWorld].RemovePlayer(nRegion, nIndex);
				SubWorld[nSubWorld].m_MissionArray.RemovePlayer(nIndex);
				SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(Player[nIndex].m_nIndex);
				SubWorld[nSubWorld].m_Region[nRegion].DecRef(Npc[Player[nIndex].m_nIndex].m_MapX, Npc[Player[nIndex].m_nIndex].m_MapY, obj_npc);
			}	
			NpcSet.Remove(Player[nIndex].m_nIndex);
		}
		// ���״̬����
		Player[nIndex].m_cTong.Clear();

		Player[nIndex].m_ItemList.RemoveAll();
		Player[nIndex].m_dwID = 0;
		Player[nIndex].m_nIndex = 0;
		Player[nIndex].m_nNetConnectIdx = -1;
		Player[nIndex].Release();
		
		m_FreeIdx.Insert(nIndex);
		m_UseIdx.Remove(nIndex);
		m_nNumPlayer --;
	}
}	

void KPlayerSet::RemoveLoginTimeOut(int nIndex)
{
	if (Player[nIndex].IsLoginTimeOut())
	{
		if (Player[nIndex].m_nIndex > 0)
		{
			int nRegion = Npc[Player[nIndex].m_nIndex].m_RegionIndex;
			int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
		
/*		if (nSubWorld >= 0)
			SubWorld[nSubWorld].RemovePlayer(nRegion, nIndex);*/
		
			if (nSubWorld >= 0 && nRegion >= 0)
			{
				SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(Player[nIndex].m_nIndex);
				SubWorld[nSubWorld].m_Region[nRegion].DecRef(Npc[Player[nIndex].m_nIndex].m_MapX, Npc[Player[nIndex].m_nIndex].m_MapY, obj_npc);
			}
			NpcSet.Remove(Player[nIndex].m_nIndex);
		}		
		Player[nIndex].m_ItemList.RemoveAll();
		Player[nIndex].m_dwID = 0;
		Player[nIndex].m_nIndex = 0;
		Player[nIndex].m_nNetConnectIdx = -1;
		Player[nIndex].LoginTimeOut();
		
		m_FreeIdx.Insert(nIndex);
		m_UseIdx.Remove(nIndex);
		m_nNumPlayer --;
	}
}

void KPlayerSet::RemoveExchanging(int nIndex)
{
	if (Player[nIndex].m_nNetConnectIdx == -1)
		return;

	if (Player[nIndex].IsExchangingServer())
	{
		int nRegion = Npc[Player[nIndex].m_nIndex].m_RegionIndex;
		int nSubWorld = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
		
		if (nSubWorld >= 0 && nRegion >= 0)
		{
			SubWorld[nSubWorld].RemovePlayer(nRegion, nIndex);
			SubWorld[nSubWorld].m_MissionArray.RemovePlayer(nIndex);
			SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(Player[nIndex].m_nIndex);
			SubWorld[nSubWorld].m_Region[nRegion].DecRef(Npc[Player[nIndex].m_nIndex].m_MapX, Npc[Player[nIndex].m_nIndex].m_MapY, obj_npc);
		}
		
		if (Player[nIndex].m_nIndex > 0)
			NpcSet.Remove(Player[nIndex].m_nIndex);
		
		Player[nIndex].m_ItemList.RemoveAll();
		Player[nIndex].m_dwID = 0;
		Player[nIndex].m_nIndex = 0;
		Player[nIndex].m_nNetConnectIdx = -1;
		Player[nIndex].Release();
		
		m_FreeIdx.Insert(nIndex);
		m_UseIdx.Remove(nIndex);
		m_nNumPlayer --;
	}
}
#endif

/*
int KPlayerSet::FindClient(int nClient)
{
	int i;

	i = m_UseIdx.GetNext(0);
	while(i)
	{
		if (Player[i].m_nNetConnectIdx == nClient)
			return i;
		i = m_UseIdx.GetNext(i);
	}
	return 0;
}
*/

#ifdef _SERVER
void KPlayerSet::ProcessClientMessage(int nIndex, const char* pChar, int nSize)
{
	if (nIndex <= 0 && nIndex >= MAX_PLAYER)
		return;

	int i = Player[nIndex].m_nNetConnectIdx;

	if (i >= 0)
	{
//		_ASSERT(g_CoreServerShell.CheckProtocolSize(pChar, nSize));
		if (Player[nIndex].m_dwID && !Player[nIndex].m_bExchangeServer)
			g_ProtocolProcess.ProcessNetMsg(nIndex, (BYTE*)pChar);
	}
}
#endif

//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KLevelAdd::KLevelAdd()
{
	memset(m_nLevelExp, 0, sizeof(int) * MAX_LEVEL);
	memset(m_nLifePerLevel, 0, sizeof(int) * series_num);
	memset(m_nManaPerLevel, 0, sizeof(int) * series_num);
	memset(m_nStaminaPerLevel, 0, sizeof(int) * series_num);
	memset(m_nLifePerVitality, 0, sizeof(int) * series_num);
	memset(m_nStaminaPerVitality, 0, sizeof(int) * series_num);
	memset(m_nManaPerEnergy, 0, sizeof(int) * series_num);
}

//---------------------------------------------------------------------------
//	���ܣ���ʼ��
//---------------------------------------------------------------------------
BOOL	KLevelAdd::Init()
{
	int			i;
	KTabFile	LevelExp;
	if ( !LevelExp.Load(PLAYER_LEVEL_EXP_FILE) )
		return FALSE;
	for (i = 0; i < MAX_LEVEL; i++)
	{
		LevelExp.GetInteger(i + 2, 2, 0, &m_nLevelExp[i]);
	}

	KTabFile	LevelAdd;
	if ( !LevelAdd.Load(PLAYER_LEVEL_ADD_FILE) )
		return FALSE;
	for (i = 0; i < series_num; i++)
	{
		LevelAdd.GetInteger(i + 2, 2, 0, &m_nLifePerLevel[i]);
		LevelAdd.GetInteger(i + 2, 3, 0, &m_nStaminaPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 4, 0, &m_nManaPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 5, 0, &m_nLifePerVitality[i]);
		LevelAdd.GetInteger(i + 2, 6, 0, &m_nStaminaPerVitality[i]);
		LevelAdd.GetInteger(i + 2, 7, 0, &m_nManaPerEnergy[i]);
		LevelAdd.GetInteger(i + 2, 8, 0, &m_nLeadExpShare[i]);
		LevelAdd.GetInteger(i + 2, 9, 0, &m_nFireResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 10, 0, &m_nColdResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 11, 0, &m_nPoisonResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 12, 0, &m_nLightResistPerLevel[i]);
		LevelAdd.GetInteger(i + 2, 13, 0, &m_nPhysicsResistPerLevel[i]);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ����ĳ�ȼ�����������
//	������nLevel  Ŀ��ȼ�
//---------------------------------------------------------------------------
int		KLevelAdd::GetLevelExp(int nLevel)
{
	if (nLevel < 1 || nLevel > MAX_LEVEL)
		return 0;
	return m_nLevelExp[nLevel - 1];
}

//---------------------------------------------------------------------------
//	���ܣ����ÿ��ϵ������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetLifePerLevel(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nLifePerLevel[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ����ÿ��ϵ������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetStaminaPerLevel(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nStaminaPerLevel[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ����ÿ��ϵ������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetManaPerLevel(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nManaPerLevel[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵ����������һ�������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetLifePerVitality(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nLifePerVitality[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵ����������һ�������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetStaminaPerVitality(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nStaminaPerVitality[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵ����������һ�������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetManaPerEnergy(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nManaPerEnergy[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵ����������һ�������������
//---------------------------------------------------------------------------
int		KLevelAdd::GetLeadExpShare(int nSeries)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	return m_nLeadExpShare[nSeries];
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵĳ���ȼ��Ļ�������
//---------------------------------------------------------------------------
int		KLevelAdd::GetFireResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel >= MAX_LEVEL)
		return 0;
	return (m_nFireResistPerLevel[nSeries] * nLevel / 100);
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵĳ���ȼ��Ļ���������
//---------------------------------------------------------------------------
int		KLevelAdd::GetColdResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel >= MAX_LEVEL)
		return 0;
	return (m_nColdResistPerLevel[nSeries] * nLevel / 100);
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵĳ���ȼ��Ļ���������
//---------------------------------------------------------------------------
int		KLevelAdd::GetPoisonResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel >= MAX_LEVEL)
		return 0;
	return (m_nPoisonResistPerLevel[nSeries] * nLevel / 100);
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵĳ���ȼ��Ļ����翹��
//---------------------------------------------------------------------------
int		KLevelAdd::GetLightResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel >= MAX_LEVEL)
		return 0;
	return (m_nLightResistPerLevel[nSeries] * nLevel / 100);
}

//---------------------------------------------------------------------------
//	���ܣ�ÿ��ϵĳ���ȼ��Ļ���������
//---------------------------------------------------------------------------
int		KLevelAdd::GetPhysicsResist(int nSeries, int nLevel)
{
	if (nSeries < 0 || nSeries >= series_num)
		return 0;
	if (nLevel <= 0 || nLevel >= MAX_LEVEL)
		return 0;
	return (m_nPhysicsResistPerLevel[nSeries] * nLevel / 100);
}

//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KTeamLeadExp::KTeamLeadExp()
{
	for (int i = 0; i < MAX_LEAD_LEVEL; i++)
	{
		m_sPerLevel[i].m_dwExp = 0;
		m_sPerLevel[i].m_dwMemNum = 1;
	}
}

//---------------------------------------------------------------------------
//	���ܣ���ʼ������ȡͳ��������
//---------------------------------------------------------------------------
BOOL	KTeamLeadExp::Init()
{
	int			i;
	KTabFile	LevelExp;

	if ( !LevelExp.Load(PLAYER_LEVEL_LEAD_EXP_FILE) )
		return FALSE;
	for (i = 0; i < MAX_LEAD_LEVEL; i++)
	{
		LevelExp.GetInteger(i + 2, 2, 0, (int*)&m_sPerLevel[i].m_dwExp);
		LevelExp.GetInteger(i + 2, 3, 1, (int*)&m_sPerLevel[i].m_dwMemNum);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ����뾭���õȼ���
//---------------------------------------------------------------------------
int		KTeamLeadExp::GetLevel(DWORD dwExp, int nCurLeadLevel)
{
	if (dwExp <= 0)
		return 1;
	if (nCurLeadLevel > 0)
	{
		if (nCurLeadLevel >= MAX_LEAD_LEVEL)
			return MAX_LEAD_LEVEL;
		if (dwExp < m_sPerLevel[nCurLeadLevel - 1].m_dwExp)
			return nCurLeadLevel;
		if (dwExp < m_sPerLevel[nCurLeadLevel].m_dwExp)
			return nCurLeadLevel + 1;
	}
	for (int i = 0; i < MAX_LEAD_LEVEL; i++)
	{
		if (dwExp < m_sPerLevel[i].m_dwExp)
		{
			return i + 1;
		}
	}
	return MAX_LEAD_LEVEL;
}

//---------------------------------------------------------------------------
//	���ܣ����뾭���ÿɴ���Ա��
//---------------------------------------------------------------------------
int		KTeamLeadExp::GetMemNumFromExp(DWORD dwExp)
{
	int nGetLevel;
	nGetLevel = GetLevel(dwExp);
	return m_sPerLevel[nGetLevel - 1].m_dwMemNum;
}

//---------------------------------------------------------------------------
//	���ܣ�����ȼ���ÿɴ���Ա��
//---------------------------------------------------------------------------
int		KTeamLeadExp::GetMemNumFromLevel(int nLevel)
{
	if (1 <= nLevel && nLevel <= MAX_LEAD_LEVEL)
		return m_sPerLevel[nLevel - 1].m_dwMemNum;
	return 1;
}

//---------------------------------------------------------------------------
//	���ܣ�����ȼ�����������辭��ֵ
//---------------------------------------------------------------------------
int		KTeamLeadExp::GetLevelExp(int nLevel)
{
	if (1 <= nLevel && nLevel <= MAX_LEAD_LEVEL)
		return this->m_sPerLevel[nLevel - 1].m_dwExp;
	return 0;
}

#ifdef _SERVER
void	KPlayerSet::Activate()
{
}
#endif

KNewPlayerAttribute::KNewPlayerAttribute()
{
	memset(m_nStrength, 0, sizeof(m_nStrength));
	memset(m_nDexterity, 0, sizeof(m_nDexterity));
	memset(m_nVitality, 0, sizeof(m_nVitality));
	memset(m_nEngergy, 0, sizeof(m_nEngergy));
	memset(m_nLucky, 0, sizeof(m_nLucky));
}

BOOL	KNewPlayerAttribute::Init()
{
	char		szSeries[5][16] = {"metal", "wood", "water", "fire", "earth"};
	KIniFile	AttributeFile;

	if ( !AttributeFile.Load(BASE_ATTRIBUTE_FILE_NAME) )
		return FALSE;

	for (int i = 0; i < series_num; i++)
	{
		AttributeFile.GetInteger(szSeries[i], "Strength", 0, &m_nStrength[i]);
		AttributeFile.GetInteger(szSeries[i], "Dexterity", 0, &m_nDexterity[i]);
		AttributeFile.GetInteger(szSeries[i], "Vitality", 0, &m_nVitality[i]);
		AttributeFile.GetInteger(szSeries[i], "Engergy", 0, &m_nEngergy[i]);
		AttributeFile.GetInteger(szSeries[i], "Lucky", PLAYER_FIRST_LUCKY, &m_nLucky[i]);
	}

	return TRUE;
}

#ifdef _SERVER
BOOL	KPlayerSet::GetPlayerName(int nIndex, char* szName)
{
//	int i = FindClient(nClient);

	int i = nIndex;

	if (!szName)
		return FALSE;

	if (i <= 0 || i >= MAX_PLAYER)
	{
		szName[0] = 0;
		return FALSE;
	}
	strcpy(szName, Player[i].m_PlayerName);
	return TRUE;
}
#endif

#ifdef _SERVER
BOOL	KPlayerSet::GetPlayerAccount(int i, char* szName)
{
//	int i = FindClient(nClient);
	if (!szName)
		return FALSE;

	if (i <= 0 || i >= MAX_PLAYER)
	{
		szName[0] = 0;
		return FALSE;
	}

	strcpy(szName, Player[i].m_AccoutName);
	return TRUE;
}
#endif


#ifdef _SERVER
int		KPlayerSet::AttachPlayer(const unsigned long lnID, GUID* pGuid)
{
	if (lnID >= MAX_PLAYER || NULL == pGuid)
		return 0;

	int nUseIdx = m_UseIdx.GetPrev(0);
	while(nUseIdx)
	{
		if (Player[nUseIdx].m_nNetConnectIdx == -1)
		{
			if (0 == memcmp(&Player[nUseIdx].m_Guid, pGuid, sizeof(GUID)))
			{
				Player[nUseIdx].m_nNetConnectIdx = lnID;
				Player[nUseIdx].m_ulLastSaveTime = g_SubWorldSet.m_nLoopRate;
///				Player[nUseIdx].m_dwLoginTime = -1;
				return nUseIdx;
			}
//			else
//			{
//				g_DebugLog("[error]Attach to a connected player named %s", Player[nUseIdx].m_PlayerName);
//				return 0;
//			}
		}
		nUseIdx = m_UseIdx.GetPrev(nUseIdx);
	}
	return 0;
}
#endif

#ifdef _SERVER
int		KPlayerSet::GetPlayerIndexByGuid(GUID* pGuid)
{
	int nUseIdx = m_UseIdx.GetNext(0);
	while(nUseIdx)
	{
		if (0 == memcmp(&Player[nUseIdx].m_Guid, pGuid, sizeof(GUID)))
		{
			if (Player[nUseIdx].m_nNetConnectIdx != -1)
			{
				return nUseIdx;
			}
			else
			{
				g_DebugLog("[error]Find Guid to a disconnect player");
				return 0;
			}
		}
		nUseIdx = m_UseIdx.GetNext(nUseIdx);
	}
	return 0;
}
#endif

#ifdef _SERVER
void KPlayerSet::AutoSave()
{
	unsigned long uTime = g_SubWorldSet.GetGameTime();

	if (uTime >= m_ulNextSaveTime)
	{
		int nUseIdx = m_UseIdx.GetNext(0);
		while(nUseIdx)
		{
			if (Player[nUseIdx].CanSave() && uTime - Player[nUseIdx].m_ulLastSaveTime >= m_ulMaxSaveTimePerPlayer)
			{
				if (Player[nUseIdx].Save())
				{
					Player[nUseIdx].m_uMustSave = SAVE_REQUEST;
//					Player[nUseIdx].m_ulLastSaveTime = uTime;
					m_ulNextSaveTime += m_ulDelayTimePerSave;
				}
				break;
			}
			nUseIdx = m_UseIdx.GetNext(nUseIdx);
		}
	}
}
#endif

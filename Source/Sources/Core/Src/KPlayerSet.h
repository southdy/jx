#ifndef KPlayerSetH
#define	KPlayerSetH

#include "KLinkArray.h"

#define		MAX_LEVEL					150		// ������ȼ�
#define		MAX_SERIES					5
#define		MAX_LEAD_LEVEL				100		// ���ͳ�����ȼ�

typedef struct
{
	int		m_nExp;
	int		m_nMoney;
	int		m_nItem;
	int		m_nEquip;
} KPK_DEATH_PUNISH_PARAM;

typedef struct
{
	int		m_nLevel;
	int		m_nLeadLevel;
	int		m_nMoney;
} STONG_CREATE_PARAM;


class KLevelAdd
{
private:
	int		m_nLevelExp[MAX_LEVEL];				// ��������
	int		m_nLifePerLevel[MAX_SERIES];			// ÿ��ϵ������������
	int		m_nStaminaPerLevel[MAX_SERIES];			// ÿ��ϵ������������
	int		m_nManaPerLevel[MAX_SERIES];			// ÿ��ϵ������������
	int		m_nLifePerVitality[MAX_SERIES];			// ÿ��ϵ����������һ�������������
	int		m_nStaminaPerVitality[MAX_SERIES];		// ÿ��ϵ����������һ�������������
	int		m_nManaPerEnergy[MAX_SERIES];			// ÿ��ϵ����������һ�������������
	int		m_nLeadExpShare[MAX_SERIES];			// ÿ��ϵ���ս������ʱ����ͳ��������Ĳ���
	int		m_nFireResistPerLevel[MAX_SERIES];		// ÿ��ϵ�����ӻ���
	int		m_nColdResistPerLevel[MAX_SERIES];		// ÿ��ϵ�����ӱ�����
	int		m_nPoisonResistPerLevel[MAX_SERIES];	// ÿ��ϵ�����Ӷ�����
	int		m_nLightResistPerLevel[MAX_SERIES];		// ÿ��ϵ�����ӵ翹��
	int		m_nPhysicsResistPerLevel[MAX_SERIES];	// ÿ��ϵ������������
public:
	KLevelAdd();
	BOOL		Init();
	int			GetLevelExp(int nLevel);
	int			GetLifePerLevel(int nSeries);
	int			GetStaminaPerLevel(int nSeries);
	int			GetManaPerLevel(int nSeries);
	int			GetLifePerVitality(int nSeries);
	int			GetStaminaPerVitality(int nSeries);
	int			GetManaPerEnergy(int nSeries);
	int			GetLeadExpShare(int nSeries);
	int			GetFireResist(int nSeries, int nLevel);
	int			GetColdResist(int nSeries, int nLevel);
	int			GetPoisonResist(int nSeries, int nLevel);
	int			GetLightResist(int nSeries, int nLevel);
	int			GetPhysicsResist(int nSeries, int nLevel);
};


class KTeamLeadExp
{
public:
	struct LEADLEVEL
	{
		DWORD	m_dwExp;								// ͳ������������
		DWORD	m_dwMemNum;								// ͳ�ʵȼ�������ͳ�ʵĶ�Ա����
	}	m_sPerLevel[MAX_LEAD_LEVEL];
public:
	KTeamLeadExp();
	BOOL		Init();									// ��ʼ��
	int			GetLevel(DWORD dwExp, int nCurLeadLevel = 0);// ���뾭���õȼ��� (������뵱ǰ�ȼ������Լӿ������ٶ�)
	int			GetMemNumFromExp(DWORD dwExp);			// ���뾭���ÿɴ���Ա��
	int			GetMemNumFromLevel(int nLevel);			// ����ȼ���ÿɴ���Ա��
	int			GetLevelExp(int nLevel);				// ����ȼ�����������辭��ֵ
};

class KNewPlayerAttribute
{
public:
	int			m_nStrength[series_num];
	int			m_nDexterity[series_num];
	int			m_nVitality[series_num];
	int			m_nEngergy[series_num];
	int			m_nLucky[series_num];
public:
	KNewPlayerAttribute();
	BOOL		Init();
};

class KPlayerSet
{
private:	// �����Ż������ٶ�
	KLinkArray		m_FreeIdx;				//	���ñ�
	KLinkArray		m_UseIdx;				//	���ñ�
	int				m_nListCurIdx;			// ���� GetFirstPlayer �� GetNextPlayer
#ifdef _SERVER
	unsigned long	m_ulNextSaveTime;
	unsigned long	m_ulDelayTimePerSave;	//1mins
	unsigned long	m_ulMaxSaveTimePerPlayer; //30mins
#endif
public:
	KLevelAdd				m_cLevelAdd;
	KTeamLeadExp			m_cLeadExp;
	KNewPlayerAttribute		m_cNewPlayerAttribute;
	STONG_CREATE_PARAM		m_sTongParam;

#ifdef _SERVER
	KPK_DEATH_PUNISH_PARAM	m_sPKPunishParam[MAX_DEATH_PUNISH_PK_VALUE + 1];	// PK�ͷ�����
#endif

public:
	KPlayerSet();
	BOOL	Init();
	int		FindSame(DWORD dwID);
	int		GetFirstPlayer();				// ����������ҵ�һ��
	int		GetNextPlayer();				// �������������һ��(��֧������������һ֧����֮����ܵ���)
	int		GetOnlinePlayerCount() { return m_UseIdx.GetCount(); }
//	int		FindClient(int nClient);

#ifdef	_SERVER
	void	Activate();
	int		Add(LPSTR szPlayerID, void* pGuid);
	void	PrepareRemove(int nClientIdx);
	void	PrepareExchange(int nIndex);
	void	PrepareLoginFailed(int nIndex);
	void	RemoveQuiting(int nIndex);
	void	RemoveLoginTimeOut(int nIndex);
	void	RemoveExchanging(int nIndex);
	void	AutoSave();
	void	ProcessClientMessage(int nClient, const char* pChar, int nSize);
	int		GetPlayerNumber() { return m_nNumPlayer; }
	BOOL	GetPlayerName(int nClient, char* szName);
	BOOL	GetPlayerAccount(int nClient, char* szName);
	int		Broadcasting(char* pMessage, int nLen);
	void	SetSaveBufferPtr(void* pData);
	int		AttachPlayer(const unsigned long lnID, GUID* pGuid);
	int		GetPlayerIndexByGuid(GUID* pGuid);
#endif

private:
	int		FindFree();

#ifdef	_SERVER
	//welcome message deliver
public:
	void	ReloadWelcomeMsg();

private:
	void	SendWelcomeMsg(int nPlayerIndex);
	void*	m_pWelcomeMsg;

private:
	int		m_nNumPlayer;
#endif
};

extern KPlayerSet PlayerSet;
#endif //KPlayerSetH

#pragma  once
#ifdef _PARTNER
enum ePartnerAction
{
	partner_action_attacksomeone,
	partner_action_assistsomeone,
	partner_action_killself,
	partner_action_followsomeone,
	partner_action_gotowhere,
	partner_action_autoai,
	partner_action_setcommonai,
};

enum ePartnerNotify
{
	partner_notify_havekillsomeone,
	partner_notify_ownerdead,
	partner_notify_
};
struct TPartnerParam
{
	int nParam1;
	int nParam2;
	int nParam3;
};

typedef struct 
{
	
}TMpsPos;

typedef struct 
{
	
	
}TMapPos;

enum ePartnerType
{
	//�ٻ���
	partnercount,
};

typedef struct 
{
	unsigned long m_ulTimeOutVanish;//�Ƿ��д��ʱ�䣬���ʱ�䵽�˾�����.0��ʾû�У� ����0��ʾ���ʱ��
	BOOL m_bDeathVanish;//�Ƿ�����֮�������.
	BOOL m_bDeathRevive;//�Ƿ�������������
	unsigned long m_ulOwnerNpcIndex;
}
tPartnerProp;

struct TPartnerParam
{
	int nNpcIndex;
	ePartnerType eType;
};

class KPlayerPartner
{
private://��Ա����
	ePartnerType m_ePartnerType;
	unsigned long m_ulNpcIndex;
	unsigned long m_ulCurLifeTime;
	tPartnerProp m_Prop;

private://��Ա����
	void SetType(ePartnerType eType)
	{
		m_ePartnerType = eType;
	};
	void SetNpcIndex(unsigned long ulNpcIndex)
	{
		m_ulNpcIndex = ulNpcIndex;
	};
	KNpc * GetNpc() const
	{
		if (m_ulNpcIndex != 0)
		{
			if (Npc[m_ulNpcIndex].m_Index > 0)
				return &Npc[m_ulNpcIndex];
		}
		return NULL;
	}
	ePartnerType GetType() const 
	{
		return m_ePartnerType;
	};
	
	int SendCommand(ePartnerType eType, TPartnerParam *pParam);
	BOOL InitData();
//---------------------------------------------------------------	
public://��Ա����
	

public://��Ա����
	KPlayerPartner();
	BOOL Create();
	
	BOOL Init(int nNpcIndex, tPartnerProp Prop);
	BOOL Init(int nNpcTemplateId, int nLevel, int nSubWorld, int nRegion, int nMapX, int nMapY, int nOffX, int nOffY , tPartnerProp Prop);
	BOOL Init(int nNpcTemplateId, int nLevel, int nSubWorld, int nMpsX, int nMpsY, tPartnerProp Prop);
	
	int	 Activate();
};

class KPartner
{
public:
	virtual int		SendCommand(ePartnerType eType, TPartnerParam *pParam);
	virtual	int		SendNotify();
	unsigned long GetIndex();
	BOOL	Create();

};

/*

class KNpcPartnerControl 
{
	
};

class 
{
	virtual void * GetData();
	virtual void * ChangeData();
};
*/
#endif
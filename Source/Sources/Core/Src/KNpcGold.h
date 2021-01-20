//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcGold.h
// Date:	2003.07.23
// Code:	�߳�����
// Desc:	KNpcGold Class
//---------------------------------------------------------------------------

#ifndef _KNPCGOLD_H
#define	_KNPCGOLD_H

#define		defMAX_NPC_GOLD_TYEP	8

class KNpcGold
{
	friend class KNpc;
private:
	int		m_nNpcIdx;
	int		m_nIsGold;			// �Ƿ��ǻƽ������
	int		m_nIsGolding;		// ��ǰ�Ƿ��ǻƽ����
	int		m_nGoldType;		// �����ǰ�ǻƽ����仯������

// ���ݵ� npc ����
	DWORD	m_dwSkill5ID;				// ������ת�ɵ�id  SkillString2Id()
	int		m_nSkill5Level;				// ���ܵȼ�
	int		m_nFireResist;				// ����
	int		m_nFireResistMax;			// �������ֵ
	int		m_nColdResist;				// ������
	int		m_nColdResistMax;			// ���������ֵ
	int		m_nLightingResist;			// �翹��
	int		m_nLightingResistMax;		// �翹�����ֵ
	int		m_nPoisonResist;			// ������
	int		m_nPoisonResistMax;			// ���������ֵ
	int		m_nPhycicsResist;			// ������
	int		m_nPhycicsResistMax;		// ���������ֵ
public:
	void	Init(int nIdx);
	void	SetGoldType(BOOL bFlag);	// �趨�Ƿ�Ϊ�ƽ����
	void	SetGoldTypeAndBackData();	// �趨����Ϊ�ƽ���ͬʱ������Ӧ����
	void	RecoverBackData();			// �ָ������ݣ������ͨnpc
	void	RandChangeGold();			// ��һ�����ʱ�ɻƽ����
	int		GetGoldType();				// ��õ�ǰ�ƽ����ͣ�0 �ǻƽ��
#ifndef _SERVER
	void	SetGoldCurrentType(int nType);
	void	ClientClearState();
#endif
};

struct	KNpcGoldTemplateInfo
{
// ���������仯���ɷ���ָ�
	int		m_nExp;						// ����
	int		m_nLife;					// ����
	int		m_nLifeReplenish;			// ��Ѫ
	int		m_nAttackRating;			// ����
	int		m_nDefense;					// ����
	int		m_nMinDamage;
	int		m_nMaxDamage;

// �ӼӼ����Ķ������ɷ���ָ�
	int		m_nTreasure;				// װ��
	int		m_nWalkSpeed;
	int		m_nRunSpeed;
	int		m_nAttackSpeed;
	int		m_nCastSpeed;

// ��Ҫ���ݵĶ��������ɷ���ָ�
	DWORD	m_dwSkill5ID;				// ������ת�ɵ�id  SkillString2Id()
	char	m_szSkill5Level[32];		// ���ܵȼ�
	int		m_nFireResist;				// ����
	int		m_nFireResistMax;			// �������ֵ
	int		m_nColdResist;				// ������
	int		m_nColdResistMax;			// ���������ֵ
	int		m_nLightingResist;			// �翹��
	int		m_nLightingResistMax;		// �翹�����ֵ
	int		m_nPoisonResist;			// ������
	int		m_nPoisonResistMax;			// ���������ֵ
	int		m_nPhycicsResist;			// ������
	int		m_nPhycicsResistMax;		// ���������ֵ
};

class KNpcGoldTemplate
{
	friend class KNpcSet;
public:
	KNpcGoldTemplateInfo	m_sInfo[defMAX_NPC_GOLD_TYEP];
	int		m_nEffectTypeNum;
public:
	KNpcGoldTemplate();
	BOOL	Init();
};

#endif

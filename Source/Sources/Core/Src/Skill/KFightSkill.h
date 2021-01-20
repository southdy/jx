#pragma once
#include "Skill.h"
#define MAX_MISSLE_DAMAGEATTRIB 8
class KFightSkill:public ISkill
{
public:
	char				m_szName[32];			//	��������
	DWORD				m_nId;					//	����Id
#ifndef _SERVER
	char				m_szSkillIcon[80];
#endif
	BOOL				m_bIsAura;				//	�Ƿ�Ϊ�⻷����
	BOOL				IsAura(){CheckAndGenSkillData(); return m_bIsAura;};
	CLIENTACTION		m_nCharActionId;		//	���������ʱ������ʲô����
	BOOL				m_bTargetOnly;			//	ֻ��ѡ��Ŀ���ʱ����ܷ���
	BOOL				m_bTargetEnemy;			//	���ܵ�Ŀ���Ƿ�Ϊ����
	BOOL				m_bTargetAlly;			//	���ܵ�Ŀ���Ƿ�Ϊͬ��
	BOOL				m_bTargetObj;			//	���ܵ�Ŀ���Ƿ�Ϊ���
	BOOL				m_bTargetSelf;			//	���ܵ�Ŀ���Ƿ����Ϊ����

	NPCATTRIB			m_nSkillCostType;		//	����ʹ��ʱ����Ҫ���ѵ���������������������Ǯ������
	int					m_nCost;				//	���ü�������������������ȵ�����
	int					m_nMinTimePerCast;		//	���ü��ܵ���С���ʱ��
	BOOL				m_bMustBeHit;			//	�Ƿ�Ϊ���м��ܣ������䴦��򵥻��Ա㽵�ͷ���������

	int					m_nAttackRadius;		// ���
	int					m_nWaitTime;			//	�ü����������������������ʱ��
	KMagicAttrib		m_DamageAttribs[MAX_MISSLE_DAMAGEATTRIB];	//Ӧ�õ���״̬��ص�����
	int					m_nDamageAttribsNum;
	BOOL				m_bClientSend;			//  �ü����Ƿ�Է�������˵��Ч
	KMagicAttrib		m_ImmediateAttribs[MAXSKILLLEVELSETTINGNUM];//��״̬������,�����˺������ܷ����ӵ����жԷ���������Ч���˺�,���漼�ܵȼ��仯���ı�.
	int					m_nImmediateAttribsNum;
	
	KMagicAttrib		m_StateAttribs[MAXSKILLLEVELSETTINGNUM];     //Ӧ�õ�״̬��ص�����
	int					m_nStateAttribsNum;		//���������10
	int					m_nChildSkillNum;	//	ͬʱ�����Ӽ��ܵ�����	
	int					m_nSkillTime;			//	�ü��ܵ�����ʱ��
	
	int					m_nChildSkillId;		//	�����������Ӽ���Id;	//���ü���Ϊ��������ʱ����������
	int					m_nChildSkillLevel;
	
	int					m_nMaxTimes;			//ĳЩ������ħ�������ͬʱ���ֶ��ٸ�����
	eSKillStyle			m_eSkillStyle;			//  ��ǰ�ļ�������
		
#ifndef _SERVER
	eSkillLRInfo		m_eLRSkillInfo;			//0 ��ʾ���Ҽ��Կɣ�1��ʾֻ�������Ҽ����ܣ�2��ʾ���Ҽ���������
	char				m_szPreCastEffectFile[100];
#endif
public:
#ifndef _SERVER
	eSkillLRInfo		GetSkillLRInfo(){CheckAndGenSkillData(); return m_eLRSkillInfo;	};
#endif

	BOOL				IsTargetOnly(){CheckAndGenSkillData();return m_bTargetOnly;};
	BOOL				IsTargetEnemy(){CheckAndGenSkillData();return m_bTargetEnemy;};
	BOOL				IsTargetAlly(){CheckAndGenSkillData();return m_bTargetAlly;};
	BOOL				IsTargetObj(){CheckAndGenSkillData();return m_bTargetObj;};

	BOOL				GetInfoFromTabFile(int nCol);
	BOOL				GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow);
	BOOL				Load(int nCol);
	int					GetAttackRadius() {CheckAndGenSkillData();	return m_nAttackRadius;};

	int					GetSkillCost(){CheckAndGenSkillData();return m_nCost;};
	NPCATTRIB			GetSkillCostType(){CheckAndGenSkillData();return m_nSkillCostType;};
	CLIENTACTION		GetActionType(){CheckAndGenSkillData();return m_nCharActionId;};
	int					GetDelayPerCast(){CheckAndGenSkillData();return m_nMinTimePerCast;	};

	
#ifndef _SERVER
	const char *		GetPreCastEffectFile(){	CheckAndGenSkillData(); return m_szPreCastEffectFile;};
#endif
	

#ifdef _SERVER
	KMissleMagicAttribsData* CreateMissleMagicAttribsData(TGameActorInfo GameActor);
	KMissleMagicAttribsData* CreateMissleMagicAttribsData(TSkillParam * pSkillParam);
#endif 


#ifndef _SERVER
public:
	void				DrawSkillIcon(int x, int y, int Width, int Height);
	KRUImage			m_RUIconImage;
	char				m_szSkillDesc[100];//���ܵ�����
	void				GetDesc(char * pszSkillDesc, int nOwnerIndex);
#endif
	virtual BOOL	ParseString2MagicAttrib(MAGIC_ATTRIB MagicAttrib, int nValue1, int nValue2, int nValue3);
	virtual void	SetSkillLevelData(int nLevel /* =0*/);
	virtual BOOL	ParseString2MagicAttrib(char * szMagicAttribName, char * szValue);
	inline const char *		MagicAttrib2String(int MagicAttrib);
	void CheckAndGenSkillData() {};
};
// KSkills.h: interface for the KSkills class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "KMissle.h"
#include "skilldef.h"
#include "KWorldMsg.h"
#include "KNpc.h"
#include "KNode.h"
#include "KMissleMagicAttribsData.h"
#include "KFightSkill.h"


//Skill[0]���ã�ÿ��skill��[0]����!!ע�⣡
#ifdef TOOLVERSION
class CORE_API KMissleSkill  :public KFightSkill
#else
class KMissleSkill	:public KFightSkill
#endif
{
private:
	int (* CastMissleFunc[SKILL_MF_COUNT])(int nSrcPX, int nSrcPY, int nDesPX , int nDesPY, TSkillParam * pSkillParam);
public:
private:
#ifndef _SERVER
	char				m_szSkillIcon[80];
#endif

	eMisslesForm		m_eMisslesForm;			//	����ӵ�����ʼ��ʽ
	//����������
	//BOOL				m_bIsPassivity;			//	�Ƿ�Ϊ��������
	BOOL				m_bIsMelee;				//	�Ƿ�Ϊ��������������ʱ
	BOOL				m_bIsPhysical;			//	�Ƿ�Ϊ������

	BOOL				m_bUseAttackRate;		//	�Ƿ���������
	
	
	BOOL				m_bBaseSkill;			//	�Ƿ�Ϊ���������
	BOOL				m_bByMissle;			//	���ɸ����ܲ���ʱ���Ƿ��Ǹ������Ϊ���㻹���Ե�ǰ���ӵ�Ϊ����

	TGameActorInfo		m_RefGameActor;
		
	
	
	BOOL				m_bFlyingEvent;			//	�Ƿ���Ҫ�ڷ��й�����Ϣ�����ǣ�������ػص�����
	BOOL				m_bStartEvent;			//	�Ƿ���Ҫ�ڼ��ܵ�һ��Activeʱ��������ػص�����
	BOOL				m_bCollideEvent;		//	�Ƿ���Ҫ���Ӽ���ħ����ײʱ��������ػص�����
	BOOL				m_bVanishedEvent;		//	�Ƿ���Ҫ���Ӽ�������ʱ��������صĻص�����
	
	int					m_nFlySkillId;			//	�������е���ؼ���
	int					m_nFlyEventTime;		//	ÿ����֡�ص�FlyEvent;
	int					m_nStartSkillId;		//	���ܸողŷ���ʱ���������¼�ʱ������Ҫ����ؼ���id
	int					m_nVanishedSkillId;		//	���ܷ������ӵ�����ʱ�����ļ���Id;
	int					m_nCollideSkillId;		//	���ܷ������ӵ���ײ����������ļ���Id;
	int					m_nEventSkillLevel;
	
		
	int					m_nValue1;				//	������������1
	int					m_nValue2;				//	������������2
	
	eMisslesGenerateStyle m_eMisslesGenerateStyle;//ͬʱ���ɵĶ���ӵ���DoWait��ʱ��˳��	
	int					m_nMisslesGenerateData;	//�������
	int					m_nMaxShadowNum;		//	������Ĳ�Ӱ��
	BOOL				m_bNeedShadow;

	KList				m_MissleLevelUpSettingList;//��¼�йظõȼ��ӵ�����������Ϣ	

	KMagicAttrib		m_MissleAttribs[MAXSKILLLEVELSETTINGNUM];//�����ӵ���������ݡ���ͬ�ȼ��ļ��ܣ��ӵ���������Կ��Ա仯��
	int					m_nMissleAttribsNum;
	
public:
	KMissleSkill();
	virtual ~KMissleSkill();
	
	
	BOOL ParseString2MagicAttrib(MAGIC_ATTRIB MagicAttrib, int nValue1, int nValue2, int nValue3);
	BOOL				IsNeedShadow(){	CheckAndGenSkillData();return m_bNeedShadow;};
	int					GetMaxTimes(){ CheckAndGenSkillData(); return m_nMaxTimes;};

	BOOL				Cast(TSkillParam * pSkillParam);	//����ʱ����
	
	inline	BOOL		IsNpcExist(int nLauncher);
	
	int					GetChildSkillNum(int nLevel);


	friend	class		KMissle;
	
	unsigned int		GetMissleGenerateTime(int nNo);		
	int					GetChildSkillNum(){CheckAndGenSkillData();return m_nChildSkillNum;};
	int					GetChildSkillId(){CheckAndGenSkillData();return m_nChildSkillId;};
	int					GetChildSkillLevel(){CheckAndGenSkillData();return m_nChildSkillLevel;};
	void				SetSkillId(int nId){m_nId = nId;};		

	BOOL				IsPhysical(){CheckAndGenSkillData();return m_bIsPhysical;};
	void				SetSkillLevel(int nLevel){	m_nLevel = nLevel;	};
	
private:
	void				Vanish(int	nMissleId);
	void				FlyEvent(int nMissleId);
	void				Collidsion(int nMissleId);
	void				TrigerMissleEventSkill(int nMissleId, int nSkillId, int nSkillLevel);

	BOOL				CastMissles(TSkillParam *pSkillParam);
	int					CastWall(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastLine(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastSpread(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastCircle(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastAtTarget(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastAtFirer(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastZone(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	int					CastExtractiveLineMissle(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam);
	void				CreateMissle(TSkillParam * pSkillParam, int ChildSkillId,  int nMissleIndex);
	BOOL				GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow);
	BOOL				ParseString2MagicAttrib(char * szMagicAttribName, char * szValue);
	
#ifndef		_SERVER
	BOOL				PrepareCast(TSkillParam * pSkillParam, int nX, int nY);
#endif
};

#define MAX_MELEEWEAPON_PARTICULARTYPE_NUM 100
#define MAX_RANGEWEAPON_PARTICULARTYPE_NUM 100
extern int		g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
extern int		g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];	
extern int		g_nHandSkill;

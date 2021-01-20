// KSkills.h: interface for the KSkills class.
//
//////////////////////////////////////////////////////////////////////
#ifndef KSkillsH
#define KSkillsH

#include "KMissle.h"
#include "KWorldMsg.h"
#include "KNpc.h"
#include "KNode.h"
#include "KMissleMagicAttribsData.h"
#include "Skill.h"
#define MAX_MISSLE_DAMAGEATTRIB 8
typedef  KSkill KFightSkill;
class KMissle;
class KSkill;
//Skill[0]���ã�ÿ��skill��[0]����!!ע�⣡
enum
{
	Missle_StartEvent = 1,
	Missle_FlyEvent,
	Missle_CollideEvent,
	Missle_VanishEvent,
};

#ifdef TOOLVERSION
class CORE_API KSkill  :public ISkill
#else
class KSkill :public ISkill
#endif
{
private:
	
public:
	unsigned long		m_ulLevel;

#ifdef _SERVER
	DWORD				m_dwSkillLevelUpScriptID;	//��ҵ��������ܵȼ�ʱ�����Ľű�ID
	DWORD				m_dwSkillLevelDataScriptId; //�ü��������ı䶯���ݶ�Ӧ�ű�ID
#endif

#ifndef _SERVER
	char				m_szSkillIcon[80];
	char				m_szPreCastEffectFile[100];
	char				m_szManPreCastSoundFile[100];
	char				m_szFMPreCastSoundFile[100];
	eSkillLRInfo		m_eLRSkillInfo;			//0 ��ʾ���Ҽ��Կɣ�1��ʾֻ�������Ҽ����ܣ�2��ʾ���Ҽ���������
#endif
	unsigned short		m_usReqLevel;//		������Ҫ����С�ȼ�
private:
	int					m_nCharClass;			//	���ĸ�ϵ��
	eSKillStyle			m_eSkillStyle;			//  ��ǰ�ļ�������
	int					m_nSkillTime;			//	�ü��ܵ�����ʱ��
	BOOL				m_bClientSend;			//  �ü����Ƿ�Է�������˵��Ч
	BOOL				m_bHaveLoad;			//	��ʶ�ü����Ƿ��Ѿ����أ����û���������
	int					m_nInteruptTypeWhenMove;//�ӵ��ļ����Ƿ��ܷ����ߵ��ƶ�����ֹ
	BOOL				m_bHeelAtParent;	//	���ӵ�ʵ�ʼ���ʱ��λ�ø��ݸ���ǰλ�ö�ȷ��,�������ɲ����ǿ�parentλ�þ���

	//����������
	CLIENTACTION		m_nCharActionId;		//	���������ʱ������ʲô����
	int					m_nWaitTime;			//	�ü����������������������ʱ��
	BOOL				m_bIsAura;				//	�Ƿ�Ϊ�⻷����
	BOOL				m_bIsPassivity;			//	�Ƿ�Ϊ��������
	BOOL				m_bIsMelee;				//	�Ƿ�Ϊ��������������ʱ
	BOOL				m_bUseAttackRate;		//	�Ƿ���������
	BOOL				m_bTargetOnly;			//	ֻ��ѡ��Ŀ���ʱ����ܷ���
	BOOL				m_bTargetEnemy;			//	���ܵ�Ŀ���Ƿ�Ϊ����
	BOOL				m_bTargetAlly;			//	���ܵ�Ŀ���Ƿ�Ϊͬ��
	BOOL				m_bTargetObj;			//	���ܵ�Ŀ���Ƿ�Ϊ���
	BOOL				m_bTargetSelf;			//	���ܵ�Ŀ���Ƿ����Ϊ����
	int					m_eRelation;
	char				m_szName[32];			//	��������
	DWORD				m_nId;					//	����Id
	BOOL				m_bBaseSkill;			//	�Ƿ�Ϊ���������
	BOOL				m_bByMissle;			//	���ɸ����ܲ���ʱ���Ƿ��Ǹ������Ϊ���㻹���Ե�ǰ���ӵ�Ϊ����
	BOOL				m_bIsPhysical;			//	�Ƿ�Ϊ������
	int					m_nCost;				//	����ʹ��ʱ����Ҫ���ѵ���������������������Ǯ������
	NPCATTRIB			m_nSkillCostType;		//	���ü�������������������ȵ�����
	int					m_nMinTimePerCast;		//	���ü��ܵ���С���ʱ��
	int					m_nChildSkillNum;		//	ͬʱ�����Ӽ��ܵ�����	
	eMisslesForm		m_eMisslesForm;			//	����ӵ�����ʼ��ʽ
	int					m_nValue1;				//	������������1
	int					m_nValue2;				//	������������2
	int					m_nEventSkillLevel;
	KMagicAttrib		m_MissleAttribs[MAXSKILLLEVELSETTINGNUM];//�����ӵ���������ݡ���ͬ�ȼ��ļ��ܣ��ӵ���������Կ��Ա仯��
	int					m_nMissleAttribsNum;
	KMagicAttrib		m_DamageAttribs[MAX_MISSLE_DAMAGEATTRIB];	//Ӧ�õ���״̬��ص�����
	int					m_nDamageAttribsNum;
	KMagicAttrib		m_ImmediateAttribs[MAXSKILLLEVELSETTINGNUM];//��״̬������,�����˺������ܷ����ӵ����жԷ���������Ч���˺�,���漼�ܵȼ��仯���ı�.
	int					m_nImmediateAttribsNum;
	KMagicAttrib		m_StateAttribs[MAXSKILLLEVELSETTINGNUM];     //Ӧ�õ�״̬��ص�����
	int					m_nStateAttribsNum;		//���������10
	eMissleFollowKind   m_eMissleFollowKind;	
	int					m_nFollowKindIndex;		
	DWORD				m_dwFollowKindID;
	int					m_nChildSkillId;		//	�����������Ӽ���Id;	//���ü���Ϊ��������ʱ����������
	int					m_nChildSkillLevel;
	BOOL				m_bFlyingEvent;			//	�Ƿ���Ҫ�ڷ��й�����Ϣ�����ǣ�������ػص�����
	BOOL				m_bStartEvent;			//	�Ƿ���Ҫ�ڼ��ܵ�һ��Activeʱ��������ػص�����
	BOOL				m_bCollideEvent;		//	�Ƿ���Ҫ���Ӽ���ħ����ײʱ��������ػص�����
	BOOL				m_bVanishedEvent;		//	�Ƿ���Ҫ���Ӽ�������ʱ��������صĻص�����
	int					m_nFlySkillId;			//	�� �����е���ؼ���
	int					m_nFlyEventTime;		//	ÿ����֡�ص�FlyEvent;
	int					m_nStartSkillId;		//	���ܸողŷ���ʱ���������¼�ʱ������Ҫ����ؼ���id
	int					m_nVanishedSkillId;		//	���ܷ������ӵ�����ʱ�����ļ���Id;
	int					m_nCollideSkillId;		//	���ܷ������ӵ���ײ����������ļ���Id;
	BOOL				m_bMustBeHit;			//	�Ƿ�Ϊ���м��ܣ������䴦��򵥻��Ա㽵�ͷ���������
	eMisslesGenerateStyle m_eMisslesGenerateStyle;//ͬʱ���ɵĶ���ӵ���DoWait��ʱ��˳��	
	int					m_nMisslesGenerateData;	//�������
	int					m_nMaxShadowNum;		//	������Ĳ�Ӱ��
	BOOL				m_bNeedShadow;
	int					m_nAttackRadius;		// ���
	int					m_nStateSpecialId;		//״̬��ħ���⻷id��
	int					m_nMaxTimes;			//ĳЩ������ħ�������ͬʱ���ֶ��ٸ�����
	
	int					m_nEquiptLimited;		//������ʱ�Ե�ǰװ��������
	
	int					m_nHorseLimited;		//�������� 0��ʾû�κ�����
												//		   1��ʾ��������
												//		   2��ʾ��������
	BOOL				m_bDoHurt;

public:
	KSkill();
	virtual ~KSkill();
	//���������g_OrdinSkillsSetting���к�
	//BOOL				
	BOOL				OnMissleEvent(unsigned short usEvent, KMissle * pMissle) const;
	BOOL				IsTargetOnly()const{return m_bTargetOnly;};
	BOOL				IsTargetEnemy()const{return m_bTargetEnemy;};
	BOOL				IsTargetAlly()const{return m_bTargetAlly;};
	BOOL				IsTargetObj()const{return m_bTargetObj;};
	BOOL				IsNeedShadow()const{	return m_bNeedShadow;};
	int					GetMaxTimes()const{  return m_nMaxTimes;};
	static	int			GetSkillIdFromName(char * szSkillName);
	unsigned short		GetSkillReqLevel()const{	return m_usReqLevel;};

#ifdef _SERVER
	unsigned long		GetSkillLevelUpScriptId()const{return m_dwSkillLevelUpScriptID;};
	KMissleMagicAttribsData* CreateMissleMagicAttribsData(int nLauncher)const;
#endif

#ifndef _SERVER
	eSkillLRInfo		GetSkillLRInfo() const{ return m_eLRSkillInfo;	};
	const char *		GetPreCastEffectFile() const{	 return m_szPreCastEffectFile;};
	const char *		GetPreCastSoundFile(BOOL bIsFeMale) const{ return bIsFeMale? m_szFMPreCastSoundFile: m_szManPreCastSoundFile;};
#endif

	const char * GetSkillName(){return m_szName;};
	int					GetSkillStyle(){ return m_eSkillStyle;};
	BOOL				GetInfoFromTabFile(int nCol);
	BOOL				GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow);
	BOOL				Load(int nCol);
	BOOL				Cast(int nLauncher,   int nParam1, int nParam2, int nWaitTime = 0, eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const;	//����ʱ����
	int					GetChildSkillNum(int nLevel)const;
	int					GetAttackRadius() const{	return m_nAttackRadius;};
	int					CalculateDamage()const;
	void				LoadSkillLevelData(unsigned long  ulLevel , int nParam); //�Ӽ��������ļ��л��nLevel�ȼ������Ա䶯��������
	friend	class		KMissle;
	int					GetSkillCost(void *)const{return m_nCost;};
	NPCATTRIB			GetSkillCostType()const{return m_nSkillCostType;};
	BOOL				GetItemLimit(int)const;
	CLIENTACTION		GetActionType()const{return m_nCharActionId;};
	int					GetDelayPerCast()const{return m_nMinTimePerCast;	};
	unsigned int		GetMissleGenerateTime(int nNo) const ;		
	int					GetChildSkillNum() const {return m_nChildSkillNum;};
	int					GetChildSkillId() const {return m_nChildSkillId;};
	int					GetChildSkillLevel() const {return m_nChildSkillLevel;};
	
	int					GetSkillId() {return m_nId;};
	void				SetSkillId(int nId) {m_nId = nId;};		
	void				SetSkillLevel(unsigned long ulLevel){m_ulLevel = ulLevel;};

	BOOL				IsAura() const { return m_bIsAura;};
	BOOL				IsPhysical() const {return m_bIsPhysical;};
	int					GetMeleeType() const { return m_eMisslesForm;};
	int					GetStateSpecailId() const {return m_nStateSpecialId;		};
	BOOL				CanCastSkill  (int nLauncher, int &nParam1, int &nParam2)  const ;
	void				PlayPreCastSound  (BOOL bIsFeMale , int nX, int nY) const ;
	KMagicAttrib*		GetDamageAttribs()  { return m_DamageAttribs;};	
	
private:
	void				Vanish(KMissle *) const;
	void				FlyEvent(KMissle *)  const ;
	void				Collidsion(KMissle *) const ;
	BOOL				CastMissles(int nLauncher,	int nParam1, int nParam2, int nWaitTime = 0 , eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const ;
	BOOL				CastInitiativeSkill(int nLauncher, int nParam1, int nParam2, int nWaitTime) const ;
	BOOL				CastPassivitySkill(int nLauncher, int nParam1, int nParam2, int nWaitTime) const ;
	
	int					CastWall					(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastLine					(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastSpread					(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastCircle					(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastZone					(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY) const ;
	int					CastExtractiveLineMissle	(TOrdinSkillParam * pSkillParam, int nDir, int nSrcX,  int nSrcY, int nXOffset, int nYOffset, int nDesX, int nDesY) const ;
	int					Param2PCoordinate			(int nLauncher, int nParam1, int nParam2 , int *npPX, int *npPY, eSkillLauncherType eLauncherType = SKILL_SLT_Npc) const ;

	
	void				CreateMissle(int nLauncher, int ChildSkillId,  int nMissleIndex) const ;
	
	BOOL				ParseString2MagicAttrib(unsigned long ulLevel, char * szMagicAttribName, char * szValue) ;
	inline const char *		MagicAttrib2String(int MagicAttrib) const ;

#ifndef _SERVER
public:
	void				DrawSkillIcon(int x, int y, int Width, int Height);
	KRUImage			m_RUIconImage;
	char				m_szSkillDesc[300];//���ܵ�����
	static void			GetDesc(unsigned long ulSkillId, unsigned long ulCurLevel, char * pszMsg, int nOwnerIndex,  bool bGetNextLevelDesc);
	void				GetDescAboutLevel(char * pszMsg);
#endif

};

#define MAX_MELEEWEAPON_PARTICULARTYPE_NUM 100
#define MAX_RANGEWEAPON_PARTICULARTYPE_NUM 100
extern int		g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
extern int		g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];	
extern int		g_nHandSkill;

#endif

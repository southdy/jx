// KMissle.h: interface for the KMissle class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	KMissleH
#define KMissleH

#ifdef  _SERVER
#define MAX_MISSLE  5000
#else
#define MAX_MISSLE 500
#endif
#include "kcore.h"
#include "skilldef.h"
#include "KObj.h"
#include "KMissleRes.h"
#include "KNode.h"
#include "KITabFile.h"
#include "KNpcSet.h"
#include "GameDataDef.h"
//#include "Skill.h"

#ifdef _SERVER
#include "KMissleMagicAttribsData.h"
#endif
enum MISSLE_RELATION
{
		MissleRelation_None = 0,
		MissleRelation_EnemyOnly  ,
		MissleRelation_AllyOnly,
		MissleRelation_All,
};

#ifndef TOOLVERSION
class KMissle
#else
class CORE_API KMissle  
#endif
{
	friend class	KSkill;
private:
public:
	//	�ӵ��趨�ļ���õ�����
	char				m_szMissleName[30];		//	�ӵ�������
	int					m_nAction;				//	��ǰ��Ϊ
	BOOL				m_bIsSlow;				//	�Ƿ񱻼���
	BOOL				m_bClientSend;			//	�Ƿ���Ҫ
	BOOL				m_bRemoving;			//	��֪��Ҫ��һ��ѭ��ɾ�����ӵ�������Դ
	BOOL				m_bIsMelee;				//	�ӵ��Ƿ��ǽ�����
	TSkillParam			m_SkillParam;
	unsigned long		m_dwID;
#ifndef _SERVER
	KMissleRes			m_MissleRes;			//	�ӵ�����Դ
	BOOL				CreateSpecialEffect(eMissleStatus eStatus,  int nPX, int nPY, int nPZ, int nNpcIndex = 0);
#endif
	eMissleMoveKind		m_eMoveKind;			//	�ӵ��˶�����(��ը��ֱ�߷��еȡ���)
	eMissleFollowKind	m_eFollowKind;			//	�ӵ�����ʱ�Ĳ�������
	int					m_nHeight;				//	�ӵ��߶�
	int					m_nHeightSpeed;			//	�ӵ����еķ����ٶ�
	int					m_nLifeTime;			//	��������
	int					m_nSpeed;				//	�����ٶ�
	int					m_nSkillId;				//	��Ӧ�ĸ�����
	BOOL				m_bRangeDamage;			//	�Ƿ�Ϊ�����˺������Ƿ�����ܵ��˺�
	MISSLE_RELATION		m_eRelation;			//	Ŀ���뷢���ߵĹ�ϵ
	bool				m_bAutoExplode;			//  Is Missle Would AutoExpode ItSelf For Collision When It's LiftTime Is Over;
	
	
	//BOOL				m_bByMissle;

	BOOL				m_bBaseSkill;			//	�Ƿ�Ϊ���������
	BOOL				m_bByMissle;			//	���ɸ����ܲ���ʱ���Ƿ��Ǹ������Ϊ���㻹���Ե�ǰ���ӵ�Ϊ����
	
	
	int					m_nCollideRange;		//	��ײ��Χ���򻯶������ײ�ã�
	int					m_nDamageRange;			//	�˺���Χ
	BOOL				m_bCollideVanish;		//	��ײ���Ƿ�����
	BOOL				m_bCollideFriend;		//	�Ƿ����ײ��ͬ��
	BOOL				m_bCanSlow;				//	�Ƿ�ᱻ���٣�����˵Slow Missle��ļ��ܣ�
	int					m_nKnockBack;			//	���˾���
	int					m_nStunTime;			//	��ѣʱ��
	
	BOOL				m_bFlyEvent;			//	�������й����е�
	int					m_nFlyEventTime;
	BOOL				m_bSubEvent;			//	�Ƿ���Ҫ�ڷ��й�����Ϣ�����ǣ�������ػص�����
	BOOL				m_bStartEvent;			//	�Ƿ���Ҫ�ڼ��ܵ�һ��Activeʱ��������ػص�����
	BOOL				m_bCollideEvent;		//	�Ƿ���Ҫ���Ӽ���ħ����ײʱ��������ػص�����
	BOOL				m_bVanishedEvent;		//	�Ƿ���Ҫ���Ӽ�������ʱ��������صĻص�����
	
	
#ifndef _SERVER
	BOOL				m_bMultiShow;			//	�ӵ���������ʾ
	bool				m_bFollowNpcWhenCollid; //	��ըЧ�����汻���е�����
#endif
	
	BOOL				m_bMustBeHit;			//  ���У����ӵ����������һ���ܴ��жԷ��������Ƿ�ʵ����ײ���Է�.
	
	//	���ܻ�õ�����
	int					m_nCurrentLife;			//	��ǰ����ʱ��
	int					m_nStartLifeTime;		//	�����ܷ����󣬵ڼ�֡��ʼ
	int					m_nCollideOrVanishTime; 
	int					m_nCurrentMapX;			//	��ǰ��X����
	int					m_nCurrentMapY;			//	��ǰ��Y����
	int					m_nCurrentMapZ;			//	��ǰ��Z����
	int					m_nXOffset;				//	��ǰ��X����ƫ��
	int					m_nYOffset;				//	��ǰ��Y����ƫ��
	int					m_nSrcMapX;				//	Դ����
	int					m_nSrcMapY;				
	int					m_nSrcXOffset;		
	int					m_nSrcYOffset;
#ifndef _SERVER	
	unsigned int		m_SceneID;
#endif
	//��һ�����ӵ�ʱ����ȷ���У�
	int					m_nXFactor;
	int					m_nYFactor;
	int					m_nLevel;				//	���ܵȼ�
	//int					m_nFollowNpcIdx;		//	����˭
	//int					m_nLauncher;			//	��������NpcSet�е�Index
	//DWORD				m_dwLauncherId;			//	�����ߵ�ΨһID
	//eSkillLauncherType	m_eSkillLauncherType;
	//	�����ɵĶ�̬����	
	int					m_nCurrentSpeed;		//	��ǰ�ٶȣ����ܱ����٣�
	int					m_nZAcceleration;		//	Z��ļ��ٶ�
	eMissleStatus		m_eMissleStatus;		//	�ӵ���ǰ��״̬
	int					m_nMissleId;			//	�ӵ���Missle�е�ID
	int					m_nSubWorldId;			//	������ID
	int					m_nRegionId;			//	����ID
	//�����˺�����
	int					m_nMaxDamage;			//	����˺�
	int					m_nElementType;			//	Ԫ���˺�����
	int					m_nMaxElementDamage;	//	���Ԫ���˺�(�״�)
	int					m_nElementTime;			//	Ԫ�س���ʱ��
	int					m_nElementInterval;		//	Ԫ�ؼ��ʱ��
	int					m_nElementPerDamage;	//	Ԫ���˺�ʱ��ÿ�η������ܵ��˺�ֵ
	int					m_nParam1;				//	����һ
	int					m_nParam2;				//	������
	int					m_nParam3;				//	������
	int					m_nDirIndex;			//	��ǰ�˶����������
	int					m_nDir;					//	��ǰ�����з���
	int					m_nAngle;				//	
	DWORD				m_dwBornTime;			//	���ӵ�����ʱ��ʱ��
public:
	KMissle();
	virtual ~KMissle();
#ifndef		_SERVER
	void				Paint();
#endif
private:
	BOOL				Init( int nLauncher, int nMissleId, int nXFactor, int nYFactor, int nLevel);
	void				OnVanish();//������ʧ
	void				OnCollision();//��ײ
	void				OnFly();//���й�����
	void				OnWait();
	void				DoWait();
	void				DoFly();
	void				PrePareFly();
	void				DoVanish();
	void				DoCollision();
	friend				class KMissleSet;
	void				Release();
	BOOL				CheckCollision(int nBarrierHeight);//����Ƿ���ײ
	BOOL				CheckBeyondRegion();//����Ƿ�Խ��
	int					GetDir(int dx,int dy);
public:
	KMissle&			operator=(KMissle& Missle);
	DWORD				GetCurrentSubWorldTime();
	BOOL				ProcessDamage(int nNpcId);
	int					ProcessCollision();//������ײ
	BOOL				IsRelationSuitable(NPC_RELATION npc_relation, MISSLE_RELATION msl_relation)
	{
		if (msl_relation == MissleRelation_All)
			return TRUE;
		else if (msl_relation == MissleRelation_AllyOnly && npc_relation == relation_ally)
			return TRUE;
		else if (msl_relation == MissleRelation_EnemyOnly && npc_relation == relation_enemy) 
			return TRUE;
		else
			return FALSE;
		
		return FALSE;
	};
	
public:	
	BOOL				IsMatch(DWORD dwID){return m_dwID == dwID;	};
	BOOL				GetInfoFromTabFile(int nMissleId);
	BOOL				GetInfoFromTabFile(KITabFile * pTabFile, int nMissleId);
	//TEffectData			*m_pEffectData;
#ifdef _SERVER
	KMissleMagicAttribsData * m_pMagicAttribsData;
	int					SetMagicAttribsData(KMissleMagicAttribsData * pData)
	{
		
		m_pMagicAttribsData = pData;
		if (pData)
			return pData->AddRef();
		else 
			return 0;
	};
#endif //_SERVER
	BOOL				Save();
	int					Activate();
	int					ProcessCollision(int nLauncherIdx, int nRegionId, int nMapX, int nMapY, int nRange , MISSLE_RELATION eRelation);
	void				GetMpsPos(int *pPosX, int *pPosY)	;

};
extern CORE_API KMissle			Missle[MAX_MISSLE];
extern CORE_API KMissle			g_MisslesLib[MAX_MISSLESTYLE];//Base 1

#endif

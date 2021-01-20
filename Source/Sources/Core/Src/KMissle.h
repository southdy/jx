//////////////////////////////////////////////////////////////////////
// KMissle.h: interface for the KMissle class.
//////////////////////////////////////////////////////////////////////
#ifndef	KMissleH
#define KMissleH

#ifdef  _SERVER
#define MAX_MISSLE  20000
#else
#define MAX_MISSLE 500
#endif
#include "KCore.h"
#include "SkillDef.h"
#include "KObj.h"
#ifndef _SERVER
#include "KMissleRes.h"
#endif

#include "KNode.h"
#include "KITabFile.h"
#include "KSkills.h"
#include "KNpcSet.h"
#include "Scene/ObstacleDef.h"
#include "KSubWorld.h"
#include "KIndexNode.h"
#include "GameDataDef.h"
#ifdef _SERVER
#include "KMissleMagicAttribsData.h"
#endif
enum 
{
	Interupt_None,
		Interupt_EndNewMissleWhenMove,
		Interupt_EndOldMissleLifeWhenMove,
};

#ifndef _SERVER
struct TMissleForShow
{
	int nPX ;
	int nPY ;
	int nPZ ;
	int nNpcIndex;
	int nLauncherIndex;
};
#endif

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
	KIndexNode			m_Node;
	char				m_szMissleName[30];		//	�ӵ�������
	int					m_nAction;				//	��ǰ��Ϊ
	BOOL				m_bIsSlow;				//	�Ƿ񱻼���
	BOOL				m_bClientSend;			//	�Ƿ���Ҫ
	BOOL				m_bRemoving;			//	��֪��Ҫ��һ��ѭ��ɾ�����ӵ�������Դ
	BOOL				m_bIsMelee;				//	�ӵ��Ƿ��ǽ�����
	eMissleMoveKind		m_eMoveKind;			//	�ӵ��˶�����(��ը��ֱ�߷��еȡ���)
	eMissleFollowKind	m_eFollowKind;			//	�ӵ�����ʱ�Ĳ�������
	int					m_nHeight;				//	�ӵ��߶�
	int					m_nHeightSpeed;			//	�ӵ����еķ����ٶ�
	int					m_nLifeTime;			//	��������
	int					m_nSpeed;				//	�����ٶ�
	int					m_nSkillId;				//	��Ӧ�ĸ�����
	BOOL				m_bRangeDamage;			//	�Ƿ�Ϊ�����˺������Ƿ�����ܵ��˺�
	int					m_eRelation;			//	Ŀ���뷢���ߵĹ�ϵ
	BOOL				m_bAutoExplode;			//  Is Missle Would AutoExpode ItSelf For Collision When It's LiftTime Is Over;
	BOOL				m_bTargetSelf;
	BOOL				m_bBaseSkill;			//	�Ƿ�Ϊ���������
	BOOL				m_bByMissle;			//	���ɸ����ܲ���ʱ���Ƿ��Ǹ������Ϊ���㻹���Ե�ǰ���ӵ�Ϊ����
	INT					m_nInteruptTypeWhenMove;		//�ӵ��ļ����Ƿ��ܷ����ߵ��ƶ�����ֹ
	BOOL				m_bHeelAtParent;		//	���ӵ�ʵ�ʼ���ʱ��λ�ø��ݸ���ǰλ�ö�ȷ��,�������ɲ����ǿ�parentλ�þ���
	int					m_nLauncherSrcPX;		
	int					m_nLauncherSrcPY;
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
	
	unsigned long		m_ulDamageInterval;		//	�˺�����ļ��ʱ��,��Ҫָ�����ͻ�ǽ����
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
	int					m_nRefPX;
	int					m_nRefPY;
	
	int					m_nDesMapX;				//�����ӵ�ʱ��Ŀ������
	int					m_nDesMapY;				
	int					m_nDesRegion;
	BOOL				m_bNeedReclaim;			//�Ƿ��Ѿ�����һ���ӵ��ķ�������
	TMisslePos			m_NeedReclaimPos[4];

	BOOL				m_bDoHurt;				//����ʱ�Ƿ���Ҫ�����˶���
	
	//��һ�����ӵ�ʱ����ȷ���У�
	int					m_nXFactor;
	int					m_nYFactor;
	int					m_nLevel;				//	���ܵȼ�
	
	int					m_nFollowNpcIdx;		//	����˭
	DWORD				m_dwFollowNpcID;		//	
	
	int					m_nLauncher;			//	��������NpcSet�е�Index
	DWORD				m_dwLauncherId;			//	�����ߵ�ΨһID
	int					m_nParentMissleIndex;   // if 0 then means parent is npclauncher
	
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
	
	int					m_nFirstReclaimTime;
	int					m_nEndReclaimTime;

	int					m_nTempParam1;			//	������ʹ�õĲ���
	int					m_nTempParam2;
	
	int					m_nDirIndex;			//	��ǰ�˶����������
	int					m_nDir;					//	��ǰ�����з���
	int					m_nAngle;				//	
	DWORD				m_dwBornTime;			//	���ӵ�����ʱ��ʱ��
	BOOL				m_bUseAttackRating;		
	
#ifdef _SERVER
	unsigned long		m_ulNextCalDamageTime;	//	��һ�μ����˺�����Ϸ����ʱ��
#endif

#ifndef _SERVER
	BOOL				m_bMultiShow;			//	�ӵ���������ʾ
	BOOL				m_bFollowNpcWhenCollid; //	��ըЧ�����汻���е�����
	int					m_btRedLum;
	int					m_btGreenLum;
	int					m_btBlueLum;
	unsigned short		m_usLightRadius;
	KMissleRes			m_MissleRes;			//	�ӵ�����Դ
	unsigned int		m_SceneID;
#endif
	
	
private:
	BOOL				Init( int nLauncher, int nMissleId, int nXFactor, int nYFactor, int nLevel);
	void				OnVanish();//������ʧ
	void				OnCollision();//��ײ
	void				OnFly();//���й�����
	void				OnWait();
	void				DoWait();
	void				DoFly();
	BOOL				PrePareFly();
	void				DoVanish();
	void				DoCollision();
	int					CheckNearestCollision();
	friend				class KMissleSet;
	void				Release();
	int					CheckCollision();//����Ƿ���ײ// 1��ʾ������ײ�����壬0��ʾδ��ײ���κ�����, -1��ʾ���
	BOOL				CheckBeyondRegion(int nDOffsetX, int nDOffsetY);//����Ƿ�Խ��   //FALSE��ʾԽ��һ����Ч��λ�ã�TRUE��ʾOK
	int					GetDir(int dx,int dy);
	KMissle&			operator=(KMissle& Missle);
	DWORD				GetCurrentSubWorldTime();
	BOOL				ProcessDamage(int nNpcId);
	int					ProcessCollision();//������ײ
	int					ProcessCollision(int nLauncherIdx, int nRegionId, int nMapX, int nMapY, int nRange , int eRelation);

	inline void	ZAxisMove()
	{
		if (m_nZAcceleration)
		{
			m_nHeight += m_nHeightSpeed;
			if (m_nHeight < 0) m_nHeight = 0;
			m_nHeightSpeed -= m_nZAcceleration;
			m_nCurrentMapZ = m_nHeight >> 10;
		}
	}
	//TRUE��ʾ�����ϰ���FALSE��ʾδ������һ������
	inline BOOL TestBarrier()
	{
		int nBarrierKind = SubWorld[m_nSubWorldId].TestBarrier(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, 0, 0);
		if (nBarrierKind == Obstacle_Normal || nBarrierKind == Obstacle_Jump)
		{
			
			return TRUE;
		}
		return FALSE;
	}
	
public:	
	KMissle();
	virtual ~KMissle();
	BOOL				GetInfoFromTabFile(int nMissleId);
	BOOL				GetInfoFromTabFile(KITabFile * pTabFile, int nMissleId);
	void				GetMpsPos(int *pPosX, int *pPosY);
	int					Activate();
static	BOOL			GetOffsetAxis(int nSubWorld, int nSrcRegionId, int nSrcMapX, int nSrcMapY,
							int nOffsetMapX, int nOffsetMapY, 
							int &nDesRegionId, int &nDesMapX, int &nDesMapY);
	
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
#endif

#ifndef _SERVER
	void				Paint();
	void				GetLightInfo(KLightInfo * pLightInfo);
	BOOL				CreateSpecialEffect(eMissleStatus eStatus,  int nPX, int nPY, int nPZ, int nNpcIndex = 0);
	static BOOL				CreateMissleForShow(char * szMovie, char * szSprInfo, char * szSound, TMissleForShow * pShowParam);
#endif //_SERVER
	
};


extern CORE_API KMissle			Missle[MAX_MISSLE];
extern CORE_API KMissle			g_MisslesLib[MAX_MISSLESTYLE];//Base 1
#endif

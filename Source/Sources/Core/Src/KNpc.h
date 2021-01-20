#ifndef KNpcH
#define KNpcH
//---------------------------------------------------------------------------
class ISkill;
#include "KCore.h"
#include "KSkillList.h"
#include "KMagicAttrib.h"
#include "GameDataDef.h"
#include "KNpcFindPath.h"
#include "KNpcDeathCalcExp.h"
#include "KIndexNode.h"
#include "KNpcGold.h"

class KSkill;
#ifndef _SERVER
#include "KNpcRes.h"
#endif
//---------------------------------------------------------------------------
#ifdef	_SERVER
#define	MAX_NPC			16384
#else
#define	MAX_NPC			256
#endif
#define MAX_NPCSTYLE	2000

#define		MAX_AI_PARAM					11
#define		MAX_NPC_USE_SKILL				4

//	�ܶ��ٶ��������ٶȵı���
//#define	WALK_RUN_TIMES	3

#define		STATE_FREEZE	0x01
#define		STATE_POISON	0x02
#define		STATE_CONFUSE	0x04
#define		STATE_STUN		0x08

enum NPCATTRIB
{
	attrib_mana,
	attrib_stamina,
	attrib_life,
	attrib_maxmana,
	attrib_maxstamina,
	attrib_maxlife,
};

enum NPCCMD
{
	do_none,		// ʲôҲ����
	do_stand,		// վ��
	do_walk,		// ����
	do_run,			// �ܶ�
	do_jump,		// ��Ծ
	do_skill,		// �����ܵ�����
	do_magic,		// ʩ��
	do_attack,		// ����
	do_sit,			// ����
	do_hurt,		// ����
	do_death,		// ����
	do_defense,		// ��
	do_idle,		// ����
	do_specialskill,// ���ܿ��ƶ���
	do_special1,	// ����1
	do_special2,	// ����2
	do_special3,	// ����3
	do_special4,	// ����4
	do_runattack,
	do_manyattack,
	do_jumpattack,
	do_revive,
};

enum CLIENTACTION
{
	cdo_fightstand,
	cdo_stand,
	cdo_stand1,
	cdo_fightwalk,
	cdo_walk,
	cdo_fightrun,
	cdo_run,
	cdo_hurt,
	cdo_death,
	cdo_attack,
	cdo_attack1,
	cdo_magic,
	cdo_sit,
	cdo_jump,
	cdo_none,   
	cdo_count,
};

enum DAMAGE_TYPE
{
	damage_physics = 0,		// �����˺�
	damage_fire,			// �����˺�
	damage_cold,			// �����˺�
	damage_light,			// �����˺�
	damage_poison,			// �����˺�
	damage_magic,			// �������˺�
	damage_num,				// �˺�������Ŀ
};

// DoDeath ʱ�Ĳ�������Ӧ��ͬ�������ͷ�
enum	enumDEATH_MODE
{
	enumDEATH_MODE_NPC_KILL = 0,		// ��npcɱ��
	enumDEATH_MODE_PLAYER_NO_PUNISH,	// �д�ģʽ�����ɱ��
	enumDEATH_MODE_PLAYER_PUNISH,		// �����PK����������PKֵ���гͷ�
	enumDEATH_MODE_PKBATTLE_PUNISH,		// ��ʽ�ڹ�սʱ�ĳͷ�����
	enumDEATH_MODE_NUM,
};

typedef struct
{
	NPCCMD		CmdKind;		// ����C
	int			Param_X;		// ����X
	int			Param_Y;		// ����Y
	int			Param_Z;		// ����Y
} NPC_COMMAND;

typedef struct
{
	int		nTotalFrame;
	int		nCurrentFrame;
} DOING_FRAME;

struct KState
{
	int	nMagicAttrib;
	int	nValue[2];
	int	nTime;
};

struct	KSyncPos
{
	DWORD	m_dwRegionID;
	int		m_nMapX;
	int		m_nMapY;
	int		m_nOffX;
	int		m_nOffY;
	int		m_nDoing;
};

class KStateNode : public KNode
{
public:

	int				m_SkillID;					// ����ID
	int				m_Level;					// ���ܵȼ�
	int				m_LeftTime;					// ʣ��ʱ��
	KMagicAttrib	m_State[MAX_SKILL_STATE];	// �޸������б�
	int				m_StateGraphics;			// ״̬��������
};

#ifndef _SERVER
/*--- ���ڱ����ͻ���npc���ĸ�region�ĵڼ���npc ��
�������һ�����������Ƶ�npc ��ID ֵΪ 0 ��No ֵΪ -1  ---*/
struct	KClientNpcID
{
	DWORD	m_dwRegionID;
	int		m_nNo;
};
#endif

#ifdef _SERVER
struct KItemDropRate
{
	struct	KItemParam
	{
		int		nGenre;
		int		nDetailType;
		int		nParticulType;
		int		nRate;
	};
	int			nCount;
	int			nMagicRate;
	int			nMaxRandRate;
	int			nMoneyRate;
	int			nMoneyScale;
	int			nMinItemLevelScale;
	int			nMaxItemLevelScale;
	int			nMinItemLevel;
	int			nMaxItemLevel;
	KItemParam*	pItemParam;
};
#endif

class KNpc
{
	friend class KNpcSet;
public:
	DWORD				m_dwID;					// Npc��ID
	int					m_Index;				// Npc������
	KIndexNode			m_Node;					// Npc's Node
	int					m_Level;				// Npc�ĵȼ�
	DWORD				m_Kind;					// Npc������
	int					m_Series;				// Npc��ϵ
	int					m_Height;				// Npc�ĸ߶�(��Ծ��ʱ�����)
	BYTE				m_btRankId;
	int					m_nStature;				//Tall 

	BYTE				m_btStateInfo[MAX_NPC_RECORDER_STATE];	// Npc��ǰ���µļ���״̬ 
	int					m_nNextStatePos;		// ��һ��״̬��m_btState��λ��	

//	DWORD				m_NpcEnchant;			// Npc�ļ�ǿ���ͣ��ƽ���
	KNpcGold			m_cGold;

	//����Npcģ����У���FALSE��ʾ��Npc���ݵ�ǰ����Ч�� ����ֵδ�����ű����㣬��Ҫ����.
	//TRUE��ʾ��Ч����
	BOOL				m_bHaveLoadedFromTemplate;// 
	
	KState				m_PowerState;			// ŭ��״̬
	KState				m_PoisonState;			// �ж�״̬
	KState				m_FreezeState;			// ����״̬
	KState				m_BurnState;			// ȼ��״̬
	KState				m_ConfuseState;			// ����״̬
	KState				m_StunState;			// ѣ��״̬
	KState				m_LifeState;			// ��Ѫ״̬
	KState				m_ManaState;			// ��MANA״̬
	KState				m_MenuState;			// �˵�
	KState				m_DrunkState;			// ���״̬

	KState				m_PhysicsArmor;
	KState				m_ColdArmor;
	KState				m_LightArmor;
	KState				m_PoisonArmor;
	KState				m_FireArmor;
	KState				m_ManaShield;

	KList				m_StateSkillList;		// ����״̬�����б�ͬ���ܲ����ӣ�
	int					m_Camp;					// Npc����Ӫ
	int					m_CurrentCamp;			// Npc�ĵ�ǰ��Ӫ
	NPCCMD				m_Doing;				// Npc����Ϊ
	CLIENTACTION		m_ClientDoing;			// Npc�Ŀͻ�����Ϊ
	DOING_FRAME			m_Frames;				// Npc����Ϊ֡��
	KSkillList			m_SkillList;			// Npc�ļ����б�
	int					m_SubWorldIndex;		// Npc���ڵ�SubWorld ID
	int					m_RegionIndex;			// Npc���ڵ�Region ID
	int					m_ActiveSkillID;		// Npc����ļ���ID

	int					m_ActiveAuraID;			// Npc����Ĺ⻷����ID

	// Npc��ʵ�����ݣ��Ѿ�����װ�������ܵ������ˣ�
	int					m_CurrentExperience;	// Npc��ɱ���ͳ��ľ���
	int					m_CurrentLife;			// Npc�ĵ�ǰ����
	int					m_CurrentLifeMax;		// Npc�ĵ�ǰ�������ֵ
	int					m_CurrentLifeReplenish;	// Npc�ĵ�ǰ�����ظ��ٶ�
	int					m_CurrentMana;			// Npc�ĵ�ǰ����
	int					m_CurrentManaMax;		// Npc�ĵ�ǰ�������
	int					m_CurrentManaReplenish;	// Npc�ĵ�ǰ�����ظ��ٶ�
	int					m_CurrentStamina;		// Npc�ĵ�ǰ����
	int					m_CurrentStaminaMax;	// Npc�ĵ�ǰ�������
	int					m_CurrentStaminaGain;	// Npc�ĵ�ǰ�����ظ��ٶ�
	int					m_CurrentStaminaLoss;	// Npc�ĵ�ǰ�����½��ٶ�
	KMagicAttrib		m_PhysicsDamage;		// Npc�ĵ�ǰ�˺�(�����������������˺�������������ֱ�Ӽ��˺���ħ������)
	KMagicAttrib		m_CurrentFireDamage;	// Npc�ĵ�ǰ���˺�
	KMagicAttrib		m_CurrentColdDamage;	// Npc�ĵ�ǰ���˺�
	KMagicAttrib		m_CurrentLightDamage;	// Npc�ĵ�ǰ���˺�
	KMagicAttrib		m_CurrentPoisonDamage;	// Npc�ĵ�ǰ���˺�
	int					m_CurrentAttackRating;	// Npc�ĵ�ǰ������
	int					m_CurrentDefend;		// Npc�ĵ�ǰ����

	int					m_CurrentFireResist;	// Npc�ĵ�ǰ����
	int					m_CurrentColdResist;	// Npc�ĵ�ǰ������
	int					m_CurrentPoisonResist;	// Npc�ĵ�ǰ������
	int					m_CurrentLightResist;	// Npc�ĵ�ǰ�翹��
	int					m_CurrentPhysicsResist;	// Npc�ĵ�ǰ������
	int					m_CurrentFireResistMax;		// Npc�ĵ�ǰ������
	int					m_CurrentColdResistMax;		// Npc�ĵ�ǰ��������
	int					m_CurrentPoisonResistMax;	// Npc�ĵ�ǰ��󶾿���
	int					m_CurrentLightResistMax;	// Npc�ĵ�ǰ���翹��
	int					m_CurrentPhysicsResistMax;	// Npc�ĵ�ǰ���������

	int					m_CurrentWalkSpeed;		// Npc�ĵ�ǰ�߶��ٶ�
	int					m_CurrentRunSpeed;		// Npc�ĵ�ǰ�ܶ��ٶ�
	int					m_CurrentJumpSpeed;		// Npc�ĵ�ǰ��Ծ�ٶ�
	int					m_CurrentJumpFrame;		// Npc�ĵ�ǰ��Ծ֡��
	int					m_CurrentAttackSpeed;	// Npc�ĵ�ǰ�����ٶ�
	int					m_CurrentCastSpeed;		// Npc�ĵ�ǰʩ���ٶ�
	int					m_CurrentVisionRadius;	// Npc�ĵ�ǰ��Ұ��Χ
	int					m_CurrentAttackRadius;	// Npc�ĵ�ǰ������Χ
	int					m_CurrentActiveRadius;	// Npc�ĵ�ǰ���Χ
	int					m_CurrentHitRecover;	// Npc�ĵ�ǰ�ܻ��ظ��ٶ�
	int					m_CurrentTreasure;		// Npc����װ��������

// ��������ֻ��Ҫ��ǰֵ������ֵ��Ϊ0
	int					m_CurrentMeleeDmgRetPercent;	// Npc�����˺����صİٷֱ�
	int					m_CurrentMeleeDmgRet;			// Npc���̱���ʱ���ص��˺�����
	int					m_CurrentRangeDmgRetPercent;	// NpcԶ���˺����صİٷֱ�
	int					m_CurrentRangeDmgRet;			// NpcԶ�̱���ʱ���ص��˺�����
	BOOL				m_CurrentSlowMissle;			// Npc�Ƿ��������ӵ�״̬��

	int					m_CurrentDamageReduce;			// �����˺�����
	int					m_CurrentElementDamageReduce;	// Ԫ���˺�����

	int					m_CurrentDamage2Mana;			// �˺�ת�����ٷֱ�
	int					m_CurrentManaPerEnemy;			// ɱһ�����˼Ӷ��ٵ�����
	int					m_CurrentLifeStolen;			// ͵�����ٷֱ�
	int					m_CurrentManaStolen;			// ͵�����ٷֱ�
	int					m_CurrentStaminaStolen;			// ͵�����ٷֱ�
	int					m_CurrentKnockBack;				// ���˰ٷֱ�
	int					m_CurrentDeadlyStrike;			// ����һ���ٷֱ�
	int					m_CurrentBlindEnemy;			// ��ä���˰ٷֱ�
	int					m_CurrentPiercePercent;			// ��͸�����ٷֱ�
	int					m_CurrentFreezeTimeReducePercent;	// ����ʱ����ٰٷֱ�
	int					m_CurrentPoisonTimeReducePercent;	// �ж�ʱ����ٰٷֱ�
	int					m_CurrentStunTimeReducePercent;		// ѣ��ʱ����ٰٷֱ�

	int					m_CurrentFireEnhance;			// ���ǿ
	int					m_CurrentColdEnhance;			// ����ǿ
	int					m_CurrentPoisonEnhance;			// ����ǿ
	int					m_CurrentLightEnhance;			// ���ǿ
	int					m_CurrentAddPhysicsDamage;		// ֱ�ӵ������˺���ǿ����
	int					m_CurrentMeleeEnhance[MAX_MELEE_WEAPON];	// ���������ǿ
	int					m_CurrentRangeEnhance;			// Զ�������ǿ
	int					m_CurrentHandEnhance;			// ���������ǿ
// ֻ��Ҫ��ǰֵ�����ݽ���
	int					m_Dir;							// Npc�ķ���
	int					m_RedLum;						// Npc������
	int					m_GreenLum;
	int					m_BlueLum;
	int					m_MapX, m_MapY, m_MapZ;			// Npc�ĵ�ͼ����
	int					m_OffX, m_OffY;					// Npc�ڸ����е�ƫ�����꣨�Ŵ���1024����
	int					m_DesX, m_DesY;					// Npc��Ŀ������
	int					m_SkillParam1, m_SkillParam2;
	int					m_OriginX, m_OriginY;			// Npc��ԭʼ����
	int					m_NextAITime;
	BYTE				m_AIMAXTime;//NpcAI
// Npc��װ���������ͻ��˵Ļ�װ����	
	int					m_HelmType;					// Npc��ͷ������
	int					m_ArmorType;				// Npc�Ŀ�������
	int					m_WeaponType;				// Npc����������
	int					m_HorseType;				// Npc����������
	BOOL				m_bRideHorse;				// Npc�Ƿ�����

	char				Name[32];				// Npc������
	int					m_nSex;					// Npc���Ա�0Ϊ�У�1ΪŮ
	int					m_NpcSettingIdx;		// Npc���趨�ļ�����
	int					m_CorpseSettingIdx;		// Npc��ʬ�嶨������
	char				ActionScript[80];		// Npc����Ϊ�ű�
	DWORD				m_ActionScriptID;		// Npc����Ϊ�ű�ID��ʹ��ʱ�������������
	
	DWORD				m_TrapScriptID;			// Npc�ĵ�ǰTrap�ű�ID;

	int					m_nPeopleIdx;			// ��������
	int					m_nLastDamageIdx;		// ���һ���˺�����������
	int					m_nLastPoisonDamageIdx;	// ���һ�ζ��˺�����������
	int					m_nObjectIdx;			// ������Ʒ

	// Npc�Ļ������ݣ�δ����װ�������ܵ�Ӱ�죩
	int					m_Experience;			// Npc��ɱ��õľ���
	int					m_LifeMax;				// Npc���������
	int					m_LifeReplenish;		// Npc�������ظ��ٶ�
	int					m_ManaMax;				// Npc���������
	int					m_ManaReplenish;		// Npc�������ظ��ٶ�
	int					m_StaminaMax;			// Npc���������
	int					m_StaminaGain;			// Npc�������ظ��ٶ�
	int					m_StaminaLoss;			// Npc�������½��ٶ�
	int					m_AttackRating;			// Npc��������
	int					m_Defend;				// Npc�ķ���
	int					m_FireResist;			// Npc�Ļ���
	int					m_ColdResist;			// Npc���俹��
	int					m_PoisonResist;			// Npc�Ķ�����
	int					m_LightResist;			// Npc�ĵ翹��
	int					m_PhysicsResist;		// Npc��������
	int					m_FireResistMax;		// Npc��������
	int					m_ColdResistMax;		// Npc����������
	int					m_PoisonResistMax;		// Npc����󶾿���
	int					m_LightResistMax;		// Npc�����翹��
	int					m_PhysicsResistMax;		// Npc�����������
	int					m_WalkSpeed;			// Npc�������ٶ�
	int					m_RunSpeed;				// Npc���ܶ��ٶ�
	int					m_JumpSpeed;			// Npc����Ծ�ٶ�
	int					m_AttackSpeed;			// Npc�Ĺ����ٶ�
	int					m_CastSpeed;			// Npc��ʩ���ٶ�
	int					m_VisionRadius;			// Npc����Ұ��Χ
	int					m_DialogRadius;			// Npc�ĶԻ���Χ
	int					m_ActiveRadius;			// Npc�Ļ��Χ
	int					m_HitRecover;			// Npc���ܻ��ظ��ٶ�
	int					m_Treasure;				// Npc����װ��������
	BOOL				m_bClientOnly;			// 

	int					m_nCurrentMeleeSkill;	// Npc��ǰ��ִ�еĸ񶷼���
	int					m_nCurrentMeleeTime;	
	
	// AI����
	int					m_AiMode;				// AIģʽ
	int					m_AiParam[MAX_AI_PARAM];// ����AIģ�����AI
	int					m_AiAddLifeTime;

	int					m_HeadImage;

	int					m_FightMode;			// �ͻ��˴������á�
	int					m_OldFightMode;
	BOOL				m_bExchangeServer;
#ifdef _SERVER
	int					m_AiSkillRadiusLoadFlag;// ս��npc���ܷ�Χ�Ƿ��Ѿ���ʼ�� ֻ��Ҫ�ڹ����ʱ���ʼ��һ��
	KNpcDeathCalcExp	m_cDeathCalcExp;		// ս��npc�������ͳ��������ͬ��player
	KItemDropRate*		m_pDropRate;
	int					m_nCurPKPunishState;	// PK����ʱ�ĳͷ����ʣ����ڹ�ս
#endif
#ifndef	_SERVER
	int					m_SyncSignal;			// ͬ���ź�
	KClientNpcID		m_sClientNpcID;			// ���ڱ����ͻ���npc���ĸ�region�ĵڼ���npc
	DWORD				m_dwRegionID;			// ��npc����region��id
	KSyncPos			m_sSyncPos;
	int					m_nPKFlag;	
	char				m_szChatBuffer[MAX_SENTENCE_LENGTH];
	int					m_nChatContentLen;
	int					m_nChatNumLine;
	int					m_nChatFontWidth;
	unsigned int		m_nCurChatTime;
	int					m_nSleepFlag;
	int					m_nHurtHeight;
	int					m_nHurtDesX;
	int					m_nHurtDesY;

	int					m_nTongFlag;			// �Ƿ�������ͼ��
#endif
private:
	int					m_LoopFrames;			// ѭ��֡��
	int					m_nPlayerIdx;
	int					m_DeathFrame;			// ����֡��
	int					m_StandFrame;
	int					m_HurtFrame;
	int					m_AttackFrame;
	int					m_CastFrame;
	int					m_WalkFrame;
	int					m_RunFrame;
	int					m_StandFrame1;
	int					m_ReviveFrame;			// ����֡��
	int					m_SitFrame;
	int					m_JumpFrame;
	int					m_JumpFirstSpeed;
	NPC_COMMAND			m_Command;				// ����ṹ
	BOOL				m_ProcessAI;			// ����AI��־
	BOOL				m_ProcessState;			// ����״̬��־
	int					m_XFactor;
	int					m_YFactor;
	int					m_JumpStep;
	int					m_JumpDir;
	int					m_SpecialSkillStep;		// ���⼼�ܲ���
	NPC_COMMAND			m_SpecialSkillCommand;	// ���⼼����Ϊ����
	KNpcFindPath		m_PathFinder;
	BOOL				m_bActivateFlag;

#ifndef	_SERVER
	int					m_ResDir;
	KNpcRes				m_DataRes;				// Npc�Ŀͻ�����Դ��ͼ��������

	// ������ͨս��npcðѪ����
	int					m_nBloodNo;
	int					m_nBloodAlpha;
	int					m_nBloodTime;
	char				m_szBloodNo[32];
#endif

private:
	BOOL				WaitForFrame();
	BOOL				IsReachFrame(int nPercent);
	void				DoStand();
	void				OnStand();
	void				DoRevive();
	void				OnRevive();
	void				DoWait();
	void				OnWait();
	void				DoWalk();
	void				OnWalk();
	void				DoRun();
	void				OnRun();
	void				DoSkill(int nX, int nY);
	int					DoOrdinSkill(KSkill * pSkill, int nX, int nY);
	void				OnSkill();
	void				DoAttack();
	void				DoMagic();
	void				DoJump();
	BOOL				OnJump();
	void				DoSit();
	void				OnSit();
	void				DoHurt(int nHurtFrames = 0, int nX = 0, int nY =0);
	void				OnHurt();

	// mode == 0 npc ���� == 1 player ���£��������� == 2 player ���£�������
	// �� DeathPunish �Ĳ�����Ӧ ������� enumDEATH_MODE
	void				DoDeath(int nMode = 0);

	void				OnDeath();
	void				DoDefense();
	void				OnDefense();
	void				DoIdle();
	void				OnIdle();
	
//	�йظ񶷼��ܵ�------------------------------------------


	BOOL				DoManyAttack();
	void				OnManyAttack();
	BOOL				DoBlurAttack();

	BOOL				DoJumpAttack();
	BOOL				OnJumpAttack();

	BOOL				DoRunAttack();
	void				OnRunAttack();
	BOOL				CastMeleeSkill(KSkill * pSkill);
//-----------------------------------------------------------

	
	void				OnPlayerTalk();

	void				DoSpecial1();
	void				OnSpecial1();
	void				DoSpecial2();
	void				OnSpecial2();
	void				DoSpecial3();
	void				OnSpecial3();
	void				DoSpecial4();
	void				OnSpecial4();
	void				Goto(int nMpsX, int nMpsY);
	void				RunTo(int nMpsX, int nMpsY);
	void				JumpTo(int nMpsX, int nMpsY);
	void				ServeMove(int nSpeed);
	void				ServeJump(int nSpeed);
//	void				Enchant(void);
//	void				ModifyEnchant(void* pData, int nAttrib);
	BOOL				NewPath(int nMpsX, int nMpsY);
	BOOL				NewJump(int nMpsX, int nMpsY);
	BOOL				CheckHitTarget(int nAR, int nDf, int nIngore = 0);
#ifdef _SERVER
	void				LoseSingleItem(int nBelongPlayer);
	void				LoseMoney(int nBelongPlayer);
	void				PlayerDeadCreateMoneyObj(int nMoneyNum);	// �������ʱ���������Ǯ����һ��object
	void				UpdateNpcStateInfo();		//���¸��½�ɫ��״̬��Ϣ
#endif

#ifndef _SERVER
	void				HurtAutoMove();
#endif

public:
	friend class KNpcAttribModify;
	friend class KThiefSkill;
	KNpc();
	void				SetActiveFlag(BOOL bFlag) { m_bActivateFlag = bFlag; };
	void				DoPlayerTalk(char *);
	void				CheckTrap();
	void				Init();
	void				Remove();
	void				Activate();
	BOOL				IsPlayer();
	void				SetFightMode(BOOL bFightMode);
	void				TurnTo(int nIdx);
	void				SendCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
	void				ProcCommand(int nAI);
	ISkill* 			GetActiveSkill();
	BOOL				ProcessState();
	void				ProcStatus();
	void				ModifyAttrib(int nAttacker, void* pData);
	int					GetSkillLevel(int nSkillId);
	void				SetId(DWORD	dwID)	{ m_dwID = dwID;};
	BOOL				IsMatch(DWORD dwID)	{ return dwID == m_dwID; };	// �Ƿ�ID���Indexƥ��
	BOOL				Cost(NPCATTRIB nType, int nCost, BOOL bOnlyCheckCanCast = FALSE);				// ��������������,���OnlyCheckCanCostΪTRUE,��ʾֻ�Ǽ�鵱ǰ���������Ƿ����ģ�����ʵ�ʵĿ�
	void				SelfDamage(int nDamage);						// ������˺�������������
	void				Load(int nNpcSettingIdx, int nLevel);						// ��TabFile�м���
	void				GetMpsPos(int * pPosX, int *pPosY);
	BOOL				SetActiveSkill(int nSkillIdx);
	void				SetAuraSkill(int nSkillID);
	void				SetCamp(int nCamp);
	void				SetCurrentCamp(int nCamp);
	void				RestoreCurrentCamp();
	void				SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime = -1);	// �������������뱻������
	void				ClearStateSkillEffect();
	void				ReCalcStateEffect();
	void				ClearNormalState();
	void				SetImmediatelySkillEffect(int nLauncher, void *pData, int nDataNum);
	void				AppendSkillEffect(BOOL bIsPhysical, BOOL bIsMelee, void *pSrcData, void *pDesData);
	int					ModifyMissleLifeTime(int nLifeTime);
	int					ModifyMissleSpeed(int nSpeed);
	BOOL				ModifyMissleCollsion(BOOL bCollsion);
	void				RestoreNpcBaseInfo(); //Set Current_Data ;
	void				RestoreState();
	void				ClearNpcState();
	BOOL				SetPlayerIdx(int nIdx);
	void				DialogNpc(int nIndex);
	void				Revive();
	void				AddBaseLifeMax(int nLife);	// ���ӻ������������
	void				AddCurLifeMax(int nLife);
	void				AddBaseStaminaMax(int nStamina);// ���ӻ������������
	void				AddCurStaminaMax(int nStamina);
	void				AddBaseManaMax(int nMana);	// ���ӻ������������
	void				AddCurManaMax(int nMana);
//	void				ResetLifeReplenish();		// ���¼��������ظ��ٶ�
//	void				CalcCurLifeMax();			// ���㵱ǰ���������
//	void				CalcCurStaminaMax();		// ���㵱ǰ���������
//	void				CalcCurManaMax();			// ���㵱ǰ���������
	void				CalcCurLifeReplenish();		// ���㵱ǰ�����ظ��ٶ�
	void				SetSeries(int nSeries);// �趨�� npc ���������ԣ����ݻ�û��ɣ�
//	int					GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam);
//	int					SkillString2Id(char * szSkillString);
	void				GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel);
//	void				InitNpcLevelData(KTabFile * pTabFile, int nNpcTemplateId, KLuaScript * pLevelScript, int nLevel);
//	void				InitNpcBaseData(int nNpcTemplateId);
	void				SetPhysicsDamage(int nMinDamage, int nMaxDamage);	// �趨�������������Сֵ
	void				SetBaseAttackRating(int nAttackRating);					// �趨����������
	void				SetBaseDefence(int nDefence);							// �趨������
//	void				SetBaseWalkSpeed(int nSpeed);							// �趨�����ٶ�
//	void				SetBaseRunSpeed(int nSpeed);							// �趨�ܲ��ٶ�
	int					GetCurActiveWeaponSkill();
	void				LoadDataFromTemplate(int nNpcTemplateId, int nLevel);

#ifdef	_SERVER
	void				ExecuteRevive(){DoRevive();};
	BOOL				SendSyncData(int nClient);						// ��һ���ͻ��˷�����ͬ������
	void				NormalSync();									// �㲥Сͬ��
	void				BroadCastState();								// �㲥��ɫ��״̬��Ϣ,�⻷��ͷ���ȵ�Ч��
	void				BroadCastRevive(int nType);
	int					GetPlayerIdx();
	void				CalcDamage(int nAttacker, int nMin, int nMax, DAMAGE_TYPE nType, BOOL bIsMelee, BOOL bDoHurt = TRUE, BOOL bReturn = FALSE);
	BOOL				ReceiveDamage(int nLauncher, BOOL bIsMelee, void *pData, BOOL bUseAR, BOOL bDoHurt);

	// mode == 0 npc ���� == 1 player ���£��������� == 2 player ���£�������
	// �� DoDeath �Ĳ�����Ӧ ������� enumDEATH_MODE
	void				DeathPunish(int nMode, int nBelongPlayer);

	void				RestoreLiveData();								// ������ָ�Npc�Ļ�������
	int					SetPos(int nX, int nY);
	int					ChangeWorld(DWORD dwSubWorldID, int nX, int nY);	// �л�����
	void				TobeExchangeServer(DWORD dwMapID, int nX, int nY);
	void				RestoreLife(){m_CurrentLife = m_CurrentLifeMax;	};
	void				RestoreMana(){m_CurrentMana = m_CurrentManaMax;};
	void				RestoreStamina(){m_CurrentStamina = m_CurrentStaminaMax; };
	void				SendDataToNearRegion(void* pBuffer, DWORD dwSize);// ����Χ�����㲥
	int					DeathCalcPKValue(int nKiller);					// ����ʱ�����PKֵ

	BOOL				CheckPlayerAround(int nPlayerIdx);// ������Χ9��Region���Ƿ���ָ���� player
#endif

#ifndef _SERVER
	void				SetSleepMode(BOOL bSleep) { m_nSleepFlag = bSleep; m_DataRes.SetSleepState(bSleep);};
	void				SetNpcState(BYTE* pNpcState);
	void				RemoveRes();
	void				ProcNetCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
	void				Paint();
	int					PaintInfo(int nHeightOffset, bool bSelect, int nFontSize = 12, DWORD	dwBorderColor = 0);
	int					PaintChat(int nHeightOffset);
	int					SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength);
	int					PaintLife(int nHeightOffset, bool bSelect);
	int					PaintMana(int nHeightOffset);
	void				DrawBorder();
	int					DrawMenuState(int n);
	void				DrawBlood();	//����Ѫ���������ڹ̶�λ��
	BOOL				IsCanInput() { return m_ProcessAI; };
	void				SetMenuState(int nState, char *lpszSentence = NULL, int nLength = 0);	// �趨ͷ��״̬
	int					GetMenuState();				// ���ͷ��״̬
	DWORD				SearchAroundID(DWORD dwID);	// ������Χ9��Region���Ƿ���ָ�� ID �� npc
	void				SetSpecialSpr(char *lpszSprName);// �趨�����ֻ����һ�������spr�ļ�
	void				SetInstantSpr(int nNo);
	int					GetNormalNpcStandDir(int nFrame);
	KNpcRes*			GetNpcRes(){return &m_DataRes;};

	int GetNpcPate();
	int GetNpcPatePeopleInfo();

	// ðѪ����
	void				ClearBlood();
	void				SetBlood(int nNo);
	int				PaintBlood(int nHeightOffset);	//����ðѪ
#endif
	
};
#ifndef TOOLVERSION
extern KNpc Npc[MAX_NPC];
#else
extern CORE_API KNpc Npc[MAX_NPC];
#endif

#endif


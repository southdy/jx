#ifndef __SKILLDEF_H__
#define __SKILLDEF_H__

#define MAX_SKILL  500
#define MAX_SKILLLEVEL 64
#define MaxMissleDir	64
#define MAXSKILLLEVELSETTINGNUM	10  //����д��������ʱ���������������
#define MAX_MISSLESTYLE  200
#define	MAX_MISSLE_STATUS 4

//���ֵȼ��¼��ܻ������Եı仯����ֵ����
enum eEffectType					
{
	//�ı�ü��ܵ�����
		ET_SKLATR_BEGIN					= 1	,
		ET_SKLATR_MANACOST					,		// ����MANA
		ET_SKLATR_COSTTYPE					,		//	
		ET_SKLATR_MINTIMEPERCAST  			,		// ÿ�η�ħ���ļ��ʱ��
		ET_SKLATR_ATTACKRATE				,		// �ı�Npc�ĳɹ���
		ET_SKLATR_MISSLENUM					,		// 
		ET_SKLATR_MISSLESFORM				,		
		ET_SKLATR_PARAM1					,		
		ET_SKLATR_PARAM2					,
		ET_SKLATR_END					=999,		//
		
		//�ı䷢���ӵ�������
		ET_CHGMSL_BEGIN				= 1000  ,
		ET_CHGMSL_MOVEKIND					,		// �ӵ���ʽ
		ET_CHGMSL_SPEED						,		// �ӵ������ٶ�
		ET_CHGMSL_LIFETIME					,		// �ӵ�������ʱ��
		ET_CHGMSL_HEIGHT					,		// �ӵ��ĸ߶�
		ET_CHGMSL_DAMAGERANGE				,		// �ӵ��ƻ���Χ
		ET_CHGMSL_RADIUS					,		// �ӵ����а뾶
		ET_CHGMSL_END				= 1999  ,
		
		//�ı�Ŀ��Npc������				(��������)
		ET_CHGNPC_BEGIN				= 2000  ,
		ET_CHGNPC_LIFE						,       // �ı�Npc��ǰ��Lifeֵ
		ET_CHGNPC_LIFELIMITED				,		// �ı�Npc��ǰ��Life����ֵ
		ET_CHGNPC_LIFERESTORESPEED			,		// �ı�Npc��ǰ��Life�ظ��ٶ�
		ET_CHGNPC_MANA						,		// �ı�Npc��ǰ��Mana
		ET_CHGNPC_MANALIMITED				,		// �ı�Npc��ǰ��Mana����ֵ
		ET_CHGNPC_MANASTORESPEED			,		// �ı�Npc��ǰ��Mana�ظ��ٶ�
		ET_CHGNPC_STAMINA					,		// �ı�Npc��ǰ������
		ET_CHGNPC_STAMINALIMITED			,		// �ı�Npc��ǰ����������ֵ
		ET_CHGNPC_STAMINARESTORESPEED		,		// �ı�Npc��ǰ�������ظ��ٶ�
		ET_CHGNPC_SPEED						,		// Npc���ɫ�����ٶ�
		ET_CHGNPC_ATTACK					,		// ������
		ET_CHGNPC_DEFEND					,		// ������
		ET_CHGNPC_VISION					,		// ��Ұ��Χ
		ET_CHGNPC_ATTACKRATE				,		// �ı�Npc��������
		//	ET_CHGNPC_LIFERESTORESPEED			,		// �����ظ��ٶ�
		ET_CHGNPC_ATTACKSPEED				,		// �����ٶ�
		ET_CHGNPC_HITRECOVER				,		// ���ٴ��ϵ��
		ET_CHGNPC_END			= 2999		,		
		
		//�ı�Npc�ĵ�ǰ״̬����			(��������)
		ET_CHGNPCST_BEGIN					,						
		ET_CHGNPCST_RESISTFIRE				,		// ������
		ET_CHGNPCST_RESISTFREEZE			,		// ������
		ET_CHGNPCST_RESISTPOISON			,		// ������
		ET_CHGNPCST_RESISTELECTRIC			,		// ������
		ET_CHGNPCST_RESISTPHYSICS			,		// ��������
		ET_CHGNPCST_RESISTALLEFECT			,		// ������Ԫ���˺�	
		
		ET_CHGNPCST_FIRETIME				,		// ȼ��ʱ�伴ȼ�մ���
		ET_CHGNPCST_FREEZETIME				,		// ����ʱ��
		ET_CHGNPCST_POISONTIME				,		// �ж�ʱ��
		ET_CHGNPCST_ELECTRICTIME			,		// ѣ��ʱ�� ��Ҳ��ܶ�
		ET_CHGNPCST_FIREPERDAMAGE			,		// ���˺�ÿ�α仯
		ET_CHGNPCST_FREEZEPERDAMAGE			,		// ���˺�ÿ�α仯
		ET_CHGNPCST_POISONPERDAMAGE			,		// ���˺�ÿ�α仯
		ET_CHGNPCST_ELECTRICPERDAMAGE		,		// ���˺�ÿ�α仯
		
		ET_CHGNPCST_FIRETHORN				,		// �𷴵�ϵ��
		ET_CHGNPCST_FREEZETHORN				,		// ��������ϵ��
		ET_CHGNPCST_POISONTHORN				,		// ������ϵ��
		ET_CHGNPCST_ELECTRICTHORN			,		// �練��ϵ��
		ET_CHGNPCST_PHYSICSTHORN			,		// ������ϵ��
		ET_CHGNPCST_PHYSICSLIMITED			,		// �����˺�ֵ������
		ET_CHGNPCST_MISSLESPEED				,		// �ӵ����ٶȱ仯
		ET_CHGNPCST_NPCCAMP					,		// �ı�Npc����Ӫ
		
		ET_CHGNPCST_END ,
		
		ET_RECV_BEGIN				= 3000  ,
		ET_RECV_FIREDAMAGE					,		// ���˺�
		ET_RECV_POISONDAMAGE				,		// ���˺�
		ET_RECV_FREEZEDAMAGE				,		// ���˺�
		ET_RECV_DAMAGE						,		// �����˺�
		ET_RECV_END					= 3999	,	
		
		ET_EFECTTYPE						,		// Ԫ���˺�����
		ET_EFECTDAMAGEPERTIME				,		// Ԫ���˺�ÿʱ��
		ET_EFECTINTERVAL					,		// Ԫ���˺��ļ��ʱ��
		ET_EFECTDAMAGENUM					,		// Ԫ���˺�����		
		
};


//---------------------------------------------------------------------------
// MoveKind �˶�����
//---------------------------------------------------------------------------
enum eMissleMoveKind
{
	MISSLE_MMK_Stand,							//	ԭ��
		MISSLE_MMK_Line,							//	ֱ�߷���
		MISSLE_MMK_Random,							//	������У����ڶ�Ů�׵�Charged Bolt��
		MISSLE_MMK_Circle,							//	���з��У�Χ������ߣ����ڶ��̿͵ļ�����
		MISSLE_MMK_Helix,							//	�����׵������ߣ����ڶ�������Bless Hammer��
		MISSLE_MMK_Follow,							//	����Ŀ�����
		MISSLE_MMK_Motion,							//	��Ҷ�����
		MISSLE_MMK_Parabola,						//	������
		MISSLE_MMK_SingleLine,						//	���еĵ�һֱ�߷���ħ��
};

//---------------------------------------------------------------------------
// FollowKind ��������	(��Ҫ�����ԭ�ء������������߷���������)
//---------------------------------------------------------------------------
enum eMissleFollowKind
{
	MISSLE_MFK_None,							//	�������κ����
	MISSLE_MFK_NPC,								//	����NPC�����
	MISSLE_MFK_Missle,							//	�����ӵ�
};

enum eMissleStatus
{
	MS_DoWait,
	MS_DoFly,
	MS_DoVanish,
	MS_DoCollision,
};

struct TEffectData 
{
	eEffectType type;
	int			value;
};


enum eSkillLRInfo
{
	BothSkill,          //���Ҽ��Կ�
	leftOnlySkill,		//���
	RightOnlySkill,		//�Ҽ�
	NoneSkill,			//������
};




//--------------------------------------------------------Skill.h


//���ܷ����ߵ�����
enum eGameActorType
{
	Actor_Npc,
	Actor_Obj,
	Actor_Missle,
	Actor_Sound,
	Actor_None,
};
enum eSkillLauncherType
{
	SKILL_SLT_Npc = 0,
	SKILL_SLT_Obj ,
	SKILL_SLT_Missle,
};

class KEffectDataNode :public KNode
{
public:
	TEffectData EffectData;
};

#ifndef _SERVER

struct	TOrginSkill
{
	int		nNpcIndex;				//	Npc��index
	DWORD	nSkillId;				//	���͵�skillid
};

#endif


enum eSkillParamType
{
	SKILL_SPT_TargetIndex	= -1,
	SKILL_SPT_Direction		= -2,
};

//���ܵ�����
enum eSKillStyle
{
	SKILL_SS_Missles = 0,			//	�ӵ���		���������ڷ����ӵ���
		SKILL_SS_Melee,
		SKILL_SS_InitiativeNpcState,	//	������		���������ڸı䵱ǰNpc������״̬
		SKILL_SS_PassivityNpcState,		//	������		���������ڸı�Npc�ı���״̬
		SKILL_SS_CreateNpc,				//	����Npc��	��������������һ���µ�Npc
		SKILL_SS_BuildPoison,			//	������		��������������
		SKILL_SS_AddPoison,				//	�Ӷ���		���������ڸ������Ӷ���
		SKILL_SS_GetObjDirectly,		//	ȡ����		���������ڸ���ȡ��
		SKILL_SS_StrideObstacle ,		//	��Խ��		���������ڿ�Խ�ϰ�
		SKILL_SS_BodyToObject,			//	������		���������ڽ�ʬ���ɱ���
		SKILL_SS_Mining,				//	�ɿ���		���������ڲɿ�������ɿ�ʯ
		SKILL_SS_RepairWeapon,			//	�޸���		�����������޸�װ��
		SKILL_SS_Capture,				//	��׽��		���������ڲ�׽����Npc
};

//ͬʱ�����Ķ���ӵ��ķ�����ʼ��ʽ
enum eMisslesForm
{
	SKILL_MF_Wall	= 0,			//ǽ��	����ӵ��ʴ�ֱ�������У���ʽ��ǽ״
		SKILL_MF_Line,					//����	����ӵ���ƽ������ҷ�������
		SKILL_MF_Spread,				//ɢ��	����ӵ���һ���ĽǶȵķ�ɢ״	
		SKILL_MF_Circle,				//Բ��	����ӵ�Χ��һ��Ȧ
		SKILL_MF_Random,				//���	����ӵ�����ŷ�
		SKILL_MF_Zone,					//����	����ӵ�������ĳ����Χ��
		SKILL_MF_AtTarget,				//����	����ӵ�����
		SKILL_MF_AtFirer,				//����	����ӵ�ͣ����ҵ�ǰλ��
		SKILL_MF_COUNT,
};

enum eSKillCostType
{
	SKILL_SCT_MANA		= 1,
		SKILL_SCT_LIFE		= 2,
		SKILL_SCT_STAMINA	= 8,
		SKILL_SCT_MONEY		= 16,
};

enum eMisslesGenerateStyle
{
	SKILL_MGS_NULL		= 0,
		SKILL_MGS_SAMETIME	,    //ͬʱ
		SKILL_MGS_ORDER		,	 //��˳��
		SKILL_MGS_RANDONORDER,
		SKILL_MGS_RANDONSAME,
		SKILL_MGS_CENTEREXTENDLINE,  //���м���������ɢ
};


struct TGameActorPos
{
	int nX;
	int nY;
	int nZ;
	int nSubWorldIndex;
};

struct TGameActorInfo
{
	eGameActorType nActorType;
	int nActorIndex;
	DWORD dwActorID;
	void	GetMps(int &nPX, int &nPY);
};

enum CASTSKILLPARAM
{
	PARAM_LAUNCHER	= 0x01, 
	PARAM_TARGET	= 0x02,
	PARAM_REF		= 0x04,
	PARAM_ONLYDIR	= 0x08,
	PARAM_USEREF    = 0X16,
};

union TGameActorUnion
{
	TGameActorPos	tPoint;
	TGameActorInfo	tActorInfo;
};

struct TSkillParam
{
	
	
	int				nDir;

	TGameActorUnion tRef;
	TGameActorUnion tTarget;
	TGameActorUnion tLauncher;

	unsigned short	usSkillID;
	unsigned short	usSkillLevel;
	DWORD	dwSkillParams;
	int		nWaitTime;
	inline int GetSubWorldIndex();
	inline int GetCellHeight();
	inline int GetCellWidth();
	
};

enum eSkillEvent
{
	Event_Start,
	Event_Fly,
	Event_Collision,
	Event_Vanish,
};

#endif //__SKILLDEF_H__
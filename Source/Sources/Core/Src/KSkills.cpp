            // KSkills.cpp: implementation of the KSkills class.
//
//////////////////////////////////////////////////////////////////////
#include "KCore.h"
#ifdef _STANDALONE
#include "KSG_StringProcess.h"
#else
#include "../../Engine/Src/KSG_StringProcess.h"
#endif
#include "KSkills.h"
#include "KMissle.h"
#include "KMissleSet.h"
#include "KNpc.h"
#include "math.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KMath.h"
#include "KEngine.h"
#include "KTabFile.h"
#include "KTabFileCtrl.h"
#include "KMissleMagicAttribsData.h"
#include "KPlayer.h"
#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"
#include "imgref.h"
#include "KMagicDesc.h"
#include "KOption.h"
#endif

//#define SHOW_SKILL_MORE_INFO
#define	 NPCINDEXOFOBJECT 0 //�����ħ��ʱ����Ӧ��Npc���
const char * g_MagicID2String(int nAttrib);
extern  const KScript * g_GetScript(DWORD dwScriptId);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*!*****************************************************************************
// Function		: KSkill::KSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KSkill::KSkill()
{
	m_nFlySkillId =  m_nCollideSkillId = m_nVanishedSkillId = 0;
	
    // add by FreewayChen in 2003.6.6
    m_nImmediateAttribsNum = m_nStateAttribsNum = m_nMissleAttribsNum = m_nDamageAttribsNum = 0;
	m_nSkillCostType = attrib_mana;
    m_nWaitTime = 0;
	m_nEquiptLimited = 0;
	m_bDoHurt = 1;
	
#ifndef _SERVER
	m_szSkillDesc[0] = 0;
	m_szManPreCastSoundFile[0] = 0;
	m_szFMPreCastSoundFile[0] = 0;
#else
	m_dwSkillLevelUpScriptID = 0;
	m_dwSkillLevelDataScriptId = 0;
#endif
	
}
/*!*****************************************************************************
// Function		: KSkill::~KSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KSkill::~KSkill()
{
	
}

/*!*****************************************************************************
// Function		: KSkill::Param2PCoordinate
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nParam3
// Argumant		: int *npPX
// Argumant		: int *npPY
// Comments		:
// Author		: RomanDou
*****************************************************************************/
inline int	KSkill::Param2PCoordinate(int nLauncher, int nParam1, int nParam2 , int *npPX, int *npPY, eSkillLauncherType eLauncherType)  const 
{
	
	int nRegionId, nDesMapX, nDesMapY ;
	int nTargetId = -1;
	if (eLauncherType == SKILL_SLT_Obj) return 0;
	
	switch(nParam1)
	{
	case -1://nParam2 ����ָ��ĳ��Npc����Obj��Index
		nTargetId		= nParam2;
		nRegionId		= Npc[nParam2].m_RegionIndex;
		nDesMapX		= Npc[nParam2].m_MapX;
		nDesMapY		= Npc[nParam2].m_MapY;
		
		if (eLauncherType == SKILL_SLT_Npc)
			SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(nRegionId, nDesMapX , nDesMapY, Npc[nParam2].m_OffX , Npc[nParam2].m_OffY, npPX, npPY);
		else if(eLauncherType == SKILL_SLT_Obj)
			SubWorld[Object[nLauncher].m_nSubWorldID].Map2Mps(nRegionId, nDesMapX, nDesMapY, Object[nParam2].m_nOffX , Object[nParam2].m_nOffY, npPX, npPY);
		else;
		break;
		
	case -2://nParam ����ָ��ĳ������
		
		break;
	default://Ĭ��ʱ, nParam1 ��nParam2 Ϊʵ�ʵ�����
		*npPX = nParam1;
		*npPY = nParam2;
		break;
	}
	
	if (*npPX < 0 || *npPY < 0)	
		g_DebugLog("Param2PCoordinate������ò���Υ����nParam1 ,nParam2 [%d,%d], nPX,nPY", nParam1, nParam2, *npPX, * npPY);
	
	return nTargetId;
}

BOOL KSkill::CanCastSkill(int nLauncher, int &nParam1, int &nParam2)  const 
{
	//�����ѵ�������������
	if (m_bTargetSelf && nParam1 != -1) 
	{
		nParam1 = -1;
		nParam2 = nLauncher;
		return TRUE;
	}
	else
	{
		if (m_bTargetOnly && nParam1 != -1) return FALSE;
		
		if (nParam1 == -1)
		{
			if ( nParam2 <= 0 || nParam2 >= MAX_NPC) return FALSE;
			NPC_RELATION  Relation = NpcSet.GetRelation(nLauncher, nParam2);
			
			if (m_bTargetEnemy)
			{
				if (Relation & relation_enemy) goto relationisvalid;
			}
			
			if (m_bTargetAlly)
			{
				if (Relation & relation_ally) goto relationisvalid;
			}
			
			if (m_bTargetSelf)
			{
				if (Relation & relation_self) goto relationisvalid;
			}
			return FALSE;
		}
		
	}
	
relationisvalid:

	if (Npc[nLauncher].IsPlayer())
	{
		if (IsPhysical())
		{
			int nWeapoinSkill = Npc[nLauncher].GetCurActiveWeaponSkill();
			if ((DWORD)nWeapoinSkill != m_nId)
			{
				return FALSE;
			}
		}

		//-2��ʾ���ܲ��ܵ�ǰװ��������,
		//-1��ʾ��������
		//0-99��ĳ�ֽ��������װ������ ȡֵΪ��װ���ľ�������
		//100-199��ĳ��Զ�̹������װ������ ȡֵΪ��װ���ľ������� ��100

		if (-2 != m_nEquiptLimited)
		{

#ifdef _SERVER
			int nPlayerIdx		= Npc[nLauncher].GetPlayerIdx();
#else
			int nPlayerIdx		= CLIENT_PLAYER_INDEX;
#endif
			int nDetailType		= Player[nPlayerIdx].m_ItemList.GetWeaponType();
			int nParticularType = Player[nPlayerIdx].m_ItemList.GetWeaponParticular();
			
			//��������
			if (nDetailType == 0)
			{
				
			}//Զ������
			else if (nDetailType == 1)
			{
				nParticularType += MAX_MELEEWEAPON_PARTICULARTYPE_NUM;
			}//����
			else if (nDetailType == -1)
			{
				nParticularType = -1;
			}
			
			if (nParticularType != m_nEquiptLimited)
				return FALSE;
		}
		
		//0��ʾ������
		//1��ʾ�����������ü���
		//2��ʾ���������ü���
		if (m_nHorseLimited)
		{
			switch(m_nHorseLimited)
			{
			case 1:
				{
					if (Npc[nLauncher].m_bRideHorse)
						return FALSE;
				}
				break;
			case 2:
				{
					if (!Npc[nLauncher].m_bRideHorse)
						return FALSE;
				}
				break;
			default:
				return FALSE;
			}
		}
		
		if ((m_bTargetOnly) && nParam1 == -1)
		{
#ifndef _SERVER
			int distance = NpcSet.GetDistance(nLauncher, nParam2);
			if (distance > GetAttackRadius()) return FALSE;
#endif
		}
		/*else
		{
		if (nParam1 < 0 || nParam2 < 0) return FALSE;
		
		  #ifndef _SERVER
		  int nLauncherPX = 0, nLauncherPY = 0;
		  Npc[nLauncher].GetMpsPos(&nLauncherPX, &nLauncherPY);
		  int ndistance = g_GetDistance(nLauncherPX, nLauncherPY, nParam1, nParam2);
		  if (ndistance > GetAttackRadius()) return FALSE;
		  #endif
		  }
		*/
	}
	return TRUE;
}

//		����ҵ���ĳ������ʱ���� [5/28/2002]
//		�ͻ��˺ͷ��������ڼ��ܵĵ��÷�����һЩ��ͬ
//		��������һ���յ��ľ����ͻ��˴�����Ĳ���
//		��Ϸ��������Ϣ�������ִ��ÿ���仯����˶��ڷ�����Ӧ��Ҳ��ͳһ���ݽӿ�
//		�ͻ���ʱ���������Ϣ�����Ա�����ҵ����룬����뽫��ת��Ϊʵ�ʵ���Ϣ
//		��ִ�С�ͬʱ��Ӧ��ת���õ���Ϣ������������
/*
�йش��Ĳ�����MapX������PointX���ݾ����ħ�����ܶ���
����һ�����ħ��ΪMap���꣬������ħ��ΪPoint����
*/
/*
ע�⵱����Castʱ��������ȷ����ǰ��nLauncherIndex��Socket���Ӧ��dwIdһ�£���IsMatch()ͨ����
*/

/*!*****************************************************************************
// Function		: KSkill::Cast
// Purpose		: �����ܵ�ͳһ�ӿ�
// Return		: 
// Argumant		: int nLauncher ������Id
// Argumant		: int nParam1   
// Argumant		: int nParam2
// Argumant		: int nWaitTime ���͵��ӳ�ʱ��
// Argumant		: eSkillLauncherType eLauncherType ����������
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::Cast(int nLauncher, int nParam1, int nParam2, int nWaitTime, eSkillLauncherType eLauncherType)  const 
{
	//-----------------�ӿں�����ڵ㣬�������Ϸ���-------------------------------
	
	if (nLauncher < 0 )
	{
		g_DebugLog("Skill::Cast(), nLauncher < 0 , Return False;"); 
		return FALSE; 
	}
	//��鷢�����Ƿ����Ҫ��

	switch(eLauncherType)
	{
	case SKILL_SLT_Npc:
		{
			if (MAX_NPC <= nLauncher) return FALSE;
			if (Npc[nLauncher].m_dwID < 0) return FALSE;
			if (nParam1 == -1)
			{
				if (nParam2 >= MAX_NPC) 
					return FALSE;
				
				if (
					(Npc[nParam2].m_Index <= 0)
					|| Npc[nLauncher].m_SubWorldIndex != Npc[nParam2].m_SubWorldIndex
					)
					return FALSE;
			}
		}
		break;
		
	case SKILL_SLT_Obj:
		{
			return FALSE;
			if (MAX_OBJECT <= nLauncher) return FALSE;
			if (Object[nLauncher].m_nDataID < 0) return FALSE;
		}
		break;
	case SKILL_SLT_Missle:
		{
			if (MAX_MISSLE <= nLauncher) 
				return FALSE;
			
			if (Missle[nLauncher].m_nMissleId < 0) 
				return FALSE;
			
			if (nParam1 == -1)
			{
				if (nParam2 >= MAX_NPC) 
					return FALSE;
				
				if ((Npc[nParam2].m_Index <= 0) ||  Missle[nLauncher].m_nSubWorldId != Npc[nParam2].m_SubWorldIndex)
					return FALSE;
			}

		}
		break;
	default:
		{
			return FALSE;
		}
	}

	
	
	if (nParam1 < 0 && nParam2 < 0 ) 
		return FALSE;
	
	if (nWaitTime < 0 ) 
	{
		g_DebugLog("Call Skill::Cast(), nWaitTime < 0 "); 
		nWaitTime = 0;
	}
	
	//------------------------------------------------------------------------------
	
	switch(m_eSkillStyle)
	{
		
	case	SKILL_SS_Missles:				//���ӵ�
		{
			CastMissles(nLauncher, nParam1, nParam2, nWaitTime, eLauncherType);
		}
		break;
		
	case	SKILL_SS_Melee:
		{
			
			
		}break;
		
	case	SKILL_SS_InitiativeNpcState:	//�ı��ɫ������״̬
		{
			CastInitiativeSkill(nLauncher, nParam1, nParam2, nWaitTime);
		}
		break;
		
	case	SKILL_SS_PassivityNpcState:	//�ı��ɫ�ı���״̬	
		{
			CastPassivitySkill(nLauncher, nParam1, nParam2, nWaitTime);
		}
		break;
		
	case	SKILL_SS_CreateNpc:			//�����µ�Npc������ 
		{
			
		}
		break;
		
	case	SKILL_SS_BuildPoison:			//������
		{
			
		}
		break;
		
	case	SKILL_SS_AddPoison:			//�Ӷ���
		{
			
		}
		break;
		
	case	SKILL_SS_GetObjDirectly:		//����ȡ��	
		{
			
		}
		break;
		
	case	SKILL_SS_StrideObstacle:		//��Խ�ϰ�
		{
			
		}
		break;
		
	case	SKILL_SS_BodyToObject:		//ʬ��
		{
			
		}
		break;
		
	case	SKILL_SS_Mining:				//�ɿ�
		{
			
		}
		break;
		
	case	SKILL_SS_RepairWeapon:		//�޸���
		{
			
		}
		break;
		
	case	SKILL_SS_Capture:				//��׽�� 
		{
			
		}
		break;
	}
	
	if (m_bStartEvent && m_nStartSkillId > 0 && m_nEventSkillLevel > 0)
	{
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nStartSkillId, m_nEventSkillLevel);
		if (!pOrdinSkill) 
            return FALSE;
		
        pOrdinSkill->Cast(nLauncher, nParam1, nParam2, nWaitTime, eLauncherType);
	}
	
	return TRUE;	  
}

/*!*****************************************************************************
// Function		: KSkill::Vanish
// Purpose		: �ӵ���������ʱ�ص�
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::Vanish(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_VanishEvent, pMissle);
}

BOOL KSkill::OnMissleEvent(unsigned short usEvent, KMissle * pMissle)  const 
{
	if (!pMissle) 
        return FALSE;

	int nLauncherIdx = pMissle->m_nLauncher;
	
    if (
		pMissle->m_nMissleId <= 0 
		|| pMissle->m_nMissleId >= MAX_MISSLE 
		|| nLauncherIdx <= 0
		|| nLauncherIdx >= MAX_NPC
		|| Npc[nLauncherIdx].m_Index <= 0
		)
        return FALSE;

	
	if (
		(!Npc[nLauncherIdx].IsMatch(pMissle->m_dwLauncherId)) 
		|| Npc[nLauncherIdx].m_SubWorldIndex != pMissle->m_nSubWorldId
		|| Npc[nLauncherIdx].m_RegionIndex < 0
		)
	{
		return FALSE;
	}
	
	int nEventSkillId = 0;
	int nEventSkillLevel = 0;
	switch(usEvent)
	{
	case Missle_FlyEvent:
		if (!m_bFlyingEvent || m_nFlySkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nFlySkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_StartEvent:
		if (!m_bStartEvent || m_nStartSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nStartSkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_VanishEvent:
		if (!m_bVanishedEvent || m_nVanishedSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nVanishedSkillId ;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
		
	case Missle_CollideEvent:
		if (!m_bCollideEvent || m_nCollideSkillId <= 0 || m_nEventSkillLevel <= 0)
			return FALSE;
		nEventSkillId = m_nCollideSkillId;
		nEventSkillLevel = m_nEventSkillLevel;
		break;
	default:
		return FALSE;
	}
		
	int nDesPX = 0, nDesPY = 0;
	
	if (m_bByMissle)
	{
		pMissle->GetMpsPos(&nDesPX, &nDesPY);
	}
	else
	{
		Npc[nLauncherIdx].GetMpsPos(&nDesPX, &nDesPY);
	}
	
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nEventSkillId, nEventSkillLevel);
	if (!pOrdinSkill) 
        return FALSE;
	
	BOOL bRetCode = FALSE;
	
    if (m_bByMissle)    //When Event
	{
		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
		{
			bRetCode = pOrdinSkill->CastMissles(pMissle->m_nMissleId, nDesPX, nDesPY, 0, SKILL_SLT_Missle);
		}
	}
	else
	{
		if (pOrdinSkill->GetSkillStyle() == SKILL_SS_Missles)
		{   
            bRetCode = pOrdinSkill->CastMissles(nLauncherIdx, nDesPX, nDesPY, 0, SKILL_SLT_Npc);
		}
	}
	
	return bRetCode;
}

/*!*****************************************************************************
// Function		: KSkill::FlyEvent
// Purpose		: 
// Return		: void 
// Argumant		: int nMissleId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void KSkill::FlyEvent(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_FlyEvent, pMissle);
}

/*!*****************************************************************************
// Function		: KSkill::Collidsion
// Purpose		: �ӵ���ײʱ�ص�
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::Collidsion(KMissle * pMissle)  const 
{
	OnMissleEvent(Missle_CollideEvent, pMissle);
}

/*!*****************************************************************************
// Function		: KSkill::CastMissles
// Purpose		: �����ӵ�����
// Return		: 
// Argumant		: int nLauncher  ������id
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime  �ӳ�ʱ��
// Argumant		: eSkillLauncherType eLauncherType ����������
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::CastMissles(int nLauncher, int nParam1, int nParam2, int nWaitTime  , eSkillLauncherType eLauncherType )  const 
{
	int nRegionId		=	0;
	int	nDesMapX		=	0;//��ͼ����
	int nDesMapY		=	0;
	int nDesOffX		=	0;
	int nDesOffY		=	0;
	int nSrcOffX		=	0;
	int nSrcOffY		=	0;
	int nSrcPX			=	0;//������
	int nSrcPY			=	0;
	int nDesPX			=	0;
	int nDesPY			=	0;
	int nDistance		=	0;
	int nDir			=	0;
	int nDirIndex		=	0;
	int nTargetId		=	-1;
	int nRefPX			=	0;
	int nRefPY			=	0;
	TOrdinSkillParam	SkillParam ;
	SkillParam.eLauncherType = SKILL_SLT_Npc;
	SkillParam.nParent = 0;
	SkillParam.eParentType = (eSkillLauncherType)0;
	SkillParam.nWaitTime = nWaitTime;
	SkillParam.nTargetId = 0;
	if (nLauncher <= 0) return FALSE;
	
	switch(m_eMisslesForm)
	{
	/*
	��ǽʱ����һ���ֲ�����ʾ�ӵ�֮��ĳ��ȼ��
	X2  = X1 + N * SinA
	Y2  = Y2 - N * CosA
	*/
		
	case	SKILL_MF_Wall:			//ǽ��	����ӵ��ʴ�ֱ�������У���ʽ��ǽ״
		{
			//ǽ��ħ��������ֻ������
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{	
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY,  SKILL_SLT_Npc);
					
					if (Npc[nLauncher].m_SubWorldIndex < 0) 
					{
						return FALSE;
					}
					
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					nDir = nDir + MaxMissleDir / 4;
					if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					
					CastWall(&SkillParam , nDir, nDesPX, nDesPY);
				}	break;
			case SKILL_SLT_Obj:
				{
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					
					SubWorld[Missle[nLauncher].m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					int nDir = pMissle->m_nDir + MaxMissleDir / 4;
					if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.nParent = SKILL_SLT_Missle;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastWall(&SkillParam,  nDir, nRefPX, nRefPY);
				}break;
			}
		}break;
		
		
	case	SKILL_MF_Line:				//����	����ӵ���ƽ������ҷ�������
		{
			
			if (nParam1 == SKILL_SPT_Direction)
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						nDir = nParam2;
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						CastLine(&SkillParam, nDir, nSrcPX,nSrcPY);
						
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						nDir = nParam2;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastWall(&SkillParam, nDir,  nRefPX, nRefPY);
					}break;
				}
				
			}
			else
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY,  SKILL_SLT_Npc);
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
						nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						if (m_nChildSkillNum == 1 && (g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Line || g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Parabola) ) 
						{
							if (nSrcPX == nDesPX && nSrcPY == nDesPY)		return FALSE ;
							nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
							
							if (nDistance == 0 ) return FALSE;
							int		nYLength = nDesPY - nSrcPY;
							int		nXLength = nDesPX - nSrcPX;
							int		nSin = (nYLength << 10) / nDistance;	// �Ŵ�1024��
							int		nCos = (nXLength << 10) / nDistance;
							
							if (abs(nSin) > 1024) 
								return FALSE;

							if (abs(nCos) > 1024) 
								return FALSE;
							
							
							CastExtractiveLineMissle(&SkillParam, nDir, nSrcPX, nSrcPY, nCos, nSin, nDesPX, nDesPY);
						}
						else
							CastLine(&SkillParam, nDir, nSrcPX,nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastLine(&SkillParam,  pMissle->m_nDir,  nRefPX, nRefPY);
					}break;
				}
			}
		}
		break;
		
		//  ���ֲ���һ��ʾ�ӵ�֮��ĽǶȲ��64����Ϊ׼
		//  ������X/Y����Ϊ��������
		
	case	SKILL_MF_Spread:				//ɢ��	����ӵ���һ���ĽǶȵķ�ɢ״	
		{
			
			if (nParam1 == SKILL_SPT_Direction)
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						nDir = nParam2;
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						CastSpread(&SkillParam, nDir, nSrcPX,nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (nParam2 > MaxMissleDir || nParam2 < 0) return FALSE;
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						nDir = nParam2;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastSpread(&SkillParam, nDir,  nRefPX, nRefPY);
					}break;
				}
			}
			else
			{
				switch(eLauncherType)
				{
				case SKILL_SLT_Npc:
					{
						nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY, SKILL_SLT_Npc);		
						SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
						nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
						nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
						SkillParam.nLauncher = nLauncher;
						SkillParam.eLauncherType = eLauncherType;
						SkillParam.nTargetId = nTargetId;
						
						if (m_nChildSkillNum == 1 && (g_MisslesLib[m_nChildSkillId].m_eMoveKind == MISSLE_MMK_Line) ) 
						{
							if (nSrcPX == nDesPX && nSrcPY == nDesPY)		return FALSE ;
							nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
							
							if (nDistance == 0 ) return FALSE;
							int		nYLength = nDesPY - nSrcPY;
							int		nXLength = nDesPX - nSrcPX;
							int		nSin = (nYLength << 10) / nDistance;	// �Ŵ�1024��
							int		nCos = (nXLength << 10) / nDistance;

							if (abs(nSin) > 1024) 
								return FALSE;
							
							if (abs(nCos) > 1024) 
								return FALSE;

							CastExtractiveLineMissle(&SkillParam, nDir, nSrcPX, nSrcPY, nCos, nSin, nDesPX, nDesPY);
						}
						else
							CastSpread(&SkillParam, nDir, nSrcPX, nSrcPY);
					}break;
				case SKILL_SLT_Obj:
					{
						
					}break;
				case SKILL_SLT_Missle:
					{
						KMissle * pMissle = &Missle[nLauncher];
						if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
						SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
						SkillParam.nLauncher = pMissle->m_nLauncher;
						SkillParam.nParent = nLauncher;
						SkillParam.eParentType = eLauncherType;
						SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
						CastSpread(&SkillParam ,pMissle->m_nDir,  nRefPX, nRefPY);
					}break;
				}
			}
			
		}break;
		
		
		//�Ե�ǰ��ΪԲ��������Χ�ŵ��ӵ�
		//�ֳ����������һ��Ϊ��ԭ��Ϊԭ�ķ�������һ��Ϊ��Ŀ���Ϊԭ�ķ���
		// ���ֲ���һ��ʾ �Ƿ�Ϊԭ�ط���
		
	case	SKILL_MF_Circle:				//Բ��	����ӵ�Χ��һ��Ȧ
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2,  &nDesPX, &nDesPY, eLauncherType);
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					
					if (m_nValue1 == 0)
						CastCircle(&SkillParam, nDir, nSrcPX, nSrcPY);
					else
						CastCircle(&SkillParam, nDir, nDesPX, nDesPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastCircle(&SkillParam, pMissle->m_nDir,  nRefPX, nRefPY);
				}break;
			}
			
		}break;
		
	case	SKILL_MF_Random:				//���	����ӵ�����ŷ�
		{
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					
				}break;
			}
		}
		break;
		
	case	SKILL_MF_AtTarget:				//����	����ӵ�����
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;	
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2, &nDesPX, &nDesPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					CastZone(&SkillParam, nDir, nDesPX, nDesPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastZone(&SkillParam, pMissle->m_nDir, nRefPX, nRefPY);
				}break;
			}
		}break;
		
	case	SKILL_MF_AtFirer:				//����	����ӵ�ͣ����ҵ�ǰλ��
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					CastZone(&SkillParam,  nDir, nSrcPX, nSrcPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastZone(&SkillParam , pMissle->m_nDir, nRefPX, nRefPY);
				}break;
			}
			
		}break;
		
	case	SKILL_MF_Zone:
		{
			if (nParam1 == SKILL_SPT_Direction) return FALSE;
			
			switch(eLauncherType)
			{
			case SKILL_SLT_Npc:
				{
					nTargetId		= Param2PCoordinate(nLauncher,nParam1, nParam2,  &nDesPX, &nDesPY);
					SubWorld[Npc[nLauncher].m_SubWorldIndex].Map2Mps(Npc[nLauncher].m_RegionIndex, Npc[nLauncher].m_MapX, Npc[nLauncher].m_MapY, Npc[nLauncher].m_OffX, Npc[nLauncher].m_OffY, &nSrcPX, &nSrcPY);
					nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
					nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
					SkillParam.nLauncher = nLauncher;
					SkillParam.eLauncherType = eLauncherType;
					SkillParam.nTargetId = nTargetId;
					CastZone(&SkillParam, nDir, nSrcPX, nSrcPY);
				}break;
			case SKILL_SLT_Obj:
				{
					
				}break;
			case SKILL_SLT_Missle:
				{
					KMissle * pMissle = &Missle[nLauncher];
					if (!Npc[pMissle->m_nLauncher].IsMatch(pMissle->m_dwLauncherId)) return FALSE;
					SubWorld[pMissle->m_nSubWorldId].Map2Mps(pMissle->m_nRegionId, pMissle->m_nCurrentMapX, pMissle->m_nCurrentMapY , pMissle->m_nXOffset, pMissle->m_nYOffset, &nRefPX, &nRefPY);
					SkillParam.nLauncher = pMissle->m_nLauncher;
					SkillParam.nParent = nLauncher;
					SkillParam.eParentType = eLauncherType;
					SkillParam.nTargetId = pMissle->m_nFollowNpcIdx;
					CastZone(&SkillParam, pMissle->m_nDir, nRefPX, nRefPY);
				}break;
			}
		}break;
	}
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::CastZone
// Purpose		: 
// Return		: int 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
//nValue1 = 0 ��ʾ��������  nValue1 = 1 ��ʾԲ������
//nValue2 = 0 
int KSkill::CastZone(TOrdinSkillParam * pSkillParam , int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int nCastMissleNum	= 0;
	int nBeginPX ;
	int nBeginPY ;
	if (m_nChildSkillNum == 1)
	{
		nBeginPX = nRefPX;
		nBeginPY = nRefPY;
	}
	else 
	{
		nBeginPX		= nRefPX - m_nChildSkillNum * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellWidth / 2;
		nBeginPY		= nRefPY - m_nChildSkillNum * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellHeight / 2;
	}
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	for (int i = 0; i < m_nChildSkillNum; i ++)
		for (int j = 0; j < m_nChildSkillNum; j ++)
		{
			if (m_bBaseSkill)
			{
				int nMissleIndex ;
				int nSubWorldId ; 
				
				nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
				
				if (m_nValue1 == 1)
					if ( ((i - m_nChildSkillNum / 2) * (i - m_nChildSkillNum / 2) + (j - m_nChildSkillNum / 2) * (j - m_nChildSkillNum / 2)) > (m_nChildSkillNum * m_nChildSkillNum / 4))			continue;
					
					
					if (nSubWorldId < 0)	goto exit;
					int nDesSubX = nBeginPX + j * SubWorld[nSubWorldId].m_nCellWidth;
					int nDesSubY = nBeginPY +  i * SubWorld[nSubWorldId].m_nCellHeight;
					nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX , nDesSubY);
					
					if (nMissleIndex < 0)	continue;
					
					Missle[nMissleIndex].m_nDir				= nDir;
					Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
					CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
					Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
					Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
					Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
					Missle[nMissleIndex].m_nLauncher		= nLauncher;
					Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
					
					if (pSkillParam->nParent)
						Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
					else 
						Missle[nMissleIndex].m_nParentMissleIndex = 0;
					
					Missle[nMissleIndex].m_nSkillId			= m_nId;
					Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j);
					Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
					Missle[nMissleIndex].m_nRefPX			= nDesSubX;
					Missle[nMissleIndex].m_nRefPY			= nDesSubY;
					
					if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
					{
						Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
						Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
					}
#ifdef _SERVER
					Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
					nCastMissleNum ++;
			}
			else
			{
				_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
				if (pOrdinSkill) 
				{
					if (!pSkillParam->nParent)
						nCastMissleNum += pOrdinSkill->Cast(nLauncher, nBeginPX + j * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellWidth , nBeginPY +  i * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellHeight, pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j ), eLauncherType);
					else 
						nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nLauncher, nBeginPX + j * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellWidth , nBeginPY +  i * SubWorld[Npc[nLauncher].m_SubWorldIndex].m_nCellHeight, pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j ), pSkillParam->eLauncherType);
				}
			}
			
		}
exit:	
#ifdef _SERVER
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::CastLine
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
// Value1 �ӵ�֮��ļ��
// Value2 
int		KSkill::CastLine(TOrdinSkillParam *pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	//�ӵ�֮��ļ��
	int nMSDistanceEach = m_nValue1;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//�ֱ����ɶ����ӵ�
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		nDesSubX	= nRefPX + ((nMSDistanceEach * (i + 1) * g_DirCos(nDirIndex, MaxMissleDir) )>>10);
		nDesSubY	= nRefPY + ((nMSDistanceEach * (i + 1) * g_DirSin(nDirIndex, MaxMissleDir) )>>10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType);
				
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		return nCastMissleNum;
}

int		KSkill::CastExtractiveLineMissle(TOrdinSkillParam* pSkillParam,  int nDir,int nSrcX, int nSrcY, int nXOffset, int nYOffset, int nDesX, int nDesY)  const 
{
	
	_ASSERT(pSkillParam);
	
	int nLauncher = pSkillParam->nLauncher;
	if (pSkillParam->eLauncherType != SKILL_SLT_Npc) return 0;	
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	//�ӵ�֮��ļ��
	
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//�ֱ����ɶ����ӵ�
	{
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nSrcX, nSrcY);
			
			if (nMissleIndex < 0)	goto exit;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				int nLength = g_GetDistance(nSrcX, nSrcY, nDesX, nDesY);
				int nTime = nLength / Missle[nMissleIndex].m_nSpeed;
				Missle[nMissleIndex].m_nHeightSpeed	= Missle[nMissleIndex].m_nZAcceleration * (nTime - 1) / 2;
				
			}
			
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
		
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(0);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			Missle[nMissleIndex].m_nRefPX			= nSrcX;
			Missle[nMissleIndex].m_nRefPY			= nSrcY;

			int nTempR = 0;
			int nTempMapX = 0;
			int nTempMapY = 0;
			int nTempOffsetX = 0;
			int nTempOffsetY = 0;

			Missle[nMissleIndex].m_bNeedReclaim = TRUE;
			int nLength = g_GetDistance(nSrcX, nSrcY, nDesX, nDesY);
			Missle[nMissleIndex].m_nFirstReclaimTime = nLength / Missle[nMissleIndex].m_nSpeed + Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nEndReclaimTime = Missle[nMissleIndex].m_nFirstReclaimTime + SubWorld[nSubWorldId].m_nCellWidth / Missle[nMissleIndex].m_nSpeed + 2;

			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				
				Missle[nMissleIndex].m_nXFactor = nXOffset;
				Missle[nMissleIndex].m_nYFactor = nYOffset;
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
		}
		else
		{
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(0), pSkillParam->eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(0), pSkillParam->eParentType);
				
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		
		return nCastMissleNum;
		
}

/*!*****************************************************************************
// Function		: KSkill::CastWall
// Purpose		: Wall Magic 
// Return		: int 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
/*
m_nValue1 ��ʾ�ӵ�֮��ľ��룬��λ���ص�
*/
int KSkill::CastWall(TOrdinSkillParam * pSkillParam,  int nDir , int nRefPX , int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int	nDirIndex		= g_Dir2DirIndex(nDir, MaxMissleDir);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	
	//�ӵ�֮��ļ��
	int nMSDistanceEach = m_nValue1;
	int nCurMSDistance	= -1 * nMSDistanceEach * m_nChildSkillNum / 2;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//�ֱ����ɶ����ӵ�
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		nDesSubX	= nRefPX + ((nCurMSDistance * g_DirCos(nDirIndex, MaxMissleDir)) >>10);
		nDesSubY	= nRefPY + ((nCurMSDistance * g_DirSin(nDirIndex, MaxMissleDir)) >>10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	
			{
				goto exit;
			}
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			if (nMissleIndex < 0)	
			{
				continue;
			}

			if (m_nValue2)
			{
				int nDirTemp = nDir - MaxMissleDir / 4;
				if (nDirTemp < 0) nDirTemp += MaxMissleDir;
				Missle[nMissleIndex].m_nDir				= nDirTemp;
				Missle[nMissleIndex].m_nDirIndex = g_Dir2DirIndex(nDirTemp, 64);

			}
			else
			{
				Missle[nMissleIndex].m_nDir				= nDir;
				Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			}
			
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			
			if (pSkillParam->nTargetId)
				Missle[nMissleIndex].m_dwFollowNpcID	= Npc[pSkillParam->nTargetId].m_dwID;
			
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line|| Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(Missle[nMissleIndex].m_nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(Missle[nMissleIndex].m_nDir, MaxMissleDir);
			}
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubX, nDesSubY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubX, nDesSubY, pSkillParam->nWaitTime +  GetMissleGenerateTime(i), pSkillParam->eParentType);
			}
		}
		
		nCurMSDistance += nMSDistanceEach;
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::CastCircle
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType  eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
// Value1  == 0 ��ʾ������ΪԲ�Ĳ���Բ��������Ŀ���ΪԲ�Ĳ���Բ
int		KSkill::CastCircle(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType  eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;	
	int nDesSubPX	= 0;
	int nDesSubPY	= 0;
	int nFirstStep	= m_nValue2;			//��һ���ĳ��ȣ��ӵ��ڸշ���ȥʱ����ҵľ���
	int nCurSubDir	= 0;
	int nDirPerNum  = 	MaxMissleDir / m_nChildSkillNum  ;
	int nCastMissleNum = 0;
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//�ֱ����ɶ���ӵ�
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		int nCurSubDir	= nDir + nDirPerNum * i ;
		
		if (nCurSubDir < 0)
			nCurSubDir = MaxMissleDir + nCurSubDir;
		
		if (nCurSubDir >= MaxMissleDir)
			nCurSubDir -= MaxMissleDir;
		
		int nSinAB	= g_DirSin(nCurSubDir, MaxMissleDir);
		int nCosAB	= g_DirCos(nCurSubDir, MaxMissleDir);
		
		nDesSubPX	= nRefPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubPY	= nRefPY + ((nSinAB * nFirstStep) >> 10);
		
		
		
		if (nDesSubPX < 0 || nDesSubPY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId ; 
			
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubPX, nDesSubPY);
			
			if (nMissleIndex < 0)	
			{
				continue;
			}
			
			Missle[nMissleIndex].m_nDir			= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex	= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			
			Missle[nMissleIndex].m_nFollowNpcIdx	= pSkillParam->nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nRefPX			= nDesSubPX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubPY;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_RollBack)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nCurSubDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nCurSubDir, MaxMissleDir);
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
			nCastMissleNum ++;
			
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum += pOrdinSkill->Cast(nLauncher, nDesSubPX, nDesSubPY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum += pOrdinSkill->Cast(pSkillParam->nParent, nDesSubPX, nDesSubPY, pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType);
			}
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::CastSpread
// Purpose		: 
// Return		: 
// Argumant		: int nLauncher
// Argumant		: eSkillLauncherType eLauncherType
// Argumant		: int nDir
// Argumant		: int nRefPX
// Argumant		: int nRefPY
// Argumant		: int nWaitTime
// Argumant		: int nTargetId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
/*
Value1 ÿ���ӵ����ĽǶȵ�λ
Value2 ÿһ���ĳ��ȣ���һ���ĳ��ȣ��ӵ��ڸշ���ȥʱ����ҵľ���
*/
int		KSkill::CastSpread(TOrdinSkillParam * pSkillParam, int nDir, int nRefPX, int nRefPY)  const 
{
	int nLauncher = pSkillParam->nLauncher;
	eSkillLauncherType eLauncherType = pSkillParam->eLauncherType;
	if (eLauncherType != SKILL_SLT_Npc) return 0;
	int nDesSubMapX		= 0;
	int nDesSubMapY		= 0;
	int nFirstStep		= m_nValue2;			//��һ���ĳ��ȣ��ӵ��ڸշ���ȥʱ����ҵľ���
	int nCurMSRadius	= m_nChildSkillNum / 2 ; 
	int nCurSubDir		= 0;
	int	nCastMissleNum  = 0;			//ʵ�ʷ��͵�Missle������
	
	// Sin A+B = SinA*CosB + CosA*SinB
	// Cos A+B = CosA*CosB - SinA*SinB
	// Sin A = nYFactor
	// Cos A = nXFactor
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif
	
	int nDesSubX = 0;
	int nDesSubY = 0;
	int nXFactor = 0;
	int nYFactor = 0;
	
	if (pSkillParam->nTargetId > 0)
	{
		int nTargetId = pSkillParam->nTargetId;
		int nDistance = 0;
		int nDesX, nDesY;
		if (Npc[nTargetId].m_Index > 0 && Npc[nTargetId].m_SubWorldIndex >= 0) 
			SubWorld[Npc[nTargetId].m_SubWorldIndex].Map2Mps(Npc[nTargetId].m_RegionIndex, Npc[nTargetId].m_MapX, Npc[nTargetId].m_MapY, Npc[nTargetId].m_OffX, Npc[nTargetId].m_OffY, &nDesX, &nDesY);
		// Fixed By MrChuCong@gmail.com
		nDistance = (int)sqrt(double(nDesX - nRefPX)*(nDesX - nRefPX) +	(nDesY - nRefPY)*(nDesY - nRefPY));
		
		nXFactor = ((nDesX - nRefPX)<<10) / nDistance;
		nYFactor = ((nDesY - nRefPY)<<10) / nDistance;
		
		nDesSubX = nRefPX + ((nXFactor * nFirstStep)>>10);
		nDesSubY = nRefPY + ((nYFactor * nFirstStep)>>10);
		
		if (nDesSubX < 0  || nDesSubY < 0 ) return 0;
	}
	
	int nTargetId = pSkillParam->nTargetId;
	
	//�ֱ����ɶ���ӵ�
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		int nDSubDir	= m_nValue1 * nCurMSRadius; 
		nCurSubDir		= nDir - m_nValue1 * nCurMSRadius;
		
		
		if (nCurSubDir < 0)
			nCurSubDir = MaxMissleDir + nCurSubDir;
		
		if (nCurSubDir >= MaxMissleDir)
			nCurSubDir -= MaxMissleDir;
		
		int nSinAB	;
		int nCosAB	;
		
		if (nTargetId > 0)
		{
			nDSubDir	+= 48;
			if (nDSubDir >= MaxMissleDir)
				nDSubDir -= MaxMissleDir;
			//sin(a - b) = sinacosb - cosa*sinb
			//cos(a - b) = cosacoab + sinasinb
			nSinAB = (nYFactor * g_DirCos(nDSubDir, MaxMissleDir) - nXFactor * g_DirSin(nDSubDir, MaxMissleDir)) >> 10;
			nCosAB = (nXFactor * g_DirCos(nDSubDir, MaxMissleDir) + nYFactor * g_DirSin(nDSubDir , MaxMissleDir)) >> 10;
		}
		else
		{
			nSinAB = g_DirSin(nCurSubDir, MaxMissleDir);
			nCosAB = g_DirCos(nCurSubDir, MaxMissleDir);
		}
		
		nDesSubX	= nRefPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubY	= nRefPY + ((nSinAB * nFirstStep) >> 10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			
			int nMissleIndex ;
			int nSubWorldId ; 
			nSubWorldId = Npc[nLauncher].m_SubWorldIndex;
			
			if (nSubWorldId < 0)	goto exit;
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(nLauncher, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_nFollowNpcIdx	= nTargetId;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nLauncher		= nLauncher;
			Missle[nMissleIndex].m_dwLauncherId		= Npc[nLauncher].m_dwID;
			
			if (pSkillParam->nParent)
				Missle[nMissleIndex].m_nParentMissleIndex = pSkillParam->nParent;
			else 
				Missle[nMissleIndex].m_nParentMissleIndex = 0;
			
			Missle[nMissleIndex].m_nSkillId			= m_nId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nXFactor			= nCosAB;
			Missle[nMissleIndex].m_nYFactor			= nSinAB;
			Missle[nMissleIndex].m_nRefPX			= nDesSubX;
			Missle[nMissleIndex].m_nRefPY			= nDesSubY;
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			nCastMissleNum ++;
		}
		else
		{
			_ASSERT(m_nChildSkillId > 0 && m_nChildSkillLevel > 0)	;
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(m_nChildSkillId, m_nChildSkillLevel);
			if (pOrdinSkill) 
			{
				if (!pSkillParam->nParent)
					nCastMissleNum +=  pOrdinSkill->Cast(nLauncher,  nRefPX, nRefPY , pSkillParam->nWaitTime + GetMissleGenerateTime(i), eLauncherType);
				else
					nCastMissleNum +=  pOrdinSkill->Cast(pSkillParam->nParent,  nRefPX, nRefPY , pSkillParam->nWaitTime + GetMissleGenerateTime(i), pSkillParam->eParentType); 
			}
		}
		
		nCurMSRadius -- ;
	}
exit:	
#ifdef _SERVER
	if (pNewMagicAttribsData)
		if (pNewMagicAttribsData->GetRef() == 0)
			delete pNewMagicAttribsData;
#endif
		
		return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KSkill::GetChildSkillNum
// Purpose		: ���ڿ���ĳЩ�����У����ż��ܵȼ����������ӵ�����ĿҲ��������ӣ�����ͨ���ú������ʵ�ʵ��Ӽ�����Ŀ
// Return		: 
// Argumant		: int nLevel
// Comments		:
// Author		: RomanDou
*****************************************************************************/
int 	KSkill::GetChildSkillNum(int nLevel)  const 
{
	return m_nChildSkillNum;
};
/*!*****************************************************************************
// Function		: KSkill::CreateMissle
// Purpose		: �����ӵ��Ļ������ݣ��Լ��ü��ܸõȼ��µĶ��ӵ���Ϣ�ı䶯����
//					����������ֵ�����ָ��
// Return		: 
// Argumant		: int nChildSkillId
// Argumant		: int nMissleIndex
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KSkill::CreateMissle(int nLauncher, int nChildSkillId, int nMissleIndex)  const 
{
	_ASSERT(nChildSkillId > 0 && nChildSkillId < MAX_MISSLESTYLE && nMissleIndex > 0);
	
	if (nLauncher <= 0) 
	{
		return ;
	}
	
	KMissle * pMissle = &Missle[nMissleIndex];
	
	g_MisslesLib[nChildSkillId] = *pMissle;//���ƿ�������
	
	pMissle->m_nLevel			= m_ulLevel;
	pMissle->m_bCollideEvent	= m_bCollideEvent;
	pMissle->m_bVanishedEvent   = m_bVanishedEvent;
	pMissle->m_bStartEvent		= m_bStartEvent;
	pMissle->m_bFlyEvent		= m_bFlyingEvent;
	pMissle->m_nFlyEventTime	= m_nFlyEventTime;
	pMissle->m_nMissleId		= nMissleIndex;
	pMissle->m_bClientSend      = m_bClientSend;
	pMissle->m_bMustBeHit		= m_bMustBeHit;
	pMissle->m_bIsMelee			= m_bIsMelee;
	pMissle->m_bByMissle		= m_bByMissle;
	pMissle->m_bTargetSelf		= (m_bTargetSelf == 1);
	pMissle->m_nInteruptTypeWhenMove = m_nInteruptTypeWhenMove;
	pMissle->m_bHeelAtParent	= m_bHeelAtParent;
	pMissle->m_bUseAttackRating	= m_bUseAttackRate;
	pMissle->m_bDoHurt			= m_bDoHurt;
	
	if (pMissle->m_nInteruptTypeWhenMove)
	{
		Npc[nLauncher].GetMpsPos(&pMissle->m_nLauncherSrcPX, &pMissle->m_nLauncherSrcPY);
	}
	pMissle->m_eRelation = m_eRelation;

	
#ifndef _SERVER
	pMissle->m_MissleRes.m_bNeedShadow   = m_bNeedShadow;
	pMissle->m_MissleRes.m_nMaxShadowNum = m_nMaxShadowNum;
	pMissle->m_MissleRes.m_nMissleId	 = nMissleIndex;
	if (!pMissle->m_MissleRes.Init()) g_DebugLog("�����ӵ���ͼʧ�ܣ�����%s", __FILE__) ;
#endif
	
	pMissle->DoWait();
	
	for (int i = 0  ; i < m_nMissleAttribsNum; i ++)
	{
		switch (m_MissleAttribs[i].nAttribType)
		{
		case magic_missle_movekind_v:
			{
				pMissle->m_eMoveKind	= (eMissleMoveKind) m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_speed_v:	
			{
				pMissle->m_nSpeed		= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_lifetime_v:
			{
				pMissle->m_nLifeTime	= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_height_v:	
			{
				pMissle->m_nHeight		= m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_damagerange_v:
			{
				pMissle->m_nDamageRange = m_MissleAttribs[i].nValue[0];
			}break;
			
		case magic_missle_radius_v:	
			{
			}break;
		}
	}
	
	if (m_bIsMelee)
		pMissle->m_nLifeTime = Npc[nLauncher].ModifyMissleLifeTime(pMissle->m_nLifeTime);
	else
	{
		pMissle->m_nSpeed = Npc[nLauncher].ModifyMissleSpeed(pMissle->m_nSpeed);
		pMissle->m_bCollideVanish = Npc[nLauncher].ModifyMissleCollsion(pMissle->m_bCollideVanish);
	}
	
}


/*!*****************************************************************************
// Function		: KSkill::GetInfoFromTabFile
// Purpose		: ��TabFile�л�øü��ܵĳ�������
// Return		: 
// Argumant		: int nCol
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::GetInfoFromTabFile(int nRow)
{
	KITabFile * pITabFile = &g_OrdinSkillsSetting;
	return GetInfoFromTabFile(&g_OrdinSkillsSetting, nRow);
}


BOOL	KSkill::GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow)
{
	if (!pSkillsSettingFile || nRow < 0) return FALSE;
	//	
	pSkillsSettingFile->GetString(nRow, "SkillName",		"", m_szName, sizeof(m_szName) ,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillId",			0, (int *)&m_nId,TRUE);
	
	int nReqLevel = 0;
	pSkillsSettingFile->GetInteger(nRow, "ReqLevel",		0, (int *)&nReqLevel, TRUE);
	m_usReqLevel = (unsigned short)nReqLevel;


	pSkillsSettingFile->GetInteger(nRow, "EqtLimit",		-2, (int *)&m_nEquiptLimited, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "HorseLimit",		0, (int *)&m_nHorseLimited, TRUE);

	pSkillsSettingFile->GetInteger(nRow, "DoHurt",			1, (int *)&m_bDoHurt);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillNum",	0, &m_nChildSkillNum,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MisslesForm",		0, (int *)&m_eMisslesForm, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharClass",		0, &m_nCharClass, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillStyle",		0, (int *)&m_eSkillStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharAnimId",		0, (int *)&m_nCharActionId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsPhysical",		0, &m_bIsPhysical, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsAura",			0, &m_bIsAura, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsUseAR",			0, &m_bUseAttackRate, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetOnly",		0, &m_bTargetOnly, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetEnemy",		0, &m_bTargetEnemy, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetAlly",		0, &m_bTargetAlly, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetObj",		0, &m_bTargetObj, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "BaseSkill",		0, &m_bBaseSkill, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ByMissle",		0, &m_bByMissle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillId",	0, &m_nChildSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEvent",		0, &m_bFlyingEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartEvent",		0, &m_bStartEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollideEvent",	0, &m_bCollideEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedEvent",	0, &m_bVanishedEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlySkillId",		0, &m_nFlySkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartSkillId",	0, &m_nStartSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedSkillId",	0, &m_nVanishedSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollidSkillId",	0, &m_nCollideSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "SkillCostType",	0, (int *)&m_nSkillCostType, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CostValue",		0, &m_nCost, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TimePerCast",		0, &m_nMinTimePerCast, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param1",			0, &m_nValue1, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param2",			0, &m_nValue2, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillLevel", 0, &m_nChildSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "EventSkillLevel", 0, &m_nEventSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsMelee",			0, &m_bIsMelee, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEventTime",	0, &m_nFlyEventTime, TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerate",	0, (int *)&m_eMisslesGenerateStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerateData",0, &m_nMisslesGenerateData, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MaxShadowNum",	0, &m_nMaxShadowNum, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "AttackRadius",	50, &m_nAttackRadius, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "WaitTime",		0, &m_nWaitTime, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ClientSend",		0, &m_bClientSend, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "TargetSelf",		0, &m_bTargetSelf, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StopWhenMove",	0, &m_nInteruptTypeWhenMove, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "HeelAtParent",  0, (int *)&m_bHeelAtParent, TRUE );
	
	//����������Ҫ��ò�֪ͨ�ͻ���
	pSkillsSettingFile->GetInteger(nRow, "StateSpecialId",  0, &m_nStateSpecialId, TRUE);

	m_eRelation = 0;
	if (m_bTargetEnemy)
		m_eRelation |= relation_enemy;
	
	if (m_bTargetAlly)
		m_eRelation |= relation_ally;
	
	if (m_bTargetSelf)
		m_eRelation |= relation_self;
	
#ifndef _SERVER
	pSkillsSettingFile->GetString(nRow, "SkillDesc", "", m_szSkillDesc, 100);
	pSkillsSettingFile->GetInteger(nRow, "NeedShadow",		0, &m_bNeedShadow, TRUE);
	pSkillsSettingFile->GetString(nRow, "SkillIcon","\\spr\\skill\\ͼ��\\ͨ��.spr",	m_szSkillIcon, 80);
	if (!m_szSkillIcon[0])	strcpy(m_szSkillIcon, "\\spr\\skill\\ͼ��\\ͨ��.spr");
	pSkillsSettingFile->GetInteger(nRow, "LRSkill",		0, (int*)&m_eLRSkillInfo);
	pSkillsSettingFile->GetString(nRow, "PreCastSpr", "", m_szPreCastEffectFile, 100);
	pSkillsSettingFile->GetString(nRow, "ManCastSnd","", m_szManPreCastSoundFile, 100);
	pSkillsSettingFile->GetString(nRow, "FMCastSnd", "", m_szFMPreCastSoundFile, 100);
#else
	char szLevelScript[MAX_PATH];
	
	//��ȡ�趨�ű�1����¼ÿһ�����ܵ����Ա仯����ֵ�趨��
	pSkillsSettingFile->GetString(nRow, "LvlSetScript", "", szLevelScript, MAX_PATH);
	if (szLevelScript[0])
	{
		strlwr(szLevelScript);
		m_dwSkillLevelDataScriptId = g_FileName2Id(szLevelScript);
	}
	
	//��ȡ�趨�ű�2����¼����������Ϣ
	pSkillsSettingFile->GetString(nRow, "LevelUpScript","",szLevelScript, MAX_PATH);
	if (szLevelScript[0])
	{
		strlwr(szLevelScript);
		m_dwSkillLevelUpScriptID = g_FileName2Id(szLevelScript);
	}
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::LoadSkillLevelData
// Purpose		: �����õ�ǰ�ȼ��µ�ǰ���ܵļ��ܡ��ӵ�����ײ��ֵӰ��
// Return		: 
// Argumant		: int nLevel
// Comments		:
// Author		: Romandou
****************************************************************************/
void		KSkill::LoadSkillLevelData(unsigned long  nLevel /* =0*/, int nParam)
{
	m_nMissleAttribsNum = 0;
	m_nDamageAttribsNum = 0;
	m_nImmediateAttribsNum = 0;
	m_nStateAttribsNum	= 0;		//���������10
	
	char szSettingScriptName[MAX_PATH];
	char szSettingNameValue[100];
	char szSettingDataValue[100];
	char szResult[300];
	int nRowId = nParam;
	if (nRowId < 2) return ;
	//Question ����˳��һ������
	KLuaScript * pScript = NULL;
#ifndef _SERVER
	g_OrdinSkillsSetting.GetString(nRowId,  "LvlSetScript", "", szSettingScriptName, MAX_PATH );
	if (!szSettingScriptName[0]) return;
	g_SetFilePath("\\");
	//����Lua�ű�
	KLuaScript Script;
	Script.Init();
	if (!Script.Load(szSettingScriptName)) 
	{
		g_DebugLog("�޷���ȡ�����趨�ű��ļ�%s����ȷ���Ƿ��ļ����ڻ�ű��﷨����", szSettingScriptName);
		return;
	}
	pScript  = &Script;
#else
	if (!m_dwSkillLevelDataScriptId) 
	{
		printf("���������޷���ü���[%s]���趨�ű��������Ƿ���ڻ��﷨����\n", GetSkillName());
		return ;
	}
	
	pScript = (KLuaScript*)g_GetScript(m_dwSkillLevelDataScriptId);
	
	if (!pScript)
	{
		printf("���������޷���ü���[%s]���趨�ű��������Ƿ���ڻ��﷨����\n", GetSkillName());
		return ;
	}
#endif
	
	
	int nSafeIndex = 1;
	pScript->SafeCallBegin(&nSafeIndex);
	
	for(int i = 0 ;  i  < MAXSKILLLEVELSETTINGNUM ; i ++)
	{
		char szSettingName[40];
		char szSettingData[40];
		sprintf(szSettingName, "LvlSetting%d", i + 1);
		sprintf(szSettingData, "LvlData%d", i + 1);
		
		g_OrdinSkillsSetting.GetString(nRowId, szSettingName, "", szSettingNameValue, 100);
		g_OrdinSkillsSetting.GetString(nRowId, szSettingData, "", szSettingDataValue, 100);
		
		if (szSettingNameValue[0] == 0 	|| szSettingDataValue[0] == '0'	)
		{
			continue;
		}
		
		pScript->CallFunction("GetSkillLevelData", 1, "ssd", szSettingNameValue, szSettingDataValue, nLevel);
		const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
		if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
		{
			int nResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			sprintf(szResult, "%d", nResult);
		}
		else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
		{
			strcpy(szResult , (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)));
		}
		else
		{
			char szMsg[300];
			sprintf(szMsg, "����øü��ܵȼ�Ϊ%d��%s,%s��ʱ����������ű�!,",nLevel, szSettingNameValue, szSettingDataValue);
			g_DebugLog(szMsg);
			break;
		}
		ParseString2MagicAttrib(nLevel, szSettingNameValue, szResult);
		
	}
	pScript->SafeCallEnd(nSafeIndex);
}

#ifdef _SERVER
//When nLauncher == 0 , means neednt  AppendSkillEffect;
KMissleMagicAttribsData* KSkill::CreateMissleMagicAttribsData(int nLauncher)  const 
{
	if (nLauncher < 0 || m_bClientSend) return NULL; 
	
	KMissleMagicAttribsData* pMissleMagicAttribsData = new KMissleMagicAttribsData;
	
	pMissleMagicAttribsData->m_pStateMagicAttribs = (KMagicAttrib *)m_StateAttribs;
	pMissleMagicAttribsData->m_nStateMagicAttribsNum = m_nStateAttribsNum;
	
	pMissleMagicAttribsData->m_pImmediateAttribs = (KMagicAttrib *)m_ImmediateAttribs;
	pMissleMagicAttribsData->m_nImmediateMagicAttribsNum = m_nImmediateAttribsNum;
	
	KMagicAttrib * pDamageAttribs =  new KMagicAttrib[MAX_MISSLE_DAMAGEATTRIB];
	pMissleMagicAttribsData->m_nDamageMagicAttribsNum = m_nDamageAttribsNum;
	
	//������ҵĻ������ԣ�ȷ���ӵ����˺�
	if (nLauncher)
	{
		Npc[nLauncher].AppendSkillEffect(m_bIsPhysical, m_bIsMelee, (KMagicAttrib *)m_DamageAttribs, pDamageAttribs);
	}
	else
	{
		memcpy(pDamageAttribs, (KMagicAttrib *)m_DamageAttribs, sizeof(m_DamageAttribs));
	}
	
	pMissleMagicAttribsData->m_pDamageMagicAttribs = pDamageAttribs;
	return pMissleMagicAttribsData;
}
#endif
/*!*****************************************************************************
// Function		: KSkill::SetMissleGenerateTime
// Purpose		: ��õ�ǰ���ӵ���ʵ�ʲ���ʱ��
// Return		: void 
// Argumant		: Missle * pMissle
// Argumant		: int nNo
// Comments		:
// Author		: RomanDou
*****************************************************************************/
unsigned int KSkill::GetMissleGenerateTime(int nNo) const 
{
	
	switch(m_eMisslesGenerateStyle)
	{
	case SKILL_MGS_NULL:
		{
			return m_nWaitTime;
		}break;
		
	case SKILL_MGS_SAMETIME:
		{
			return  m_nWaitTime + m_nMisslesGenerateData;
		}break;
		
	case SKILL_MGS_ORDER:		
		{
			return  m_nWaitTime + nNo * m_nMisslesGenerateData;
		}break;
		
	case SKILL_MGS_RANDONORDER:	
		{
			if (g_Random(2) == 1) 
				return m_nWaitTime + nNo * m_nMisslesGenerateData + g_Random(m_nMisslesGenerateData);
			else 
				return m_nWaitTime + nNo * m_nMisslesGenerateData  - g_Random(m_nMisslesGenerateData / 2);
		}break;
		
	case SKILL_MGS_RANDONSAME:	
		{
			return  m_nWaitTime + g_Random(m_nMisslesGenerateData);
		}break;
		
	case SKILL_MGS_CENTEREXTENDLINE:
		{
			if (m_nChildSkillNum <= 1) return m_nWaitTime;
			int nCenter = m_nChildSkillNum / 2	;
			return m_nWaitTime + abs(nNo - nCenter) * m_nMisslesGenerateData ;
		}
	}
	return m_nWaitTime;
}

int KSkill::GetSkillIdFromName(char * szSkillName)  
{
	//	
	if (!szSkillName || !szSkillName[0]) 
        return -1;
	
	for (int i = 0; i < MAX_SKILL; i ++)
	{
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(i, 1);
		if (pOrdinSkill) 
		{
			if (!strcmp(pOrdinSkill->m_szName, szSkillName))
            {
                return i;
            }
		}
	}
	return -1;
	
}


/*!*****************************************************************************
// Function		: KSkill::CastInitiativeSkill
// Purpose		: ������������
// Return		: BOOL 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL KSkill::CastInitiativeSkill(int nLauncher, int nParam1, int nParam2, int nWaitTime)  const 
{
	
#ifdef _SERVER
	//�����ѵ�������������
	if (nParam1 != -1 && m_bTargetSelf) 
	{
		nParam1 = -1;
		nParam2 = nLauncher;
	}
	else
	{
		if (nParam1 != -1 || nParam2 <= 0 || nParam2 >= MAX_NPC) return FALSE;
		
		NPC_RELATION  Relation = NpcSet.GetRelation(nLauncher, nParam2);
		
		if (m_bTargetEnemy)
		{
			if (Relation & relation_enemy) 
				goto lab_processdamage;
		}
		
		if (m_bTargetAlly)
		{
			if (Relation & relation_ally) 
				goto lab_processdamage;
		}
		
		if (m_bTargetSelf)
		{
			if (Relation & relation_self) 
				goto lab_processdamage;
		}
		return FALSE;
		
	}
	
lab_processdamage:			
	
	KMissleMagicAttribsData * pAttribsData = CreateMissleMagicAttribsData(nLauncher);
	if (pAttribsData) 
	{
		if (Npc[nParam2].ReceiveDamage(nLauncher, m_bIsMelee, pAttribsData->m_pDamageMagicAttribs, m_bUseAttackRate, m_bDoHurt))
		{		
			if (pAttribsData->m_nStateMagicAttribsNum > 0)
				Npc[nParam2].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, pAttribsData->m_pStateMagicAttribs, pAttribsData->m_nStateMagicAttribsNum, pAttribsData->m_pStateMagicAttribs[0].nValue[1]);
			
			if (pAttribsData->m_nImmediateMagicAttribsNum > 0)
				Npc[nParam2].SetImmediatelySkillEffect(nLauncher, pAttribsData->m_pImmediateAttribs, pAttribsData->m_nImmediateMagicAttribsNum);
		}
		return TRUE;
	}
	if (pAttribsData->DelRef() == 0)
		delete pAttribsData;
#endif //_SERVER
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::CastPassivitySkill
// Purpose		: ����
// Return		: BOOL 
// Argumant		: int nLauncher
// Argumant		: int nParam1
// Argumant		: int nParam2
// Argumant		: int nWaitTime
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL KSkill::CastPassivitySkill(int nLauncher, int nParam1, int nParam2, int nWaitTime)  const 
{
#ifdef _SERVER
	//�Ǳ�������ʱ���Ƿ���Ҫ����MissleMagicAttribs?
	KMissleMagicAttribsData * pAttribsData = (KMissleMagicAttribsData*)m_StateAttribs;//CreateMissleMagicAttribsData(nLauncher);
	if (m_nStateAttribsNum > 0)
	{
		Npc[nLauncher].SetStateSkillEffect(nLauncher, m_nId, m_ulLevel, (KMagicAttrib *)m_StateAttribs, m_nStateAttribsNum, -1);
	}
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KSkill::ParseString2MagicAttrib
// Purpose		: ����ͨ���ű������õļ�������
// Return		: 
// Argumant		: char * szMagicAttribName
// Argumant		: char * szValue
// Comments		:
// Author		: RomanDou
*****************************************************************************/
BOOL	KSkill::ParseString2MagicAttrib(unsigned long ulLevel, char * szMagicAttribName, char * szValue)  
{
	int nValue1 = 0;
	int nValue2 = 0;
	int nValue3 = 0;
    const char *pcszTemp = NULL;
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return FALSE;
	//nValue2 ��ֵΪ-1ʱΪ������״̬��0Ϊ��״̬������ֵΪ��ʱЧ��״̬ħ��Ч��
	//��Ҫ��״̬�������״̬���ݷ��������������Ӧ�������ڣ�����¼������
	
	for (int i  = 0 ; i <= magic_normal_end; i ++)
	{
		if (!strcmp(szMagicAttribName, MagicAttrib2String(i)))
		{
            pcszTemp = szValue;
            nValue1 = KSG_StringGetInt(&pcszTemp, 0);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            nValue2 = KSG_StringGetInt(&pcszTemp, 0);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            nValue3 = KSG_StringGetInt(&pcszTemp, 0);
			//sscanf(szValue, "%d,%d,%d", &nValue1, &nValue2, &nValue3);


			if (i > magic_missle_begin && i < magic_missle_end)
			{
				m_MissleAttribs[m_nMissleAttribsNum].nAttribType = i;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[0] = nValue1;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[1] = nValue2;
				m_MissleAttribs[m_nMissleAttribsNum].nValue[2] = nValue3;
				m_nMissleAttribsNum ++;
				return TRUE;
			}
			if (i > magic_skill_begin && i < magic_skill_end)
			{
				switch(i)
				{
				case magic_skill_cost_v:				// ����MANA
					{
						m_nCost = nValue1;
					}
					break;
					
				case magic_skill_costtype_v:
					{
						m_nSkillCostType = (NPCATTRIB)nValue1;
					}break;
					
				case magic_skill_mintimepercast_v: 		// ÿ�η�ħ���ļ��ʱ��
					{
						m_nMinTimePerCast = nValue1;
					}break;
					
				case magic_skill_misslenum_v:
					{
						m_nChildSkillNum = nValue1;
					}break;
					
				case magic_skill_misslesform_v:
					{
						m_eMisslesForm = (eMisslesForm) nValue1;
					}break;
				case magic_skill_param1_v:
					{
						m_nValue1 = nValue1;
					}
					break;
				case magic_skill_param2_v:	
					{
						m_nValue2 = nValue2;
					}break;
				case magic_skill_eventskilllevel:
					{
						m_nEventSkillLevel = nValue1;
					}
				}
				return TRUE;
			}
			
			if (i > magic_damage_begin && i < magic_damage_end)
			{
				switch(i)
				{
				case magic_attackrating_v:
				case magic_attackrating_p:
					m_DamageAttribs[0].nAttribType = i;
					m_DamageAttribs[0].nValue[0] = nValue1;
					m_DamageAttribs[0].nValue[1] = nValue2;
					m_DamageAttribs[0].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_ignoredefense_p:
					m_DamageAttribs[1].nAttribType = i;
					m_DamageAttribs[1].nValue[0] = nValue1;
					m_DamageAttribs[1].nValue[1] = nValue2;
					m_DamageAttribs[1].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_physicsdamage_v:
				case magic_physicsenhance_p:
					m_DamageAttribs[2].nAttribType = i;
					m_DamageAttribs[2].nValue[0] = nValue1;
					m_DamageAttribs[2].nValue[1] = nValue2;
					m_DamageAttribs[2].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_colddamage_v:
				case magic_coldenhance_p:
					m_DamageAttribs[3].nAttribType = i;
					m_DamageAttribs[3].nValue[0] = nValue1;
					m_DamageAttribs[3].nValue[1] = nValue2;
					m_DamageAttribs[3].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_firedamage_v:
				case magic_fireenhance_p:
					m_DamageAttribs[4].nAttribType = i;
					m_DamageAttribs[4].nValue[0] = nValue1;
					m_DamageAttribs[4].nValue[1] = nValue2;
					m_DamageAttribs[4].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_lightingdamage_v:
				case magic_lightingenhance_p:
					m_DamageAttribs[5].nAttribType = i;
					m_DamageAttribs[5].nValue[0] = nValue1;
					m_DamageAttribs[5].nValue[1] = nValue2;
					m_DamageAttribs[5].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_poisondamage_v:
				case magic_poisonenhance_p:
					m_DamageAttribs[6].nAttribType = i;
					m_DamageAttribs[6].nValue[0] = nValue1;
					m_DamageAttribs[6].nValue[1] = nValue2;
					m_DamageAttribs[6].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				case magic_magicdamage_v:
				case magic_magicenhance_p:
					m_DamageAttribs[7].nAttribType = i;
					m_DamageAttribs[7].nValue[0] = nValue1;
					m_DamageAttribs[7].nValue[1] = nValue2;
					m_DamageAttribs[7].nValue[2] = nValue3;
					m_nDamageAttribsNum ++;
					break;
				}
				return TRUE;
			}
			
			//ʣ�µ�Ϊ���ݼ���ʱ�����ݲ���
			//����nValue2ֵ����״̬�������Ƿ�״̬����
			if (nValue2 == 0) 
			{
				m_ImmediateAttribs[m_nImmediateAttribsNum].nAttribType = i;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[0] = nValue1;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[1] = nValue2;
				m_ImmediateAttribs[m_nImmediateAttribsNum].nValue[2] = nValue3;
				m_nImmediateAttribsNum ++;
				return TRUE;
				
			}
			else
			{
				m_StateAttribs[m_nStateAttribsNum].nAttribType = i;
				m_StateAttribs[m_nStateAttribsNum].nValue[0] = nValue1;
				m_StateAttribs[m_nStateAttribsNum].nValue[1] = nValue2;
				m_StateAttribs[m_nStateAttribsNum].nValue[2] = nValue3;
				m_nStateAttribsNum ++;
				return TRUE;
			}
			
		}
	}
	return FALSE;
}

const char * KSkill::MagicAttrib2String(int MagicAttrib)  const 
{
	return 	g_MagicID2String(MagicAttrib);
}

#ifndef _SERVER
void	KSkill::DrawSkillIcon(int x, int y, int Width, int Height)  
{
	
	if (!m_szSkillIcon[0]) return ;
	
	m_RUIconImage.nType = ISI_T_SPR;
	m_RUIconImage.Color.Color_b.a = 255;
	m_RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	m_RUIconImage.uImage = 0;
	m_RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	m_RUIconImage.bRenderFlag = 0;
	strcpy(m_RUIconImage.szImage, m_szSkillIcon);
	m_RUIconImage.oPosition.nX = x;
	m_RUIconImage.oPosition.nY = y;
	m_RUIconImage.oPosition.nZ = 0;
	m_RUIconImage.nFrame = 0;
	g_pRepresent->DrawPrimitives(1, &m_RUIconImage, RU_T_IMAGE, 1);
}

void	KSkill::GetDesc(unsigned long ulSkillId, unsigned long ulCurLevel, char * pszMsg, int nOwnerIndex,  bool bGetNextLevelDesc)
{
	
	if (!pszMsg) return;
	if (nOwnerIndex <= 0 )	return ;
	strcpy(pszMsg,"");
	char szTemp[300];
	
	KSkill * pTempSkill = NULL;
	KSkill * pCurSkill = NULL;
	KSkill * pNextSkill = NULL;
	
	if(ulCurLevel == 0)
	{
		pNextSkill = (KSkill *)g_SkillManager.GetSkill(ulSkillId, 1);
		pTempSkill = pNextSkill;
	}
	else
	{
		pCurSkill = (KSkill *) g_SkillManager.GetSkill(ulSkillId, ulCurLevel);
		pNextSkill = (KSkill *) g_SkillManager.GetSkill(ulSkillId, ulCurLevel + 1);
		pTempSkill = pCurSkill;
	}
	
	if (pTempSkill == NULL)
	{
		return;
	}
	
	strcat(pszMsg, "<color=Yellow>");
	strcat(pszMsg, pTempSkill->m_szName);
	strcat(pszMsg, "\n<bclr=Black><color>");
	
	
#ifdef SHOW_SKILL_MORE_INFO
	switch(pTempSkill->m_eSkillStyle)
	{
	case SKILL_SS_Missles:
		{
			strcat(pszMsg, "�ӵ���\n");
			szTemp[0] = 0;
			switch(pTempSkill->m_eMisslesForm)
			{
			case SKILL_MF_Wall:
				{
					sprintf(szTemp, "ǽ�� ����%d\n", pTempSkill->m_nChildSkillNum);
				}break;			//ǽ��	����ӵ��ʴ�ֱ�������У���ʽ��ǽ״
				
			case SKILL_MF_Line:
				{
					sprintf(szTemp, "���� ����%d\n", pTempSkill->m_nChildSkillNum);
				}break;					//����	����ӵ���ƽ������ҷ�������
			case SKILL_MF_Spread:
				{
					sprintf(szTemp, "ɢ�� ����%d\n", pTempSkill->m_nChildSkillNum);
				}break;				//ɢ��	����ӵ���һ���ĽǶȵķ�ɢ״	
			case SKILL_MF_Circle:
				{
					sprintf(szTemp, "Բ�η�ɢ ����%d\n", pTempSkill->m_nChildSkillNum);
				}break;				//Բ��	����ӵ�Χ��һ��Ȧ
			case SKILL_MF_Random:{}break;				//���	����ӵ�����ŷ�
			case SKILL_MF_Zone:
				{
					if (pTempSkill->m_nValue1)
						sprintf(szTemp, "Բ������ ����%d\n", pTempSkill->m_nChildSkillNum);
					else 
						sprintf(szTemp, "�������� ����%d\n", pTempSkill->m_nChildSkillNum);
					
				}break;					//����	����ӵ�������ĳ����Χ��
			case SKILL_MF_AtTarget:
				{
					if (pTempSkill->m_nValue1)
						sprintf(szTemp, "����Բ������ ����%d\n", pTempSkill->m_nChildSkillNum);
					else 
						sprintf(szTemp, "���㷽������ ����%d\n", pTempSkill->m_nChildSkillNum);
				}break;				//����	����ӵ�����
			case SKILL_MF_AtFirer:
				{
					if (pTempSkill->m_nValue1)
						sprintf(szTemp, "����Բ������ ����%d\n", pTempSkill->m_nChildSkillNum);
					else 
						sprintf(szTemp, "���㷽������ ����%d\n", pTempSkill->m_nChildSkillNum);
					
				}break;				//����	����ӵ�ͣ����ҵ�ǰλ��
			}
			strcat(pszMsg, szTemp);
			
		}break;			//	�ӵ���		���������ڷ����ӵ���
		
	case SKILL_SS_Melee:
		{
			strcat(pszMsg, "�񶷼�\n");
		}break;
	case SKILL_SS_InitiativeNpcState:
		{
			strcat(pszMsg, "���������书\n");			
			if (pTempSkill->m_StateAttribs[0].nValue[1] > 0)
			{
				sprintf (szTemp, "״̬����ʱ��:%d\n" ,pTempSkill->m_StateAttribs[0].nValue[1]);
				strcat(pszMsg,szTemp);
			}
		}break;	//	������		���������ڸı䵱ǰNpc������״̬
	case SKILL_SS_PassivityNpcState:
		{
			strcat(pszMsg, "���������书\n");
		}break;		//	������		���������ڸı�Npc�ı���״̬
		
		
	case SKILL_SS_CreateNpc:{}break;				//	����Npc��	��������������һ���µ�Npc
	case SKILL_SS_BuildPoison:{}break;			//	������		��������������
	case SKILL_SS_AddPoison:{}break;				//	�Ӷ���		���������ڸ������Ӷ���
	case SKILL_SS_GetObjDirectly:{}break;		//	ȡ����		���������ڸ���ȡ��
	case SKILL_SS_StrideObstacle :{}break;		//	��Խ��		���������ڿ�Խ�ϰ�
	case SKILL_SS_BodyToObject:{}break;			//	������		���������ڽ�ʬ���ɱ���
	case SKILL_SS_Mining:{}break;				//	�ɿ���		���������ڲɿ�������ɿ�ʯ
	case SKILL_SS_RepairWeapon:{}break;			//	�޸���		�����������޸�װ��
	case SKILL_SS_Capture:{}break;				//	��׽��		���������ڲ�׽����Npc
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_bRangeDamage) strcat(pszMsg, "�����˺� ");
	
	switch(g_MisslesLib[pTempSkill->m_nChildSkillId].m_eMoveKind)
	{
	case MISSLE_MMK_Stand:
		{
			strcat(pszMsg, "ԭ�� ");
		}break;							//	ԭ��
	case MISSLE_MMK_Line:
		{
			strcat(pszMsg, "ֱ�߷��� ");
		}break;							//	ֱ�߷���
	case MISSLE_MMK_Random:{}break;							//	������У����ڶ�Ů�׵�Charged Bolt��
	case MISSLE_MMK_Circle:
		{
			strcat(pszMsg, "���η��� ");
		}break;							//	���з��У�Χ������ߣ����ڶ��̿͵ļ�����
	case MISSLE_MMK_Helix:
		{
			strcat(pszMsg, "���η��� ");
		}break;							//	�����׵������ߣ����ڶ�������Bless Hammer��
	case MISSLE_MMK_Follow:{}break;							//	����Ŀ�����
	case MISSLE_MMK_Motion:{}break;							//	��Ҷ�����
	case MISSLE_MMK_Parabola:
		{
			strcat(pszMsg, "������� ");
		}break;						//	������
	case MISSLE_MMK_SingleLine:{}break;						//	���еĵ�һֱ�߷���ħ��
	}
	
	if (!g_MisslesLib[pTempSkill->m_nChildSkillId].m_bCollideVanish)
		strcat(pszMsg, "��͸ ");
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_nDamageRange > 1) 
	{
		sprintf(szTemp, "�˺���Χ:%d ", g_MisslesLib[pTempSkill->m_nChildSkillId].m_nDamageRange);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_nKnockBack)
	{
		sprintf(szTemp, "���˾���:%d ", g_MisslesLib[pTempSkill->m_nChildSkillId].m_nKnockBack);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[pTempSkill->m_nChildSkillId].m_bAutoExplode)
	{
		strcat(pszMsg, "�����Ա� ");
	}
	
	if (pTempSkill->m_bIsAura) strcat(pszMsg, "�⻷ ");
	if (pTempSkill->m_bIsPhysical) strcat(pszMsg, "���� ");
	if (pTempSkill->m_bIsMelee) strcat(pszMsg, "���� ");
	if (pTempSkill->m_bTargetOnly) strcat(pszMsg, "���� ");
	if (pTempSkill->m_bTargetAlly) strcat(pszMsg, "���� ");
	if (pTempSkill->m_bTargetEnemy) strcat(pszMsg, "�Ե� ");
	if (pTempSkill->m_bTargetObj)	  strcat(pszMsg, "���� ");
	if (pTempSkill->m_bTargetSelf) strcat(pszMsg, "���� ");
	if (pTempSkill->m_bUseAttackRate) strcat(pszMsg, "���������� ");
#endif
	
	strcat (pszMsg, "\n");
	strcat(pszMsg, pTempSkill->m_szSkillDesc);
	
	strcat(pszMsg, "\n\n");
	
	if (!pTempSkill->IsPhysical())
	{
		sprintf(szTemp, "��ǰ�ȼ�:%d", ulCurLevel);
		strcat(pszMsg, szTemp);
		strcat(pszMsg, "\n");
	}
	
	int i = 0;
	
	if (pCurSkill)
	{
		pCurSkill->GetDescAboutLevel(pszMsg);
	}
	
	if (pTempSkill->m_nHorseLimited)
	{
		switch(pTempSkill->m_nHorseLimited)
		{
		case 1:
			{
				strcat(pszMsg, "�����в���ʩչ\n");
			}
			break;
		case 2:
			{
				strcat(pszMsg, "��������ʩչ\n");
			}
			break;
		default:
			return ;
		}
	}

	if (!pTempSkill->IsPhysical())
	{
		if (bGetNextLevelDesc)
		{
			if (pNextSkill)
			{
				strcat(pszMsg, "\n<color=Red>��һ��\n");
				pNextSkill->GetDescAboutLevel(pszMsg);
			}
			else
			{
				
			}
		}
	}
}

void KSkill::GetDescAboutLevel(char * pszMsg)
{
	
	char szTemp[100];
	int nGetCost = GetSkillCost(NULL);

	if (nGetCost)
	{
		switch(m_nSkillCostType)
		{
		case attrib_mana:
			
			sprintf(szTemp, "��������:%d\n", nGetCost);
			strcat(pszMsg,szTemp);
			break;
		case attrib_stamina:
			sprintf(szTemp, "��������:%d\n", nGetCost);
			strcat(pszMsg,szTemp);
			break;
		case attrib_life:
			sprintf(szTemp, "��������:%d\n", nGetCost);
			strcat(pszMsg,szTemp);
			break;
		}
	}
	
	int nGetAttackRadius = GetAttackRadius();
	if (nGetAttackRadius)
	{
		sprintf(szTemp,"��Ч����:%d\n", nGetAttackRadius);
		strcat(pszMsg,szTemp);
	}

	//����ȼ��仯�������˺�
	for (int i  = 0; i < m_nImmediateAttribsNum; i ++)
	{
		if (!m_ImmediateAttribs[i].nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_ImmediateAttribs[i]);
		if (!pszInfo) continue;
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}
	//�ӵ���������Լ�����ɵ��˺�
	//KMagicAttrib *DamageAttribs[MAX_MISSLE_DAMAGEATTRIB];
	KMagicAttrib *DamageAttribs = m_DamageAttribs;
	//������ҵĻ������ԣ�ȷ���ӵ����˺�
	//Npc[nOwnerIndex].AppendSkillEffect(m_DamageAttribs, DamageAttribs);
int i; //Fixed By MrChuCong@gmail.com
	for (i  = 0; i < MAX_MISSLE_DAMAGEATTRIB; i ++)
	{
		if (!(DamageAttribs + i)->nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc((DamageAttribs + i));
		if (!pszInfo) continue;
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}
	//״̬����Ч��
	for (i  = 0; i < m_nStateAttribsNum; i ++)
	{
		if (!m_StateAttribs[i].nAttribType) continue;
		char * pszInfo = (char *)g_MagicDesc.GetDesc(&m_StateAttribs[i]);
		if (!pszInfo) continue;
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}
}

void KSkill::PlayPreCastSound(BOOL bIsFeMale, int nX, int nY)  const 
{
	char * pSoundFile = NULL;
	
	if (!bIsFeMale)
		pSoundFile = (char *)m_szManPreCastSoundFile;
	else 
		pSoundFile = (char *)m_szFMPreCastSoundFile;
	
	int		nCenterX = 0, nCenterY = 0, nCenterZ = 0;
	
	// �����Ļ���ĵ�ĵ�ͼ���� not end
	g_ScenePlace.GetFocusPosition(nCenterX, nCenterY, nCenterZ);
	KCacheNode * pSoundNode = NULL;
	pSoundNode = (KCacheNode*) g_SoundCache.GetNode(pSoundFile, (KCacheNode*)pSoundNode);
	KWavSound * pWave = (KWavSound*)pSoundNode->m_lpData;
	if (pWave)
	{
		pWave->Play((nX - nCenterX) * 5, (10000 - (abs(nX - nCenterX) + abs(nY - nCenterY))) * Option.GetSndVolume() / 100 - 10000, 0);
	}
}
#endif

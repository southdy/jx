#include "Skill.h"
// KMissleSkills.cpp: implementation of the KMissleSkills class.
//
//////////////////////////////////////////////////////////////////////
#include "KCore.h"
#include "KMissleSkill.h"
#include "kmissle.h"
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
#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"
#include "imgref.h"
#include "KMagicDesc.h"
#include "skill.h"
#endif

#define	 NPCINDEXOFOBJECT 0 //�����ħ��ʱ����Ӧ��Npc���

const char * g_MagicID2String(int nAttrib);

/*!*****************************************************************************
// Function		: KMissleSkill::KMissleSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KMissleSkill::KMissleSkill()
{
	m_nSkillCostType = attrib_mana;
	m_nFlySkillId =  m_nCollideSkillId = m_nVanishedSkillId = -1;
	m_nWaitTime = 0;
	m_nRowIdInSkillTabFile = m_nImmediateAttribsNum = m_nStateAttribsNum = m_nMissleAttribsNum = m_nDamageAttribsNum = 0;
#ifndef _SERVER
	m_szSkillDesc[0] = 0;
	m_szPreCastEffectFile[0] = 0;
#endif
	
	
}
/*!*****************************************************************************
// Function		: KMissleSkill::~KMissleSkill
// Purpose		: 
// Return		: 
// Comments		:
// Author		: RomanDou
*****************************************************************************/
KMissleSkill::~KMissleSkill()
{
	while (m_MissleLevelUpSettingList.GetTail())
	{
		KEffectDataNode * pNode = (KEffectDataNode*) m_MissleLevelUpSettingList.GetTail();
		delete pNode;
		m_MissleLevelUpSettingList.RemoveTail();
	}
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
// Function		: KMissleSkill::Cast
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

BOOL	KMissleSkill::Cast(TSkillParam * pSkillParam)
{
	BOOL bResult = KFightSkill::Cast(pSkillParam);
	if (!bResult)  return FALSE;

	CastMissles(pSkillParam);
	if (m_bStartEvent)
	{
		TSkillParam StartSkillParam = *pSkillParam;
		StartSkillParam.usSkillID = m_nStartSkillId;
		StartSkillParam.usSkillLevel = m_nEventSkillLevel;
		g_CastSkill(&StartSkillParam);
	}
}

void	KMissleSkill::TrigerMissleEventSkill(int nMissleId, int nSkillId, int nSkillLevel)
{
	CheckAndGenSkillData();
	
	if (nMissleId > MAX_MISSLE || nMissleId < 0) return ;
	
	KMissle * pMissle = &(Missle[nMissleId]);
	
	if (pMissle->m_nMissleId < 0) return;

	if (pMissle->m_SkillParam.dwSkillParams & PARAM_LAUNCHER)
	{
		switch(pMissle->m_SkillParam.tLauncher.tActorInfo.nActorType)
		{
		case Actor_Npc:
			if (!Npc[pMissle->m_SkillParam.tLauncher.tActorInfo.nActorIndex].IsMatch(pMissle->m_SkillParam.tLauncher.tActorInfo.dwActorID)) 
				return;
			break;
		case Actor_Obj:
		//	if (!Object[pMissle->m_SkillParam.tLauncher.tActorInfo.nActorIndex].IsMatch(pMissle->m_SkillParam.tLauncher.tActorInfo.dwActorID)) 
				return;
			break;
		case Actor_Missle:
			if (!Missle[pMissle->m_SkillParam.tLauncher.tActorInfo.nActorIndex].IsMatch(pMissle->m_SkillParam.tLauncher.tActorInfo.dwActorID)) 
				return;
			break;
		default:
			return;
		}
	}

	if (nSkillId > 0)//�����������������������
	{
		if (nSkillLevel <= 0) return ;
		TSkillParam SkillParam ;
		SkillParam.usSkillID = nSkillId;
		SkillParam.usSkillLevel = nSkillLevel;
		SkillParam.nWaitTime = 0;
		SkillParam.tLauncher = pMissle->m_SkillParam.tLauncher;
		
		if (m_bByMissle)
		{	
			SkillParam.dwSkillParams |= PARAM_REF;
			SkillParam.tRef.tActorInfo.nActorType = Actor_Missle;
			SkillParam.tRef.tActorInfo.dwActorID = pMissle->m_dwID;
			SkillParam.tRef.tActorInfo.nActorIndex = nMissleId;
		}
		else
		{
			SkillParam.dwSkillParams |= (pMissle->m_SkillParam.dwSkillParams & PARAM_LAUNCHER != 0);
			SkillParam.tLauncher.tActorInfo = pMissle->m_SkillParam.tLauncher.tActorInfo;
		}
		
		pMissle->GetMpsPos(&SkillParam.tTarget.tPoint.nX, &SkillParam.tTarget.tPoint.nY);
		SkillParam.tTarget.tPoint.nSubWorldIndex = pMissle->m_nSubWorldId;
		
		g_CastSkill(&SkillParam);
	}


}

/*!*****************************************************************************
// Function		: KMissleSkill::Vanish
// Purpose		: �ӵ���������ʱ�ص�
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KMissleSkill::Vanish(int nMissleId)
{
	TrigerMissleEventSkill(nMissleId, m_nVanishedSkillId, m_nEventSkillLevel);
}
/*!*****************************************************************************
// Function		: KMissleSkill::FlyEvent
// Purpose		: 
// Return		: void 
// Argumant		: int nMissleId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void KMissleSkill::FlyEvent(int nMissleId)
{
	TrigerMissleEventSkill(nMissleId, m_nFlySkillId, m_nEventSkillLevel);
}

/*!*****************************************************************************
// Function		: KMissleSkill::Collidsion
// Purpose		: �ӵ���ײʱ�ص�
// Return		: 
// Argumant		: KMissle* Missle
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KMissleSkill::Collidsion(int nMissleId)
{
	TrigerMissleEventSkill(nMissleId, m_nCollideSkillId, m_nEventSkillLevel);
}
/*!*****************************************************************************
// Function		: KMissleSkill::CastMissles
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
BOOL	KMissleSkill::CastMissles(TSkillParam * pSkillParam)
{
	_ASSERT(pSkillParam);
	int nRegionId		=	0;
	int nSrcPX			=	0;//������
	int nSrcPY			=	0;
	int nDesPX			=	0;
	int nDesPY			=	0;

	
	if (pSkillParam->dwSkillParams & PARAM_USEREF)
	{
		if(pSkillParam->dwSkillParams & PARAM_REF)
		{
			pSkillParam->tRef.tActorInfo.GetMps(nSrcPX, nSrcPY);
		}
		else
		{
			nSrcPX = pSkillParam->tRef.tPoint.nX;
			nSrcPY = pSkillParam->tRef.tPoint.nY;

		}
	}
	else if (pSkillParam->dwSkillParams & PARAM_LAUNCHER)
	{
		pSkillParam->tLauncher.tActorInfo.GetMps(nSrcPX, nSrcPY);
	}
	else
	{
		nSrcPX = pSkillParam->tLauncher.tPoint.nX;
		nSrcPY = pSkillParam->tLauncher.tPoint.nY;
	}


	if (pSkillParam->dwSkillParams & PARAM_TARGET)
	{
		pSkillParam->tTarget.tActorInfo.GetMps(nDesPX,nDesPY);
	}
	else
	{
		nDesPX = pSkillParam->tTarget.tPoint.nX;
		nDesPY = pSkillParam->tTarget.tPoint.nY;
	}
	
	CastMissleFunc[m_eMisslesForm](nSrcPX, nSrcPY, nDesPX, nDesPY, pSkillParam);
	return TRUE;
}

/*!*****************************************************************************
// Function		: KMissleSkill::CastZone
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
int KMissleSkill::CastZone(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam)
{
	if (pSkillParam->dwSkillParams & PARAM_ONLYDIR) return 0;
	
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
		nBeginPX	= nRefPX - m_nChildSkillNum * pSkillParam->GetCellWidth() / 2;
		nBeginPY	= nRefPY - m_nChildSkillNum * pSkillParam->GetCellHeight() / 2;
	}
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(pSkillParam);
#endif //_SERVER
	
	for (int i = 0; i < m_nChildSkillNum; i ++)
		for (int j = 0; j < m_nChildSkillNum; j ++)
		{
			if (m_bBaseSkill)
			{
				int nMissleIndex ;
				int nSubWorldId = pSkillParam->GetSubWorldIndex(); 
				if (nSubWorldId < 0) goto exit;
				
				if (m_nValue1 == 1)
					if ( ((i - m_nChildSkillNum / 2) * (i - m_nChildSkillNum / 2) + (j - m_nChildSkillNum / 2) * (j - m_nChildSkillNum / 2)) > (m_nChildSkillNum * m_nChildSkillNum / 4))
						continue;

					nMissleIndex = MissleSet.Add(nSubWorldId, nBeginPX + j * SubWorld[nSubWorldId].m_nCellWidth , nBeginPY +  i * SubWorld[nSubWorldId].m_nCellHeight);
					if (nMissleIndex < 0)	continue;
					Missle[nMissleIndex].m_nDir				= g_GetDirIndex(nSrcPX, nSrcPY, nRefPX, nRefPY);
					Missle[nMissleIndex].m_nDirIndex		= g_DirIndex2Dir(Missle[nMissleIndex].m_nDir, MaxMissleDir);
					CreateMissle(pSkillParam, m_nChildSkillId, nMissleIndex);
					Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
					Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
					Missle[nMissleIndex].m_SkillParam		= *pSkillParam;
					Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j);
					Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
					
					if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line)
					{
						Missle[nMissleIndex].m_nXFactor = g_DirCos(Missle[nMissleIndex].m_nDir, MaxMissleDir);
						Missle[nMissleIndex].m_nYFactor = g_DirSin(Missle[nMissleIndex].m_nDir, MaxMissleDir);
					}
#ifdef _SERVER
					Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //
					
					nCastMissleNum ++;
#ifndef _SERVER				
					Missle[nMissleIndex].m_MissleRes.PlaySound(Missle[nMissleIndex].m_eMissleStatus, 0, 0, 0);
#endif
					
			}
			else
			{
				if (m_nChildSkillLevel <= 0) continue;
					
				TSkillParam ChildSkillParam;
				ChildSkillParam.dwSkillParams = 0;
				
				ChildSkillParam.dwSkillParams &= ((pSkillParam->dwSkillParams & PARAM_LAUNCHER ) != 0);
				ChildSkillParam.tLauncher.tActorInfo = pSkillParam->tLauncher.tActorInfo;

				ChildSkillParam.tTarget.tPoint.nX = nBeginPX + j * pSkillParam->GetCellWidth() ;
				ChildSkillParam.tTarget.tPoint.nY = nBeginPY +  i * pSkillParam->GetCellHeight();
				ChildSkillParam.nWaitTime = pSkillParam->nWaitTime + GetMissleGenerateTime(i * m_nChildSkillNum + j );
				ChildSkillParam.usSkillID = m_nChildSkillId;
				ChildSkillParam.usSkillLevel = m_nChildSkillLevel;
				nCastMissleNum += g_CastSkill(&ChildSkillParam);
			}
			
		}
exit:	
#ifdef _SERVER
		if (!nCastMissleNum) 
			if (pNewMagicAttribsData)
				if (pNewMagicAttribsData->GetRef() == 0)
					delete pNewMagicAttribsData;
#endif
				return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KMissleSkill::CastLine
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
int		KMissleSkill::CastLine(int nSrcPX, int nSrcPY, int nRefPX, int nRefPY, TSkillParam * pSkillParam)
{
	_ASSERT(pSkillParam);
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	int nDirIndex = 0;
	int nDir = 0;
	
	if (pSkillParam->dwSkillParams & PARAM_ONLYDIR)
	{
		nDir = pSkillParam->nDir;
		nDirIndex = g_Dir2DirIndex(nDir, MaxMissleDir);
	}
	else
	{
		nDirIndex = g_GetDirIndex(nSrcPX, nSrcPY, nRefPX, nRefPY);
		nDir = g_DirIndex2Dir(nDirIndex , MaxMissleDir);
	}
	
	//�ӵ�֮��ļ��
	int nMSDistanceEach = m_nValue1;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(pSkillParam);
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
			int nSubWorldId = pSkillParam->GetSubWorldIndex(); 

			if (nSubWorldId < 0)	goto exit;
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			CreateMissle(pSkillParam, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_SkillParam		= *pSkillParam;
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
#ifndef _SERVER				
			Missle[nMissleIndex].m_MissleRes.PlaySound(Missle[nMissleIndex].m_eMissleStatus, 0, 0, 0)		;		
#endif
			nCastMissleNum ++;
		}
		else
		{
				if (m_nChildSkillLevel <= 0) continue;

				TSkillParam ChildSkillParam;
				ChildSkillParam.dwSkillParams = 0;

				ChildSkillParam.dwSkillParams &= ((pSkillParam->dwSkillParams & PARAM_LAUNCHER ) != 0);
				ChildSkillParam.tLauncher.tActorInfo = pSkillParam->tLauncher.tActorInfo;

				ChildSkillParam.tTarget.tPoint.nX = nDesSubX ;
				ChildSkillParam.tTarget.tPoint.nY = nDesSubY;
				ChildSkillParam.nWaitTime = pSkillParam->nWaitTime + GetMissleGenerateTime(i);
				ChildSkillParam.usSkillID = m_nChildSkillId;
				ChildSkillParam.usSkillLevel = m_nChildSkillLevel;
				nCastMissleNum += g_CastSkill(&ChildSkillParam);
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (!nCastMissleNum) 
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			return nCastMissleNum;
}
/*
int		KMissleSkill::CastExtractiveLineMissle(int nSrcX, int nSrcY, int nDesX, int nDesY, int nXOffset, int nYOffset, int nDesX, int nDesY, TSkillParam * pSkillParam)
{
	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	//�ӵ�֮��ļ��

	if (nSrcPX == nDesPX && nSrcPY == nDesPY)		return FALSE ;
	nDistance = g_GetDistance(nSrcPX, nSrcPY, nDesPX, nDesPY);
	if (nDistance == 0 ) return FALSE;
	int		nYLength = nDesPY - nSrcPY;
	int		nXLength = nDesPX - nSrcPX;
	int		nSin = (nYLength << 10) / nDistance;	// �Ŵ�1024��
	int		nCos = (nXLength << 10) / nDistance;
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(nLauncher);
#endif //_SERVER
	
	//�ֱ����ɶ����ӵ�
	{
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId = pSkillParam->GetSubWorldIndex(); 
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nSrcPX, nSrcPY);
			
			if (nMissleIndex < 0)	goto exit;
			
			Missle[nMissleIndex].m_nDir				= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
			Missle[nMissleIndex].m_nDirIndex		= g_DirIndex2Dir(Missle[nMissleIndex].m_nDir, MaxMissleDir);
			CreateMissle(pSkillParam, m_nChildSkillId, nMissleIndex);

			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				int nLength = g_GetDistance(nSrcPX, nSrcPY, nDesX, nDesY);
				int nTime = nLength / Missle[nMissleIndex].m_nSpeed;
				Missle[nMissleIndex].m_nHeightSpeed	= Missle[nMissleIndex].m_nZAcceleration * (nTime - 1) / 2;
			}

			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_SkillParam		= *pSkillParam;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(0);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;	
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line || Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Parabola)
			{
				
				Missle[nMissleIndex].m_nXFactor = nCos;
				Missle[nMissleIndex].m_nYFactor = nSin;
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
#ifndef _SERVER				
			Missle[nMissleIndex].m_MissleRes.PlaySound(Missle[nMissleIndex].m_eMissleStatus, 0, 0, 0)		;		
#endif
			nCastMissleNum ++;
		}
		else
		{
				if (m_nChildSkillLevel <= 0) continue;

				TSkillParam ChildSkillParam;
				ChildSkillParam.dwSkillParams = 0;
				
				ChildSkillParam.dwSkillParams &= ((pSkillParam->dwSkillParams & PARAM_LAUNCHER ) != 0);
				ChildSkillParam.tLauncher.tActorInfo = pSkillParam.tLauncher.tActorInfo;

				ChildSkillParam.tTarget.tPoint.nX = nSrcPX ;
				ChildSkillParam.tTarget.tPoint.nY = nSrcPY;
				ChildSkillParam.nWaitTime = pSkillParam->nWaitTime + GetMissleGenerateTime(0);
				ChildSkillParam.usSkillID = m_nChildSkillId;
				ChildSkillParam.usSkillLevel = m_nChildSkillLevel;
				nCastMissleNum += g_CastSkill(&ChildSkillParam);

		}
		
	}
	
exit:	
#ifdef _SERVER
	if (!nCastMissleNum) 
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			
			return nCastMissleNum;
			
}
*/
/*!*****************************************************************************
// Function		: KMissleSkill::CastWall
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
	/*
	��ǽʱ����һ���ֲ�����ʾ�ӵ�֮��ĳ��ȼ��
	X2  = X1 + N * SinA
	Y2  = Y2 - N * CosA
	*/

int KMissleSkill::CastWall(int nSrcPX, int nSrcPY, int nDesPX, int nDesPY, TSkillParam * pSkillParam)
						   
{
	_ASSERT(pSkillParam);
	int nDir = 0;
	int nDirIndex = 0;

	//ǽ��ħ��������ֻ������
	if (pSkillParam->dwSkillParams & PARAM_ONLYDIR) return FALSE;
			
	nDirIndex		= g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
	nDir			= g_DirIndex2Dir(nDirIndex, MaxMissleDir);
	nDir = nDir + MaxMissleDir / 4;
	if (nDir >= MaxMissleDir) nDir -= MaxMissleDir;

	int nDesSubX		= 0;
	int nDesSubY		= 0;
	int nCastMissleNum	= 0;
	
	//�ӵ�֮��ļ��
	int nMSDistanceEach = m_nValue1;
	int nCurMSDistance	= -1 * nMSDistanceEach * m_nChildSkillNum / 2;
	
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(pSkillParam->tLauncher.tActorInfo);
#endif //_SERVER
	
	
	//�ֱ����ɶ����ӵ�
	for(int i = 0; i < m_nChildSkillNum; i++)
	{
		nDesSubX	= nDesPX + ((nCurMSDistance * g_DirCos(nDirIndex,MaxMissleDir)) >>10);
		nDesSubY	= nDesPY + ((nCurMSDistance * g_DirSin(nDirIndex,MaxMissleDir)) >>10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			int nMissleIndex ;
			int nSubWorldId = pSkillParam->GetSubWorldIndex(); 
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			if (nMissleIndex < 0)	continue;
			
			Missle[nMissleIndex].m_nDir				= nDir;
			Missle[nMissleIndex].m_nDirIndex		= nDirIndex;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			CreateMissle(pSkillParam, m_nChildSkillId, nMissleIndex);
			
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_SkillParam		= *pSkillParam;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nDir, MaxMissleDir);
			}
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
#ifndef _SERVER							
			Missle[nMissleIndex].m_MissleRes.PlaySound(Missle[nMissleIndex].m_eMissleStatus, 0, 0, 0);		
#endif
			nCastMissleNum ++;
		}
		else
		{
				if (m_nChildSkillLevel <= 0) continue;
				TSkillParam ChildSkillParam;
				ChildSkillParam.dwSkillParams = 0;
				
				ChildSkillParam.dwSkillParams &= ((pSkillParam->dwSkillParams & PARAM_LAUNCHER ) != 0);
				ChildSkillParam.tLauncher.tActorInfo = pSkillParam->tLauncher.tActorInfo;

				ChildSkillParam.tTarget.tPoint.nX = nDesSubX ;
				ChildSkillParam.tTarget.tPoint.nY = nDesSubY;
				ChildSkillParam.nWaitTime = pSkillParam->nWaitTime + GetMissleGenerateTime(i);
				ChildSkillParam.usSkillID = m_nChildSkillId;
				ChildSkillParam.usSkillLevel = m_nChildSkillLevel;
				nCastMissleNum += g_CastSkill(&ChildSkillParam);
		}
		
		nCurMSDistance += nMSDistanceEach;
	}
	
exit:	
#ifdef _SERVER
	if (!nCastMissleNum) 
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			
			return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KMissleSkill::CastCircle
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
int		KMissleSkill::CastCircle(int nSrcPX, int nSrcPY, int nDesPX, int nDesPY, TSkillParam * pSkillParam)
{
	int nDesSubPX	= 0;
	int nDesSubPY	= 0;
	int nFirstStep	= m_nValue2;			//��һ���ĳ��ȣ��ӵ��ڸշ���ȥʱ����ҵľ���
	int nCurSubDir	= 0;
	int nDirPerNum  = 	MaxMissleDir / m_nChildSkillNum  ;
	int nCastMissleNum = 0;
	
	if (pSkillParam->dwSkillParams & PARAM_ONLYDIR) return 0;
	int nDirIndex = g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
	int nDir      = g_DirIndex2Dir(nDirIndex, MaxMissleDir);
	
	int nRefPX = 0;
	int nRefPY = 0;

	//�Ե�ǰ��ΪԲ��������Χ�ŵ��ӵ�
	//�ֳ����������һ��Ϊ��ԭ��Ϊԭ�ķ�������һ��Ϊ��Ŀ���Ϊԭ�ķ���
	// ���ֲ���һ��ʾ �Ƿ�Ϊԭ�ط���
	if (m_nValue1 == 0)
	{
		nRefPX = nSrcPX;
		nRefPY = nSrcPY;
	}
	else
	{
		nRefPX = nDesPX;
		nRefPY = nDesPY;
	}
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(pSkillParam);
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
			int nSubWorldId = pSkillParam->GetSubWorldIndex(); 
			
			if (nSubWorldId < 0)	goto exit;
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubPX, nDesSubPY);
			
			if (nMissleIndex < 0)	continue;

			Missle[nMissleIndex].m_nDir			= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex	= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(pSkillParam, m_nChildSkillId, nMissleIndex);

			
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_SkillParam		= *pSkillParam;	
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			
			if (Missle[nMissleIndex].m_eMoveKind == MISSLE_MMK_Line)
			{
				Missle[nMissleIndex].m_nXFactor = g_DirCos(nCurSubDir, MaxMissleDir);
				Missle[nMissleIndex].m_nYFactor = g_DirSin(nCurSubDir, MaxMissleDir);
			}
			
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
#ifndef _SERVER							
			Missle[nMissleIndex].m_MissleRes.PlaySound(Missle[nMissleIndex].m_eMissleStatus, 0, 0, 0);
#endif			
			nCastMissleNum ++;
			
		}
		else
		{
				if (m_nChildSkillLevel <= 0) continue;
				TSkillParam ChildSkillParam;
				ChildSkillParam.dwSkillParams = 0;
				
				ChildSkillParam.dwSkillParams &= ((pSkillParam->dwSkillParams & PARAM_LAUNCHER ) != 0);
				ChildSkillParam.tLauncher.tActorInfo = pSkillParam->tLauncher.tActorInfo;

				ChildSkillParam.tTarget.tPoint.nX = nDesSubPX ;
				ChildSkillParam.tTarget.tPoint.nY = nDesSubPX;
				ChildSkillParam.nWaitTime = pSkillParam->nWaitTime + GetMissleGenerateTime(i);
				ChildSkillParam.usSkillID = m_nChildSkillId;
				ChildSkillParam.usSkillLevel = m_nChildSkillLevel;
				nCastMissleNum += g_CastSkill(&ChildSkillParam);
		}
		
	}
	
exit:	
#ifdef _SERVER
	if (!nCastMissleNum) 
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			
			return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KMissleSkill::CastSpread
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
//  ���ֲ���һ��ʾ�ӵ�֮��ĽǶȲ��64����Ϊ׼
		//  ������X/Y����Ϊ��������
int		KMissleSkill::CastSpread(int nSrcPX, int nSrcPY, int nDesPX, int nDesPY, TSkillParam * pSkillParam)
{
	int nDesSubMapX		= 0;
	int nDesSubMapY		= 0;
	int nFirstStep		= m_nValue2;			//��һ���ĳ��ȣ��ӵ��ڸշ���ȥʱ����ҵľ���
	int nCurMSRadius	= m_nChildSkillNum / 2 ; 
	int nCurSubDir		= 0;
	int	nCastMissleNum  = 0;			//ʵ�ʷ��͵�Missle������
	int nDir = 0;
	int nDirIndex = 0;
	// Sin A+B = SinA*CosB + CosA*SinB
	// Cos A+B = CosA*CosB - SinA*SinB
	// Sin A = nYFactor
	// Cos A = nXFactor
	
#ifdef _SERVER
	KMissleMagicAttribsData * pNewMagicAttribsData = CreateMissleMagicAttribsData(pSkillParam->tLauncher.tActorInfo);
#endif
	
	int nDesSubX = 0;
	int nDesSubY = 0;
	int nXFactor = 0;
	int nYFactor = 0;
	
	if (pSkillParam->dwSkillParams & PARAM_ONLYDIR)
	{
		nDir = pSkillParam->nDir;
		nDirIndex = g_Dir2DirIndex(nDir, MaxMissleDir);
	}
	else
	{
		nDirIndex = g_GetDirIndex(nSrcPX, nSrcPY, nDesPX, nDesPY);
		nDir = g_DirIndex2Dir(nDirIndex , MaxMissleDir);
	}

	
	if (pSkillParam->dwSkillParams & PARAM_TARGET)
	{
		int nDistance = 0;
		nDistance = (int)sqrt((nDesPX - nSrcPX)*(nDesPX - nSrcPX) +	(nDesPY - nSrcPY)*(nDesPY - nSrcPY));
		nXFactor = ((nDesPX - nSrcPX) << 10) / nDistance;
		nYFactor = ((nDesPY - nSrcPY) << 10) / nDistance;
		
		nDesSubX = nSrcPX + ((nXFactor * nFirstStep) >> 10);
		nDesSubY = nSrcPY + ((nYFactor * nFirstStep) >> 10);
		
		if (nDesSubX < 0  || nDesSubY < 0 ) return 0;
	}
	
	
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
		
		if (pSkillParam->dwSkillParams & PARAM_TARGET)
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
		
		nDesSubX	= nDesPX + ((nCosAB * nFirstStep) >> 10);
		nDesSubY	= nDesPY + ((nSinAB * nFirstStep) >> 10);
		
		if (nDesSubX < 0 || nDesSubY < 0) 	continue;
		
		if (m_bBaseSkill)
		{
			
			int nMissleIndex ;
			int nSubWorldId = pSkillParam->GetSubWorldIndex(); 
			if (nSubWorldId < 0)	goto exit;
			
			
			nMissleIndex = MissleSet.Add(nSubWorldId, nDesSubX, nDesSubY);
			
			
			if (nMissleIndex < 0)	continue;

			Missle[nMissleIndex].m_nDir				= nCurSubDir;
			Missle[nMissleIndex].m_nDirIndex		= g_Dir2DirIndex(nCurSubDir, MaxMissleDir);
			CreateMissle(pSkillParam, m_nChildSkillId, nMissleIndex);
			Missle[nMissleIndex].m_dwBornTime		= SubWorld[nSubWorldId].m_dwCurrentTime;
			Missle[nMissleIndex].m_nSubWorldId		= nSubWorldId;
			Missle[nMissleIndex].m_nSrcMapX			= nDesPX;
			Missle[nMissleIndex].m_nSrcMapY			= nDesPX;
			Missle[nMissleIndex].m_nStartLifeTime	= pSkillParam->nWaitTime + GetMissleGenerateTime(i);
			Missle[nMissleIndex].m_nLifeTime		+= Missle[nMissleIndex].m_nStartLifeTime;
			Missle[nMissleIndex].m_nXFactor			= nCosAB;
			Missle[nMissleIndex].m_nYFactor			= nSinAB;
			
#ifdef _SERVER
			Missle[nMissleIndex].SetMagicAttribsData(pNewMagicAttribsData);
#endif //_SERVER
			
#ifndef _SERVER							
			Missle[nMissleIndex].m_MissleRes.PlaySound(Missle[nMissleIndex].m_eMissleStatus, 0, 0, 0)		;
#endif			
			nCastMissleNum ++;
		}
		else
		{
			if (m_nChildSkillLevel <= 0) continue;
				TSkillParam ChildSkillParam;
				ChildSkillParam.dwSkillParams = 0;
				
				ChildSkillParam.dwSkillParams &= ((pSkillParam->dwSkillParams & PARAM_LAUNCHER ) != 0);
				ChildSkillParam.tLauncher.tActorInfo = pSkillParam->tLauncher.tActorInfo;

				ChildSkillParam.tTarget.tPoint.nX = nDesSubX ;
				ChildSkillParam.tTarget.tPoint.nY = nDesSubY;
				ChildSkillParam.nWaitTime = pSkillParam->nWaitTime + GetMissleGenerateTime(i);
				ChildSkillParam.usSkillID = m_nChildSkillId;
				ChildSkillParam.usSkillLevel = m_nChildSkillLevel;
				nCastMissleNum += g_CastSkill(&ChildSkillParam);
		}
		
		nCurMSRadius -- ;
	}
exit:	
#ifdef _SERVER
	if (!nCastMissleNum) 
		if (pNewMagicAttribsData)
			if (pNewMagicAttribsData->GetRef() == 0)
				delete pNewMagicAttribsData;
#endif
			
			return nCastMissleNum;
}

/*!*****************************************************************************
// Function		: KMissleSkill::CreateMissle
// Purpose		: �����ӵ��Ļ������ݣ��Լ��ü��ܸõȼ��µĶ��ӵ���Ϣ�ı䶯����
//					����������ֵ�����ָ��
// Return		: 
// Argumant		: int nChildSkillId
// Argumant		: int nMissleIndex
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void	KMissleSkill::CreateMissle(TSkillParam * pSkillParam, int nChildSkillId, int nMissleIndex)
{
	_ASSERT(pSkillParam);
	KMissle * pMissle = &Missle[nMissleIndex];
	g_MisslesLib[nChildSkillId] = *pMissle;//���ƿ�������
	
	pMissle->m_nLevel			= m_nLevel;
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
	
	//�����ӵ���Ŀ���ϵ
	if (m_bTargetEnemy)
	{
		if (m_bTargetAlly)
			pMissle->m_eRelation = MissleRelation_All;
		else
			pMissle->m_eRelation = MissleRelation_EnemyOnly;
	}
	else
	{
		if (m_bTargetAlly)
		{
			pMissle->m_eRelation = MissleRelation_AllyOnly;
		}
		else
			pMissle->m_eRelation = MissleRelation_None;
	}
	
#ifndef _SERVER
	pMissle->m_MissleRes.m_bNeedShadow   = m_bNeedShadow;
	pMissle->m_MissleRes.m_nMaxShadowNum = m_nMaxShadowNum;
	pMissle->m_MissleRes.m_nMissleId	 = nMissleIndex;
	if (!pMissle->m_MissleRes.Init()) g_DebugLog("�����ӵ���ͼʧ�ܣ�����%s", __FILE__) ;
	pMissle->DoWait();
#endif
	
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
	
	if (!(pSkillParam->dwSkillParams & PARAM_LAUNCHER) && pSkillParam->tLauncher.tActorInfo.nActorType == Actor_Npc) 
	{
		if (m_bIsMelee)
			pMissle->m_nLifeTime = Npc[pSkillParam->tLauncher.tActorInfo.nActorIndex].ModifyMissleLifeTime(pMissle->m_nLifeTime);
		else
		{
			pMissle->m_nSpeed = Npc[pSkillParam->tLauncher.tActorInfo.nActorIndex].ModifyMissleSpeed(pMissle->m_nSpeed);
			pMissle->m_bCollideVanish = Npc[pSkillParam->tLauncher.tActorInfo.nActorIndex].ModifyMissleCollsion(pMissle->m_bCollideVanish);
		}
	}
	
}




/*!*****************************************************************************
// Function		: KMissleSkill::SetMissleGenerateTime
// Purpose		: ��õ�ǰ���ӵ���ʵ�ʲ���ʱ��
// Return		: void 
// Argumant		: Missle * pMissle
// Argumant		: int nNo
// Comments		:
// Author		: RomanDou
*****************************************************************************/
unsigned int KMissleSkill::GetMissleGenerateTime(int nNo)
{
	CheckAndGenSkillData();
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

BOOL	KMissleSkill::GetInfoFromTabFile(KITabFile *pSkillsSettingFile, int nRow)
{
	if (!pSkillsSettingFile || nRow < 0) return FALSE;
	pSkillsSettingFile->GetInteger(nRow, "ChildSkillNum",	0, &m_nChildSkillNum,TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MisslesForm",		0, (int *)&m_eMisslesForm, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CharAnimId",		0, (int *)&m_nCharActionId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsPhysical",		0, &m_bIsPhysical, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsAura",			0, &m_bIsAura, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsUseAR",			0, &m_bUseAttackRate, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "BaseSkill",		0, &m_bBaseSkill, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ByMissle",		0, &m_bByMissle, TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "FlyEvent",		0, &m_bFlyingEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartEvent",		0, &m_bStartEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollideEvent",	0, &m_bCollideEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedEvent",	0, &m_bVanishedEvent, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlySkillId",		0, &m_nFlySkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "StartSkillId",	0, &m_nStartSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "VanishedSkillId",	0, &m_nVanishedSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "CollidSkillId",	0, &m_nCollideSkillId, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param1",			0, &m_nValue1, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "Param2",			0, &m_nValue2, TRUE);
	
	pSkillsSettingFile->GetInteger(nRow, "EventSkillLevel", 0, &m_nEventSkillLevel, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "IsMelee",			0, &m_bIsMelee, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "FlyEventTime",	0, &m_nFlyEventTime, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "ClientSend",		0, &m_bClientSend, TRUE);

	pSkillsSettingFile->GetInteger(nRow, "MslsGenerate",	0, (int *)&m_eMisslesGenerateStyle, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MslsGenerateData",0, &m_nMisslesGenerateData, TRUE);
	pSkillsSettingFile->GetInteger(nRow, "MaxShadowNum",	0, &m_nMaxShadowNum, TRUE);
#ifndef _SERVER
	pSkillsSettingFile->GetInteger(nRow, "NeedShadow",		0, &m_bNeedShadow, TRUE);
#endif
}


BOOL	KMissleSkill::ParseString2MagicAttrib(char * szMagicAttribName, char * szValue)
{
	int nValue1 = 0;
	int nValue2 = 0;
	int nValue3 = 0;
	if ((!szMagicAttribName) || (!szMagicAttribName[0])) return FALSE;
	
	//nValue2 ��ֵΪ-1ʱΪ������״̬��0Ϊ��״̬������ֵΪ��ʱЧ��״̬ħ��Ч��
	//��Ҫ��״̬�������״̬���ݷ��������������Ӧ�������ڣ�����¼������
	for (int i  = 0 ; i <= magic_normal_end; i ++)
	{
		if (!strcmp(szMagicAttribName, MagicAttrib2String(i)))
		{
			sscanf(szValue, "%d,%d,%d", &nValue1, &nValue2, &nValue3);
			if (KFightSkill::ParseString2MagicAttrib((MAGIC_ATTRIB)i, nValue1, nValue2, nValue3)) continue;
			if (!ParseString2MagicAttrib((MAGIC_ATTRIB)i, nValue1, nValue2, nValue3))
				g_DebugLog("[Skill]Can Not Find Magic Attrib %d",i);
		}
	}
			
	return FALSE;
}


BOOL	KMissleSkill::ParseString2MagicAttrib(MAGIC_ATTRIB MagicAttrib, int nValue1, int nValue2, int nValue3)
{
	if (MagicAttrib > magic_missle_begin && MagicAttrib < magic_missle_end)
	{
		m_MissleAttribs[m_nMissleAttribsNum].nAttribType = MagicAttrib;
		m_MissleAttribs[m_nMissleAttribsNum].nValue[0] = nValue1;
		m_MissleAttribs[m_nMissleAttribsNum].nValue[1] = nValue2;
		m_MissleAttribs[m_nMissleAttribsNum].nValue[2] = nValue3;
		m_nMissleAttribsNum ++;
		return TRUE;
	}
	
	if (MagicAttrib > magic_skill_begin && MagicAttrib < magic_skill_end)
	{
		switch(MagicAttrib)
		{
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
		return FALSE;
	}
}

#ifndef _SERVER
void	KMissleSkill::GetDesc(char * pszMsg, int nOwnerIndex)
{
	if (!pszMsg) return;
	if (nOwnerIndex <= 0 )	return ;
	strcpy(pszMsg,"");
	char szTemp[100];
	strcat(pszMsg, m_szName);
	strcat(pszMsg, "\n");
	switch(m_eSkillStyle)
	{
	case SKILL_SS_Missles:
		{
			strcat(pszMsg, "�ӵ���\n");
			szTemp[0] = 0;
			switch(m_eMisslesForm)
			{
			case SKILL_MF_Wall:
				{
					sprintf(szTemp, "ǽ�� ����%d\n", m_nChildSkillNum);
				}break;			//ǽ��	����ӵ��ʴ�ֱ�������У���ʽ��ǽ״
				
			case SKILL_MF_Line:
				{
					sprintf(szTemp, "���� ����%d\n", m_nChildSkillNum);
				}break;					//����	����ӵ���ƽ������ҷ�������
			case SKILL_MF_Spread:
				{
					sprintf(szTemp, "ɢ�� ����%d\n", m_nChildSkillNum);
				}break;				//ɢ��	����ӵ���һ���ĽǶȵķ�ɢ״	
			case SKILL_MF_Circle:
				{
					sprintf(szTemp, "Բ�η�ɢ ����%d\n", m_nChildSkillNum);
				}break;				//Բ��	����ӵ�Χ��һ��Ȧ
			case SKILL_MF_Random:{}break;				//���	����ӵ�����ŷ�
			case SKILL_MF_Zone:
				{
					if (m_nValue1)
						sprintf(szTemp, "Բ������ ����%d\n", m_nChildSkillNum);
					else 
						sprintf(szTemp, "�������� ����%d\n", m_nChildSkillNum);
					
				}break;					//����	����ӵ�������ĳ����Χ��
			case SKILL_MF_AtTarget:
				{
					if (m_nValue1)
						sprintf(szTemp, "����Բ������ ����%d\n", m_nChildSkillNum);
					else 
						sprintf(szTemp, "���㷽������ ����%d\n", m_nChildSkillNum);
				}break;				//����	����ӵ�����
			case SKILL_MF_AtFirer:
				{
					if (m_nValue1)
						sprintf(szTemp, "����Բ������ ����%d\n", m_nChildSkillNum);
					else 
						sprintf(szTemp, "���㷽������ ����%d\n", m_nChildSkillNum);
					
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
			if (m_StateAttribs[0].nValue[1] > 0)
			{
				sprintf (szTemp, "״̬����ʱ��:%d\n" ,m_StateAttribs[0].nValue[1]);
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
	if (g_MisslesLib[m_nChildSkillId].m_bRangeDamage) strcat(pszMsg, "�����˺� ");
	
	switch(g_MisslesLib[m_nChildSkillId].m_eMoveKind)
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
	
	if (!g_MisslesLib[m_nChildSkillId].m_bCollideVanish)
		strcat(pszMsg, "��͸ ");
	if (g_MisslesLib[m_nChildSkillId].m_nDamageRange > 1) 
	{
		sprintf(szTemp, "�˺���Χ:%d ", g_MisslesLib[m_nChildSkillId].m_nDamageRange);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[m_nChildSkillId].m_nKnockBack)
	{
		sprintf(szTemp, "���˾���:%d ", g_MisslesLib[m_nChildSkillId].m_nKnockBack);
		strcat(pszMsg, szTemp);
	}
	if (g_MisslesLib[m_nChildSkillId].m_bAutoExplode)
	{
		strcat(pszMsg, "�����Ա� ");
	}

	if (m_bIsAura) strcat(pszMsg, "�⻷ ");
	if (this->m_bIsPhysical) strcat(pszMsg, "���� ");
	if (this->m_bIsMelee) strcat(pszMsg, "���� ");
	if (this->m_bTargetOnly) strcat(pszMsg, "���� ");
	if (this->m_bTargetAlly) strcat(pszMsg, "���� ");
	if (this->m_bTargetEnemy) strcat(pszMsg, "�Ե� ");
	if (this->m_bTargetObj)	  strcat(pszMsg, "���� ");
	if (this->m_bTargetSelf) strcat(pszMsg, "���� ");
	if (this->m_bUseAttackRate) strcat(pszMsg, "���������� ");
	
	strcat (pszMsg, "\n");
	
	strcat(pszMsg, m_szSkillDesc);
	strcat(pszMsg, "\n");
	sprintf(szTemp, "��ǰ�ȼ�:%d", m_nLevel);
	strcat(pszMsg, szTemp);
	strcat(pszMsg, "\n");
	int i = 0;

	switch(m_nSkillCostType)
	{
	case attrib_mana:
		sprintf(szTemp, "��������:%d\n", GetSkillCost());
		strcat(pszMsg,szTemp);
		break;
	case attrib_stamina:
		sprintf(szTemp, "��������:%d\n", GetSkillCost());
		strcat(pszMsg,szTemp);
		break;
	case attrib_life:
		sprintf(szTemp, "��������:%d\n", GetSkillCost());
		strcat(pszMsg,szTemp);
		break;
	}
	
	sprintf(szTemp,"��Ч����:%d\n", GetAttackRadius());
	strcat(pszMsg,szTemp);

	
	//����ȼ��仯�������˺�
	for (i  = 0; i < m_nImmediateAttribsNum; i ++)
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
#endif
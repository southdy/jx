//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcRes.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------

#include "KCore.h"

#ifndef _SERVER

#include "KSprite.h"
#include "KNpc.h"
#include "KNpcResList.h"
#include "KNpcRes.h"
#include "ImgRef.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#include "KSubWorld.h"
#include "KOption.h"

//---------------------------------------------------------------------------
//	���ܣ�	���캯��
//---------------------------------------------------------------------------
KNpcRes::KNpcRes()
{
	m_nAction = 0;
	m_nNpcKind = 1;
	m_nHelmType = 0;
	m_nArmorType = 0;
	m_nWeaponType = 0;
	m_nHorseType = 0;
	m_bRideHorse = FALSE;
	m_nBlurState = 0;
	memset(m_szSoundName, 0, sizeof(m_szSoundName));
	memset(m_nSortTable, 0, sizeof(m_nSortTable));
	m_pSoundNode = NULL;
	m_pWave = NULL;

	m_SceneID_NPCIdx = 0;
	m_SceneID = 0;

	m_nMenuState = 0;
	m_nBackMenuState = 0;
	m_nSleepState = 0;
	//memset(m_szSentence, 0, sizeof(m_szSentence));
	//memset(m_szBackSentence, 0, sizeof(m_szBackSentence));
}

//---------------------------------------------------------------------------
//	���ܣ�	��ʼ��
//---------------------------------------------------------------------------
BOOL	KNpcRes::Init(char *lpszNpcName, KNpcResList *pNpcResList)
{
	// ��ʼ�� NpcResNode
	if (!lpszNpcName || !lpszNpcName[0])
		return FALSE;
	m_pcResNode = pNpcResList->AddNpcRes(lpszNpcName);
	if ( m_pcResNode == NULL )
		return FALSE;

	m_nNpcKind = m_pcResNode->GetNpcKind();
	m_nAction = 0;
	m_nHelmType = 0;
	m_nArmorType = 0;
	m_nWeaponType = 0;
	m_nHorseType = 0;
	m_bRideHorse = FALSE;
	memset(m_szSoundName, 0, sizeof(m_szSoundName));
	memset(m_nSortTable, 0, sizeof(m_nSortTable));
	m_pSoundNode = NULL;
	m_pWave = NULL;

	m_SceneID_NPCIdx = 0;
	m_SceneID = 0;


//	m_pSprNode = NULL;

	int		i;
	char	szBuffer[80];
	for (i = 0; i < MAX_PART; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, 0, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, 0, 16), m_pcResNode->GetTotalDirs(i, m_nAction, 0, 16), m_pcResNode->GetInterval(i, m_nAction, 0, 0));
		}
		// ����˲��������ڣ���Ӧ���ļ��������
		else
		{
			m_cNpcImage[i].Release();
		}
	}

	int		nShadowFrame, nShadowDir, nShadowInterval, nShadowCgX, nShadowCgY;
	if ( m_pcResNode->m_cShadowInfo.GetFile(
		m_nAction,
		&nShadowFrame,
		&nShadowDir,
		&nShadowInterval,
		&nShadowCgX,
		&nShadowCgY,
		szBuffer) )
	{
		this->m_cNpcShadow.SetSprFile(szBuffer, nShadowFrame, nShadowDir, nShadowInterval);
		this->m_cNpcShadow.SetCenterPos(nShadowCgX, nShadowCgY);
	}
	else
	{
		this->m_cNpcShadow.Release();
	}

	for (i = 0; i < 6; i++)
	{
		m_cStateSpr[i].Release();
	}
	m_cSpecialSpr.Release();
	m_cMenuStateSpr.Release();
	m_nMenuState = 0;
	m_nBackMenuState = 0;
	m_nSleepState = 0;
	//memset(m_szSentence, 0, sizeof(m_szSentence));
	//memset(m_szBackSentence, 0, sizeof(m_szBackSentence));

	for (i = 0; i < MAX_NPC_IMAGE_NUM; i++)
	{
		m_cDrawFile[i].nType = ISI_T_SPR;
		m_cDrawFile[i].Color.Color_b.a = 255;
		m_cDrawFile[i].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		m_cDrawFile[i].uImage = 0;
		m_cDrawFile[i].nISPosition = IMAGE_IS_POSITION_INIT;
		m_cDrawFile[i].bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
	}
	return m_cNpcBlur.Init();

}

void	KNpcRes::Remove(int nNpcIdx)
{
	if (m_SceneID)
	{
		g_ScenePlace.RemoveObject(CGOG_NPC, nNpcIdx, m_SceneID);
		m_SceneID = 0;
	}
	m_cNpcBlur.Remove();
}
//---------------------------------------------------------------------------
//	���ܣ�	����
//---------------------------------------------------------------------------
void	KNpcRes::Draw(int nNpcIdx, int nDir, int nAllFrame, int nCurFrame, BOOL bInMenu)
{
	int		i, nGetFrame = 1, nGetDir = 1, nFirst, nPos;
	int		nCurFrameNo = 0, nCurDirNo = 0;
	int		nScreenX = m_nXpos, nScreenY = m_nYpos, nScreenZ = m_nZpos;

	if (nDir < 0 || nAllFrame < 0 || nCurFrame < 0)
		return;

	if (!m_pcResNode)
		return;

	// �ⲿ���ƻ�֡
	if (nAllFrame > 0)
	{
		// ������Ӱ��ǰ֡
		nGetDir = this->m_cNpcShadow.m_nTotalDir;
		if (nGetDir <= 0)
			nGetDir = 1;
		nCurDirNo = (nDir + (32 / nGetDir)) / (64 / nGetDir);
		if (nCurDirNo >= nGetDir)
			nCurDirNo -= nGetDir;
		nGetFrame = this->m_cNpcShadow.m_nTotalFrame;
		nCurFrameNo = nCurDirNo * (nGetFrame / nGetDir) + (nGetFrame / nGetDir) * nCurFrame / nAllFrame;
		this->m_cNpcShadow.SetCurFrame(nCurFrameNo);

		// ��������
		nFirst = 0;
		// �ҵ������ĵ�ǰ��
		for (i = 0; i < MAX_PART; i++)
		{
			if (m_cNpcImage[i].CheckExist())
			{
				if (nFirst == 0)
				{
#ifdef TOOLVERSION
					KImageParam	sImage;
					if (g_pRepresent->GetImageParam(m_cNpcImage[i].m_szName, &sImage, ISI_T_SPR))
					{
						m_cNpcImage[i].m_nTotalDir = sImage.nNumFramesGroup;
						m_cNpcImage[i].m_nTotalFrame = sImage.nNumFrames;
					}
					else continue;
#endif					

					nGetDir = m_cNpcImage[i].m_nTotalDir;
					if (nGetDir <= 0)
						nGetDir = 1;

					nCurDirNo = (nDir + (32 / nGetDir)) / (64 / nGetDir);
					if (nCurDirNo >= nGetDir)
						nCurDirNo -= nGetDir;

					nGetFrame = m_cNpcImage[i].m_nTotalFrame;

					nCurFrameNo = nCurDirNo * (nGetFrame / nGetDir) + (nGetFrame / nGetDir) * nCurFrame / nAllFrame;
					m_cNpcImage[i].SetCurFrame(nCurFrameNo);
					nFirst = 1;
				}
				else
				{
					KImageParam	sImage;
					if (g_pRepresent->GetImageParam(m_cNpcImage[i].m_szName, &sImage, ISI_T_SPR))
					{
						m_cNpcImage[i].m_nTotalDir = sImage.nNumFramesGroup;
						m_cNpcImage[i].m_nTotalFrame = sImage.nNumFrames;
						m_cNpcImage[i].SetCurFrame(nCurFrameNo);
					}
				}
			}
		}
	}
	else
	{
		// ������Ӱ��ǰ֡
		if (m_cNpcShadow.SetCurDir64(nDir))
		{
			m_cNpcShadow.GetNextFrame();
		}

		// ��������
		for (i = 0; i < MAX_PART; i++)
		{
			if ( !m_cNpcImage[i].CheckExist() )
				continue;
			if ( m_cNpcImage[i].SetCurDir64(nDir) )
			{
				m_cNpcImage[i].GetNextFrame();

#ifdef TOOLVERSION
				KImageParam	sImage;
				g_pRepresent->GetImageParam(m_cNpcImage[i].m_szName, &sImage, ISI_T_SPR);
				{
					m_cNpcImage[i].m_nTotalDir = sImage.nNumFramesGroup;
					m_cNpcImage[i].m_nTotalFrame = sImage.nNumFrames;
					nGetDir = sImage.nNumFramesGroup;
					nCurFrameNo = sImage.nNumFrames;
				}
#else
				nCurDirNo = m_cNpcImage[i].m_nCurDir;
				nCurFrameNo = m_cNpcImage[i].m_nCurFrame;
#endif
			}
			
		}
	}

	// ״̬��Ч��֡
	for (i = 0; i < 6; i++)
		m_cStateSpr[i].m_SprContrul.GetNextFrame();
	if ( m_cSpecialSpr.GetNextFrame(FALSE) )
	{
		if ( m_cSpecialSpr.CheckEnd() )
			m_cSpecialSpr.Release();
	}

	// ��������
	if (nCurFrame < nAllFrame / 4)
	{
		this->GetSoundName();
		this->PlaySound(this->m_nXpos, this->m_nYpos);
	}

	// �õ���������˳��
	if ( !m_pcResNode->GetSort(m_nAction, nCurDirNo, nCurFrameNo, m_nSortTable, MAX_PART) )
		return;

// ---------------------------------- ��������б� -------------------------------
	nPos = 0;
	// ��Ӱ�ļ���
	strcpy(m_cDrawFile[nPos].szImage, this->m_cNpcShadow.m_szName);
	m_cDrawFile[nPos].uImage = m_cNpcShadow.m_dwNameID;
	m_cDrawFile[nPos].nFrame = this->m_cNpcShadow.m_nCurFrame;
	m_cDrawFile[nPos].oPosition.nX = nScreenX;
	m_cDrawFile[nPos].oPosition.nY = nScreenY;
	m_cDrawFile[nPos].oPosition.nZ = 0;//nScreenZ;
	nPos++;

	// �ŵ�״̬��Ч
	for ( i = 4; i < 6; i++)
	{
		if (m_cStateSpr[i].m_nID)
		{
			strcpy(m_cDrawFile[nPos].szImage, m_cStateSpr[i].m_SprContrul.m_szName);
			m_cDrawFile[nPos].uImage = m_cStateSpr[i].m_SprContrul.m_dwNameID;
			m_cDrawFile[nPos].nFrame = m_cStateSpr[i].m_SprContrul.m_nCurFrame;
			m_cDrawFile[nPos].oPosition.nX = nScreenX;
			m_cDrawFile[nPos].oPosition.nY = nScreenY;
			m_cDrawFile[nPos].oPosition.nZ = 0;
			nPos++;
		}
	}

	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, bInMenu);
	nPos = 0;

	// ����״̬��Ч(npc����)
	for (i = 2; i < 4; i++)
	{
		if (m_cStateSpr[i].m_nID)
		{
			if (m_cStateSpr[i].m_nBackStart <= m_cStateSpr[i].m_SprContrul.m_nCurFrame && 
				m_cStateSpr[i].m_SprContrul.m_nCurFrame < m_cStateSpr[i].m_nBackEnd)
			{
				strcpy(m_cDrawFile[nPos].szImage, m_cStateSpr[i].m_SprContrul.m_szName);
				m_cDrawFile[nPos].uImage = m_cStateSpr[i].m_SprContrul.m_dwNameID;
				m_cDrawFile[nPos].nFrame = m_cStateSpr[i].m_SprContrul.m_nCurFrame;
				m_cDrawFile[nPos].oPosition.nX = nScreenX;
				m_cDrawFile[nPos].oPosition.nY = nScreenY;
				int nHeightOff = 0;
				if (m_bRideHorse)
					nHeightOff += 38;
				m_cDrawFile[nPos].oPosition.nZ = nScreenZ + nHeightOff;
				nPos++;
			}
		}
	}

	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, bInMenu);
	nPos = 0;

	// npc����
	for (i = 0; i < MAX_PART; i++)
	{
		if (m_nSortTable[i] >= 0 && m_nSortTable[i] < MAX_PART)
		{
			if (m_ulAdjustColorId > 0 && m_ulAdjustColorId <= g_ulAdjustColorCount)
			{
				m_cDrawFile[i].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA_COLOR_ADJUST;
				m_cDrawFile[i].Color.Color_dw = g_pAdjustColorTab[m_ulAdjustColorId - 1];
			}
			else
			{
				m_cDrawFile[i].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				m_cDrawFile[i].Color.Color_b.a = 255;
			}

			strcpy(m_cDrawFile[nPos].szImage, m_cNpcImage[m_nSortTable[i]].m_szName);
			m_cDrawFile[nPos].uImage = m_cNpcImage[m_nSortTable[i]].m_dwNameID;
			m_cDrawFile[nPos].nFrame = m_cNpcImage[m_nSortTable[i]].m_nCurFrame;
			m_cDrawFile[nPos].oPosition.nX = nScreenX;
			m_cDrawFile[nPos].oPosition.nY = nScreenY;
			m_cDrawFile[nPos].oPosition.nZ = nScreenZ;
			nPos++;
		}
	}
	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, bInMenu);
	nPos = 0;
	// ����״̬��Ч(npc��ǰ)
	for (i = 2; i < 4; i++)
	{
		if (m_cStateSpr[i].m_nID)
		{
			if (m_cStateSpr[i].m_SprContrul.m_nCurFrame < m_cStateSpr[i].m_nBackStart || 
				m_cStateSpr[i].m_SprContrul.m_nCurFrame >= m_cStateSpr[i].m_nBackEnd)
			{
				strcpy(m_cDrawFile[nPos].szImage, m_cStateSpr[i].m_SprContrul.m_szName);
				m_cDrawFile[nPos].uImage = m_cStateSpr[i].m_SprContrul.m_dwNameID;
				m_cDrawFile[nPos].nFrame = m_cStateSpr[i].m_SprContrul.m_nCurFrame;
				m_cDrawFile[nPos].oPosition.nX = nScreenX;
				m_cDrawFile[nPos].oPosition.nY = nScreenY;
				int nHeightOff = 0;
				if (m_bRideHorse)
					nHeightOff += 38;
				m_cDrawFile[nPos].oPosition.nZ = nScreenZ + nHeightOff;
				nPos++;
			}
		}
	}
	// �����ֻ����һ���spr�ļ�
	if (m_cSpecialSpr.m_szName[0])
	{
		strcpy(m_cDrawFile[nPos].szImage, m_cSpecialSpr.m_szName);
		m_cDrawFile[nPos].uImage = m_cSpecialSpr.m_dwNameID;
		m_cDrawFile[nPos].nFrame = m_cSpecialSpr.m_nCurFrame;
		m_cDrawFile[nPos].oPosition.nX = nScreenX;
		m_cDrawFile[nPos].oPosition.nY = nScreenY;
		int nHeightOff = 0;
		if (m_bRideHorse)
			nHeightOff += 38;
		m_cDrawFile[nPos].oPosition.nZ = nScreenZ + nHeightOff;
		nPos++;
	}
	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, bInMenu);

	nPos = 0;
	// ͷ��״̬��Ч
	for ( i = 0; i < 2; i++)
	{
		if (m_cStateSpr[i].m_nID)
		{
			strcpy(m_cDrawFile[nPos].szImage, m_cStateSpr[i].m_SprContrul.m_szName);
			m_cDrawFile[nPos].uImage = m_cStateSpr[i].m_SprContrul.m_dwNameID;
			m_cDrawFile[nPos].nFrame = m_cStateSpr[i].m_SprContrul.m_nCurFrame;
			m_cDrawFile[nPos].oPosition.nX = nScreenX;
			m_cDrawFile[nPos].oPosition.nY = nScreenY;
			int nHeightOff = 0;
			if (m_bRideHorse)
				nHeightOff += 38;
			m_cDrawFile[nPos].oPosition.nZ = nScreenZ + nHeightOff;
			nPos++;
		}
	}
	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, bInMenu);

	nPos = 0;

// -------------------------------- ��������б� end -----------------------------

// ----------------------------------- �����Ӱ ----------------------------------
	int j = 0;
	m_cNpcBlur.ChangeAlpha();
	if (m_nBlurState == TRUE && m_cNpcBlur.NowGetBlur())
	{
		m_cNpcBlur.ClearCurNo();
		for (i = 0, j = 0; i < MAX_PART; i++)
		{
			if (m_nSortTable[i] >= 0 && m_nSortTable[i] < MAX_PART)
			{
				m_cNpcBlur.SetFile(j, m_cNpcImage[m_nSortTable[i]].m_szName, m_cNpcImage[m_nSortTable[i]].m_dwNameID, m_cNpcImage[m_nSortTable[i]].m_nCurFrame, nScreenX, nScreenY, nScreenZ);
				j++;
			}
		}
		m_cNpcBlur.SetMapPos(m_nXpos, m_nYpos, m_nZpos, nNpcIdx);

		m_cNpcBlur.SetNextNo();
	}
//	m_cNpcBlur.Draw();
// --------------------------------- �����Ӱ end --------------------------------
	// ����

//	g_pRepresent->DrawPrimitives(MAX_NPC_IMAGE_NUM, m_cDrawFile, RU_T_IMAGE, bInMenu);	

}

void	KNpcRes::GetShadowName(char *lpszShadow, char *lpszSprName)
{
	KNpcResNode::GetShadowName(lpszShadow, lpszSprName);
}

KNpcRes::~KNpcRes()
{
    // ��ΪNPC�л��Զ�����Remove
    //if (m_SceneID)
    //{
	//    //Remove(m_SceneID_NPCIdx);
    //    m_SceneID_NPCIdx = 0;
    //}
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨ͷ������
//---------------------------------------------------------------------------
BOOL	KNpcRes::SetHelm(int nHelmType)
{
	int		i;
	char	szBuffer[80];

	if (nHelmType < 0)
		return FALSE;
	if ( !m_pcResNode )
		return FALSE;
	if (m_nHelmType == nHelmType)
		return TRUE;
	m_nHelmType = nHelmType;

	for (i = MAX_BODY_PART_SECT * 0; i < MAX_BODY_PART_SECT * 0 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, nHelmType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, nHelmType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, nHelmType, 16), m_pcResNode->GetInterval(i, m_nAction, nHelmType, 0));
		}
		// ����˲��������ڣ���Ӧ���ļ��������
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	return TRUE;
}


//---------------------------------------------------------------------------
//	���ܣ�	�趨��������
//---------------------------------------------------------------------------
BOOL	KNpcRes::SetArmor(int nArmorType)
{
	int		i;

	if (nArmorType < 0)
		return FALSE;
	if ( !m_pcResNode )
		return FALSE;
	if (m_nArmorType == nArmorType)
		return TRUE;
	m_nArmorType = nArmorType;

	char	szBuffer[80];
	for (i = MAX_BODY_PART_SECT * 1; i < MAX_BODY_PART_SECT * 1 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, nArmorType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, nArmorType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, nArmorType, 16), m_pcResNode->GetInterval(i, m_nAction, nArmorType, 0));
		}
		// ����˲��������ڣ���Ӧ���ļ��������
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��������
//---------------------------------------------------------------------------
BOOL	KNpcRes::SetWeapon(int nWeaponType)
{
	int		i;

	if (nWeaponType < 0)
		return FALSE;
	if ( !m_pcResNode )
		return FALSE;
	if (m_nWeaponType == nWeaponType)
		return TRUE;
	m_nWeaponType = nWeaponType;

	m_nAction = m_pcResNode->GetActNo(m_nDoing, m_nWeaponType, m_bRideHorse);

	char	szBuffer[80];
	int		nFrame, nDir, nInterval, nCgX, nCgY;

	if ( m_pcResNode->m_cShadowInfo.GetFile(m_nAction, &nFrame, &nDir, &nInterval, &nCgX, &nCgY, szBuffer) )
	{
		m_cNpcShadow.SetSprFile(szBuffer, nFrame, nDir, nInterval);
		m_cNpcShadow.SetCenterPos(nCgX, nCgY);
	}
	else
	{
		m_cNpcShadow.Release();
	}

	for (i = MAX_BODY_PART_SECT * 0; i < MAX_BODY_PART_SECT * 0 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHelmType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHelmType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHelmType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHelmType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 1; i < MAX_BODY_PART_SECT * 1 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nArmorType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nArmorType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nArmorType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nArmorType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 2; i < MAX_BODY_PART_SECT * 2 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nWeaponType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nWeaponType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nWeaponType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nWeaponType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 3; i < MAX_BODY_PART_SECT * 3 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHorseType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHorseType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ƥ����
//---------------------------------------------------------------------------
BOOL	KNpcRes::SetHorse(int nHorseType)
{
	int		i;

	if (nHorseType < 0)
		return FALSE;
	if ( !m_pcResNode )
		return FALSE;
	if (m_nHorseType == nHorseType)
		return TRUE;
	m_nHorseType = nHorseType;

	char	szBuffer[80];
	for (i = MAX_BODY_PART_SECT * 3; i < MAX_BODY_PART_SECT * 3 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHorseType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHorseType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��������
//---------------------------------------------------------------------------
BOOL	KNpcRes::SetAction(int nDoing)
{
	if (!m_pcResNode)
		return FALSE;
	if (nDoing < 0)
		return FALSE;
	if (m_nDoing == nDoing)
		return TRUE;
	m_nDoing = nDoing;

	if (m_pcResNode)
		m_nAction = m_pcResNode->GetActNo(nDoing, m_nWeaponType, m_bRideHorse);

	int		i;
	char	szBuffer[80];
	int		nFrame, nDir, nInterval, nCgX, nCgY;

	if ( m_pcResNode->m_cShadowInfo.GetFile(m_nAction, &nFrame, &nDir, &nInterval, &nCgX, &nCgY, szBuffer) )
	{
		m_cNpcShadow.SetSprFile(szBuffer, nFrame, nDir, nInterval);
		m_cNpcShadow.SetCenterPos(nCgX, nCgY);
	}
	else
	{
		m_cNpcShadow.Release();
	}

	for (i = MAX_BODY_PART_SECT * 0; i < MAX_BODY_PART_SECT * 0 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHelmType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHelmType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHelmType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHelmType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 1; i < MAX_BODY_PART_SECT * 1 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nArmorType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nArmorType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nArmorType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nArmorType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 2; i < MAX_BODY_PART_SECT * 2 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nWeaponType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nWeaponType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nWeaponType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nWeaponType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 3; i < MAX_BODY_PART_SECT * 3 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHorseType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHorseType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨�Ƿ�����
//---------------------------------------------------------------------------
BOOL	KNpcRes::SetRideHorse(BOOL bRideHorse)
{
	if (!m_pcResNode)
		return FALSE;
	if (m_bRideHorse == bRideHorse)
		return TRUE;

	m_bRideHorse = bRideHorse;
	if (m_pcResNode)
		m_nAction = m_pcResNode->GetActNo(m_nDoing, m_nWeaponType, m_bRideHorse);

	int		i;
	char	szBuffer[80];
	int		nFrame, nDir, nInterval, nCgX, nCgY;

	if ( m_pcResNode->m_cShadowInfo.GetFile(m_nAction, &nFrame, &nDir, &nInterval, &nCgX, &nCgY, szBuffer) )
	{
		m_cNpcShadow.SetSprFile(szBuffer, nFrame, nDir, nInterval);
		m_cNpcShadow.SetCenterPos(nCgX, nCgY);
	}
	else
	{
		m_cNpcShadow.Release();
	}

	for (i = MAX_BODY_PART_SECT * 0; i < MAX_BODY_PART_SECT * 0 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHelmType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHelmType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHelmType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHelmType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 1; i < MAX_BODY_PART_SECT * 1 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nArmorType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nArmorType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nArmorType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nArmorType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 2; i < MAX_BODY_PART_SECT * 2 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nWeaponType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nWeaponType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nWeaponType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nWeaponType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}
	for (i = MAX_BODY_PART_SECT * 3; i < MAX_BODY_PART_SECT * 3 + MAX_BODY_PART_SECT; i++)
	{
		if ( m_pcResNode->CheckPartExist(i) )
		{
			m_pcResNode->GetFileName(i, m_nAction, m_nHorseType, "", szBuffer, sizeof(szBuffer));
			m_cNpcImage[i].SetSprFile(szBuffer, m_pcResNode->GetTotalFrames(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetTotalDirs(i, m_nAction, m_nHorseType, 16), m_pcResNode->GetInterval(i, m_nAction, m_nHorseType, 0));
		}
		else
		{
			m_cNpcImage[i].Release();
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨 npc λ��
//---------------------------------------------------------------------------
void	KNpcRes::SetPos(int nNpcIdx, int x, int y, int z, BOOL bFocus, BOOL bMenu)
{
	m_nXpos = x;
	m_nYpos = y;
	m_nZpos = z;

	if (bFocus)
		g_ScenePlace.SetFocusPosition(x, y, z);

	if (!bMenu)
    {
		m_SceneID_NPCIdx = nNpcIdx; 
        g_ScenePlace.MoveObject(CGOG_NPC, nNpcIdx, x, y, z, m_SceneID, IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL | IPOT_RL_LIGHT_PROP);

    }
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨״̬��Ч
//---------------------------------------------------------------------------
void	KNpcRes::SetState(KList *pNpcStateList, KNpcResList *pNpcResList)
{
	if ( !pNpcStateList || !pNpcResList)
		return;

	int		i, nFind, nFindFlag[6];
	KStateNode	*pNode;
	int		nType, nPlayType, nBackStart, nBackEnd, nTotalFrame, nTotalDir, nInterVal;
	char	szBuffer[80];

	// ������е��Ƿ�Ҫֹͣ
	memset(nFindFlag, 0, sizeof(nFindFlag));
	pNode = (KStateNode*)pNpcStateList->GetHead();
	while (pNode)
	{
		if (pNode->m_StateGraphics > 0)
		{
			for (i = 0; i < 6; i++)
			{
				if (pNode->m_StateGraphics == m_cStateSpr[i].m_nID)
					nFindFlag[i] = 1;
			}
		}
		pNode = (KStateNode*)pNode->GetNext();
	}
	for (i = 0; i < 6; i++)
	{
		if ( !nFindFlag[i] && m_cStateSpr[i].m_nID)
			m_cStateSpr[i].Release();
	}

	pNode = (KStateNode*)pNpcStateList->GetHead();
	while (pNode)
	{
		if (pNode->m_StateGraphics <= 0)
			goto WHILE_END;
		// ����Ƿ��Ѿ�����
		nFind = 0;
		for (i = 0; i < 6; i++)
		{
			if (pNode->m_StateGraphics == m_cStateSpr[i].m_nID)
			{
				nFind = 1;
				break;
			}
		}
		if (nFind > 0)
			goto WHILE_END;
		szBuffer[0] = 0;
		pNpcResList->m_cStateTable.GetInfo(pNode->m_StateGraphics, szBuffer, &nType, &nPlayType, &nBackStart, &nBackEnd, &nTotalFrame, &nTotalDir, &nInterVal);
		if ( !szBuffer[0] )
			goto WHILE_END;

		if (nType < 0 || nType >= STATE_MAGIC_TYPE_NUM)
			goto WHILE_END;
		for (i = nType * 2; i < nType * 2 + 2; i++)
		{
			if (m_cStateSpr[i].m_nID == 0)
			{
				// ����µ�
				m_cStateSpr[i].Release();
				m_cStateSpr[i].m_nID = pNode->m_StateGraphics;
				m_cStateSpr[i].m_nType = nType;
				m_cStateSpr[i].m_nPlayType = nPlayType;
				m_cStateSpr[i].m_nBackStart = nBackStart;
				m_cStateSpr[i].m_nBackEnd = nBackEnd;
				m_cStateSpr[i].m_SprContrul.SetSprFile(szBuffer, nTotalFrame, nTotalDir, nInterVal);
				break;
			}
		}
WHILE_END:
		pNode = (KStateNode*)pNode->GetNext();
	}
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨�����ֻ����һ�������spr�ļ�
//---------------------------------------------------------------------------
void	KNpcRes::SetSpecialSpr(char *lpszSprName)
{
	KImageParam	sImage;
	g_pRepresent->GetImageParam(lpszSprName, &sImage, ISI_T_SPR);
	if (sImage.nInterval <= 0)
		sImage.nInterval = 1;
	if (sImage.nInterval > 1000)
		sImage.nInterval = 1000;
	if (sImage.nNumFramesGroup <= 0)
		sImage.nNumFramesGroup = 1;
	if (sImage.nNumFrames < sImage.nNumFramesGroup)
		sImage.nNumFrames = sImage.nNumFramesGroup;
	m_cSpecialSpr.SetSprFile(lpszSprName, sImage.nNumFrames, sImage.nNumFramesGroup, (sImage.nNumFrames / sImage.nNumFramesGroup) * sImage.nInterval / 50);
}

//---------------------------------------------------------------------------
//	���ܣ�set menu state spr
//---------------------------------------------------------------------------
void	KNpcRes::SetMenuStateSpr(int nMenuState)
{
	if (nMenuState < PLAYER_MENU_STATE_NORMAL || nMenuState >= PLAYER_MENU_STATE_NUM)
	{
		this->m_cMenuStateSpr.Release();
		return;
	}
	char	szName[80];
	g_NpcResList.m_cMenuState.GetStateSpr(nMenuState, szName);
	if (szName[0])
	{
		KImageParam	sImage;
		g_pRepresent->GetImageParam(szName, &sImage, ISI_T_SPR);
		if (sImage.nInterval <= 0)
			sImage.nInterval = 1;
		if (sImage.nInterval > 1000)
			sImage.nInterval = 1000;
		if (sImage.nNumFramesGroup <= 0)
			sImage.nNumFramesGroup = 1;
		if (sImage.nNumFrames < sImage.nNumFramesGroup)
			sImage.nNumFrames = sImage.nNumFramesGroup;
		m_cMenuStateSpr.SetSprFile(szName, sImage.nNumFrames, sImage.nNumFramesGroup, (sImage.nNumFrames / sImage.nNumFramesGroup) * sImage.nInterval / 50);
	}
	else
	{
		this->m_cMenuStateSpr.Release();
	}
}

//---------------------------------------------------------------------------
//	���ܣ�	��Ӱ�򿪹ر�
//	������	bBlur	if == TRUE  ��  if == FLASE  �ر�
//---------------------------------------------------------------------------
void	KNpcRes::SetBlur(BOOL bBlur)
{
	if (m_nBlurState == bBlur)
		return;

	m_nBlurState = bBlur;
	if (bBlur)
	{
		m_cNpcBlur.AddObj();
	}
	else
	{
		m_cNpcBlur.RemoveObj();
	}
}


//---------------------------------------------------------------------------
//	���ܣ�	��õ�ǰ��������Ч�ļ���
//---------------------------------------------------------------------------
void	KNpcRes::GetSoundName()
{
	if (m_pcResNode)
		m_pcResNode->GetActionSoundName(this->m_nAction, this->m_szSoundName);
}

//---------------------------------------------------------------------------
//	���ܣ�	���ŵ�ǰ��������Ч
//---------------------------------------------------------------------------
void	KNpcRes::PlaySound(int nX, int nY)
{
	if (!m_szSoundName[0])
		return;

	int		nCenterX = 0, nCenterY = 0, nCenterZ = 0;

	// �����Ļ���ĵ�ĵ�ͼ���� not end
	g_ScenePlace.GetFocusPosition(nCenterX, nCenterY, nCenterZ);

	m_pSoundNode = (KCacheNode*) g_SoundCache.GetNode(m_szSoundName, (KCacheNode*)m_pSoundNode);
	m_pWave = (KWavSound*)m_pSoundNode->m_lpData;
	if (m_pWave)
	{
		if (m_pWave->IsPlaying())
			return;
		int nVol = -(abs(nX - nCenterX) + abs(nY - nCenterY));
		m_pWave->Play((nX - nCenterX) * 5,  GetSndVolume(nVol), 0);
	}
}

int	KNpcRes::GetSndVolume(int nVol)
{
	return (10000 + nVol) * Option.GetSndVolume() / 100 - 10000;
}

void	KNpcRes::StopSound()
{
	m_pSoundNode = (KCacheNode*)g_SoundCache.GetNode(m_szSoundName, (KCacheNode*)m_pSoundNode);
	m_pWave = (KWavSound*)m_pSoundNode->m_lpData;
	if (m_pWave)
	{
		m_pWave->Stop();
	}
}
//---------------------------------------------------------------------------
//	���ܣ��趨ͷ��״̬
//---------------------------------------------------------------------------
void	KNpcRes::SetMenuState(int nState, char *lpszSentence, int nSentenceLength)
{
	if (nState < PLAYER_MENU_STATE_NORMAL || nState >= PLAYER_MENU_STATE_NUM)
		return;

	if (nState != m_nMenuState)
	{
		m_nBackMenuState = m_nMenuState;
		//strcpy(m_szBackSentence, m_szSentence);
		m_nMenuState = nState;
	}

	if (nSentenceLength > 0 && lpszSentence)
	{
		if (nSentenceLength >= MAX_SENTENCE_LENGTH)
			nSentenceLength = MAX_SENTENCE_LENGTH - 1;
		//memcpy(m_szSentence, lpszSentence, nSentenceLength);
		//m_szSentence[nSentenceLength] = 0;
	}
	else
	{
		//m_szSentence[0] = 0;
	}

	if (!m_nSleepState)
		SetMenuStateSpr(m_nMenuState);
}

//---------------------------------------------------------------------------
//	���ܣ����ͷ��״̬
//---------------------------------------------------------------------------
int		KNpcRes::GetMenuState()
{
	if (m_nSleepState)
		return m_nSleepState;
	return this->m_nMenuState;
}

//---------------------------------------------------------------------------
//	���ܣ��趨˯��״̬
//---------------------------------------------------------------------------
void	KNpcRes::SetSleepState(BOOL bFlag)
{
	if (bFlag)
	{
		m_nSleepState = PLAYER_MENU_STATE_IDLE;
		SetMenuStateSpr(m_nSleepState);
	}
	else
	{
		m_nSleepState = 0;
		if (m_nMenuState)
		{
			SetMenuStateSpr(m_nMenuState);
		}
		else
		{
			SetMenuStateSpr(PLAYER_MENU_STATE_NORMAL);
		}
	}
}

//---------------------------------------------------------------------------
//	���ܣ����˯��״̬
//---------------------------------------------------------------------------
BOOL	KNpcRes::GetSleepState()
{
	return (m_nSleepState ? 1 : 0);
}

//---------------------------------------------------------------------------
//	���ܣ�����npc�ı߿�(3Dģʽ�и�Ϊ����)
//---------------------------------------------------------------------------
void	KNpcRes::DrawBorder()
{
	if (!m_pcResNode)
		return;

	int		i, nPos = 0;

	for (i = 0; i < MAX_PART; i++)
	{
		if (m_nSortTable[i] >= 0 && m_nSortTable[i] < MAX_PART)
		{
			strcpy(m_cDrawFile[nPos].szImage, m_cNpcImage[m_nSortTable[i]].m_szName);
			m_cDrawFile[nPos].uImage = m_cNpcImage[m_nSortTable[i]].m_dwNameID;
			m_cDrawFile[nPos].nFrame = m_cNpcImage[m_nSortTable[i]].m_nCurFrame;
			m_cDrawFile[nPos].oPosition.nX = m_nXpos;
			m_cDrawFile[nPos].oPosition.nY = m_nYpos;
			m_cDrawFile[nPos].oPosition.nZ = m_nZpos;
			m_cDrawFile[nPos].bRenderStyle = IMAGE_RENDER_STYLE_BORDER;
			nPos++;
		}
	}
	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, FALSE);
	for (i = 0; i < nPos; i++)
		m_cDrawFile[i].bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	nPos = 0;
}

//��ָ���߶Ȼ���ͷ��״̬
int	KNpcRes::DrawMenuState(int nHeightOffset)
{
	int		nScreenX = m_nXpos, nScreenY = m_nYpos, nScreenZ = 0;

	if (!m_pcResNode)
		return nHeightOffset;

	// ͷ��״̬��Ч
	int i;
	for ( i = 0; i < 2; i++)
	{
		if (m_cStateSpr[i].m_nID)
		{
			return nHeightOffset;	//��ͷ����Чʱ�����ƽ��׵�״̬
		}
	}

	int nPos = 0;
	nHeightOffset += 10;
	// MenuState
	if (m_cMenuStateSpr.m_szName[0])
	{
		m_cMenuStateSpr.GetNextFrame();

		strcpy(m_cDrawFile[nPos].szImage, m_cMenuStateSpr.m_szName);
		m_cDrawFile[nPos].uImage = m_cMenuStateSpr.m_dwNameID;
		m_cDrawFile[nPos].nFrame = m_cMenuStateSpr.m_nCurFrame;
		m_cDrawFile[nPos].oPosition.nX = nScreenX;
		m_cDrawFile[nPos].oPosition.nY = nScreenY;
		m_cDrawFile[nPos].oPosition.nZ = nScreenZ + nHeightOffset;
		nPos++;
	}
	g_pRepresent->DrawPrimitives(nPos, m_cDrawFile, RU_T_IMAGE, false);

	return nHeightOffset;
}

//---------------------------------------------------------------------------
//	���ܣ�����֡��ת�����߼�����(0 - 63)
//---------------------------------------------------------------------------
int		KNpcRes::GetNormalNpcStandDir(int nFrame)
{
	if (!m_pcResNode)
		return 0;

	int nTotalFrames = m_pcResNode->GetTotalFrames(NORMAL_NPC_PART_NO, cdo_stand, m_nHelmType, 16);
	if (nTotalFrames <= 0)
		return 0;

	nFrame %= nTotalFrames;

	return (MAX_NPC_DIR * nFrame) / nTotalFrames;
}


//---------------------------------------------------------------------------
//	���ܣ�	���캯��
//---------------------------------------------------------------------------
KStateSpr::KStateSpr()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ�	��գ���ʼ��
//---------------------------------------------------------------------------
void	KStateSpr::Release()
{
	m_nID = 0;
	m_nType = 0;
	m_nPlayType = 0;
	m_nBackStart = 0;
	m_nBackEnd = 0;
	m_SprContrul.Release();
}

//---------------------------------------------------------------------------
//	���ܣ�	���캯��
//---------------------------------------------------------------------------
KNpcBlur::KNpcBlur()
{
	m_nActive = 0;
	m_nCurNo = 0;
	m_dwInterval = 3;
	m_dwTimer = 0;
}

//---------------------------------------------------------------------------
//	���ܣ�	��������
//---------------------------------------------------------------------------
KNpcBlur::~KNpcBlur()
{
    //Remove();
}


//---------------------------------------------------------------------------
//	���ܣ�	��ǰ���ָ��ָ����һ��(�ܹ�7����ָ��ѭ��)
//---------------------------------------------------------------------------
void	KNpcBlur::SetNextNo()
{
	m_nCurNo++;
	if (m_nCurNo >= MAX_BLUR_FRAME)
		m_nCurNo = 0;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰ��Ӱ֡��ͼ����
//---------------------------------------------------------------------------
void	KNpcBlur::SetMapPos(int x, int y, int z, int nNpcIdx)
{
	m_nMapXpos[m_nCurNo] = x;
	m_nMapYpos[m_nCurNo] = y;
	m_nMapZpos[m_nCurNo] = z;
    m_SceneIDNpcIdx[m_nCurNo] = nNpcIdx;
	g_ScenePlace.MoveObject(CGOG_NPC_BLUR_DETAIL(m_nCurNo), nNpcIdx, x, y, z, m_SceneID[m_nCurNo]);
}

//---------------------------------------------------------------------------
//	���ܣ�	�ı�alpha��
//---------------------------------------------------------------------------
void	KNpcBlur::ChangeAlpha()
{
	if (m_nActive == 0)
		return;

	int		i, j;
	int		nScreenX, nScreenY, nScreenZ;
	for (i = 0; i < MAX_BLUR_FRAME; i++)
	{
		nScreenX = m_nMapXpos[i];
		nScreenY = m_nMapYpos[i];
		nScreenZ = m_nMapZpos[i];
//		SubWorld[0].Mps2Screen(&nScreenX, &nScreenY);
		for (j = 0; j < MAX_PART; j++)
		{
			if (m_Blur[i][j].Color.Color_b.a)
			{
				m_Blur[i][j].oPosition.nX = nScreenX;
				m_Blur[i][j].oPosition.nY = nScreenY;
				m_Blur[i][j].oPosition.nZ = nScreenZ;
			}
		}
	}

	m_dwTimer++;
	if (m_dwTimer < m_dwInterval)
		return;
	m_dwTimer = 0;

	m_nActive = 0;
	for (i = 0; i < MAX_BLUR_FRAME; i++)
	{
		for (j = 0; j < MAX_PART; j++)
		{
			if (m_Blur[i][j].Color.Color_b.a)
			{
				if (m_Blur[i][j].Color.Color_b.a > BLUR_ALPHA_CHANGE)
					m_Blur[i][j].Color.Color_b.a -= BLUR_ALPHA_CHANGE;
				else
					m_Blur[i][j].Color.Color_b.a = 0;
				m_nActive = 1;
			}
		}
	}
}

//---------------------------------------------------------------------------
//	���ܣ�	��յ�ǰָ��ָ�������
//---------------------------------------------------------------------------
void	KNpcBlur::ClearCurNo()
{
	for (int i = 0; i < MAX_PART; i++)
	{
//		m_Blur[m_nCurNo][i].Release();
		m_Blur[m_nCurNo][i].Color.Color_b.a = 0;
	}
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰĳһ�������
//---------------------------------------------------------------------------
void	KNpcBlur::SetFile(int nNo, char *lpszFileName, int nSprID, int nFrameNo, int nXpos, int nYpos, int nZpos)
{
	if (nNo < 0 || nNo >= MAX_PART)
		return;
	if (!lpszFileName)
		return;
	strcpy(m_Blur[m_nCurNo][nNo].szImage, lpszFileName);
	m_Blur[m_nCurNo][nNo].uImage = nSprID;
	m_Blur[m_nCurNo][nNo].nFrame = nFrameNo;
	m_Blur[m_nCurNo][nNo].oPosition.nX = nXpos;
	m_Blur[m_nCurNo][nNo].oPosition.nY = nYpos;
	m_Blur[m_nCurNo][nNo].oPosition.nZ = nZpos;
	m_Blur[m_nCurNo][nNo].Color.Color_b.a = START_BLUR_ALPHA;
	m_nActive = 1;
}

//---------------------------------------------------------------------------
//	���ܣ�	���Ʋ�Ӱ
//---------------------------------------------------------------------------
void	KNpcBlur::Draw(int nIdx)
{
	if (m_nActive == 0)
		return;

	g_pRepresent->DrawPrimitives(MAX_PART, m_Blur[nIdx], RU_T_IMAGE, FALSE);
}

//---------------------------------------------------------------------------
//	���ܣ�	����ʱ���ж��Ƿ�ȡ��Ӱ
//---------------------------------------------------------------------------
BOOL	KNpcBlur::NowGetBlur()
{
	if (m_dwTimer == 0)
		return TRUE;
	return FALSE;
}

BOOL	KNpcBlur::Init()
{
	for (int i = 0; i < MAX_BLUR_FRAME; i++)
	{
		for (int j = 0; j < MAX_PART; j++)
		{
			m_Blur[i][j].nType = ISI_T_SPR;
			m_Blur[i][j].uImage = 0;
			m_Blur[i][j].nISPosition = IMAGE_IS_POSITION_INIT;
			m_Blur[i][j].bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		}
	}
	return TRUE;
}

void	KNpcBlur::Remove()
{
	for (int i = 0; i < MAX_BLUR_FRAME; i++)
	{
		if (m_SceneID[i])
		{
			g_ScenePlace.RemoveObject(CGOG_NPC_BLUR_DETAIL(i), m_SceneIDNpcIdx[i], m_SceneID[i]);
			m_SceneID[i] = 0;
		}
	}
}

void	KNpcBlur::AddObj()
{
}

void	KNpcBlur::RemoveObj()
{
}

#endif
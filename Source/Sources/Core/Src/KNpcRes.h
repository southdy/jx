
#pragma once

#ifndef _SERVER

#include "KWavSound.h"
#include "KList.h"
#include "KNpcResNode.h"
#include "KNpcResList.h"
#include "KSprControl.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"

class KCacheNode;

class KStateSpr
{

public:
	int				m_nID;						// �ڱ���ļ��е�λ�ã��� 1 ��ʼ��0Ϊ�գ�
	int				m_nType;					// ���ͣ�ͷ�����ŵס�����
	int				m_nPlayType;				// ��������
	int				m_nBackStart;				// �������� ����ʼ֡
	int				m_nBackEnd;					// �������� �������֡
	KSprControl		m_SprContrul;				// spr ����
public:
	KStateSpr();
	void			Release();
};

#define		MAX_BLUR_FRAME		7
#define		START_BLUR_ALPHA	128
#define		BLUR_ALPHA_CHANGE	16

class KNpcBlur
{
public:
	int				m_nActive;							// ��ǰ��Ӱ�����Ƿ��ڼ���״̬
	int				m_nCurNo;							// ��ǰָ֡��
	DWORD			m_dwTimer;							// ʱ�������
	DWORD			m_dwInterval;						// ����֡ȡһ�β�Ӱ
	int				m_nMapXpos[MAX_BLUR_FRAME];			// ��Ӧ�ĵ�ͼ���� x
	int				m_nMapYpos[MAX_BLUR_FRAME];			// ��Ӧ�ĵ�ͼ���� y
	int				m_nMapZpos[MAX_BLUR_FRAME];			// ��Ӧ�ĵ�ͼ���� z
	unsigned int	m_SceneIDNpcIdx[MAX_BLUR_FRAME];
	unsigned int	m_SceneID[MAX_BLUR_FRAME];			// 
	KRUImage		m_Blur[MAX_BLUR_FRAME][MAX_PART];	// ��Ӱ�����б�
public:
	KNpcBlur();
	~KNpcBlur();
	BOOL			Init();
	void			Remove();
	void			SetNextNo();
	void			SetMapPos(int x, int y, int z, int nNpcIdx);
	void			ChangeAlpha();
	void			ClearCurNo();
	void			SetFile(int nNo, char *lpszFileName, int nSprID, int nFrameNo, int nXpos, int nYpos, int nZpos);
	void			Draw(int nIdx);
	BOOL			NowGetBlur();
	void			AddObj();
	void			RemoveObj();
};

#ifdef TOOLVERSION
class CORE_API  KNpcRes
#else
class KNpcRes
#endif
{

	enum
	{
		SHADOW_BEGIN	= 0,
		STATE_BEGIN		= 1,
		PART_BEGIN		= 1 + 6,
		SPEC_BEGIN		= 1 + MAX_PART + 6,
		MENUSTATE_BEGIN = 1 + MAX_PART + 6 + 1,
	};
private:
	int				m_nDoing;							// Npc�Ķ���
	int				m_nAction;							// Npc��ʵ�ʶ������������������йأ�
	int				m_nNpcKind;							// ���� ��ͨ
	int				m_nXpos;							// ���� x
	int				m_nYpos;							// ���� y
	int				m_nZpos;							// ���� z
	unsigned int 	m_SceneID_NPCIdx;                   // �ڳ����е�ID ��Ӧ��NPCidx
	unsigned int	m_SceneID;							// �ڳ����е�ID
	int				m_nHelmType;						// ��ǰͷ������
	int				m_nArmorType;						// ��ǰ��������
	int				m_nWeaponType;						// ��ǰ��������
	int				m_nHorseType;						// ��ǰ��ƥ����
	BOOL			m_bRideHorse;						// ��ǰ�Ƿ�����
	int				m_nBlurState;
	char			m_szSoundName[80];					// ��ǰ��Ч�ļ���
	KCacheNode		*m_pSoundNode;						// ��Чָ��
	KWavSound		*m_pWave;							// ��Чwavָ��
public:
	enum
	{
		adjustcolor_physics = 0,		// �����˺�
			adjustcolor_poison, 
			adjustcolor_freeze,			// �����˺�
			adjustcolor_burn,			// �����˺�
			adjustcolor_confuse,			// �����˺�
			adjustcolor_stun,			// �����˺�
	};
	KSprControl		m_cNpcImage[MAX_PART];				// ���ж���������spr�ļ���
	KSprControl		m_cNpcShadow;						// npc��Ӱ
	KStateSpr		m_cStateSpr[6];						// ״̬��Ч��0 1 Ϊͷ�� 2 3 Ϊ�ŵ� 4 5 Ϊ����
	KSprControl		m_cSpecialSpr;						// �����ֻ����һ�������spr�ļ�
	unsigned int	m_ulAdjustColorId;

	KSprControl		m_cMenuStateSpr;
	int				m_nMenuState;
	int				m_nBackMenuState;
	int				m_nSleepState;
	//char			m_szSentence[MAX_SENTENCE_LENGTH];
	//char			m_szBackSentence[MAX_SENTENCE_LENGTH];
	
	int				m_nSortTable[MAX_PART];				// �����

	KRUImage		m_cDrawFile[MAX_NPC_IMAGE_NUM];// �����б� ���岿�� + ��Ӱ + ħ��״̬ + ���⶯�� + ͷ��״̬
	KNpcBlur		m_cNpcBlur;							// npc ��Ӱ

	KNpcResNode		*m_pcResNode;						// npc ��Դ

private:
	// ��һ��ͼ����Դ�ļ����õ�������Ӱͼ���ļ���
	void			GetSoundName();						// ��õ�ǰ��������Ч�ļ���
	void			PlaySound(int nX, int nY);			// ���ŵ�ǰ��������Ч
	void			SetMenuStateSpr(int nMenuState);					// set menu state spr
public:
	KNpcRes();
	~KNpcRes();
	BOOL			Init(char *lpszNpcName, KNpcResList *pNpcResList);	// ��ʼ��
	void			Remove(int nNpcIdx);								// ���
	void			Draw(int nNpcIdx, int nDir, int nAllFrame, int nCurFrame, BOOL bInMenu = FALSE);		// ����
	void			DrawBorder();
	int				DrawMenuState(int nHeightOffset);
	BOOL			SetHelm(int nHelmType);								// �趨ͷ������
	BOOL			SetArmor(int nArmorType);							// �趨��������
	BOOL			SetWeapon(int nWeaponType);							// �趨��������
	BOOL			SetHorse(int nHorseType);							// �趨��ƥ����
	BOOL			SetAction(int nDoing);								// �趨��������
	int				GetHelm(){return m_nHelmType;};
	int				GetArmor(){return m_nArmorType;	};
	int				GetWeapon(){return m_nWeaponType;};
	BOOL			SetRideHorse(BOOL bRideHorse);						// �趨�Ƿ�����
	void			SetPos(int nNpcIdx, int x, int y, int z = 0, BOOL bFocus = FALSE, BOOL bMenu = FALSE);// �趨 npc λ��
	void			SetState(KList *pNpcStateList, KNpcResList *pNpcResList);	// �趨״̬��Ч
	void			SetSpecialSpr(char *lpszSprName);					// �趨�����ֻ����һ�������spr�ļ�
	void			SetBlur(BOOL bBlur);								// ��Ӱ�򿪹ر�
	void			SetAdjustColorId(unsigned long ulColorId){m_ulAdjustColorId = ulColorId;};			// ����ƫɫ��������Ϊ0��ʾ��ƫɫ.
	int				GetAction(){return m_nAction;};
	void			SetMenuState(int nState, char *lpszSentence = NULL, int nSentenceLength = 0);	// �趨ͷ��״̬
	int				GetMenuState();						// ���ͷ��״̬
	void			SetSleepState(BOOL bFlag);			// �趨˯��״̬
	BOOL			GetSleepState();						// ���˯��״̬
	void			StopSound();
	int				GetSndVolume(int nVol);
	static void		GetShadowName(char *lpszShadow, char *lpszSprName);
	int				GetNormalNpcStandDir(int nFrame);	// ����֡��ת�����߼�����(0 - 63)
};
#endif

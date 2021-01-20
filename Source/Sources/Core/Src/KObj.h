//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObj.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KObj_H
#define KObj_H

#ifdef	_SERVER
#define	MAX_OBJECT	20000
#else
#define	MAX_OBJECT	256
#endif

#include "KPolygon.h"

#ifndef _SERVER
#include "KSprite.h"
#include "KWavSound.h"
#include "KSprControl.h"
#endif

#include "KIniFile.h"
#include "GameDataDef.h"
#ifndef	_SERVER
#include "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#define		OBJ_BAR_WIDTH		3								// ����ϰ� ��
#define		OBJ_BAR_HEIGHT		5								// ����ϰ� ��
#define		OBJ_BAR_SIZE		(OBJ_BAR_WIDTH*OBJ_BAR_HEIGHT)	// ����ϰ� ��x��

#define		OBJ_BOX_STATE_CLOSE		0		// ����״̬ ��
#define		OBJ_BOX_STATE_OPEN		1		// ����״̬ ��
#define		OBJ_DOOR_STATE_CLOSE	0		// ��״̬ ��
#define		OBJ_DOOR_STATE_OPEN		1		// ��״̬ ��
#define		OBJ_PROP_STATE_DISPLAY	0		// ����״̬ �� ����ʾ
#define		OBJ_PROP_STATE_HIDE		1		// ����״̬ �� ��ʾ
#define		OBJ_TRAP_STATE_STOP		0		// ����״̬ ��
#define		OBJ_TRAP_STATE_ACTIVE	1		// ����״̬ ��
#define		OBJ_TRAP_STATE_ACTING	2		// ����״̬ ���ڶ���

#define		OBJ_BELONG_TIME		600

enum	Obj_Bar_Type			// �����������ϰ�����
{
	Obj_Bar_Empty = 0,			// �գ����ϰ�
	Obj_Bar_Full,				// �����ϰ�
	Obj_Bar_LT,					// ��������Ϊ�ϰ�
	Obj_Bar_RT,					// ��������Ϊ�ϰ�
	Obj_Bar_LB,					// ��������Ϊ�ϰ�
	Obj_Bar_RB,					// ��������Ϊ�ϰ�
	Obj_Bar_Type_Num,			// �ϰ�������
};

typedef struct SOBJ_LIGHT_DATA
{
	int			m_nRadius;			// ����Ĺ�����
	int			m_nRed;				// ��ƫɫ
	int			m_nGreen;			// ��ƫɫ
	int			m_nBlue;			// ��ƫɫ
	int			m_nAlpha;			// alpha ֵ
	int			m_nReflectType;		// �ܹⷽʽ�����ܹ⡢���ġ�ˮƽ����б����б��
	SOBJ_LIGHT_DATA() {Release();};
	void	Release() {	m_nRadius = 0; m_nRed = 0; m_nGreen = 0; m_nBlue = 0; m_nAlpha = 0; m_nReflectType = 0; };
} OBJ_LIGHT;		// ����������

enum	ObjTrap_Kind
{
	ObjTrap_Kind_Cycle_Image_Static = 0,
	ObjTrap_Kind_Cycle_Image_Dir,
	ObjTrap_Kind_Auto_Image_Static,
	ObjTrap_Kind_Auto_Image_Dir,
	ObjTrap_Kind_Auto_Image_Target,
	ObjTrap_Kind_Auto_Delay_Static,
	ObjTrap_Kind_Auto_Delay_Dir,
	ObjTrap_Kind_Auto_Delay_Target,
};

typedef struct OBJ_SKILL_DATA
{
	int		m_nKind;			// ��ͼ��������
	int		m_nCamp;			// ������������һ��npc
	int		m_nRange;			// ���崥����Χ
	int		m_nCastTime;		// �ӵ�����ʱ��
	int		m_nID;				// ��������
	int		m_nLevel;			// ���ܵȼ�
	int		m_nTarX;			// Ŀ������ x
	int		m_nTarY;			// Ŀ������ y
	OBJ_SKILL_DATA() {Release();};
	void	Release() {m_nKind = 0; m_nCamp = camp_animal; m_nRange = 0; m_nCastTime = 0; m_nID = 0; m_nLevel = 0; m_nTarX = 0; m_nTarY = 0;};
} OBJ_SKILL;		// ��ͼ�������(���似��)

//---------------------------------------------------------------------------

#ifdef _SERVER
class ServerImage
{
public:
	int			m_nTotalFrame;						// ��֡��
	int			m_nCurFrame;						// ��ǰ֡
	int			m_nTotalDir;						// �ܷ�����
	int			m_nCurDir;							// ��ǰ����
	DWORD		m_dwTimer;							// ʱ�������
	DWORD		m_dwInterval;						// ֡����������ٶȣ�

protected:
	int			m_nDirFrames;						// ÿ����֡��

public:
	ServerImage();									// ���캯��
	void		Release();							// ���
	void		SetTotalFrame(int nTotalFrame);		// �趨��֡��
	void		SetCurFrame(int nCurFrame);			// �趨��ǰ֡
	void		SetTotalDir(int nTotalDir);			// �趨�ܷ�����
	BOOL		SetCurDir(int nDir);				// �趨��ǰ����
	void		SetInterVal(DWORD dwInterval);		// �趨֡���
	int			GetOneDirFrames();					// ��õ�����֡��

	BOOL		GetNextFrame(BOOL bLoop = TRUE);	// �Զ����֡
	BOOL		GetPrevFrame(BOOL bLoop = TRUE);	// �Զ���ǰ��֡
	BOOL		CheckEnd();							// �ж϶����Ƿ񲥷ŵ����
	void		SetDirStart();						// �趨��ǰ����ĵ�ǰ֡Ϊ��һ֡
	void		SetDirEnd();						// �趨��ǰ����ĵ�ǰ֡Ϊ���һ֡
	BOOL		SetCurDir64(int nDir);				// �趨��ǰ����(�������64����ת���������ķ���)
	int			GetCurDirFrameNo();
};
#endif
#ifdef TOOLVERSION
class CORE_API KObj
#else
class KObj
#endif
{
	friend class KObjSet;
public:
	int			m_nID;					// ���������ΨһID���ͻ������������һһ��Ӧ�����ֵΪ 0 ����Ϊ�ͻ��� obj ����������û�ж�Ӧ�� obj
	int			m_nDataID;				// �������������ݱ��е�λ��(������ģ���еı��)
	int			m_nKind;				// ����	��Ӧ Obj_Kind_XXXX
	int			m_nIndex;				// �� Object �����е�λ��
	int			m_nBelongRegion;		// �����������ĸ�region���������꣬������Ա䶯������������ʱ�Ǹ�region���ͻ��˴洢���� region id

	int			m_nSubWorldID;			// ������ID
	int			m_nRegionIdx;			// ����idx
	int			m_nMapX;				// ��ͼ�������� x
	int			m_nMapY;				// ��ͼ�������� y
	int			m_nOffX;				// ��ͼ�����е�ƫ�� x (ע����ֵΪ���ص�ֵ * 1024)
	int			m_nOffY;				// ��ͼ�����е�ƫ�� y (ע����ֵΪ���ص�ֵ * 1024)
	int			m_nDir;					// ���򣬴˷���Ϊ������߼����򣬰�64������㣬���Ǿ����ͼ����

	int			m_nState;				// ״̬
	int			m_nLifeTime;			// �������ʧʱ�䣨��Ҫ����ʬ���ࡢװ���ࡢ������ȣ�
	int			m_nBornTime;			// ��������ʱ��box��״̬����lifetime�Զ��رգ�prop����״̬����lifetime�Զ���ʾ��
	int			m_nWaitTime;			// �ȴ�ʱ�䣨����obj trap����ʱ���ã�
	OBJ_SKILL	m_cSkill;				// �������ħ�����ܣ�����obj trap����ʱ���ã�

	char		m_szName[32];			// �������
//	char		m_szScriptName[80];		// �����ű��ļ���
	DWORD		m_dwScriptID;			// �����ű��ļ�ID
	int			m_nColorID;				// ���������ʾʱ����ɫid���ͻ���ͨ������ҵ���Ӧ��ɫ

	int			m_nItemDataID;			// ������Ʒ��ָ����Ʒ�ľ�������
	int			m_nItemWidth;			// ��Ʒ���������������е�ʱ��
	int			m_nItemHeight;			// ��Ʒ�������������е�ʱ��
	int			m_nMoneyNum;			// ����ǵ��ڵ��ϵ�Ǯ��Ǯ������

#ifdef _SERVER
	// ����Ǵ�����������װ����Ǯ��˭�������(��¼����player index) if == -1 ���������κ��ˣ��κ��˶��ܼ�
	int			m_nBelong;
	// m_nBelong �����õ�ʱ��(�Ǹ�����ʱ)
	int			m_nBelongTime;

	ServerImage m_cImage;				// ��������ͼ�����
#endif

#ifndef _SERVER
	char		m_szImageName[80];		// ͼ���ļ���
	char		m_szSoundName[80];		// �����ļ���
	KSprControl	m_cImage;				// ���ͼ��

	char		m_szImageDropName[80];	// �����item���ͣ���������ʱ�򲥷ŵĶ����ļ���
	KSprControl	m_cImageDrop;			// �����item���ͣ���������ʱ�򲥷ŵĶ�����ͼ�����
	int			m_nDropState;			// �����item���ͣ���ǰ�ǵ�������״̬(1)���Ƿ����ڵذ���״̬(0)

	KRUImage	m_Image;
	unsigned int	m_SceneID;

	int			m_nLayer;				// ���λ�ڵڼ���
	int			m_nHeight;				// ������߼��߶�
	OBJ_LIGHT	m_sObjLight;			// ��������ȡ�������ص�����
	DWORD		m_dwNameColor;			// ������ֵ���ɫ
#endif

	KPolygon	Polygon;				// ��������
	BYTE		m_btBar[OBJ_BAR_SIZE];	// ��������ϰ���Ϣ

private:
	DWORD		m_dwTimer;				// ʱ�������(������������)

#ifndef _SERVER
	KCacheNode*	m_pSoundNode;			// ��Чָ��
	KWavSound*	m_pWave;				// ��Чwavָ��

	BOOL		m_bDrawFlag;			// �����Ƿ����⴦��FALSE ����   TRUE �����ƣ�
#endif

public:
	KObj();
	void		GetMpsPos(int *pX, int *pY);
	void		Release();
	void		Activate();
//	void		Save(KIniFile *IniFile, LPSTR Section);
//	void		Load(int nObjIndex, int nSubWorldID, KIniFile *IniFile, LPSTR Section);

	void		ExecScript(int nPlayerIdx);
	void		CastSkill(int nDir);
	void		CastSkill(int nXpos, int nYpos);

	void		SetIndex(int nIndex);
	void		SetWorldID(int nID);			// ���������Ψһ ID
	void		SetLifeTime(int nLifeTime);
	BOOL		SetDir(int n64Dir);
	void		SetState(int nState, int nPlaySoundFlag = 0);
	void		SetImageDir(int nDir);
	void		Remove(BOOL bSoundFlag);
	void		SetScriptFile(char *lpszScriptFile);
	int			GetKind();

#ifdef _SERVER
	int			FindEnemy();
	void		TrapAct();
	void		SetItemDataID(int nItemDataID);
	BOOL		SyncAdd(int nClient);
	void		SyncState();
	void		SyncDir();
	void		SyncRemove(BOOL bSoundFlag);
	int			GetItemDataID();
	void		SetItemBelong(int nPlayerIdx);
	void		SetEntireBelong(int nPlayerIdx);
#endif

#ifndef _SERVER
	void		Draw();
	void		DrawInfo();
	void		DrawBorder();
#endif

	// ͹�����ת��Ϊ�ϰ���Ϣ
	void		PolygonChangeToBar(
					KPolygon Polygon,		// ͹�����
					int nGridWidth,			// ���ӳ�
					int nGridHeight,		// ���ӿ�
					int nTableWidth,		// ���
					int nTableHeight,		// ����
					BYTE *lpbBarTable);		// �������

private:
	void		BoxOpen();
	void		BoxClose();
	void		DoorOpen();
	void		DoorClose();
#ifndef _SERVER
	void		PlayLoopSound();
	void		PlayRandSound();
	int			GetSoundPan();
	int			GetSoundVolume();
	void		PlaySound();
#endif
};

extern	KObj	Object[MAX_OBJECT];
//---------------------------------------------------------------------------
#endif

/*
��ͼ�����������ڵ�ͼ�ļ��� obj �����У������������д������ϰ���ϢԤ�ȴ洢
          �ڵ�ͼ�� obj �ϰ���Ϣ�����档�ڿͻ��ˣ����������ꡢspr �ļ�������
		  ��֡������֡����ĸ��ݣ�֡�����������Ϣ���洢�� obj �� ini �ļ�
		  ���Ա㶯̬���ء�
ѭ����Ч�������Ч�������ڵ�ͼ�ļ��� obj �����У������������д������ϰ���
                    Ϣ����ͼ����Ϣ���ڿͻ��ˣ���������������ҵ�λ�ò���������
npc ��ʬ�壺ֻ�����ڿͻ��ˣ��������˲����д����ϰ���ϢΪ�ա��ڿͻ��ˣ�����
            �����ꡢ��������֡��spr �ļ������д���
*/
/*
��ͼ����״̬�������ֹ
��ͼ���崥����ʽ����״̬�ı䷽ʽ��
1�����Զ����ű��������������ڻ״̬ʱ������ͼ��ѭ����ͼ��λ�÷��侲ֹ�ӵ�
   �򳯹̶�����������ӵ�
2���Զ����ű����Կ��ƴ������Ƿ������ã���������ʱ���Զ�����Ƿ���Ŀ��npc����
   �÷�Χ�ڣ�����У������ӵ����ӵ��ķ��䷽ʽ�����֣���ͼ��ѭ���ڹ̶�֡��
   ��Ͱ�һ����ʱ�������䡣���䷽�򻹿ɷ�Ϊ��ͼ��λ�÷��侲ֹ�ӵ������̶�
   ����������ӵ�����Ŀ��npc�����ӵ�

�����ϵķ��࣬��ͼ��������ͷ�Ϊ���¼��ࣺ
1  ObjTrap_Kind_Cycle_Image_Static
2  ObjTrap_Kind_Cycle_Image_Dir
3  ObjTrap_Kind_Auto_Image_Static
4  ObjTrap_Kind_Auto_Image_Dir
5  ObjTrap_Kind_Auto_Image_Target
6  ObjTrap_Kind_Auto_Delay_Static
7  ObjTrap_Kind_Auto_Delay_Dir
8  ObjTrap_Kind_Auto_Delay_Target
*/






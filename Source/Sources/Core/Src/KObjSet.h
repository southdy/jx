//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObjSet.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------
#ifndef	KObjSetH
#define	KObjSetH

#include "KObj.h"
#include "KLinkArray.h"

#define		MAX_OBJ_NAME_COLOR		16

typedef struct
{
	char	m_szCopyRight[31];
	BYTE	m_btVersion;
	DWORD	m_dwNum;
} KObjMapDataHead;

typedef struct
{
	int		m_nTemplateID;
	int		m_nState;
	int		m_nDir;
	int		m_nPos[3];
	DWORD	m_dwScriptLength;
	char	m_szScript[80];
} KObjMapData;

typedef struct
{
	int		m_nItemID;
	int		m_nMoneyNum;
	int		m_nItemWidth;
	int		m_nItemHeight;
	int		m_nColorID;
	int		m_nMovieFlag;
	int		m_nSoundFlag;
	char	m_szName[FILE_NAME_LENGTH];
} KObjItemInfo;

class KObjSet
{
public:
	KTabFile	m_cTabFile;
	KTabFile	m_cMoneyFile;
	int			m_nObjID;
#ifndef _SERVER
	DWORD		m_dwNameColor[MAX_OBJ_NAME_COLOR];
	int			m_nShowNameFlag;		// �Ƿ�ȫ����ʾ item �� money ��� object ��������ͷ����
#endif
private:
	KLinkArray	m_UseIdx;
	KLinkArray	m_FreeIdx;
public:
	KObjSet();
	~KObjSet();
	BOOL	Init();
	int		GetID();
	void	Remove(int nIdx);
#ifdef _SERVER
	// ���һ��obj��������obj�����е�λ�ñ�ţ���� < 0 ��ʧ�ܣ�
	int		Add(int nDataID, KMapPos MapPos, KObjItemInfo sItemInfo);

	// ��obj�����ļ���������Ӧ����
	int		AddData(int nDataID, KMapPos MapPos, int nMoneyNum = 0, int nItemID = 0, int nItemWidth = 0, int nItemHeight = 0);

	int		AddMoneyObj(KMapPos MapPos, int nMoneyNum);

	// ������������һ��Region������Obj
	BOOL	ServerLoadRegionObj(char *lpszMapPath, int nRegionX, int nRegionY, int nSubWorld);

	// ������������һ��Region������Obj
	BOOL	ServerLoadRegionObj(int nSubWorld, KPakFile *pFile, DWORD dwDataSize);

	// �趨obj�����������Ϣ���������id�������װ�����еĳ�����
	BOOL	SetObjItem(int nObjIndex, int nItemID, int nItemWidth, int nItemHeight);

	BOOL	SyncAdd(int nID, int nClient);
#endif

#ifndef _SERVER
	int		AddData(int nDataID, int nSubWorld, int nRegion, int nMapX, int nMapY, int nOffX, int nOffY);
	int		ClientAdd(int nID, int nDataID, int nState, int nDir, int nCurFrame, int nXpos, int nYpos, KObjItemInfo sInfo);
	BOOL	ClientLoadRegionObj(char *lpszMapPath, int nRegionX, int nRegionY, int nSubWorld, int nRegion);
	BOOL	ClientLoadRegionObj(KPakFile *pFile, DWORD dwDataSize);
	void	RemoveIfClientOnly(int nIdx);
	int		SearchObjAt(int nX, int nY, int nRange);
	DWORD	GetNameColor(int nColorID);
	void	SetShowNameFlag(BOOL bFlag);
	BOOL	CheckShowName();
#endif
	int		FindID(int nID);
	int		FindName(char *lpszObjName);
private:
	int		FindFree();
	int		CheckClientKind(int nKind);
	int		GetDataIDKind(int nDataID);
#ifdef _SERVER
	int		GetMoneyDataId(int nMoney);
#endif
};

extern	KObjSet	ObjSet;
extern	char	g_szObjKind[Obj_Kind_Num][32];

#endif

/*
�����������յ���Ҫ����һ���µ������Ҫ��������ڵذ��Ϸ���һ��װ����Ϊ��ȷ��
���װ������ҪһЩ��Ϣ��
    1��λ����Ϣ������������ı������Ϣ
    2���������ݵ���Ϣ��DataID������Щ��Ϣ��������������ơ����͡��ϰ���Ϣ��ͬһ
       ��DataID��ͨ����Ϣ����Щ��Ϣ����� ObjData.txt �ļ����棬ͨ����ȡ��Щ��
       Ϣ���Գ���ȷ����������
    3�������ÿ��ʵ��֮�䲻ͬ����Ϣ�����磺��ǰ��״̬����ǰ�ķ�����Щ���͵���
       ��������Ҫ��������Ϣ������ǰ������ʱ�䣨��Щ���͵����������ʱ�估��Ӧ
       �Ĵ�������ǰ�����ű��ļ����ơ���ǰ��װ������ID��װ���ࣩ��������������
       �������Ƶĸ�����Ϣ����ͼ������ȣ�����ǰ�ϰ���Ϣ����������д򿪡��ر�״
       ̬�ȣ�
��ȷ�����㹻����Ϣ�󣬿��Ե��þ���ķ����ڷ�����������һ���µ���������嵽ʹ��
�ϣ����ȵ��� AddData() �������ѵ� 1 ���� 2 ����Ϣ���룬����������Զ��� Object
������Ѱ��һ�������������Ӧ�Ļ������ݣ�����һ���µ�����������������������
��λ��xxx��Ȼ����������һ������ΨһID��֪ͨ�ͻ��˲���һ����Ӧ���������Ȼ���
����֪ͨ��ӦSubWorld����ӦRegion��ʼ��������������
    SubWorld[nSubWorld].m_WorldMessage.Send(GWM_OBJ_ADD, nRegion, xxx);
Ȼ���趨�������һЩ������Ϣ�����趨�� 3 ����Ϣ��������Ҫ���õķ����У�
    Object[xxx].SetLifeTime()
    Object[xxx].SetState()
    Object[xxx].SetDir()
    Object[xxx].SetItemDataID()
    Object[xxx].SetScriptFile() �ȣ�
���ˣ�һ���µ�����ڷ��������Ѿ��������ˣ�
���ͻ��˵����յ��������˵Ĳ��������������󣬵��� ClientAdd() �����ڿͻ��˲���
һ����Ӧ��������������������Ϊ�������Զ��� Object ������Ѱ��һ�����������
��Ӧ�Ļ������ݣ�����һ���µ����������������������е�λ��xxx���ѽ��յ�������Ψ
һ ID �������ϣ�Ȼ���趨���ֽ��յ��Ĳ��������֪ͨ��ӦSubWorld����ӦRegion��ʼ
�����������������ˣ�������Ѿ����������ˣ��������ĸ��������û�������趨��ɣ�
��Ҫ�Ժ�������˷�������ͬ����Ϣ�������յõ���ȷ�ı��֡�
*/








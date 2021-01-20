#ifndef	KRegionH
#define	KRegionH

//-----------------------------------------------------------------------
#include "KEngine.h"
#include "KWorldMsg.h"
//-----------------------------------------------------------------------
#ifdef _SERVER
#define	MAX_BROADCAST_COUNT		100
#else
#define	MAX_REGION		9
#endif

#define	REGION_GRID_WIDTH	16
#define	REGION_GRID_HEIGHT	32

enum MOVE_OBJ_KIND
{
	obj_npc,
	obj_object,
	obj_missle,
};
#ifndef TOOLVERSION
class KRegion
#else
class CORE_API KRegion
#endif
{
	friend class	KSubWorld;
public:
	int			m_nIndex;							// ��ͼ����
	int			m_RegionID;
	KList		m_NpcList;							// �����б�
	KList		m_ObjList;							// ����б�
	KList		m_MissleList;						// �ӵ��б�
	KList		m_PlayerList;						// ����б�
	int			m_nConnectRegion[8];				// ���ڵĵ�ͼ����
	int			m_nConRegionID[8];					// ���ڵĵ�ͼID
	int			m_nRegionX;							// �������е�λ��X�����ص㣩
	int			m_nRegionY;							// �������е�λ��Y�����ص㣩
	int			m_nWidth;
	int			m_nHeight;
private:
#ifdef _SERVER
	long		m_Obstacle[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	// ��ͼ�ϰ���Ϣ��
	DWORD		m_dwTrap[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	// ��ͼtrap��Ϣ��
#endif
	int			m_nNpcSyncCounter;					// ͬ��������
	int			m_nObjSyncCounter;
	int			m_nActive;							// �Ƿ񼤻�Ƿ�������ڸ�����
	BYTE*		m_pNpcRef;							// ÿ�������ϵ�NPC��Ŀ
	BYTE*		m_pObjRef;							// ÿ�������ϵ�OBJ��Ŀ
	BYTE*		m_pMslRef;							// ÿ�������ϵ�MISSLE��Ŀ
public:
	KRegion();
	~KRegion();
	BOOL		Init(int nWidth, int nHeight);
	BOOL		Load(int nX, int nY);
#ifdef _SERVER
	// ����������˵�ͼ�ϱ�region �� object���ݣ�����npc��trap��box�ȣ�
	BOOL		LoadObject(int nSubWorld, int nX, int nY);
	// ����������˵�ͼ�ϱ� region ���ϰ�����
	BOOL		LoadServerObstacle(KPakFile *pFile, DWORD dwDataSize);
	// ����������˵�ͼ�ϱ� region �� trap ����
	BOOL		LoadServerTrap(KPakFile *pFile, DWORD dwDataSize);
	// ����������˵�ͼ�ϱ� region �� npc ����
	BOOL		LoadServerNpc(int nSubWorld, KPakFile *pFile, DWORD dwDataSize);
	// ����������˵�ͼ�ϱ� region �� obj ����
	BOOL		LoadServerObj(int nSubWorld, KPakFile *pFile, DWORD dwDataSize);
#endif

#ifndef _SERVER
	// ����ͻ��˵�ͼ�ϱ�region �� object���ݣ�����npc��box�ȣ�
	// ��� bLoadNpcFlag == TRUE ��Ҫ���� clientonly npc else ������
	BOOL		LoadObject(int nSubWorld, int nX, int nY, char *lpszPath);
	// ����ͻ��˵�ͼ�ϱ� region �� clientonlynpc ����
	BOOL		LoadClientNpc(KPakFile *pFile, DWORD dwDataSize);
	// ����ͻ��˵�ͼ�ϱ� region �� clientonlyobj ����
	BOOL		LoadClientObj(KPakFile *pFile, DWORD dwDataSize);
	// �����ϰ����ݸ�С��ͼ
	static void		LoadLittleMapData(int nX, int nY, char *lpszPath, BYTE *lpbtObstacle);
#endif
	void		Close();
	void		Activate();
	BYTE		GetBarrier(int MapX, int MapY, int nDx, int nDy);	//	��ͼ�߶�

	// �� ���ص����� * 1024 �ľ����ж�ĳ��λ���Ƿ��ϰ�
	// ������nGridX nGirdY ����region��������
	// ������nOffX nOffY �������ڵ�ƫ����(���ص� * 1024 ����)
	// ������bCheckNpc ���Ƿ��ж�npc�γɵ��ϰ�
	// ����ֵ���ϰ�����(if ���� == Obstacle_NULL ���ϰ�)
	BYTE		GetBarrierMin(int nGridX, int nGridY, int nOffX, int nOffY, BOOL bCheckNpc);

	DWORD		GetTrap(int MapX, int MapY);						//	�õ�Trap���
	inline BOOL		IsActive() 
	{
#ifdef _SERVER
		return m_nActive; 
#else
		return TRUE;
#endif
	};
	int			GetRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
	BOOL		AddRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
	BOOL		DecRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
	int			FindNpc(int nMapX, int nMapY, int nNpcIdx, int nRelation);
	int			FindEquip(int nMapX, int nMapY);
	int			FindObject(int nMapX, int nMapY);
	int			FindObject(int nObjID);
	void*		GetObjNode(int nIdx);
	int			SearchNpc(DWORD dwNpcID);		// Ѱ�ұ��������Ƿ���ĳ��ָ�� id �� npc (zroc add)
#ifdef _SERVER
	void		SendSyncData(int nClient);
	void		BroadCast(void *pBuffer, DWORD dwSize, int &nMaxCount, int nX, int nY);
	int			FindPlayer(DWORD dwId);
	BOOL		CheckPlayerIn(int nPlayerIdx);
#endif

#ifndef _SERVER
	void		Paint();
#endif
	void		AddNpc(int nIdx);
	void		RemoveNpc(int nIdx);
	void		AddMissle(int nIdx);
	void		RemoveMissle(int nIdx);
	void		AddObj(int nIdx);
	void		RemoveObj(int nIdx);
	BOOL		AddPlayer(int nIdx);
	BOOL		RemovePlayer(int nIdx);
};

//--------------------------------------------------------------------------
//	Find Npc
//--------------------------------------------------------------------------
inline int KRegion::FindNpc(int nMapX, int nMapY, int nNpcIdx, int nRelation)
{
	if (m_pNpcRef[nMapY * m_nWidth + nMapX] == 0)
		return 0;

	KIndexNode *pNode = NULL;
	
	pNode = (KIndexNode *)m_NpcList.GetHead();
	
	while(pNode)
	{
		if (Npc[pNode->m_nIndex].m_MapX == nMapX && Npc[pNode->m_nIndex].m_MapY == nMapY)
		{
			if (NpcSet.GetRelation(nNpcIdx, pNode->m_nIndex) & nRelation)
			{
				return pNode->m_nIndex;
			}
		}
		pNode = (KIndexNode *)pNode->GetNext();
	}	
	return 0;
}
#endif

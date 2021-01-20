#ifndef	KSkillListH
#define	KSkillListH
#include "gamedatadef.h"

#define	MAX_NPCSKILL	80
#define	MAX_TRAPNUM		5

typedef struct tagNpcSkill
{
	int		SkillId;
	int		SkillLevel;
	int		MaxTimes;
	int		RemainTimes;
	DWORD	NextCastTime;		//�´οɷ���ʱ����Сʱ��
	int		CurrentSkillLevel;
} NPCSKILL, *LPNPCSKILL;
struct TDBPlayerData;
#ifndef TOOLVERSION
class KSkillList
#else 
class CORE_API KSkillList
#endif
{
public:
	int			m_nTrapNpcID[5];
	NPCSKILL	m_Skills[MAX_NPCSKILL];
	int			m_nNpcIndex;
public:
	KSkillList();
	~KSkillList();
	int			Add(int nSkillID, int nLevel = 1, int nMaxTimes = 0, int RemainTimes = 0);
	void		Remove(int nSkillID){};//Question :��Ҫ�ӣ�
	void		Clear(){memset(m_Skills, 0, sizeof(m_Skills));};
#ifndef _SERVER
	void		SetSkillLevel(int nId, int nLevel);
	BOOL		SetLevel(int nIndex, int nLevel);		// ��ĳ����ż�����Ϊĳһ��
	int			GetSkillSortList(KUiSkillData *);		// ��ý�ɫ��ǰ���м��������б�
	int			GetSkillPosition(int nSkillId);
	int			GetLeftSkillSortList(KUiSkillData*);	// ��ý�ɫ��ǰ������������б�
	int			GetRightSkillSortList(KUiSkillData*);	// ��ý�ɫ��ǰ�Ҽ����������б�
#endif
	BOOL		IncreaseLevel(int nIdx, int nLvl);
	int			GetLevel(int nSkillID);
	int			GetSkillIdxLevel(int nIdx) 
	{
		if (nIdx <= 0) return 0;
		return m_Skills[nIdx].SkillLevel;
	};
	int			GetCurrentLevel(int nSkillID);
	int			GetCount();
	int			FindSame(int nSkillID);
	int			FindFree();
	BOOL		CanCast(int nSkillID, DWORD dwTime);
	void		SetNextCastTime(int nSkillID, DWORD dwTime);
	
	int			GetSkillId(int nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].SkillId;
		else 
			return -1;
	};
	
#ifdef _SERVER
	int		UpdateDBSkillList(BYTE *);
#endif
};
#endif
//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.h
// Date:	2002.08.26
// Code:	DongBo
// Desc:    header file. ���ļ���������������ɵ���
//---------------------------------------------------------------------------

#ifndef	KItemGeneratorH
#define	KItemGeneratorH

#include "KBasPropTbl.h"
#include "KItem.h"

#define		IN
#define		OUT

#define	NUMOFCMA	150		// ����ֵ. ��ÿ��װ��ʹ�õ�ħ����������������

//class KItem;

class KItemGenerator
{
public:
	KItemGenerator();
	~KItemGenerator();

// �����Ǻ��ĳ�Ա����
protected:
	KLibOfBPT	m_BPTLib;

// �����Ǹ�����Ա����
	int			m_GMA_naryMA[2][NUMOFCMA];	// [0][x]: ǰ׺; [1][x]: ��׺
	int			m_GMA_nCount[2];
	int			m_GMA_naryLevel[2][NUMOFCMA];
	int			m_GMA_naryDropRate[2][NUMOFCMA];
	int			m_GMA_naryCandidateMA[NUMOFCMA];
	int			m_EquipNumOfEntries[equip_detailnum];
	int			m_MedNumOfEntries;
// �����Ƕ���ӿ�
public:
	BOOL Init();
	BOOL Gen_Item(IN int, IN int, IN int, IN int, IN OUT KItem*);
	BOOL Gen_Quest(IN int, IN OUT KItem*);
	BOOL Gen_TownPortal(IN OUT KItem*);
	BOOL Gen_Medicine(IN int, IN int, IN int, IN OUT KItem*);
	BOOL Gen_Equipment(IN int, IN int, IN int, IN int, IN const int*, IN int,
						IN int, IN OUT KItem*);
	BOOL Gen_ExistEquipment(IN int, IN int, IN int, IN int, IN const int*, IN int,
						IN int, IN OUT KItem*);	
	BOOL GetEquipmentCommonAttrib(IN int, IN int, IN int, IN int, IN OUT KItem*);
	BOOL GetMedicineCommonAttrib(IN int, IN int, IN OUT KItem*);
	// Add by flying on May.30.2003
	// Try to get a "Gold Item" by random or by index.
	BOOL GetGoldItemByRandom(IN int, OUT KItem*);
	BOOL GetGoldItemByIndex(IN int, OUT KItem*);
// �����Ǹ�������
private:
	BOOL Gen_MagicAttrib(int, const int*, int, int, KItemNormalAttrib*, int nVersion);
	const KMAGICATTRIB_TABFILE* GetMARecord(int) const;
	BOOL GMA_GetAvaliableMA(int);
	BOOL GMA_GetLevelAndDropRate(int);
	int  GMA_GetCandidateMA(int, int, int);
	void GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA);
};

extern KItemGenerator	ItemGen;			//	װ��������
#endif	// #ifndef	KItemGeneratorH

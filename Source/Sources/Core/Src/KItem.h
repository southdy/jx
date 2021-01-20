//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItem.h
// Date:	2002.08
// Code:	LiXueWu, DongBo
// Desc:	Header File
//---------------------------------------------------------------------------

#ifndef	KItemH
#define	KItemH

#include	"KBasPropTbl.h"
#include	"KMagicAttrib.h"
#include	"GameDataDef.h"
#ifndef _SERVER
#include	 "../../Represent/iRepresent/KRepresentUnit.h"
#endif
#include	"KTabFile.h"
//#include	"KPlayer.h"
class KPlayer;
class KIniFile;

#define		IN
#define		OUT

#ifdef _SERVER
#define		MAX_ITEM	160000
#else
#define		MAX_ITEM	512
#endif
#define		KItemNormalAttrib KMagicAttrib
/*
//װ������������Player�ĺ�������
#define	ID_LIFE		0
#define	ID_MAXLIFE	1
#define	ID_LIEFREPLENISH		2

#define	ID_MANA		3
#define	ID_MAXMANA	4
#define	ID_MANAREPLENISH		5
*/

enum ITEMGENRE
{
	item_equip = 0,			// װ��
	item_medicine,			// ҩƷ
	item_mine,				// ��ʯ
	item_materials,			// ҩ��
	item_task,				// ����
	item_townportal,		// ������
	item_number,			// ������Ŀ
};

enum EQUIPLEVEL
{
	equip_normal = 0,		// ��ͨװ��
	equip_magic,			// ħ��װ����1 ~ 2��ħ��ǰ��׺��
	equip_rare,				// ϡ��װ����3 ~ 6��ħ��ǰ��׺��
	equip_unique,			// Ψһװ��
	equip_set,				// ��װ
	equip_number,			// װ���ȼ���Ŀ
	// flying add this item
	equip_gold,				//�ƽ�װ��
};

enum EQUIPDETAILTYPE
{
	equip_meleeweapon = 0,
	equip_rangeweapon,
	equip_armor,
	equip_ring,
	equip_amulet,
	equip_boots,
	equip_belt,
	equip_helm,
	equip_cuff,
	equip_pendant,
	equip_horse,
	equip_detailnum,
};

enum MEDICINEDETAILTYPE
{
	medicine_blood = 0,
	medicine_mana,
	medicine_both,
	medicine_stamina,
	medicine_antipoison,
	medicine_detailnum,
};

typedef struct
{
	int		nItemGenre;				// �������� (����? ҩƷ? ��ʯ?)
	int		nDetailType;			// �ڸ������еľ������
	int		nParticularType;		// ��ϸ���
	int		nObjIdx;				// ��ͼ�ϰڷ�ʱ��Ӧ��������ݱ��
	BOOL	bStack;					// �Ƿ�ɵ���
	int		nWidth;					// ����������ռ���
	int		nHeight;				// ����������ռ�߶�
	int		nPrice;					// �۸�
	int		nLevel;					// �ȼ�
	int		nSeries;				// ��������
	char	szItemName[80];			// ����
#ifndef _SERVER
	char	szImageName[80];		// �����еĶ����ļ���
	char	szIntro[256];			// ˵������
#endif
	int		nNpcImage;				// NPCͼ��Ӱ��
} KItemCommonAttrib;

typedef struct
{
	UINT	uRandomSeed;
	int		nGeneratorLevel[6];
	int		nVersion;
	int		nLuck;
} KItemGeneratorParam;

class KNpc;

class KItem
{
public:
	KItem();
	~KItem();

// �����Ǻ��ĳ�Ա����
private:
	KItemCommonAttrib	m_CommonAttrib;			// ���ֵ��ߵĹ�ͬ����
	KItemNormalAttrib	m_aryBaseAttrib[7];		// ���ߵĻ�������
	KItemNormalAttrib	m_aryRequireAttrib[6];	// ���ߵ���������
	KItemNormalAttrib	m_aryMagicAttrib[6];	// ���ߵ�ħ������

// �����Ǹ����Եĳ�Ա����
public:
	KItemGeneratorParam	m_GeneratorParam;		// ���ߵ����ɲ���
private:
	int		m_nIndex;							// �����ڵ��������е�����
	DWORD	m_dwID;								// ������ID�����ڿͻ�����������˵Ľ���
	int		m_nCurrentDur;						// ��ǰ�;ö�
#ifndef _SERVER
	KRUImage	m_Image;
#endif
// �����Ƕ���ӿ�
public:
	void	ApplyMagicAttribToNPC(IN KNpc*, IN int = 0) const;
	void	RemoveMagicAttribFromNPC(IN KNpc*, IN int = 0) const;
	void	ApplyHiddenMagicAttribToNPC(IN KNpc*, IN int) const;
	void	RemoveHiddenMagicAttribFromNPC(IN KNpc*, IN int) const;
	KItemGeneratorParam * GetItemParam(){return &m_GeneratorParam;};
	void	SetID(DWORD dwID) { m_dwID = dwID; };
	DWORD	GetID() const { return m_dwID; };
	int		GetDetailType() const { return m_CommonAttrib.nDetailType; };
	int		GetGenre() const { return m_CommonAttrib.nItemGenre; };
	int		GetSeries() const { return m_CommonAttrib.nSeries; };
	int		GetParticular() { return m_CommonAttrib.nParticularType; };
	int		GetLevel() { return m_CommonAttrib.nLevel; };
	void	SetSeries(int nSeries) { m_CommonAttrib.nSeries = nSeries; };
	int		GetWidth() const { return m_CommonAttrib.nWidth; };
	int		GetHeight() const { return m_CommonAttrib.nHeight; };
	int		GetPrice() const { return m_CommonAttrib.nPrice; };
	char*	GetName() const { return (char *)m_CommonAttrib.szItemName; };
	int		GetObjIdx() { return m_CommonAttrib.nObjIdx;};
	void*	GetRequirement(IN int);
	int		GetMaxDurability();
	int		GetTotalMagicLevel();
	int		GetRepairPrice();
	void	Remove();
	BOOL	SetBaseAttrib(IN const KItemNormalAttrib*);
	BOOL	SetRequireAttrib(IN const KItemNormalAttrib*);
	BOOL	SetMagicAttrib(IN const KItemNormalAttrib*);
	void	SetDurability(IN const int nDur) { m_nCurrentDur = nDur; };
	int		GetDurability() { return m_nCurrentDur; };
	KItemGeneratorParam*	GetGeneratorParam() { return &m_GeneratorParam; }
	int		Abrade(IN const int nRange);
	BOOL	CanBeRepaired();
#ifndef _SERVER
	void	Paint(int nX, int nY);
	void	GetDesc(char* pszMsg, bool bShowPrice = false, int nPriceScale = 1, int nActiveAttrib = 0);
#endif

// �����Ǹ����ӿ�
friend class	KItemGenerator;
friend class	KPlayer;
friend class	KItemList;
private:
	BOOL SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT*);
	BOOL SetAttrib_MA(IN const KItemNormalAttrib*);
	BOOL SetAttrib_MA(IN const KMACP*);
	void operator = (const KBASICPROP_EQUIPMENT&);
	void operator = (const KBASICPROP_MEDMATERIAL&);
	void operator = (const KBASICPROP_MINE&);
	void operator = (const KBASICPROP_MEDICINE&);
	void operator = (const KBASICPROP_QUEST&);
	void operator = (const KBASICPROP_TOWNPORTAL&);
	void operator = (const KBASICPROP_EQUIPMENT_UNIQUE&);
	// flying add this overload operator.
	void operator = (const KBASICPROP_EQUIPMENT_GOLD&);
	BOOL Gen_Equipment_Unique(const KBASICPROP_EQUIPMENT*, const KBASICPROP_EQUIPMENT_UNIQUE*);

// �����Ǹ�������
private:
	BOOL SetAttrib_Base(const KEQCP_BASIC*);
	BOOL SetAttrib_Req(const KEQCP_REQ*);
};

extern KItem Item[MAX_ITEM];

#endif

#ifndef KItemSetH
#define	KItemSetH

#include "KLinkArray.h"

#define		IN
#define		OUT
class	KItem;

enum enumAbrade
{
	enumAbradeAttack = 0,
	enumAbradeDefend,
	enumAbradeMove,
	enumAbradeNum,
};
// ���֮�佻�׽���ʱ�������ж������Ʒ���ܷ�����������Ʒ
typedef struct
{
	int		m_nIdx;
	int		m_nX;
	int		m_nY;
	int		m_nWidth;
	int		m_nHeight;
} TRADE_ITEM_INFO;

typedef struct
{
	int		nPriceScale;
	int		nMagicScale;
} REPAIR_ITEM_PARAM;

class KItemSet
{
private:
	DWORD			m_dwIDCreator;		//	ID�����������ڿͻ�����������˵Ľ���
	KLinkArray		m_FreeIdx;			//	���ñ�
	KLinkArray		m_UseIdx;			//	���ñ�

public:
	int				m_nItemAbradeRate[enumAbradeNum][itempart_num];
	REPAIR_ITEM_PARAM	m_sRepairParam;
#ifdef _SERVER
	// ���֮�佻�׽���ʱ�������ж������Ʒ���ܷ�����������Ʒ
	TRADE_ITEM_INFO		*m_psItemInfo;
	// ���֮�佻�׽���ʱ�������ж������Ʒ���ܷ�����������Ʒ
	TRADE_ITEM_INFO		*m_psBackItemInfo;
	// copy m_psItemInfo to m_psBackItemInfo
	void			BackItemInfo();
	// ������ұ�PK������ĳͷ����㣬����������Ʒ
	PlayerItem		m_sLoseItemFromEquipmentRoom[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	// ������ұ�PK������ĳͷ����㣬���䴩�����ϵ�װ��
	PlayerItem		m_sLoseEquipItem[itempart_num];
#endif

public:
	KItemSet();
	~KItemSet();
	void			Init();
	int				GetItemCount(IN int nItemGenre = -1);
	int				SearchID(IN DWORD dwID);
	int				Add(KItem* pItem);
	int				Add(IN int nItemGenre, IN int nSeries, IN int nLevel, IN int nLuck, IN int nDetail = -1, IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int nVersion = 0, IN UINT nRandomSeed = 0);
	void			Remove(IN int nIdx);
	int				GetAbradeRange(IN int nType, IN int nPart);
private:
	void			SetID(IN int nIdx);
	int				FindFree();
};

extern KItemSet	ItemSet;
#endif

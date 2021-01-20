#ifndef KInventoryH
#define	KInventoryH

class CORE_API KInventory
{
	friend	class	KItemList;

private:
	int*	m_pArray;
	int		m_nMoney;
	int		m_nWidth;
	int		m_nHeight;

public:
	KInventory();
    ~KInventory();

	BOOL	Init(int nWidth, int nHeight);
	void	Release();
	void	Clear();
	BOOL	PickUpItem(int nIdx, int nX, int nY, int nWidth, int nHeight);
	int		FindItem(int nX, int nY);
	BOOL	PlaceItem(int nXpos, int nYpos, int nIdx, int nWidth, int nHeight);
	BOOL	HoldItem(int nIdx, int nWidth, int nHeight);
	BOOL	FindRoom(int nWidth, int nHeight, POINT* pPos);
	BOOL	CheckRoom(int nXpos, int nYpos, int nWidth, int nHeight);
	BOOL	AddMoney(int nMoney);
	int		GetMoney() { return m_nMoney; }
	BOOL	SetMoney(int nMoney) { if (nMoney < 0) return FALSE; m_nMoney = nMoney; return TRUE; }
	int		GetNextItem(int nStartIdx, int nXpos, int nYpos, int *pX, int *pY);
	int		CalcSameDetailType(int nGenre, int nDetail);
	BOOL	FindEmptyPlace(int nWidth, int nHeight, POINT *pPos);

	// ������Ʒ���ͺ;������ͣ��쿴Inventory������û����ͬ����Ʒ�����λ�úͱ��
	BOOL	FindSameDetailType(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	// ������Ʒ���ͺ;������ͣ��쿴Inventory������û����ͬ����Ʒ
	BOOL	CheckSameDetailType(int nGenre, int nDetail);
};
#endif //KInventoryH

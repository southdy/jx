// KOctree.h: interface for the KOctree class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	KOctreeH
#define KOctreeH

#include "KOctreeNode.h"
#include "KPalette.h"
#define	defMaxLayer	5


class ENGINE_API KOctree  
{
public:
	int				m_nColors;	// �ܹ��м���ɫ��
	KOctreeNode		RootNode;	// ���ĸ��ڵ�
	KPAL24			*m_Palette;
private:
	int				m_nMinRef;	// ��С���ô���
	int				m_nPalOff;	// ��ɫ��ƫ��
	KOctreeNode*	m_pMinNode;	// ��С���������ĸ��ڵ�	
public:
	void			Insert(BYTE r, BYTE g, BYTE b);
	void			ReduceNode();
	void			GetMinSumNode(KOctreeNode *pNode);
	void			InitPalette();
	void			MakePalette(KOctreeNode *pNode);
	int				QueryIndex(BYTE r, BYTE g, BYTE b);
	KOctree();
	virtual ~KOctree();

};

#endif // !defined(AFX_KOCTREE_H__23B11C8F_8C0F_483F_950D_58B1977101FE__INCLUDED_)

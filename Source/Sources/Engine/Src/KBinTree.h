#ifndef KBINTREE_H
#define KBINTREE_H

#include "KEngine.h"

class ENGINE_API TBinTreeNode
{
	TBinTreeNode(){ pParent = pLeftChild = pRightChild =  NULL;	}
public:
	virtual BOOL			operator>(TBinTreeNode& p) = 0;
	virtual BOOL			operator==(TBinTreeNode& p) = 0;
 	virtual BOOL			operator<(TBinTreeNode& p) = 0;
	TBinTreeNode			* pParent;				//��
	TBinTreeNode			* pLeftChild;			//����
	TBinTreeNode			* pRightChild;			//����
};		//�����ļ���������������Ϊ�ű���ʶ�����������


class ENGINE_API KBinTree
{
public:
	KBinTree();

	//���ҹؼ�����pKeyNodeһ�µĽ�㣬�ɹ�*pResultΪTrue�����ؽ��ָ��.
	TBinTreeNode * Search(TBinTreeNode* pKeyNode, BOOL * pResult);

	//�����㣬�ɹ������²�Ľ��ָ�룬����Ϊ��
	TBinTreeNode * Insert(TBinTreeNode *pNewNode);
	
	
	//ɾ�������
	BOOL		   RemoveThisNode(TBinTreeNode * pNode);
	
	//ɾ������������pKeyNodeһ�µĽ��
	BOOL		   RemoveKeyNode(TBinTreeNode * pNode);
		
	DWORD		   GetCount()
	{ int OldCount = m_TempCount;
	return InOrder(m_pTreeRoot) - OldCount;
	};
	TBinTreeNode * GetRoot(){return m_pTreeRoot;	};
		

private:
	TBinTreeNode * m_pTreeRoot;		//��������֧��
	int				m_TempCount;
	TBinTreeNode * AddNode(TBinTreeNode *pNewNode, TBinTreeNode *pTBinTreeNode);
	TBinTreeNode * RemoveNode(TBinTreeNode * pTBinTreeNode, TBinTreeNode ** ppRootTBinTreeNode);
	TBinTreeNode * Search(TBinTreeNode * pParentTBinTreeNode, TBinTreeNode * pTBinTreeNode, TBinTreeNode * pKeyNode, BOOL * pResult);
	TBinTreeNode * FindLess(TBinTreeNode * pTBinTreeNode);
	DWORD		   InOrder(TBinTreeNode * pTBinTreeNode);//�������
};



#endif	//KBINTREE_H

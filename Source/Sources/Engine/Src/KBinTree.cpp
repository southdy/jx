#include "KBinTree.h"

KBinTree::KBinTree()
{
	m_pTreeRoot = NULL;
}

//---------------------------------------------------------------------------
// ����:	BTSearch
// ����:	
// ����:	TBinTreeNode * pParentTBinTreeNode
// ����:	TBinTreeNode * pTBinTreeNode
// ����:	DWORD nKey
// ����:	BOOL * pResult
// ����:	TBinTreeNode * 

//---------------------------------------------------------------------------
TBinTreeNode * KBinTree::Search(TBinTreeNode * pParentTBinTreeNode, TBinTreeNode * pTBinTreeNode, TBinTreeNode *pKeyNode, BOOL * pResult)
{
	
	if (pTBinTreeNode == NULL || pParentTBinTreeNode == NULL)
	{
		*pResult = FALSE;
		return pParentTBinTreeNode;
	}
	
	
	if (*pKeyNode == *pTBinTreeNode)
	{
		*pResult = TRUE;
		return pTBinTreeNode;
	}
	else if (*pKeyNode > *pTBinTreeNode)
		return Search(pTBinTreeNode, pTBinTreeNode->pRightChild, pKeyNode, pResult);
	else
		return Search(pTBinTreeNode, pTBinTreeNode->pLeftChild, pKeyNode, pResult);
	
}



//---------------------------------------------------------------------------
// ����:	BTInsert
// ����:	��pTScrptΪ����������szKeyΪ�ؼ���Ľ��
// ����:	TBinTreeNode *pTBinTreeNode 
// ����:	char * szKey
// ����:	TBinTreeNode * ���ز���Ľ��
//---------------------------------------------------------------------------
TBinTreeNode * KBinTree::AddNode(TBinTreeNode *pNewTBinTreeNode, TBinTreeNode *pTBinTreeNode)
{

	int nResult = 0;
	if (pNewTBinTreeNode == NULL)
		return NULL;
	
	pNewTBinTreeNode->pLeftChild = NULL;
	pNewTBinTreeNode->pParent = NULL;
	pNewTBinTreeNode->pRightChild = NULL;


	if (*pNewTBinTreeNode == *pTBinTreeNode)
		return NULL;

	//���ݴ�Сȷ�����ӻ�������
	if (nResult = *pNewTBinTreeNode > *pTBinTreeNode)
	{
		pTBinTreeNode->pRightChild = pNewTBinTreeNode;
		pNewTBinTreeNode->pParent = pTBinTreeNode;
	}
	else 
	{
		pTBinTreeNode->pLeftChild = pNewTBinTreeNode;
		pNewTBinTreeNode->pParent = pTBinTreeNode;
	}
	return pNewTBinTreeNode;
	
}




//---------------------------------------------------------------------------
// ����:	 BTDelete
// ����:	ɾ�����
// ����:	TBinTreeNode * pTBinTreeNode
// ����:	TBinTreeNode ** ppRootTBinTreeNode
// ����:	TBinTreeNode * 
//---------------------------------------------------------------------------
TBinTreeNode *  KBinTree::RemoveNode(TBinTreeNode * pTBinTreeNode, TBinTreeNode ** ppRootTBinTreeNode)
{
	TBinTreeNode * pFindTBinTreeNode;
	if (pTBinTreeNode == NULL)
		return NULL;
	
	if (pTBinTreeNode->pLeftChild == NULL && pTBinTreeNode->pRightChild == NULL)//�ý��û��������
	{
		if (*ppRootTBinTreeNode == pTBinTreeNode)
		{
			*ppRootTBinTreeNode = NULL;
		}
		else
		{
			int nResult ;
			
			if (*pTBinTreeNode < *pTBinTreeNode->pParent)
				nResult = -1;
			else 
				nResult = 1;
			
			if (nResult < 0)//С��,���ڸ������
				pTBinTreeNode->pParent->pLeftChild = NULL;
			else
				pTBinTreeNode->pParent->pRightChild = NULL;
			
		}
		
		//	delete pTBinTreeNode;
		pTBinTreeNode = NULL;
		return NULL;
	}
	
	else if (!(pTBinTreeNode->pLeftChild && pTBinTreeNode->pRightChild))//ֻ�е���ʱ
	{
		if (pTBinTreeNode == *ppRootTBinTreeNode)
		{
			if (pTBinTreeNode->pLeftChild)
			{
				*ppRootTBinTreeNode = pTBinTreeNode->pLeftChild;
				pTBinTreeNode->pLeftChild->pParent = NULL;
			}
			else
			{
				*ppRootTBinTreeNode = pTBinTreeNode->pRightChild;
				pTBinTreeNode->pRightChild->pParent = NULL;
			}
			//	delete pTBinTreeNode;
			pTBinTreeNode = NULL;
			return	NULL;
		}
		
		
		int nResult ;

		if (*pTBinTreeNode < *pTBinTreeNode->pParent)
				nResult = -1;
		else 
				nResult = 1;
	
		if (nResult < 0)//�ڸ������
		{
			
			if (pTBinTreeNode->pLeftChild)//ֻ������
			{
				pTBinTreeNode->pParent->pLeftChild = pTBinTreeNode->pLeftChild;
				pTBinTreeNode->pLeftChild->pParent = pTBinTreeNode->pParent;
			}
			else
			{
				pTBinTreeNode->pParent->pLeftChild = pTBinTreeNode->pRightChild;
				pTBinTreeNode->pRightChild->pParent = pTBinTreeNode->pParent;
			}
			
			//delete pTBinTreeNode;
			pTBinTreeNode = NULL;
			
		}
		else 
		{
			if (pTBinTreeNode->pLeftChild)//ֻ������
			{
				pTBinTreeNode->pParent->pRightChild = pTBinTreeNode->pLeftChild;
				pTBinTreeNode->pLeftChild->pParent = pTBinTreeNode->pParent;
			}
			else
			{
				pTBinTreeNode->pParent->pRightChild = pTBinTreeNode->pRightChild;
				pTBinTreeNode->pRightChild->pParent = pTBinTreeNode->pParent;
			}
			//delete pTBinTreeNode;
			pTBinTreeNode = NULL;
			
		} 
		
		
		return NULL;
		
	}  
	else//��ȫ�� 
	{
		
		
		{
			
			pFindTBinTreeNode = FindLess(pTBinTreeNode->pLeftChild);
			
			if (pFindTBinTreeNode)
			{
				
				//��һ���֣�����ý��������ʱ�������ӽ���ָ��ĸı�
				//�ý����������
				if (pFindTBinTreeNode->pLeftChild)
				{
					//����������Ҫ����Ľ��������������ʱ����ԭ���Ĺ�ϵ�����仯����������˼·�ı�
					if (pFindTBinTreeNode != pTBinTreeNode->pLeftChild)
					{
						pFindTBinTreeNode->pParent->pRightChild = pFindTBinTreeNode->pLeftChild;
						pFindTBinTreeNode->pLeftChild->pParent = pFindTBinTreeNode->pParent;
					}
				}
				else
				{
					if (pFindTBinTreeNode != pTBinTreeNode->pLeftChild)
					{
						pFindTBinTreeNode->pParent->pRightChild = NULL;
					}
					
				}
				
				//�ڶ����֣��ı��滻������ᣬʵ����ԭ�����ͬ��
				//�������滻�Ľ��Ϊɾ���������ӵ��������
				if (pFindTBinTreeNode == pTBinTreeNode->pLeftChild)
				{
					//�����Ӳ��ý���������ԭ״
					pTBinTreeNode->pRightChild->pParent = pFindTBinTreeNode;
					
					//�����Ӳ��ý���,����ԭ״	
					pFindTBinTreeNode->pRightChild = pTBinTreeNode->pRightChild;
					pFindTBinTreeNode->pParent = pTBinTreeNode->pParent;
					
				}
				else
				{
					
					pTBinTreeNode->pLeftChild->pParent = pFindTBinTreeNode;
					pTBinTreeNode->pRightChild->pParent = pFindTBinTreeNode;
					
					
					pFindTBinTreeNode->pLeftChild = pTBinTreeNode->pLeftChild ; 
					pFindTBinTreeNode->pRightChild = pTBinTreeNode->pRightChild;
					pFindTBinTreeNode->pParent = pTBinTreeNode->pParent;
					
				}
				
				
				//��������   ɾ�����֮���������
				
				if (*ppRootTBinTreeNode == pTBinTreeNode)
				{
					*ppRootTBinTreeNode = pFindTBinTreeNode;
					pFindTBinTreeNode->pParent = NULL;
				}
				else 
				{
					
					int nResult ;
					
				if (*pTBinTreeNode < *pTBinTreeNode->pParent)
					nResult = -1;
					else 
					nResult = 1;
					
					
					if (nResult < 0)//�ڸ�������
					{
						pTBinTreeNode->pParent->pLeftChild = pFindTBinTreeNode;
					}
					else
					{
						pTBinTreeNode->pParent->pRightChild = pFindTBinTreeNode;
					}
				}
				
				
				//delete pTBinTreeNode;
				pTBinTreeNode = NULL;
				return pFindTBinTreeNode;
			}
			else
				return NULL;
		}
		
	}	
	
	
}

//---------------------------------------------------------------------------
// ����:	BTFindLess
// ����:	
// ����:	TBinTreeNode * pTBinTreeNode
// ����:	TBinTreeNode * 
//---------------------------------------------------------------------------
TBinTreeNode * KBinTree::FindLess(TBinTreeNode * pTBinTreeNode)
{
	if (pTBinTreeNode == NULL)
		return NULL;
	
	if (pTBinTreeNode->pRightChild == NULL )
		return pTBinTreeNode;
	else
		return FindLess(pTBinTreeNode->pRightChild);
	
	
}

//---------------------------------------------------------------------------
// ����:	BTPreorder
// ����:	
// ����:	TBinTreeNode * pTBinTreeNode
// ����:	DWORD  
//---------------------------------------------------------------------------
DWORD  KBinTree::InOrder(TBinTreeNode * pTBinTreeNode)//�������
{
	static int Count = 0	;
	if (pTBinTreeNode != NULL)
	{


		InOrder(pTBinTreeNode->pLeftChild);
		//		printf("%d\n", pTBinTreeNode->nKey);
		InOrder(pTBinTreeNode->pRightChild);
		Count ++;
	}	
	m_TempCount = Count;
	return Count;
	
	
}

TBinTreeNode*  KBinTree::Insert(TBinTreeNode *pNewNode)
{
	BOOL nResult = 0;
	TBinTreeNode * pNode = NULL;
	
	if (m_pTreeRoot == NULL) 
	{
		TBinTreeNode * pTBinTreeNode = pNewNode;
		m_pTreeRoot = pTBinTreeNode;
		pTBinTreeNode->pLeftChild = NULL;
		pTBinTreeNode->pRightChild = NULL;
		pTBinTreeNode->pParent = NULL;
		return pTBinTreeNode;
	}
	
	pNode = Search(m_pTreeRoot, m_pTreeRoot, pNewNode, &nResult);
	
	if (nResult == 1)
	{
		return NULL;
		
	}
	else if (nResult == 0)
		return AddNode(pNewNode, pNode);
	else return NULL;
}




BOOL   KBinTree::RemoveKeyNode(TBinTreeNode * pNode)
{
	BOOL nResult = FALSE;//���ҵĽ��
	
	TBinTreeNode * pTBinTreeNode = NULL;//���ҷ��ص�ָ��
	
	if (m_pTreeRoot == NULL)
		return FALSE;
	
	pTBinTreeNode = Search(m_pTreeRoot, m_pTreeRoot, pNode, &nResult);
	
	if (nResult && pTBinTreeNode) //���������ҵ��˸ùؼ���
	{
		
		return RemoveThisNode(pTBinTreeNode);
	}
	else
		return FALSE;
	return TRUE;
	
}

BOOL   KBinTree::RemoveThisNode(TBinTreeNode * pNode)
{
	if (pNode == NULL)
		return FALSE;
	if (m_pTreeRoot == NULL)
		return FALSE;
	TBinTreeNode * pDelNode;
	//�ý�㲢�����ڱ��еĽ�㣬�򷵻�FALSE
	if (pNode->pLeftChild == NULL&&pNode->pRightChild == NULL && pNode->pParent == NULL && pNode != m_pTreeRoot)
		return FALSE;
	pDelNode = RemoveNode(pNode, &m_pTreeRoot);
	return TRUE;
}

TBinTreeNode * KBinTree::Search(TBinTreeNode * pKeyNode, BOOL * pResult)
{
	TBinTreeNode * pNode;
	if (m_pTreeRoot == NULL)
		return NULL;
	
	pNode = Search(m_pTreeRoot, m_pTreeRoot, pKeyNode, pResult);
	return pNode;
}

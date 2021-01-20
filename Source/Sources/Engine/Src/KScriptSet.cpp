//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KScriptEngine.cpp
// Date:	2001-9-11 10:33:35
// Code:	Romandou
// Desc:	�ű����Ͽ��ƻ���
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KScriptSet.h"
//---------------------------------------------------------------------------
// ����:	KScriptSet::KScriptSet
// ����:	
// ����:	
//---------------------------------------------------------------------------
KScriptSet::KScriptSet()
{
	m_nKeyStyle = KEY_STRING;
	m_pRootList = NULL;
	m_pLestRecent = NULL;
	m_pMostRecent = NULL;
}

KScriptSet::KScriptSet(int Key_Style)
{
	if (Key_Style == KEY_NUMBER || Key_Style == KEY_STRING)
	m_nKeyStyle = Key_Style;
	else 
	{
		m_nKeyStyle = KEY_STRING;
		printf("UnKnown ScriptSet KeyStyle:[%d]. So Set Key As Default KeyStyle KEY_STRING.\n", Key_Style);
	}
	m_pRootList = NULL;
	m_pLestRecent = NULL;
	m_pMostRecent = NULL;
}


//---------------------------------------------------------------------------
// ����:	KScriptSet::~KScriptSet
// ����:	
// ����:	
//---------------------------------------------------------------------------
KScriptSet::~KScriptSet()
{
	TScript * pTScript = NULL;
	TScript * pDelTScript = NULL;
	pTScript = m_pLestRecent;
	
	while(pTScript)
	{
		pDelTScript =  pTScript;
		pTScript = pTScript->pMoreRecent;
		DeleteScript(pDelTScript->szKey);
	}
	
}

//---------------------------------------------------------------------------
// ����:	KScriptSet::GetScript
// ����:	
// ����:	char * szKey     //�ýű��Ĺؼ���
// ����:	BOOL nNeedCreate //��δ���������ҵ�ʱ�Ƿ���Ҫֱ���½�
// ����:	BOOL nNeedUpdateRecent //�Ƿ���Ҫ����Recent����������
// ����:	KScript * 
//---------------------------------------------------------------------------
KScript * KScriptSet::GetScript(char * szKey, BOOL nNeedCreate = 1, BOOL nNeedUpdateRecent = 1)
{
	if (szKey == NULL ) return NULL;
	KScript * pScript = NULL;
	TScript * pTScript = NULL;
	TScript * pGetTScript = NULL;
	

	BOOL nResult = 0;

	if (m_nKeyStyle != KEY_STRING)
	{
		printf("The ScriptSet's Key Is Not a String. So Can Not Find A CorrRespond Script.\n");
		return NULL;
	}
	
	
	
	if (m_pRootList == NULL) 
	{
		if ((pScript = CreateScript(szKey, GetInitStackSize(szKey))) == NULL) 
			return NULL;
		
		pGetTScript = new TScript;
		if (pGetTScript == NULL) 
			return NULL;
		
		//		��ʼ��Recent��
		pGetTScript->pLessRecent = NULL;
		pGetTScript->pMoreRecent = NULL;
		pGetTScript->pLeftChild = NULL;
		pGetTScript->pRightChild = NULL;
		pGetTScript->pScript = pScript;
		pGetTScript->pParent = NULL;
		
		strcpy(pGetTScript->szKey, szKey);
		m_pRootList = pGetTScript;
		m_pLestRecent = pGetTScript;
		m_pMostRecent = pGetTScript;
		
		return pScript;
		
	}
	
	pTScript = BTSearch(m_pRootList, m_pRootList, szKey, &nResult);
	
	if (nResult == 0)
	{
		if (nNeedCreate)//�����Ҳ���ʱ���Ƿ���Ҫ����һ��
		{
			KScript * pNewScript;
			if ( (pNewScript = CreateScript(szKey, GetInitStackSize(szKey)) ) == NULL )
			{
				printf("Can not Create Script Instance ,Mayby No Memorys.\n");
				return NULL;
				
			}
			pGetTScript = BTInsert(pTScript, szKey);
			if (pGetTScript == NULL) return NULL;
			pGetTScript->pScript = pNewScript;
		}
		else 
			return NULL;//����Ҫ�����Ļ����ͷ��ؿ�ֵ
		
		
	}
	else
		pGetTScript = pTScript;
	
	if (nNeedUpdateRecent)	//�����Ҫ����Recent�������
		UpdateRecent(nResult, pGetTScript);	
	
	return pGetTScript->pScript;
}

KScript * KScriptSet::GetScript(DWORD nKey, BOOL nNeedCreate = 1, BOOL nNeedUpdateRecent = 1)
{
	
	KScript * pScript = NULL;
	TScript * pTScript = NULL;
	TScript * pGetTScript = NULL;
	
	BOOL nResult = 0;
	
	if (m_nKeyStyle != KEY_NUMBER)
	{
		printf("The ScriptSet's Key Is Not a Number. So Can Not Find A CorrRespond Script.\n");
		return NULL;
	}

	if (m_pRootList == NULL) 
	{
		if ((pScript = CreateScript(nKey, GetInitStackSize(nKey))) == NULL) 
			return NULL;
		
		pGetTScript = new TScript;
		if (pGetTScript == NULL) 
			return NULL;
		
		//		��ʼ��Recent��
		pGetTScript->pLessRecent = NULL;
		pGetTScript->pMoreRecent = NULL;
		pGetTScript->pLeftChild = NULL;
		pGetTScript->pRightChild = NULL;
		pGetTScript->pScript = pScript;
		pGetTScript->pParent = NULL;
		
		strcpy(pGetTScript->szKey, "");
		pGetTScript->nKey = nKey;

		m_pRootList = pGetTScript;
		m_pLestRecent = pGetTScript;
		m_pMostRecent = pGetTScript;
		
		return pScript;
		
	}
	
	pTScript = BTSearch(m_pRootList, m_pRootList, nKey, &nResult);
	
	if (nResult == 0)
	{
		if (nNeedCreate)//�����Ҳ���ʱ���Ƿ���Ҫ����һ��
		{
			KScript * pNewScript;
			if ( (pNewScript = CreateScript(nKey, GetInitStackSize(nKey)) ) == NULL )
			{
				printf("Can not Create Script Instance ,Mayby No Memorys.\n");
				return NULL;
				
			}
			pGetTScript = BTInsert(pTScript, nKey);
			if (pGetTScript == NULL) return NULL;
			pGetTScript->pScript = pNewScript;
		}
		else 
			return NULL;//����Ҫ�����Ļ����ͷ��ؿ�ֵ
		
		
	}
	else
		pGetTScript = pTScript;
	
	if (nNeedUpdateRecent)	//�����Ҫ����Recent�������
		UpdateRecent(nResult, pGetTScript);	
	
	return pGetTScript->pScript;
}

//---------------------------------------------------------------------------
// ����:	KScriptSet::UpdateRecent
// ����:	����Recent��
// ����:	BOOL bExistedScirpt  �Ƿ�pGetTScript�Ǹ����Ѵ��ڵĽű�����
// ����:	TScript pGetTScript  ����ʹ�õĽű�����
// ����:	void 
//---------------------------------------------------------------------------
void KScriptSet::UpdateRecent(BOOL bExistedScript, TScript *pGetTScript)
{
	BOOL nResult  = bExistedScript;
	
	if (nResult == 0)//�����ɵĽű�
	{
		if (m_pRootList == NULL)//��һ�����
		{
			pGetTScript->pLessRecent = NULL;
			pGetTScript->pMoreRecent = NULL;
			m_pMostRecent = pGetTScript;
			m_pLestRecent = pGetTScript;
		}
		else
		{
			pGetTScript->pLessRecent = m_pMostRecent;
			pGetTScript->pMoreRecent = NULL;
			m_pMostRecent->pMoreRecent = pGetTScript;
			
		}
		
	}
	else //ʹ�õ����Ѵ��ڵĽű�
	{
		
		if (pGetTScript->pLessRecent == NULL && pGetTScript->pMoreRecent == NULL)
		{
			return ;
		}
		
		if (pGetTScript == m_pLestRecent)//�ý���������ĩβ
		{
			if (pGetTScript->pMoreRecent)//�Ƿ�ֻһ�����
			{
				
				m_pLestRecent = pGetTScript->pMoreRecent;//����m_pLestRecent
				m_pLestRecent->pLessRecent = NULL;
				
				
				pGetTScript->pMoreRecent = NULL;
				pGetTScript->pLessRecent = m_pMostRecent;
				m_pMostRecent->pMoreRecent = pGetTScript;
				
			}
			
			
		}
		else if (pGetTScript == m_pMostRecent)//��ͷ�����
		{
			
		}
		else
		{
			
			pGetTScript->pMoreRecent->pLessRecent = pGetTScript->pLessRecent;
			pGetTScript->pLessRecent->pMoreRecent = pGetTScript->pMoreRecent;
			
			pGetTScript->pLessRecent = m_pMostRecent;
			pGetTScript->pMoreRecent = NULL;
			m_pMostRecent->pMoreRecent = pGetTScript;
			
		}
	}
	
	
	m_pMostRecent = pGetTScript;//����m_pMostRecent
}




//---------------------------------------------------------------------------
// ����:	KScriptSet::SearchScript
// ����:	
// ����:	char * szKey
// ����:	BOOL 
//---------------------------------------------------------------------------
TScript *  KScriptSet::SearchScript(char * szKey, BOOL * pnResult)
{
	TScript * pTScript;
	if (m_pRootList == NULL)
		return NULL;
	
	pTScript = BTSearch(m_pRootList, m_pRootList, szKey, pnResult 	);
	return pTScript;
}

//---------------------------------------------------------------------------
// ����:	 KScriptSet::SearchScript
// ����:	
// ����:	DWORD nKey
// ����:	BOOL * pnResult
// ����:	TScript * 
//---------------------------------------------------------------------------
TScript *  KScriptSet::SearchScript(DWORD nKey, BOOL * pnResult)
{
	TScript * pTScript;
	if (m_pRootList == NULL)
		return NULL;
	
	pTScript = BTSearch(m_pRootList, m_pRootList, nKey, pnResult);
	return pTScript;
}


//---------------------------------------------------------------------------
// ����:	 KScriptSet::CreateScript
// ����:	�½��ű�ʵ����Ϊ�麯�����������ฺ����
// ����:	char * szKey
// ����:	int StackSize
// ����:	KScript * 
//---------------------------------------------------------------------------
KScript *  KScriptSet::CreateScript(char * szKey , int StackSize)
{
	return NULL;
}

//---------------------------------------------------------------------------
// ����:	 KScriptSet::CreateScript
// ����:	
// ����:	DWORD nKey
// ����:	int StackSize
// ����:	KScript * 
//---------------------------------------------------------------------------
KScript *  KScriptSet::CreateScript(DWORD nKey , int StackSize)
{
	return NULL;
}

//---------------------------------------------------------------------------
// ����:	KScriptSet::DeleteScript
// ����:	
// ����:	char * szKey
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KScriptSet::DeleteScript(char * szKey)
{
	BOOL nResult = FALSE;//���ҵĽ��
	TScript * pTScript;//���ҷ��ص�ָ��
	pTScript = SearchScript(szKey, &nResult);
	
	if (nResult && pTScript) //���������ҵ��˸ùؼ���
	{
		if (pTScript->pScript)
			pTScript->pScript->Exit();
		
		if (pTScript->pLessRecent == NULL  && pTScript->pMoreRecent == NULL)//ȫ��Ϊ�գ���ֻ��һ�����
		{
			m_pMostRecent = NULL;
			m_pLestRecent = NULL;
			//if (m_pRootList == pTScript) 
			//	m_pRootList = NULL;
			return DeleteScript(pTScript);
		}
		
		else if (!(pTScript->pLessRecent && pTScript->pMoreRecent))//��һ��Ϊ��
		{
			
			if (pTScript->pLessRecent)
			{
				pTScript->pLessRecent->pMoreRecent = NULL;
				
				if (m_pMostRecent == pTScript)
					m_pMostRecent = pTScript->pLessRecent;
				
				
			}
			else
			{
				pTScript->pMoreRecent->pLessRecent = NULL;
				
				if (m_pLestRecent == pTScript)
					m_pLestRecent = pTScript->pMoreRecent;
				
				
			}
			
			
			return DeleteScript(pTScript);
		}
		else//ȫʵ
		{
			pTScript->pLessRecent->pMoreRecent = pTScript->pMoreRecent;
			pTScript->pMoreRecent->pLessRecent = pTScript->pLessRecent;
			
			if (m_pMostRecent == pTScript)
				m_pMostRecent = pTScript->pLessRecent;
			
			if (m_pLestRecent == pTScript)
				m_pLestRecent = pTScript->pMoreRecent;
			
			return DeleteScript(pTScript);
		}
	}
	else
		return FALSE;
	
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KScriptSet::DeleteScript
// ����:	
// ����:	DWORD nKey
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KScriptSet::DeleteScript(DWORD nKey)
{
	BOOL nResult = FALSE;//���ҵĽ��
	TScript * pTScript;//���ҷ��ص�ָ��
	pTScript = SearchScript(nKey, &nResult);
	
	if (nResult && pTScript) //���������ҵ��˸ùؼ���
	{
		if (pTScript->pScript)
			pTScript->pScript->Exit();
		
		if (pTScript->pLessRecent == NULL  && pTScript->pMoreRecent == NULL)//ȫ��Ϊ�գ���ֻ��һ�����
		{
			m_pMostRecent = NULL;
			m_pLestRecent = NULL;
			
			return DeleteScript(pTScript);
		}
		
		else if (!(pTScript->pLessRecent && pTScript->pMoreRecent))//��һ��Ϊ��
		{
			
			if (pTScript->pLessRecent)
			{
				pTScript->pLessRecent->pMoreRecent = NULL;
				
				if (m_pMostRecent == pTScript)
					m_pMostRecent = pTScript->pLessRecent;
				
				
			}
			else
			{
				pTScript->pMoreRecent->pLessRecent = NULL;
				
				if (m_pLestRecent == pTScript)
					m_pLestRecent = pTScript->pMoreRecent;
				
				
			}
			
			
			return DeleteScript(pTScript);
		}
		else//ȫʵ
		{
			pTScript->pLessRecent->pMoreRecent = pTScript->pMoreRecent;
			pTScript->pMoreRecent->pLessRecent = pTScript->pLessRecent;
			
			if (m_pMostRecent == pTScript)
				m_pMostRecent = pTScript->pLessRecent;
			
			if (m_pLestRecent == pTScript)
				m_pLestRecent = pTScript->pMoreRecent;
			
			return DeleteScript(pTScript);
		}
	}
	else
		return FALSE;
	
	return TRUE;
}



//---------------------------------------------------------------------------
// ����:	KScriptSet::DeleteScript
// ����:	
// ����:	TScript * pTScript
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KScriptSet::DeleteScript(TScript * pTScript)
{
	
	if (pTScript == NULL)
		return FALSE;
	TScript * pDelTScript;
	
	pDelTScript = BTDelete(pTScript, &m_pRootList, m_nKeyStyle);
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KScriptSet::InsertScript
// ����:	
// ����:	char * szKey
// ����:	KScript * pScript
// ����:	BOOL 
//---------------------------------------------------------------------------
TScript*  KScriptSet::InsertScript(char * szKey)
{
	BOOL nResult = 0;
	TScript * pTScript = NULL;
	//TScript * pNewTScript = NULL;
	pTScript = BTSearch(m_pRootList, m_pRootList, szKey, &nResult);
	
	if (nResult == 1)
	{
		printf("Have Exist This Key In TScript Sets\n");
		return NULL;
		
	}
	else if (nResult == 0)
		return BTInsert(pTScript, szKey);
	else return NULL;
}

//---------------------------------------------------------------------------
// ����:	KScriptSet::InsertScript
// ����:	
// ����:	DWORD nKey
// ����:	TScript*  
//---------------------------------------------------------------------------
TScript*  KScriptSet::InsertScript(DWORD nKey)
{
	BOOL nResult = 0;
	TScript * pTScript = NULL;
	//TScript * pNewTScript = NULL;
	pTScript = BTSearch(m_pRootList, m_pRootList, nKey, &nResult);
	
	if (nResult == 1)
	{
		printf("Have Exist This Key In TScript Sets\n");
		return NULL;
		
	}
	else if (nResult == 0)
		return BTInsert(pTScript, nKey);
	else return NULL;
}



//---------------------------------------------------------------------------
// ����:	KScriptSet::GetScriptStackSize
// ����:	
// ����:	char * szKey
// ����:	int 
//---------------------------------------------------------------------------
int KScriptSet::GetInitStackSize(char * szKey)
{
	return 0;
}

//---------------------------------------------------------------------------
// ����:	KScriptSet::GetScriptStackSize
// ����:	
// ����:	DWORD nKey
// ����:	int 
//---------------------------------------------------------------------------
int KScriptSet::GetInitStackSize(DWORD nKey)
{
	return 0;
}


//---------------------------------------------------------------------------
// ����:	KScriptSet::ListScriptsKey
// ����:	
// ����:	void 
//---------------------------------------------------------------------------
DWORD  KScriptSet::ListScriptsKey()
{
	return BTPreorder(m_pRootList);
}

DWORD  KScriptSet::ListRecent(int order = 0)
{
	TScript * pTScript;
	DWORD nCount = 0;
	
	if (order == 0)
	{
		pTScript = m_pMostRecent;
		
		while(pTScript)
		{
			//	printf("%s\n",pTScript->szKey);
			pTScript = pTScript->pLessRecent;
			nCount ++;
		}
		return nCount;
		
	}
	else
	{
		pTScript = m_pLestRecent;
		
		while(pTScript)
		{
			//	printf("%s\n",pTScript->szKey);
			pTScript = pTScript->pMoreRecent;
			nCount ++;
		}
		return nCount;
		
	}
	
}


BOOL KScriptSet::Run(char * szKey)
{		
	return TRUE;
}

BOOL KScriptSet::Run(DWORD nKey)
{		
	return TRUE;
}


BOOL KScriptSet::RunFunction(char * szKey, char * szFuncName, char * szFormat, ...)
{
	return TRUE;
}


BOOL KScriptSet::RunFunction(DWORD nKey, char * szFuncName, char * szFormat, ...)
{
	return TRUE;
}


//---------------------------------------------------------------------------
// ����:	BTSearch
// ����:	�����Ƿ����ָ���Ĺؼ��ֵĶ�������㣬���������*pResult = 1,������Ӧָ�룻
//			����Ļ�*pResult = 0,������ӽ���ָ�루��ָ��Խ��������ã�
// ����:	TScript * pParentTScript Ϊ�ݹ���ô��븸���
// ����:	TScript * pTScript
// ����:	char * szKey	�ؼ���
// ����:	BOOL * pResult ָ���Ƿ��ҵ��ý��ı�־
// ����:	TScript * 
//---------------------------------------------------------------------------
TScript * BTSearch(TScript * pParentTScript, TScript * pTScript, char * szKey, BOOL * pResult)
{
	
	if (pTScript == NULL || pParentTScript == NULL)
	{
		*pResult = FALSE;
		return pParentTScript;
	}
	
	int nResult = strcmp(szKey, pTScript->szKey);
	
	if (nResult == 0)
	{
		*pResult = TRUE;
		return pTScript;
	}
	else if (nResult > 0)
		return BTSearch(pTScript, pTScript->pRightChild, szKey, pResult);
	else
		return BTSearch(pTScript, pTScript->pLeftChild, szKey, pResult);
	
}


//---------------------------------------------------------------------------
// ����:	BTSearch
// ����:	
// ����:	TScript * pParentTScript
// ����:	TScript * pTScript
// ����:	DWORD nKey
// ����:	BOOL * pResult
// ����:	TScript * 
//---------------------------------------------------------------------------
TScript * BTSearch(TScript * pParentTScript, TScript * pTScript, DWORD nKey, BOOL * pResult)
{
	
	if (pTScript == NULL || pParentTScript == NULL)
	{
		*pResult = FALSE;
		return pParentTScript;
	}
	
		
	if (nKey == pTScript->nKey)
	{
		*pResult = TRUE;
		return pTScript;
	}
	else if (nKey > pTScript->nKey)
		return BTSearch(pTScript, pTScript->pRightChild, nKey, pResult);
	else
		return BTSearch(pTScript, pTScript->pLeftChild, nKey, pResult);
	
}



//---------------------------------------------------------------------------
// ����:	BTInsert
// ����:	��pTScrptΪ����������szKeyΪ�ؼ���Ľ��
// ����:	TScript *pTScript 
// ����:	char * szKey
// ����:	TScript * ���ز���Ľ��
//---------------------------------------------------------------------------
TScript * BTInsert(TScript *pTScript, char * szKey)
{
	TScript * pNewTScript = new TScript;
	int nResult = 0;
	if (pNewTScript == NULL)
		return NULL;
	
	pNewTScript->pLeftChild = NULL;
	pNewTScript->pParent = NULL;
	pNewTScript->pRightChild = NULL;
	strcpy(pNewTScript->szKey,szKey);
	
	nResult = strcmp(szKey, pTScript->szKey);
	
	if (nResult == 0)
		return NULL;
	//���ݴ�Сȷ�����ӻ�������
	if (nResult > 0)
	{
		pTScript->pRightChild = pNewTScript;
		pNewTScript->pParent = pTScript;
	}
	else 
	{
		pTScript->pLeftChild = pNewTScript;
		pNewTScript->pParent = pTScript;
	}
	return pNewTScript;
	
}

//---------------------------------------------------------------------------
// ����:	BTInsert
// ����:	
// ����:	TScript *pTScript
// ����:	DWORD nKey
// ����:	TScript * 
//---------------------------------------------------------------------------
TScript * BTInsert(TScript *pTScript, DWORD nKey)
{
	TScript * pNewTScript = new TScript;
	int nResult = 0;
	if (pNewTScript == NULL)
		return NULL;
	
	pNewTScript->pLeftChild = NULL;
	pNewTScript->pParent = NULL;
	pNewTScript->pRightChild = NULL;
	pNewTScript->nKey = nKey;
	
		
	if (nKey == pTScript->nKey)
		return NULL;
	//���ݴ�Сȷ�����ӻ�������
	if (nKey > pTScript->nKey)
	{
		pTScript->pRightChild = pNewTScript;
		pNewTScript->pParent = pTScript;
	}
	else 
	{
		pTScript->pLeftChild = pNewTScript;
		pNewTScript->pParent = pTScript;
	}
	return pNewTScript;
	
}




//---------------------------------------------------------------------------
// ����:	 BTDelete
// ����:	ɾ�����
// ����:	TScript * pTScript
// ����:	TScript ** ppRootTScript
// ����:	TScript * 
//---------------------------------------------------------------------------
TScript *  BTDelete(TScript * pTScript, TScript ** ppRootTScript, int nKeyStyle)
{
	TScript * pFindTScript;
	if (pTScript == NULL)
		return NULL;
	
	if (pTScript->pLeftChild == NULL && pTScript->pRightChild == NULL)//�ý��û��������
	{
		if (*ppRootTScript == pTScript)
		{
			*ppRootTScript = NULL;
		}
		else
		{
			int nResult ;
			
			if (nKeyStyle == KEY_STRING)
			{
				nResult = strcmp(pTScript->szKey, pTScript->pParent->szKey);
			}
			else
			{
				if (pTScript->nKey < pTScript->pParent->nKey)
					nResult = -1;
				else
					nResult = 1;

			}
			

			if (nResult < 0)//С��
				pTScript->pParent->pLeftChild = NULL;
			else
				pTScript->pParent->pRightChild = NULL;
			
		}
		
		delete pTScript;
		return NULL;
	}
	
	else if (!(pTScript->pLeftChild && pTScript->pRightChild))//ֻ�е���ʱ
	{
		if (pTScript == *ppRootTScript)
		{
			if (pTScript->pLeftChild)
			{
				*ppRootTScript = pTScript->pLeftChild;
				pTScript->pLeftChild->pParent = NULL;
			}
			else
			{
				*ppRootTScript = pTScript->pRightChild;
				pTScript->pRightChild->pParent = NULL;
			}
			delete pTScript;
			return	NULL;
		}
		

		int nResult ;
		
		if (nKeyStyle == KEY_STRING)
		{
			nResult = strcmp(pTScript->szKey, pTScript->pParent->szKey);
		}
		else
		{
			if (pTScript->nKey < pTScript->pParent->nKey)
				nResult = -1;
			else
				nResult = 1;
			
		}

		
		if (nResult < 0)//�ڸ������
		{
			
			if (pTScript->pLeftChild)//ֻ������
			{
				pTScript->pParent->pLeftChild = pTScript->pLeftChild;
				pTScript->pLeftChild->pParent = pTScript->pParent;
			}
			else
			{
				pTScript->pParent->pLeftChild = pTScript->pRightChild;
				pTScript->pRightChild->pParent = pTScript->pParent;
			}
			
			delete pTScript;
			
		}
		else 
		{
			if (pTScript->pLeftChild)//ֻ������
			{
				pTScript->pParent->pRightChild = pTScript->pLeftChild;
				pTScript->pLeftChild->pParent = pTScript->pParent;
			}
			else
			{
				pTScript->pParent->pRightChild = pTScript->pRightChild;
				pTScript->pRightChild->pParent = pTScript->pParent;
			}
			delete pTScript;
			
		} 
		
		
		return NULL;
		
	}  
	else//��ȫ�� 
	{
		
		//if (strcmp(pTScript, pTScript->pParent) < 0)//�ڸ�������
		{
			
			pFindTScript = BTFindLess(pTScript->pLeftChild);
			
			if (pFindTScript)
			{
				
				//��һ���֣�����ý��������ʱ�������ӽ���ָ��ĸı�
				//�ý����������
				if (pFindTScript->pLeftChild)
				{
					//����������Ҫ����Ľ��������������ʱ����ԭ���Ĺ�ϵ�����仯����������˼·�ı�
					if (pFindTScript != pTScript->pLeftChild)
					{
						pFindTScript->pParent->pRightChild = pFindTScript->pLeftChild;
						pFindTScript->pLeftChild->pParent = pFindTScript->pParent;
					}
				}
				else
				{
					if (pFindTScript != pTScript->pLeftChild)
					{
						pFindTScript->pParent->pRightChild = NULL;
					}
					
				}
				
				//�ڶ����֣��ı��滻������ᣬʵ����ԭ�����ͬ��
				//�������滻�Ľ��Ϊɾ���������ӵ��������
				if (pFindTScript == pTScript->pLeftChild)
				{
					//�����Ӳ��ý���������ԭ״
					pTScript->pRightChild->pParent = pFindTScript;
					
					//�����Ӳ��ý���,����ԭ״	
					pFindTScript->pRightChild = pTScript->pRightChild;
					pFindTScript->pParent = pTScript->pParent;
					
				}
				else
				{
					
					pTScript->pLeftChild->pParent = pFindTScript;
					pTScript->pRightChild->pParent = pFindTScript;
					
					
					pFindTScript->pLeftChild = pTScript->pLeftChild ; 
					pFindTScript->pRightChild = pTScript->pRightChild;
					pFindTScript->pParent = pTScript->pParent;
					
				}
				
				
				//��������   ɾ�����֮���������
				
				if (*ppRootTScript == pTScript)
				{
					*ppRootTScript = pFindTScript;
					pFindTScript->pParent = NULL;
				}
				else 
				{

					int nResult ;
					
					if (nKeyStyle == KEY_STRING)
					{
						nResult = strcmp(pTScript->szKey, pTScript->pParent->szKey);
					}
					else
					{
						if (pTScript->nKey, pTScript->pParent->nKey)
							nResult = -1;
						else
							nResult = 1;
						
					}


					if (nResult < 0)//�ڸ�������
					{
						pTScript->pParent->pLeftChild = pFindTScript;
					}
					else
					{
						pTScript->pParent->pRightChild = pFindTScript;
					}
				}
				
				
				delete pTScript;
				return pFindTScript;
			}
			else
				return NULL;
		}
	
	}	
	
	
}

//---------------------------------------------------------------------------
// ����:	BTFindLess
// ����:	
// ����:	TScript * pTScript
// ����:	TScript * 
//---------------------------------------------------------------------------
TScript * BTFindLess(TScript * pTScript)
{
	if (pTScript == NULL)
		return NULL;
	
	if (pTScript->pRightChild == NULL )
		return pTScript;
	else
		return BTFindLess(pTScript->pRightChild);
	
	
}

//---------------------------------------------------------------------------
// ����:	BTPreorder
// ����:	
// ����:	TScript * pTScript
// ����:	DWORD  
//---------------------------------------------------------------------------
DWORD  BTPreorder(TScript * pTScript)//�������
{
	static DWORD nCount = 0;
	if (pTScript != NULL)
	{
		
		BTPreorder(pTScript->pLeftChild);
		//printf("nKey [%d%s\n", pTScript->szKey);
		nCount ++;
		BTPreorder(pTScript->pRightChild);
	}	
	return nCount;
	
}




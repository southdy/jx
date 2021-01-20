//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KLuaScriptSet.cpp
// Date:	2001-10-18 11:11:18
// Code:	Romandou
// Desc:	
//---------------------------------------------------------------------------
#include "KWin32.h"

/*
#include "KDebug.h"
#include "KLuaScriptSet.h"

//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::KLuaScriptSet
// ����:	
// ����:	
//---------------------------------------------------------------------------
KLuaScriptSet::KLuaScriptSet()
{
	KScriptSet::KScriptSet();
	m_nMaxScriptNum = MAXLUASCRIPTNUM;
	m_nNumPerClear =  NUMPERCLEAR;
}

//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::~KLuaScriptSet
// ����:	
// ����:	
//---------------------------------------------------------------------------
KLuaScriptSet::~KLuaScriptSet()
{
	KScriptSet::~KScriptSet();
}

//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::CreateScript
// ����:	
// ����:	char * szKey
// ����:	int StackSize
// ����:	KScript * 
//---------------------------------------------------------------------------
KScript * KLuaScriptSet::CreateScript(char * szKey, int StackSize)
{
	//����
	ClearUpSet();	
	if (szKey == NULL && StackSize < 0) return NULL;
	char * szFileName =  GetScriptFileNameFromKey(szKey);
	if (szFileName == NULL ) return NULL;
	
	KLuaScript * pScript = new KLuaScript(StackSize);
	if (! pScript) return NULL;
	
	//�ű�����Ľű���Ϊ�ؼ�����
	if (! pScript->Init()) return NULL;
	if (! pScript->Load(szFileName)) return NULL;
	return pScript;
}

KScript * KLuaScriptSet::CreateScript(DWORD nKey, int StackSize)
{
	//����
	ClearUpSet();	
	if (StackSize < 0) return NULL;
	char * szFileName =  GetScriptFileNameFromKey(nKey);
	if (szFileName == NULL ) return NULL;
	
	KLuaScript * pScript = new KLuaScript(StackSize);
	if (! pScript) return NULL;
	
	//�ű�����Ľű���Ϊ�ؼ�����
	if (! pScript->Init()) return NULL;
	if (! pScript->Load(szFileName)) return NULL;
	
	return pScript;
}


//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::ClearUpSet
// ����:	
// ����:	void
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KLuaScriptSet::ClearUpSet(void)
{
	
	TScript * pTScript = NULL;
	TScript * pDelTScript = NULL;

	if (GetCount() >= m_nMaxScriptNum) 
	{
		pTScript = m_pLestRecent;
//	printf("ClearUpSet\n");
		for (DWORD i = 0; i < m_nNumPerClear;i++)
		{
			pDelTScript = pTScript;
			pTScript = pTScript->pMoreRecent;
			DeleteScript(pDelTScript->szKey);
		}
	
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::ClearUpSet
// ����:	�������ʹ�ýű���˳��ɾ��nCount���ű�ʵ��
// ����:	DWORD nCount
// ����:	int ����ʵ��ɾ���Ľű�����
//---------------------------------------------------------------------------
DWORD KLuaScriptSet::ClearUpSet(DWORD nCount)
{
	
	TScript * pTScript = NULL;
	TScript * pDelTScript = NULL;
	
	DWORD	nUsedCount = GetCount();
	
	if (nUsedCount < nCount)
		nCount = nUsedCount;

		pTScript = m_pLestRecent;
		
		for (DWORD i = 0; i < nCount; i++)
		{
			pDelTScript = pTScript;
			pTScript = pTScript->pMoreRecent;
			DeleteScript(pDelTScript->szKey);
		}
		
	return nCount;

}

//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::SetMaxScriptNum
// ����:	
// ����:	DWORD nMaxCount
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KLuaScriptSet::SetMaxScriptNum(DWORD nMaxCount )
{
	if (nMaxCount < 0)
		return FALSE;
	
	m_nMaxScriptNum = nMaxCount;
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScriptSet::SetNumPerClear
// ����:	
// ����:	DWORD nNumPerClear
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL KLuaScriptSet::SetNumPerClear(DWORD nNumPerClear)
{
	if (nNumPerClear < 0 )
		return FALSE;
	
	if (m_nMaxScriptNum < nNumPerClear)
		return FALSE;
	
	m_nNumPerClear = nNumPerClear;
	return TRUE;
}


BOOL KLuaScriptSet::Run(DWORD nKey)
{
	KLuaScript *pScript ;
	pScript = (KLuaScript* ) this->GetScript(nKey,1,1);
	if (pScript)
	{
		return pScript->Execute();
	}
	return FALSE;
	
}

BOOL KLuaScriptSet::Run(char * szKey)
{
	KLuaScript *pScript ;
	pScript = (KLuaScript* ) this->GetScript(szKey,1,1);
	if (pScript)
	{
		return pScript->Execute();
	}
	return FALSE;
	
}


BOOL KLuaScriptSet::RunFunction(DWORD nKey, char * szFuncName, int nResults, char * szFormat, ...)
{
	KLuaScript * pScript;
	pScript = (KLuaScript *) this->GetScript(nKey, 1, 1);
	if (pScript)
	{
		va_list va;
		BOOL bResult;
		va_start(va, szFormat);
		bResult = pScript->CallFunction(szFuncName, nResults, szFormat, va);
		va_end(va);
		return bResult;

	}
	return FALSE;

}

BOOL KLuaScriptSet::RunFunction(char * szKey, char * szFuncName, int nResults, char * szFormat, ...)
{
	KLuaScript * pScript;
	pScript = (KLuaScript *) this->GetScript(szKey, 1, 1);
	
	if (pScript)
	{
		va_list va;
		BOOL bResult;
		va_start(va, szFormat);
		bResult = pScript->CallFunction(szFuncName, nResults, szFormat, va);
		va_end(va);
		return bResult;
		
	}
	return FALSE;

}

*/

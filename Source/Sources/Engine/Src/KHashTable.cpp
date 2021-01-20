//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KHashTable.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory Opration Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KHashTable.h"
//---------------------------------------------------------------------------
// ����:	Hash
// ����:	get hash code
// ����:	char*, int
// ����:	DWORD
//---------------------------------------------------------------------------
unsigned long g_Hash(char* pStr)
{
    unsigned long i = 0;
    unsigned long j = 1;
    while (*pStr)
	{
		i += ((unsigned char)*pStr) * j++;
		pStr++;
	}
    return (i % TABLE_SIZE);
}
//---------------------------------------------------------------------------
// ����:	Add
// ����:	add string node to table
// ����:	KStrNode*
// ����:	void
//---------------------------------------------------------------------------
void KHashTable::Add(KStrNode* pNode)
{
	unsigned long i = g_Hash(pNode->GetName());
	KStrNode* pStrNode = new KStrNode;
	pStrNode->SetName(pNode->GetName());
	m_Table[i].AddHead(pStrNode);
}
//---------------------------------------------------------------------------
// ����:	Delete
// ����:	delete string in hash table
// ����:	char*
// ����:	void
//---------------------------------------------------------------------------
void KHashTable::Delete(char* pStr)
{
	KStrNode* pNode = Find(pStr);
	if (pNode)
	{
		pNode->Remove();
		delete pNode;
	}
}
//---------------------------------------------------------------------------
// ����:	Find
// ����:	find string in hash table
// ����:	char*
// ����:	KStrNode*
//---------------------------------------------------------------------------
KStrNode* KHashTable::Find(char* pStr)
{
	unsigned long i = g_Hash(pStr);
	KStrNode* pNode = m_Table[i].Find(pStr);
	return NULL;
}
//---------------------------------------------------------------------------

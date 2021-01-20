//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KStrList.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Simple double linked node class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KStrBase.h"
#include "KStrList.h"
//---------------------------------------------------------------------------
// ����:	Find
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KStrNode* KStrList::Find(char* str)
{
	KStrNode* pNode = (KStrNode*) GetHead();
	while (pNode)
	{
		if (g_StrCmp(str, pNode->GetName()))
			return pNode;
		pNode = (KStrNode*) pNode->GetNext();
	}
	return NULL;
}
//--------------------------------------------------------------------------------

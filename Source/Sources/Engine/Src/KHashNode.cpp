//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KHashNode.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Simple double linked node class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KStrBase.h"
#include "KHashNode.h"
//---------------------------------------------------------------------------
// ����:	KHashNode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KHashNode::KHashNode(void)
{
	m_pHashTable = NULL;
}
//---------------------------------------------------------------------------
// ����:	SetName
// ����:	set node name
// ����:	char*
// ����:	void
//---------------------------------------------------------------------------
void KHashNode::SetName(char* pStr)
{
	if (KStrNode::IsLinked())
	{
		m_pHashTable->Delete(m_Name);
		KStrNode::SetName(pStr);
		m_pHashTable->Add(this);
	}
	else
	{
		KStrNode::SetName(pStr);
	}
}
//--------------------------------------------------------------------------------

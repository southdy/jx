//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMsgNode.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Simple double linked node class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KMemManager.h"
#include "KMsgNode.h"
//---------------------------------------------------------------------------
// ����:	KMsgNode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMsgNode::KMsgNode()
{
	m_pBuffer = NULL;
	m_nBufLen = 0;
}
//---------------------------------------------------------------------------
// ����:	~KMsgNode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMsgNode::~KMsgNode()
{
	if (m_pBuffer)
		g_MemManager.Free(m_pBuffer);
}
//---------------------------------------------------------------------------
// ����:	SetName
// ����:	set node name
// ����:	char*
// ����:	void
//---------------------------------------------------------------------------
void KMsgNode::SetMsg(PVOID pBuf, ULONG nLen)
{
	if (m_pBuffer)
		g_MemManager.Free(m_pBuffer);
	m_pBuffer = g_MemManager.Malloc(nLen);
	g_MemCopy(m_pBuffer, pBuf, nLen);
	m_nBufLen = nLen;
}
//--------------------------------------------------------------------------------

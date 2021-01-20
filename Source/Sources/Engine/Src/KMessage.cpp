//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMessage.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Message Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KList.h"
#include "KNode.h"
#include "KMemManager.h"
#include "KMessage.h"
//---------------------------------------------------------------------------
static KList		m_MsgList;
static int			m_nMsgNum = 0;
//---------------------------------------------------------------------------
#define MAX_MSG		100
//---------------------------------------------------------------------------
class KMsgNode : public KNode
{
public:
	int	m_nType;		// ��Ϣ����
	int	m_nParam[3];	// ��Ϣ����
};
//---------------------------------------------------------------------------
// ����:	Send Message
// ����:	������Ϣ
// ����:	Type		��Ϣ����
//			Param1		����1
//			Param2		����2
//			Param3		����3
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_SendMessage(int nType, int nP0, int nP1, int nP2)
{
	if (m_nMsgNum >= MAX_MSG)
		return;
	m_nMsgNum++;

	KMsgNode* pNode = (KMsgNode *) g_MemManager.Calloc(sizeof(KMsgNode));
	pNode->m_pNext = NULL;
	pNode->m_pPrev = NULL;
	pNode->m_nType = nType;
	pNode->m_nParam[0] = nP0;
	pNode->m_nParam[1] = nP1;
	pNode->m_nParam[2] = nP2;
	m_MsgList.AddHead(pNode);
}
//---------------------------------------------------------------------------
// ����:	Peek Message
// ����:	��̽��һ����Ϣ
// ����:	pMsg	��Ϣָ��
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
ENGINE_API BOOL g_PeekMessage(KMessage* pMsg)
{
	KMsgNode* pNode = (KMsgNode *)m_MsgList.GetHead();
	if (pNode)
	{
		pMsg->nType = pNode->m_nType;
		pMsg->nParam[0] = pNode->m_nParam[0];
		pMsg->nParam[1] = pNode->m_nParam[1];
		pMsg->nParam[2] = pNode->m_nParam[2];
		return TRUE;
	}
	pMsg->nType = TM_NONE;
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Get Message
// ����:	ȡ����һ����Ϣ
// ����:	pMsg	��Ϣָ��
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
ENGINE_API BOOL g_GetMessage(KMessage* pMsg)
{
	KMsgNode* pNode = (KMsgNode *)m_MsgList.GetHead();
	if (pNode)
	{
		pMsg->nType = pNode->m_nType;
		pMsg->nParam[0] = pNode->m_nParam[0];
		pMsg->nParam[1] = pNode->m_nParam[1];
		pMsg->nParam[2] = pNode->m_nParam[2];
		pNode->Remove();
		g_MemManager.Free(pNode);
		m_nMsgNum--;
		return TRUE;
	}
	pMsg->nType = TM_NONE;
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Clear
// ����:	�����Ϣ����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
ENGINE_API void g_ClearMessage()
{
	KMessage Msg;
	while (g_GetMessage(&Msg)){};
}
//---------------------------------------------------------------------------

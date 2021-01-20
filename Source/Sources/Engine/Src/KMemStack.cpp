//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemStack.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory Stack Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KMemStack.h"
//---------------------------------------------------------------------------
// ����:	KMemStack
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMemStack::KMemStack()
{
	g_MemZero(m_pStack, sizeof(m_pStack));
	m_nStackTop  = 0;
	m_nStackEnd  = 0;
	m_nChunkTop  = 0;
	m_nChunkSize = 65536;
}
//---------------------------------------------------------------------------
// ����:	~KMemStack
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMemStack::~KMemStack()
{
	FreeAllChunks();
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	��ʼ��
// ����:	nChunkSize	Chunk�ߴ��С
// ����:	void
//---------------------------------------------------------------------------
void KMemStack::Init(int nChunkSize)
{
	FreeAllChunks();
	m_nChunkSize = nChunkSize;
}
//---------------------------------------------------------------------------
// ����:	Push
// ����:	�����ڴ�
// ����:	nSize	�ڴ��С
// ����:	PVOID	�ڴ�ָ�룬= NULL ��ʾ����ʧ��
//---------------------------------------------------------------------------
PVOID KMemStack::Push(int nSize)
{
	PBYTE pResult;

	if (m_nChunkSize < nSize)
		m_nChunkSize = nSize;

	if (m_nStackTop + nSize > m_nStackEnd)
	{
		if (!AllocNewChunk())
			return NULL;
	}

	pResult = m_pStack[m_nChunkTop - 1];
	pResult += m_nStackTop;
	m_nStackTop += nSize;
	return pResult;
}
//---------------------------------------------------------------------------
// ����:	Free
// ����:	�ͷ��ڴ��
// ����:	pMem	�ڴ��ָ��
// ����:	void
//---------------------------------------------------------------------------
void KMemStack::Free(PVOID pMem)
{
	return;
}
//---------------------------------------------------------------------------
// ����:	AllocNewChunk
// ����:	�����µ��ڴ�飬�������Ϊ0
// ����:	void
// ����:	TRUE	�ɹ� 
//			FALSE	ʧ��
//---------------------------------------------------------------------------
BOOL KMemStack::AllocNewChunk()
{
	if (m_nChunkTop >= MAX_CHUNK)
	{
		g_MessageBox("KMemStack : Chunk over flow");
		return FALSE;
	}

	m_pStack[m_nChunkTop] = (PBYTE)g_MemAlloc(m_nChunkSize);
	if (m_pStack[m_nChunkTop] == NULL)
		return FALSE;

	m_nChunkTop++;
	m_nStackTop = 0;
	m_nStackEnd = m_nChunkSize;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FreeAllChunks
// ����:	�ͷ����е��ڴ��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMemStack::FreeAllChunks()
{
	for (int i = 0; i < m_nChunkTop; i++)
	{
		g_MemFree(m_pStack[i]);
		m_pStack[i] = NULL;
	}
	m_nStackTop = 0;
	m_nStackEnd = 0;
	m_nChunkTop = 0;
}
//---------------------------------------------------------------------------
// ����:	GetChunkSize
// ����:	ȡ�ö�ջChunk�Ĵ�С
// ����:	void
// ����:	Chunk �Ĵ�С in bytes
//---------------------------------------------------------------------------
int KMemStack::GetChunkSize()
{
	return m_nChunkSize;
}
//---------------------------------------------------------------------------
// ����:	GetStackSize
// ����:	ȡ���ڴ��ջ�Ĵ�С
// ����:	void
// ����:	�ڴ��ջ�Ĵ�С in bytes
//---------------------------------------------------------------------------
int KMemStack::GetStackSize()
{
	return (m_nStackTop + (m_nChunkTop - 1) * m_nChunkSize);
}
//---------------------------------------------------------------------------

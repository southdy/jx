//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KSortList.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory Opration Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KSortList.h"
//---------------------------------------------------------------------------
// ����:	KSortList
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KSortList::KSortList()
{
	m_nItemNum   = 0;
	m_nItemSize  = 0;
	m_nItemCount = 0;
}
//---------------------------------------------------------------------------
// ����:	~KSortList
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KSortList::~KSortList()
{
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	��ʼ��
// ����:	nItemSize		Size of Item
//			nItemCount		Count of Item
// ����:	TRUE			Success
//			FALSE			Fail
//---------------------------------------------------------------------------
BOOL KSortList::Init(int nItemSize, int nItemCount)
{
	m_nItemNum   = 0;
	m_nItemSize  = nItemSize;
	m_nItemCount = nItemCount;
	if (!m_ListMem.Alloc(m_nItemSize * m_nItemCount))
		return FALSE;
	if (!m_SortMem.Alloc(sizeof(SORT) * m_nItemCount))
		return FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Free
// ����:	�ͷ��ڴ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KSortList::Free()
{
	m_nItemNum   = 0;
	m_nItemSize  = 0;
	m_nItemCount = 0;
	m_ListMem.Free();
	m_SortMem.Free();
}
//---------------------------------------------------------------------------
// ����:	Insert
// ����:	����
// ����:	dwHashCode		��ϣֵ
//			pvItem			��������ڴ��
// ����:	TRUE			Success
//			FALSE			Fail
//---------------------------------------------------------------------------
BOOL KSortList::Insert(DWORD dwHashCode, PVOID pvItem)
{
	// check item number range
	if (m_nItemNum >= m_nItemCount)
		return FALSE;

	// insert item to list
	PBYTE pList = (PBYTE)m_ListMem.GetMemPtr();
	if (pList == NULL)
		return FALSE;
	pList += m_nItemNum * m_nItemSize;
	g_MemCopy(pList, pvItem, m_nItemSize);

	// insert item to sort table
	PSORT pSort = (PSORT)m_SortMem.GetMemPtr();
	if (pSort == NULL)
		return FALSE;
	int  i, j;
	for (i = 0; i < m_nItemNum; i++)
	{
		if (dwHashCode < pSort[i].HashCode)
			break;
		if (dwHashCode == pSort[i].HashCode)
			return FALSE;
	}
	for (j = m_nItemNum; j > i; j--)
	{
		pSort[j] = pSort[j - 1];
	}
	pSort[i].HashCode = dwHashCode;
	pSort[i].ListItem = pList;
	m_nItemNum++;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Search
// ����:	�ڱ�������ƥ�����
// ����:	dwHashCode		������Ĺ�ϣֵ
//			pvItem			��������ڴ��
// ����:	TRUE			Success
//			FALSE			Fail
//---------------------------------------------------------------------------
BOOL KSortList::Search(DWORD dwHashCode, PVOID pvItem)
{
	int		nBegin;
	int		nEnd;
	int		nMid;
	PSORT	pSort;

	// Get sort table entrey
	pSort = (PSORT)m_SortMem.GetMemPtr();
	if (pSort == NULL)
		return FALSE;

	// ���ַ�����ƥ���HashCode
	nBegin = 0;
	nEnd = m_nItemNum - 1;
	while (nBegin <= nEnd)
	{
		nMid = (nBegin + nEnd) / 2;
		if (dwHashCode == pSort[nMid].HashCode)
		{
			g_MemCopy(pvItem, pSort[nMid].ListItem, m_nItemSize);
			return TRUE;
		}
		if (dwHashCode < pSort[nMid].HashCode)
			nEnd = nMid - 1;
		else
			nBegin = nMid + 1;
	}
	return FALSE;
}
//---------------------------------------------------------------------------

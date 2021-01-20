/*******************************************************************************
// FileName			:	KMRU.cpp
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-12 17:43:32
// FileDescription	:	���ʹ�ñ���
// Revision Count	:
*******************************************************************************/
#include "KMRU.h"
#include "memory.h"
#include "malloc.h"

/*!**************************************************************************
// Purpose		: ���캯��
*****************************************************************************/
KMRU::KMRU()
{
	m_nCountInTurn = 0;
	m_nItemCount = 0;
	m_nMaxItemNum = 0;
	m_pItemTable = 0;
}

/*!**************************************************************************
// Purpose		: ��������
*****************************************************************************/
KMRU::~KMRU()
{
	Terminate();
}

/*!**************************************************************************
// Function		: KMRU::Init
// Purpose		: ��ʼ��
// Return		: bool �ɹ���
// Argumant		: int nMaxItemNum �����������ɵ������Ŀ
*****************************************************************************/
bool KMRU::Init(int nMaxItemNum)
{
	Terminate();
	if ((m_nMaxItemNum = nMaxItemNum) <= 0)
		return false;
	m_pItemTable = (KMruItem*)malloc(sizeof(KMruItem) *m_nMaxItemNum);
	memset(m_pItemTable, 0, sizeof(KMruItem) * m_nMaxItemNum);
	for( int i = 0; i < m_nMaxItemNum; i++)
	{
		m_pItemTable[i].Data = i;
	}
	return true;
}

/*!**************************************************************************
// Function		: KMRU::Terminate
// Purpose		: �������������
*****************************************************************************/
void KMRU::Terminate()
{
	m_nCountInTurn = 0;
	m_nItemCount = 0;
	m_nMaxItemNum = 0;
	if (m_pItemTable)
	{
		free(m_pItemTable);
		m_pItemTable = 0;
	}
}

/*!**************************************************************************
// Function		: KMRU::GetItemData
// Purpose		: ��ñ���ָ�������������
// Return		: unsigned short ����ָ������������ݣ����ָ��������ڷ���0
// Argumant		: int nIndex 	 ָ���������
*****************************************************************************/
unsigned short KMRU::GetItemData(int nIndex)
{
	if (nIndex >= m_nItemCount || nIndex < 0)
		return 0;
	return m_pItemTable[nIndex].Data;
}

/*!***************************************************************************
// Function		: KMRU::Update
// Purpose		: ִ��һ�ֽ���ʱMRU�����Ϣ���¡�
*****************************************************************************/
void KMRU::Update()
{
	int		i;
	for(i = 0; i < m_nItemCount; i++)
	{
		if (m_pItemTable[i].MRURecord != 0xff)
			m_pItemTable[i].MRURecord ++;
	}
}

/*!***************************************************************************
// Function		: KMRU::CommitText
// Purpose		: �ύ�ַ������õ�����ȫ���ַ�����ͼ�е�λ��
// Argumant		: unsigned char  *pszString  �ύ���ַ���
// Argumant		: int nCount	             �ύ�ַ��ĸ���
// Argumant		: unsigned short *pPositions ָ�����ڴ洢���ַ�����ͼ�е�λ�õĻ�����
// Argumant		: unsigned short *pNewItems  ָ�����ڴ洢�½�����е���Ļ�����
// Argumant		: int& nNumNewItem			 ���½�����е������Ŀ
*****************************************************************************/
void KMRU::Commit(unsigned short *pString, int nCount, unsigned short *pPositions,
					  unsigned short *pNewItems, int& nNumNewItem)
{
	if (pString == 0 || pPositions == 0 || pNewItems == 0 || nCount <= 0)
		return;

	unsigned short	nCode, i;
	unsigned int	nInsertSpace, nDueSpace;

	nNumNewItem = 0;

	//����Ƿ�ʼ�µ�һ�֣�
	if (m_nCountInTurn + nCount > m_nMaxItemNum)
	{
		m_nCountInTurn = 0;
		Update();
//		if (nCount > m_nMaxItemNum)
//			nCount = m_nMaxItemNum;
	}
	m_nCountInTurn += nCount;

	//��������е��ַ�
	for (i = 0; i < nCount; i++)
	{
		//ȡ���ַ��ı���
		nCode = *pString++;
		if (!GetCharPos(nCode, 0, m_nItemCount, nDueSpace))
		{	//�ַ�û���ڱ���
			if (m_nItemCount < m_nMaxItemNum)	//�������Ƿ��пյ���
				nInsertSpace = m_nItemCount++;
			else
				nInsertSpace = GetMostLongestUnusedItem();

			pNewItems[nNumNewItem * 2] = pPositions[i] = m_pItemTable[nInsertSpace].Data;
			pNewItems[nNumNewItem * 2 + 1] = m_pItemTable[nInsertSpace].Id = nCode;
			m_pItemTable[nInsertSpace].MRURecord = 0;

			//�����¼������ڱ��е�λ��
			if (nInsertSpace != nDueSpace)
				AdjustCharPos(nInsertSpace, nDueSpace);
			
			nNumNewItem ++;
		}
		else
		{
			pPositions[i] = m_pItemTable[nDueSpace].Data;
			m_pItemTable[nDueSpace].MRURecord = 0;
		}
	}	
}


/*!**************************************************************************
// Function		: KMRU::AdjustCharPos
// Purpose		: �����¼������ڱ��е�λ�ã�ʹ���е������Id����
// Argumant		: unsigned int nIndex    Ҫ��������ĵ�ǰλ��
// Argumant		: unsigned int nDueIndex Ҫ���������Ŀ��λ��
*****************************************************************************/
void KMRU::AdjustCharPos(unsigned int nIndex, unsigned int nDueIndex)
{
	KMruItem		Temp;

	Temp = m_pItemTable[nIndex];
	if (nDueIndex < nIndex)
	{	//Ӧ�ð�����ǰ��
		while(nIndex > nDueIndex)
		{
			m_pItemTable[nIndex] = m_pItemTable[nIndex - 1];
			nIndex --;
		}
		m_pItemTable[nDueIndex] = Temp;
	}
	else if (nDueIndex > nIndex + 1)
	{	//Ӧ�ð���������
		nDueIndex --;
		while (nIndex  < nDueIndex)
		{
			m_pItemTable[nIndex] = m_pItemTable[nIndex + 1];
			nIndex ++;
		}
		m_pItemTable[nDueIndex] = Temp;
	}
}

/*!**************************************************************************
// Function		: KMRU::GetCharPos
// Purpose		: �ڸ���Χ�ڶ�λһ���ַ���û�ҵ���������������Ӧ���õ�λ�ã�
// Return		: bool �Ƿ��ҵ�
// Argumant		: unsigned short Id	       Ҫ���ҵ����Id(�ַ��ı���)
// Argumant		: int nFrom	               ��Χ����
// Argumant		: int nTo		           �뷶Χ����
// Argumant		: unsigned int& nPosition  λ��
*****************************************************************************/
bool KMRU::GetCharPos(unsigned short Id, int nFrom, int nCount, unsigned int& nPosition)
{
	unsigned int	nMiddle;
	int		nTo = nFrom + nCount - 1;
	//Ҫ��nFromһ��ҪС�ڵ���nTo���Ҷ�����С��m_nItemCount
	//��Ϊ�˺����ǽӿڵ��ڲ�������ʡȥ�ԷǷ��������ж�
	
	if (!nCount)
	{
		nPosition = nFrom;
		return false;
	}
	//�Զ��ַ�����
	while(nFrom < nTo)
	{
		nMiddle = (nFrom + nTo) >> 1;   // div 2
		if (Id < m_pItemTable[nMiddle].Id)
		{
			nTo = nMiddle - 1;
			continue;
		}
		if (Id > m_pItemTable[nMiddle].Id)
		{
			nFrom = nMiddle + 1;
			continue;
		}
		nPosition = nMiddle;
		return true;
	};
	if (Id == m_pItemTable[nFrom].Id)
	{
		nPosition = nFrom;
		return true;
	}
	if (Id < m_pItemTable[nFrom].Id)
		nPosition = nFrom;
	else
		nPosition = nTo + 1;
	return false;
}

/*!*****************************************************************************
// Function		: KMRU::GetMostEarlyUsedItem
// Purpose		: �ڱ��������û�б�ʹ����
// Return		: unsigned int �ҵ����������
*****************************************************************************/
unsigned int KMRU::GetMostLongestUnusedItem()
{
	int	i, nMEU;
	nMEU = 0;
	for (i = 1; i < m_nItemCount; i++)
	{
		if (m_pItemTable[i].MRURecord > m_pItemTable[nMEU].MRURecord)
			nMEU = i;
	}
	return nMEU;
}

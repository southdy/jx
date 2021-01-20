/*******************************************************************************
// FileName			:	KCharSet.cpp
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-13 10:06:31
// FileDescription	:	�ַ�����
// Revision Count	:	2002-9-4��д����Ϊ����d3d��Wooy
*******************************************************************************/
#include "KCharSet.h"

/*!*****************************************************************************
// Purpose		:  ���캯��
*****************************************************************************/
KCharSet::KCharSet()
{
	m_nFontW = 0;
	m_nFontH = 0;
	m_nNumCharH = 0;
	m_fFontW = 0;
	m_fFontH = 0;
	m_fCharHInterval = 0;
	m_fCharVInterval = 0;
}

/*!*****************************************************************************
// Purpose		:  ��������
*****************************************************************************/
KCharSet::~KCharSet()
{
	Terminate();
}

/*!*****************************************************************************
// Function		: KCharSet::Init
// Purpose		: ��ʼ��
// Return		: bool �Ƿ�ɹ�
// Argumant		: const char *pszFontFile �ֿ��ļ�
// Argumant		: LPDIRECT3DDEVICE8 pd3dDevice -> directx 3d device�ӿڵ�ʵ����ָ��
*****************************************************************************/
bool KCharSet::Init(const char *pszFontFile, LPDIRECT3DDEVICE9 pd3dDevice)
{
	int		nCount;
	if (!m_Resource.Init(pszFontFile, pd3dDevice))
		return false;
	m_Resource.GetInfo(m_nFontW, m_nFontH, m_nNumCharH, nCount);
	m_Resource.GetInfo(m_fFontW, m_fFontH, m_fCharHInterval, m_fCharVInterval);

	if (!m_MruTable.Init(nCount))
		return false;
	return true;
}

/*!*****************************************************************************
// Function		: KCharSet::GetInfo
// Purpose		: ��ȡ������Ϣ
// Argumant		: int& nFontW �����
// Argumant		: int& nFontH �����
*****************************************************************************/
void KCharSet::GetInfo(int& nFontW, int& nFontH)
{
	nFontW = m_nFontW;
	nFontH = m_nFontH;
}

/*!*****************************************************************************
// Function		: KCharSet::Terminate
// Purpose		: �������������
*****************************************************************************/
void KCharSet::Terminate()
{
	m_MruTable.Terminate();
	m_Resource.Terminate();
	m_nFontW = 0;
	m_nFontH = 0;
	m_nNumCharH = 0;
}

/*!***************************************************************************
// Function		: KCharSet::CommitText
// Purpose		: �ύ�ַ��������㴮�и��ַ����õ�������ͼ���꣬���������ڡ�
// Argumant		: unsigned short *pszString	�ύ���ַ���������Ҫ��ʾ���ַ���û�п���������
// Argumant		: int nNumChars		        �ύ�˼����ַ�
// Argumant		: KFontVertex* pCharPolys   �ַ�����Ӧ�����ڻ����ַ����ı��δ�
*****************************************************************************/
void KCharSet::CommitText(unsigned short *pString, int nNumChars,  KFontVertex* pCharPolys)
{
	unsigned short	nPosInTexture[KCS_CHAR_NUM_LIMIT];
	unsigned short	nNewCharIntoTable[KCS_CHAR_NUM_LIMIT * 2];
	int				nCount, i;

	if (!pCharPolys)
		return;
	if (nNumChars > KCS_CHAR_NUM_LIMIT)
		nNumChars = KCS_CHAR_NUM_LIMIT;

	m_MruTable.Commit(pString, nNumChars, nPosInTexture, nNewCharIntoTable, nCount);
	
    for(i = 0; i < nNumChars; i++, pCharPolys += 6)
	{
        int nValue  = nPosInTexture[i];
        int nValue1 = nValue / m_nNumCharH;
        int nValue2 = nValue % m_nNumCharH;

		pCharPolys[0].u = nValue2 * m_fCharHInterval;
		pCharPolys[0].v = nValue1 * m_fCharVInterval;

		pCharPolys[4].u = pCharPolys[1].u = pCharPolys[0].u + m_fFontW;
		pCharPolys[4].v = pCharPolys[1].v = pCharPolys[0].v;
		
		pCharPolys[3].u = pCharPolys[2].u = pCharPolys[0].u;
		pCharPolys[3].v = pCharPolys[2].v = pCharPolys[0].v + m_fFontH;

		pCharPolys[5].u = pCharPolys[4].u;
		pCharPolys[5].v = pCharPolys[2].v;

	}
	m_Resource.Update(nNewCharIntoTable, nCount);
}

//�õ���ͼ���
LPDIRECT3DTEXTURE9 KCharSet::GetTexture() const
{
	return m_Resource.GetTexture();
}
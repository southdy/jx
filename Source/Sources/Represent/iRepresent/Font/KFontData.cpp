/*******************************************************************************
// FileName			:	KFontData.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2002-9-4
// FileDescription	:	����ͼ��������ͷ�ļ�
// Revision Count	:	
*******************************************************************************/
#include "KFontData.h"
#include "../../../Engine/Src/KWin32.h"
#include "../../../Engine/Src/KPakFile.h"

/*!*****************************************************************************
// Purpose		:  ���캯��
*****************************************************************************/
KFontData::KFontData()
{
	m_pdwOffs = NULL;
	m_pFontData = NULL;
	m_dwDataSize = 0;
	m_nFontW = 0;
	m_nFontH = 0;
}

/*!*****************************************************************************
// Purpose		:  ��������
*****************************************************************************/
KFontData::~KFontData()
{
	Terminate();
}

/*!*****************************************************************************
// Function		: KFontData::Init
// Purpose		: �����ֿ��ļ�
// Return		: �Ƿ�ɹ�
// Argumant		: const char* pszFontFile      -> �ֿ��ļ���
*****************************************************************************/
bool KFontData::Load(const char* pszFontFile)
{
	Terminate();

	bool bOk = false;
	while(pszFontFile)
	{
		KPakFile	File;
		KFontHead	Header;

		//���ֿ��ļ�
		if (File.Open((char*)pszFontFile) == FALSE)
			break;
		//���ֿ��ļ�ͷ�ṹ
		if (File.Read(&Header, sizeof(Header)) != sizeof(Header))
			break;

		//����ֿ��ļ�ID
		if (*((int*)(&Header.Id)) != 0x465341 ||	//"ASF"
			Header.Count <= 0 || Header.Size <= 0)
		{
			break;
		}

		//�����ַ�������Ϣ���ݵĴ洢�ռ�
		m_pdwOffs = (unsigned int*)malloc(sizeof(unsigned int) * Header.Count);
		m_pFontData = (unsigned char*)malloc(Header.Size);
		if (m_pdwOffs == NULL || m_pFontData == NULL)
			break;

		//�õ�������Ϣ
		m_dwDataSize = Header.Size;
		m_uCharCount = Header.Count;
		m_nFontW = Header.Width;
		m_nFontH = Header.Height;

		//��ȡƫ�Ʊ�
		if (File.Read(m_pdwOffs, m_uCharCount * sizeof(unsigned int)) != m_uCharCount * sizeof(unsigned int))
			break;

		//��ȡ�ֿ��������
		if (File.Read(m_pFontData, m_dwDataSize) == m_dwDataSize)
			bOk = true;

		//�ر��ֿ��ļ�
		File.Close();
		break;
	};


	if (bOk == false)
		Terminate();
	return bOk;
}

/*!*****************************************************************************
// Function		: KFontData::GetInfo
// Purpose		: �õ�����Ŀ�ȸ߶ȣ���ͼ���Ի�����ַ��ĸ���
// Argumant		: int &nWidth	�ַ���
// Argumant		: int &nHeight	�ַ���
*****************************************************************************/
void KFontData::GetInfo(int &nWidth, int &nHeight) const
{
	nWidth  = m_nFontW;
	nHeight = m_nFontH;
}

/*!*****************************************************************************
// Function		: KFontData::Terminate
// Purpose		: �������������
*****************************************************************************/
void KFontData::Terminate()
{
	if (m_pFontData)
	{
		free(m_pFontData);
		m_pFontData = NULL;
	}
	if (m_pdwOffs)
	{
		free (m_pdwOffs);
		m_pdwOffs = NULL;
	}
	m_uCharCount = 0;
	m_dwDataSize = 0;
}

/*!*****************************************************************************
// Function		: KFontData::GetCharacterData
// Purpose		: ȡ�õ����ַ���������
*****************************************************************************/
unsigned char*	KFontData::GetCharacterData(unsigned char cFirst, unsigned char cNext) const
{
	if (m_pFontData)
	{
		//�ַ����ֿ��������	//GBK���뷽ʽ����������
		unsigned int	uCharIndex = (cFirst - 0x81) * 190 + (cNext - 0x40) - (cNext >> 7);
		if (uCharIndex < m_uCharCount && m_pdwOffs[uCharIndex])
			return (m_pFontData + m_pdwOffs[uCharIndex]);
	}
	return NULL;
}
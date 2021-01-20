//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakData.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	DataFile class used in package
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KFilePath.h"
#include "KPakData.h"
//---------------------------------------------------------------------------
// ����:	KPakData()
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KPakData::KPakData()
{
	m_pCodec = NULL;
	m_nFilesInPack = 0;
	m_nCompressMethod = CODEC_NONE;
}
//---------------------------------------------------------------------------
// ����:	~KPakData()
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KPakData::~KPakData()
{
	Close();
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	��һ��Pak�ļ���
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KPakData::Open(LPSTR FileName)
{
	TPakFileHeader	Header;

	// colse opened file
	if (m_nFilesInPack)
		Close();

	// open data file
	if (!m_File.Open(FileName))
		return FALSE;

	// read file header
	m_File.Read(&Header, sizeof(Header));

	// check file signature
	if (!g_MemComp(Header.Signature, "PACKAGE", 7))
		return FALSE;

	// get file number in pack
	m_nFilesInPack = Header.FilesInPack;

	// get compress method
	m_nCompressMethod = Header.CompressMethod;

	// alloc index table memory
	if (!m_MemIndex.Alloc(m_nFilesInPack * sizeof(TPakFileIndex)))
		return FALSE;

	// read index table
	m_File.Read(m_MemIndex.GetMemPtr(),
		m_nFilesInPack * sizeof(TPakFileIndex));
	
	// init decoder
	g_InitCodec(&m_pCodec, m_nCompressMethod);

	// return true
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Read
// ����:	���ļ�����
// ����:	lpBuffer		��ȡ�����ݻ���
//			dwLength		��ȡ���ֽڳ���
// ����:	ʵ�ʶ�ȡ���ֽ���
//---------------------------------------------------------------------------
DWORD KPakData::Read(LPVOID lpBuffer, DWORD dwLength)
{
	return m_File.Read(lpBuffer, dwLength);
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ƶ��ļ�ָ��λ��
// ����:	pOffset			ƫ����
//			dwMethod		��׼λ��
// ����:	ʵ���ƶ�����λ��
//---------------------------------------------------------------------------
DWORD KPakData::Seek(LONG lOffset, DWORD dwMethod)
{
	return m_File.Seek(lOffset, dwMethod);
}
//---------------------------------------------------------------------------
// ����:	Tell
// ����:	ȡ���ļ�ָ��λ��
// ����:	��
// ����:	ʵ���ļ�ָ���λ��
//---------------------------------------------------------------------------
DWORD KPakData::Tell()
{
	return m_File.Tell();
}
//---------------------------------------------------------------------------
// ����:	Close
// ����:	�ر�һ�������ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KPakData::Close()
{
	g_FreeCodec(&m_pCodec, m_nCompressMethod);
	m_File.Close();
	m_MemIndex.Free();
	m_nFilesInPack = 0;
	m_nCompressMethod = 0;
}
//---------------------------------------------------------------------------
// ����:	Search
// ����:	�ڰ���Ѱ��һ���ļ�
// ����:	pFileName	Ҫ�ҵ��ļ���
//			pOffset		�ҵ��ļ���ƫ��
//			pLen		�ҵ��ļ��ĳ���
// ����:	TRUE		�ҵ���
//			FALSE		û�ҵ�
//---------------------------------------------------------------------------
BOOL KPakData::Search(LPSTR pFileName, PDWORD pOffset, PDWORD pLen)
{
	char	szPathName[MAXPATH];
	DWORD	dwFileId;
	int		nBegin;
	int		nEnd;
	int		nMid;

	// check file number
	if (m_nFilesInPack <= 0)
		return FALSE;

	// get file index table pointer
	TPakFileIndex*	pIndex;
	pIndex = (TPakFileIndex*)m_MemIndex.GetMemPtr();

	// get pack file name with path
	g_GetPackPath(szPathName, pFileName);

	// get file id 
	dwFileId = g_FileName2Id(szPathName);

	// �ö��ַ�����ƥ����ļ�
	nBegin = 0;
	nEnd = m_nFilesInPack - 1;
	while (nBegin <= nEnd)
	{
		nMid = (nBegin + nEnd) / 2;
		if (dwFileId == pIndex[nMid].FileId)
		{
			// �ҵ���
			*pOffset = pIndex[nMid].FileOffset;
			*pLen = pIndex[nMid].FileLength;
			return TRUE;
		}
		if (dwFileId < pIndex[nMid].FileId)
			nEnd = nMid - 1;
		else
			nBegin = nMid + 1;
	}

	// û�ҵ�
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Decode
// ����:	��ѹ��
// ����:	pCodeInfo	ѹ������ָ��
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KPakData::Decode(TCodeInfo* pCodeInfo)
{
	return m_pCodec->Decode(pCodeInfo);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KZipFile.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	File In Zip Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KFilePath.h"
#include "KZipList.h"
#include "KZipFile.h"
//---------------------------------------------------------------------------
// �ļ���ȡģʽ 0 = ���ȴӴ��̶�ȡ 1 = ���ȴ��ļ�����ȡ
static int m_nZipFileMode = 0;
//---------------------------------------------------------------------------
// ����:	SetFileMode
// ����:	�����ļ���ȡģʽ
// ����:	int
// ����:	void
//---------------------------------------------------------------------------
void g_SetZipFileMode(int nFileMode)
{
	m_nZipFileMode = nFileMode;
}
//---------------------------------------------------------------------------
// ����:	KZipFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KZipFile::KZipFile()
{
	m_nZipFile = -1;
}
//---------------------------------------------------------------------------
// ����:	~KZipFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KZipFile::~KZipFile()
{
	Close();
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	���ļ�, ��Ѱ�ҵ�ǰĿ¼���Ƿ���ͬ���ĵ����ļ�,
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KZipFile::Open(LPSTR FileName)
{
	BOOL bRet = FALSE;
	
	Close();

	if (m_nZipFileMode == 0)
	{
		bRet = m_DiskFile.Open(FileName);
	}
	else if (g_pZipList)
	{
		g_pZipList->Lock();
		bRet = OpenZip(FileName);
		g_pZipList->Unlock();
	}
	return bRet;
}
//---------------------------------------------------------------------------
// ����:	Read
// ����:	���ļ��ж�ȡ����
// ����:	pBuffer		������ָ��
//			dwSize		Ҫ��ȡ�ĳ���
// ����:	�������ֽڳ���
//---------------------------------------------------------------------------
DWORD KZipFile::Read(PVOID pBuffer, DWORD dwSize)
{
	if (m_nZipFile < 0)
	{
		return m_DiskFile.Read(pBuffer, dwSize);
	}
	else
	{
		g_pZipList->Lock();
		dwSize = ReadZip(pBuffer, dwSize);
		g_pZipList->Unlock();
	}
	return dwSize;
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ļ���ָ�붨λ
// ����:	pOffset			ƫ����
//			dwMethod		��λ����
// ����:	�ļ���ָ��
//---------------------------------------------------------------------------
DWORD KZipFile::Seek(LONG lOffset,DWORD dwMethod)
{
	if (m_nZipFile < 0)
	{
		return m_DiskFile.Seek(lOffset, dwMethod);
	}
	else
	{
		g_pZipList->Lock();
		lOffset = SeekZip(lOffset, dwMethod);
		g_pZipList->Unlock();
	}
	return lOffset;
}
//---------------------------------------------------------------------------
// ����:	Tell
// ����:	�����ļ���ָ��
// ����:	void
// ����:	�ļ���ָ��
//---------------------------------------------------------------------------
DWORD KZipFile::Tell()
{
	if (m_nZipFile < 0)
		return m_DiskFile.Tell();
	return m_FilePointer;
}
//---------------------------------------------------------------------------
// ����:	Size
// ����:	�����ļ���С
// ����:	void
// ����:	�ļ��Ĵ�С in bytes
//---------------------------------------------------------------------------
DWORD KZipFile::Size()
{
	if (m_nZipFile < 0)
		return m_DiskFile.Size();
	return m_FileSize;
}
//---------------------------------------------------------------------------
// ����:	Close
// ����:	�ر�һ���ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KZipFile::Close()
{
	m_FileBuff.Free();
	m_nZipFile = -1;
}
//---------------------------------------------------------------------------
// ����:	OpenZip
// ����:	���ļ�, ��Ѱ�ҵ�ǰĿ¼���Ƿ���ͬ���ĵ����ļ�,
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KZipFile::OpenZip(LPSTR FileName)
{
	KMemClass Buffer;// ѹ��Դ�ļ�����

	// search file in zip pack
	m_nZipFile = g_pZipList->Search(FileName, &m_FileOffs, &m_FileSize);
	if (m_nZipFile < 0)
		return FALSE;

	// read pack file
	g_pZipList->Seek(m_FileOffs, FILE_BEGIN);
	LF	Header;
	g_pZipList->Read(&Header, sizeof(Header));
	m_PackSize = Header.lf_csize;
	g_pZipList->Seek(Header.lf_fn_len, FILE_CURRENT);
	g_pZipList->Seek(Header.lf_ef_len, FILE_CURRENT);

	// alloc pack file buffer memory
	if (!Buffer.Alloc(m_PackSize))
		return FALSE;
	PVOID pPackBuf = Buffer.GetMemPtr();

	// alloc unpack file buffer memory
	if (!m_FileBuff.Alloc(m_FileSize))
		return FALSE;
	PVOID pFileBuf = m_FileBuff.GetMemPtr();

	// read pack file data
	g_pZipList->Read(pPackBuf, m_PackSize);
	
	// decode zip file
	g_pZipList->Decode((PBYTE)pPackBuf, (PBYTE)pFileBuf, &Header);

	// reset file pointer
	m_FilePointer = 0;

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	ReadZip
// ����:	���ļ��ж�ȡ����
// ����:	pBuffer		������ָ��
//			dwSize		Ҫ��ȡ�ĳ���
// ����:	�������ֽڳ���
//---------------------------------------------------------------------------
DWORD KZipFile::ReadZip(PVOID pBuffer, DWORD dwSize)
{
	if (dwSize + m_FilePointer > m_FileSize)
	{
		dwSize = m_FileSize - m_FilePointer;
	}

	if (dwSize == 0)
		return 0;

	PBYTE pFileBuf = (PBYTE)m_FileBuff.GetMemPtr();
	pFileBuf += m_FilePointer;
	g_MemCopy(pBuffer, pFileBuf, dwSize);
	m_FilePointer += dwSize;

	return dwSize;
}
//---------------------------------------------------------------------------
// ����:	SeekZip
// ����:	�ļ���ָ�붨λ
// ����:	pOffset			ƫ����
//			dwMethod		��λ����
// ����:	�ļ���ָ��
//---------------------------------------------------------------------------
DWORD KZipFile::SeekZip(LONG lOffset,DWORD dwMethod)
{
	switch (dwMethod)
	{
	case FILE_BEGIN:
		m_FilePointer = lOffset;
		break;
		
	case FILE_END:
		m_FilePointer = m_FileSize + lOffset - 1;
		break;
		
	case FILE_CURRENT:
		m_FilePointer = m_FilePointer + lOffset;
		break;
	}
	
	if (m_FilePointer < 0)
	{
		m_FilePointer = 0;
	}
	if (m_FilePointer >= m_FileSize)
	{
		m_FilePointer = m_FileSize - 1;
	}
	
	return m_FilePointer;
}
//---------------------------------------------------------------------------
// ����:	Save
// ����:	�����ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KZipFile::Save(LPSTR FileName)
{
	if (m_nZipFile < 0)
		return TRUE;
	if (!m_DiskFile.Create(FileName))
		return FALSE;
	m_DiskFile.Write(m_FileBuff.GetMemPtr(), m_FileSize);
	m_DiskFile.Close();
	return TRUE;
}
//---------------------------------------------------------------------------

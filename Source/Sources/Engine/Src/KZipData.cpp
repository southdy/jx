//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KZipData.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Zip File Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KFilePath.h"
#include "KZipData.h"
//---------------------------------------------------------------------------
// ����:	Open
// ����:	��һ��ZIP�ļ�
// ����:	lpZipFileName	Zip�ļ���
// ����:	TRUE			Success
//			FALSE			Fail
//---------------------------------------------------------------------------
BOOL KZipData::Open(LPSTR lpZipFileName)
{
	ZIPHeader		Head;
	ZIPEnd			End;
	ZIPCtrlHeader	FileInZip;
	ZIPFileInfo		FileInfo;
	char			FileName[MAX_PATH];
	DWORD			dwHashCode;
	int				i, n;

	// Close opened zip file
	Close();
	
	// Open zip file
	if (!m_ZipFile.Open(lpZipFileName))
		return FALSE;

	// Read the file header
	m_ZipFile.Read(&Head, sizeof(Head));
	if (Head.Signature != ZIPHeaderSig)
		return FALSE;

	// Read the final header
	m_ZipFile.Seek(-22, FILE_END);
	m_ZipFile.Read(&End, sizeof(End));
	if (End.Signature != ZIPEndSig)
		return FALSE;

	// Init zip file list
	if (!m_FileList.Init(sizeof(ZIPFileInfo), End.FilesOnDisk))
		return FALSE;
	
	// Read the header of each file
	m_ZipFile.Seek(End.Offset, FILE_BEGIN);
	for (n = 0; n < End.FilesOnDisk; n++)
	{
		m_ZipFile.Read(&FileInZip, sizeof(FileInZip));
		if (FileInZip.Signature != ZIPCtrlHeaderSig)
			return FALSE;
		FileInfo.UnpackSize = FileInZip.UnCompressedSize;
		FileInfo.OffsetInZip = FileInZip.Offset;
		// Read filename
		m_ZipFile.Read(FileName, FileInZip.FileNameLength);
		// Set filename tailer
		FileName[FileInZip.FileNameLength] = 0;
		// Convert all '/' to '\'
		for (i = 0; FileName[i]; i++)
		{
			if (FileName[i] == '/')
				FileName[i] = '\\';
		}
		g_StrLower(FileName);
		//g_StrCpy(FileInfo.FileName, FileName);
		dwHashCode = g_FileName2Id(FileName);
		m_FileList.Insert(dwHashCode, &FileInfo);
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Close
// ����:	�رմ򿪵�ZIP�ļ�
// ����:	void
// ����:	TRUE			Success
//			FALSE			Fail
//---------------------------------------------------------------------------
void KZipData::Close()
{
	m_FileList.Free();
	m_ZipFile.Close();
}
//---------------------------------------------------------------------------
// ����:	Read
// ����:	���ļ�����
// ����:	lpBuffer		��ȡ�����ݻ���
//			dwLength		��ȡ���ֽڳ���
// ����:	ʵ�ʶ�ȡ���ֽ���
//---------------------------------------------------------------------------
DWORD KZipData::Read(LPVOID lpBuffer, DWORD dwLength)
{
	return m_ZipFile.Read(lpBuffer, dwLength);
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ƶ��ļ�ָ��λ��
// ����:	Offset			ƫ����
//			Method			��׼λ��
// ����:	ʵ���ƶ�����λ��
//---------------------------------------------------------------------------
DWORD KZipData::Seek(LONG lOffset,DWORD dwMethod)
{
	return m_ZipFile.Seek(lOffset, dwMethod);
}
//---------------------------------------------------------------------------
// ����:	Tell
// ����:	ȡ���ļ�ָ��λ��
// ����:	void
// ����:	ʵ���ļ�ָ���λ��
//---------------------------------------------------------------------------
DWORD KZipData::Tell()
{
	return m_ZipFile.Tell();
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
BOOL KZipData::Search(LPSTR pFileName, PDWORD pOffset, PDWORD pLen)
{
	ZIPFileInfo FileInfo;
	DWORD dwHashCode;
	char szPathName[MAXPATH];

	g_GetPackPath(szPathName, pFileName);
	dwHashCode = g_FileName2Id(szPathName);
	if (!m_FileList.Search(dwHashCode, &FileInfo))
		return FALSE;
	*pOffset = FileInfo.OffsetInZip;
	*pLen = FileInfo.UnpackSize;
	return TRUE;
}
//---------------------------------------------------------------------------

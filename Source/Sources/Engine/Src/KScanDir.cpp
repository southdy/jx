//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KScanFile.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory Opration Class Using Heap Memory Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KFilePath.h"
#include "KScanDir.h"
//---------------------------------------------------------------------------
// ����:	
// ����:	
// ����:	lpFileExt
// ����:	void
//---------------------------------------------------------------------------
KScanFile::KScanFile()
{
	g_MemZero(m_FileExt, sizeof(m_FileExt));
}
//---------------------------------------------------------------------------
// ����:	
// ����:	
// ����:	lpFileExt
// ����:	void
//---------------------------------------------------------------------------
void KScanFile::DiscardFileExt(LPSTR lpFileExt)
{
	g_MemCopy(m_FileExt, lpFileExt, 3);
}
//---------------------------------------------------------------------------
// ����:	RunSearch
// ����:	ɨ��Ŀ¼
// ����:	lpRootPath		��Ŀ¼
//			lpScanPath		ɨ��Ŀ¼
//			lpOutFile		����ļ�
// ����:	TRUE			�ɹ�
//			FALSE			ʧ��
//---------------------------------------------------------------------------
BOOL KScanFile::RunSearch(LPSTR lpRootPath, LPSTR lpScanPath, LPSTR lpOutFile)
{
	m_pFile = fopen(lpOutFile, "wb");
	if (m_pFile == NULL)
		return FALSE;
	m_nPathLen = strlen(lpRootPath);
	if (lpRootPath[m_nPathLen - 1] != '\\')
		m_nPathLen++;
	fprintf(m_pFile, "%s\r\n", lpRootPath);
	SetCurrentDirectory(lpScanPath);
	SearchDirectory();
	fclose(m_pFile);
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	SearchDirectory
// ����:	ɨ��Ŀ¼�����Ŀ¼�������ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KScanFile::SearchDirectory() 
{
	WIN32_FIND_DATA FindData;
	HANDLE hFind = NULL;
	
	// set findfirstfile to find everthing
	hFind = FindFirstFile("*", &FindData);

	// if handle fails, drive is empty...
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	
	// get first entity on drive - check if it's a directory
	if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		// if so, change to that directory and recursively call SearchDirectory
		if (FindData.cFileName[0] != '.')
		{
			// enter the directory
			SetCurrentDirectory(FindData.cFileName);
			// search the directory
			SearchDirectory();
			// go back up one directory level
			SetCurrentDirectory("..");
		}
	}
	else
	{
		if (CheckFileExt(FindData.cFileName))
			OutputFileName(FindData.cFileName);
	}

	while (FindNextFile(hFind, &FindData))
	{
		
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindData.cFileName[0] != '.')
			{
				SetCurrentDirectory(FindData.cFileName);
				SearchDirectory();
				SetCurrentDirectory("..");
			}
		}
		else
		{
			if (CheckFileExt(FindData.cFileName))
				OutputFileName(FindData.cFileName);
		}
	}

	FindClose(hFind);
}
//---------------------------------------------------------------------------
// ����:	CheckFileExt
// ����:	����ļ���չ��
// ����:	FileName	
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KScanFile::CheckFileExt(LPSTR FileName)
{
	if (memcmp(m_FileExt, &FileName[strlen(FileName) - 3], 3) == 0)
		return FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	OutputFileName
// ����:	����ļ���
// ����:	FileName	
// ����:	void
//---------------------------------------------------------------------------
void KScanFile::OutputFileName(LPSTR FileName)
{
	char szPathName[MAXPATH];

	GetCurrentDirectory(MAXPATH, szPathName);
	strcat(szPathName, "\\");
	strcat(szPathName, FileName);
	strcat(szPathName, "\r\n");
	fprintf(m_pFile, "%s", &szPathName[m_nPathLen]);
}
//---------------------------------------------------------------------------

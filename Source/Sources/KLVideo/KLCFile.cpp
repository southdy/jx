//////////////////////////////////////////////////////////////////////////////////////
//	FileName		:	CFile.cpp
//	FileAuthor		:	zroc
//	FileCreateDate	:
//	FileDescription	:	File operation class
//
//////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>
#include "KLCFile.h"
//---------------------------------------------------------------------------
//  Function:   Constructor
//	Comment:	���캯��
//---------------------------------------------------------------------------
KLCFile::KLCFile()
{
	hFile = INVALID_HANDLE_VALUE;
	nSize = 0;
	dwPos = 0;
}
//---------------------------------------------------------------------------
//  Function:   Destructor
//	Comment:	��������
//---------------------------------------------------------------------------
KLCFile::~KLCFile()
{
	Close();
}
//---------------------------------------------------------------------------
//  Function:   Open exist file
//  Commemt:	��һ���ļ���׼��������
//---------------------------------------------------------------------------
BOOL KLCFile::Open(LPCSTR FileName)
{
	char PathName[MAX_PATH];

	strcpy(PathName,FileName);
	// close prior file handle
	if (hFile != INVALID_HANDLE_VALUE)
		Close();
	// Open the file for read
	hFile = CreateFile(
					PathName,		// pointer to name of the file with path
					GENERIC_READ,	// access (read-write) mode
					FILE_SHARE_READ,// share mode
					NULL,			// pointer to security attributes
					OPEN_EXISTING,	// how to create
					FILE_ATTRIBUTE_NORMAL,// file attributes
					NULL);			// template file
	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	// get the open file size
	nSize = GetFileSize(hFile, NULL);
	// return true
	return TRUE;
}
//---------------------------------------------------------------------------
//  Function:   Create new file
//  Comment:	����һ�����ļ���׼��д����
//---------------------------------------------------------------------------
BOOL KLCFile::Create(LPCSTR FileName)
{
	char PathName[MAX_PATH];

	strcpy(PathName,FileName);
	// close prior file handle
	if (hFile != INVALID_HANDLE_VALUE)
		Close();
	// create file for write
	hFile = CreateFile(
					PathName,		// pointer to name of the file with path
					GENERIC_WRITE,	// access (read-write) mode
					FILE_SHARE_READ,// share mode
					NULL,			// pointer to security attributes
					CREATE_ALWAYS,	// create or over write
					FILE_ATTRIBUTE_NORMAL, // file attributes
					NULL);			// template file
	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	// return true
	return TRUE;
}
/*******************************************************
// FUNCTION	: CzFileOperate::Eof
// PURPOSE	: 
// RETURN	: BOOL  TRUE End of File
// COMMENTS	:
*******************************************************/
BOOL KLCFile::Eof()
{
	if (dwPos >= nSize)
		return TRUE;
	return FALSE;
}
//---------------------------------------------------------------------------
//  Function:   Close file
//  Comment:	�ر��Ѿ��򿪵��ļ�
//---------------------------------------------------------------------------
BOOL KLCFile::Close()
{
	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	// close file handle
	CloseHandle(hFile);
	// reset file param
	hFile = INVALID_HANDLE_VALUE;
	nSize = 0;
	dwPos = 0;
	// return ture
	return TRUE;
}
//---------------------------------------------------------------------------
//  Function:   Read data from file
//  Return:		success : number of bytes read from file
//              fail    : 0
//  Comment:	��ȡ�ļ�����
//---------------------------------------------------------------------------
DWORD KLCFile::Read(LPVOID lpBuffer,DWORD nReadBytes)
{
	DWORD BytesRead;

	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;
	// read data form file
	ReadFile(hFile, lpBuffer, nReadBytes, &BytesRead, NULL);
	// move file pointer
	dwPos += BytesRead;
	// return bytes readed
	return BytesRead;
}
//---------------------------------------------------------------------------
//  Function:   Write data to file
//  Return:		success : number of bytes write to file
//              fail    : 0
//  Comment:	д���ļ�����
//---------------------------------------------------------------------------
DWORD KLCFile::Write(LPVOID lpBuffer,DWORD nWriteBytes)
{
	DWORD BytesWrite;

	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;
	// write data to file
	WriteFile(hFile, lpBuffer, nWriteBytes, &BytesWrite, NULL);
	// move file pointer
	dwPos += BytesWrite;
	// return bytes writed
	return BytesWrite;
}
//---------------------------------------------------------------------------
//  Function:   Move file pointer
//  Params:		Distance = file offset
//              MoveMethod = FILE_BEGIN
//                           FILE_CURRENT
//                           FILE_END
//	Return:		success : pointer of file
//				fail    : 0xffffffff
//  Comment:	�ƶ��ļ�ָ��
//---------------------------------------------------------------------------
DWORD KLCFile::Seek(LONG Distance, DWORD MoveMethod)
{
	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return 0xffffffff;
	// move file pointer
	dwPos = SetFilePointer(hFile, Distance, NULL, MoveMethod);
	// return file pointer position
	return dwPos;
}
//---------------------------------------------------------------------------
//  Function:   Get file pointer pos
//	Return:		success : low dword of file size
//				fail    : 0xffffffff
//  Comment:	ȡ���ļ�ָ��λ��
//---------------------------------------------------------------------------
DWORD KLCFile::Tell()
{
	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return 0xffffffff;
	// return file size
	return dwPos;
}
//---------------------------------------------------------------------------
//  Function:   Get file size (number of bytes)
//	Return:		success : low dword of file size
//				fail    : 0xffffffff
//  Comment:	ȡ���ļ��Ĵ�С
//---------------------------------------------------------------------------
DWORD KLCFile::Size()
{
	// check file handle
	if (hFile == INVALID_HANDLE_VALUE)
		return 0xffffffff;
	// return file size
	return nSize;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KZipFile.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KZipFile_H
#define KZipFile_H
//---------------------------------------------------------------------------
#include "KFile.h"
#include "KMemClass.h"
//---------------------------------------------------------------------------
class ENGINE_API KZipFile
{
private:
	KFile		m_DiskFile;		// �����ļ�(���ڰ���)
	KMemClass	m_FileBuff;		// ��ѹ���ļ�����
	int			m_nZipFile;		// �Ƿ�����ڰ���
	DWORD		m_FileOffs;		// ��ZIP�ļ��еď���
	DWORD		m_FileSize;		// ûѹ��ǰ���ļ�����
	DWORD		m_PackSize;		// ѹ���ļ�����
	DWORD		m_FilePointer;	// �ļ�ָ���λ��(�����)
	BOOL		OpenZip(LPSTR FileName);
	DWORD		ReadZip(PVOID pBuffer, DWORD dwSize);
	DWORD		SeekZip(LONG lOffset, DWORD dwMethod);

public:
	KZipFile();
	~KZipFile();
	BOOL		Open(LPSTR FileName);
	void		Close();
	DWORD		Read(PVOID pBuffer, DWORD dwSize);
	DWORD		Seek(LONG lOffset, DWORD dwMethod);
	DWORD		Tell();
	DWORD		Size();
	BOOL		Save(LPSTR FileName);
};
//---------------------------------------------------------------------------
ENGINE_API void g_SetZipFileMode(int nFileMode);
//---------------------------------------------------------------------------
#endif

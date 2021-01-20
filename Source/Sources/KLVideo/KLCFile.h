//////////////////////////////////////////////////////////////////////////////////////
//	FileName		:	CFile.h
//	FileAuthor		:	zroc
//	FileCreateDate	:
//	FileDescription	:	
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __KLCFILE_H__
#define __KLCFILE_H__
//---------------------------------------------------------------------------
class KLCFile {
private:
	HANDLE		hFile;		// �ļ����
	DWORD		nSize;		// �ļ�����
	DWORD		dwPos;		// �ļ�ָ��
public:
	KLCFile();
	~KLCFile();
	BOOL		Open(LPCSTR FileName);
	BOOL		Create(LPCSTR FileName);
	BOOL		Close();
	DWORD		Read(LPVOID lpBuffer,DWORD nReadBytes);
	DWORD		Write(LPVOID lpBuffer,DWORD nWriteBytes);
	BOOL		Eof();
	DWORD		Seek(LONG Distance,DWORD MoveMethod);
	DWORD		Tell();
	DWORD		Size();
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakData.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KPakData_H
#define KPakData_H
//---------------------------------------------------------------------------
#include "KMemClass.h"
#include "KFile.h"
#include "KCodec.h"
//---------------------------------------------------------------------------
// pack file header
typedef struct {
	BYTE		Signature[8];		// �ļ���ʶ ("PACKAGE")
	DWORD		FilesInPack;		// ���е��ļ�����
	DWORD		CompressMethod;		// ʹ�õ�ѹ���㷨
} TPakFileHeader;

// pack file index 
typedef struct {
	DWORD		FileId;				// 32bit Idֵ
	DWORD		FileOffset;			// �ļ��ڰ��е�ƫ��
	DWORD		FileLength;			// �ļ���ԭʼ����
} TPakFileIndex;
//---------------------------------------------------------------------------
class ENGINE_API KPakData
{
private:
	KFile		m_File;				// PAK�ļ�
	KMemClass	m_MemIndex;			// �����ڴ�
	KCodec*		m_pCodec;			// ������
	DWORD		m_nFilesInPack;		// ���е��ļ�����
	DWORD		m_nCompressMethod;	// ѹ���㷨

public:
	KPakData();
	~KPakData();
	BOOL		Open(LPSTR FileName);
	DWORD		Read(LPVOID lpBuffer, DWORD dwLength);
	DWORD		Seek(LONG lOffset, DWORD dwMethod);
	DWORD		Tell();
	void		Close();
	BOOL		Search(LPSTR pFileName, PDWORD pOffset, PDWORD pLen);
	BOOL		Decode(TCodeInfo* pCodeInfo);
};
//---------------------------------------------------------------------------
#endif

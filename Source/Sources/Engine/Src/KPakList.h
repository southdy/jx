//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakList.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
// Modify:	Wooy(2003-9-17)
//---------------------------------------------------------------------------
#ifndef KPakList_H
#define KPakList_H

#include "XPackFile.h"
//---------------------------------------------------------------------------


class ENGINE_API KPakList
{
public:
	KPakList();
	~KPakList();
	bool		Open(const char* pPakListFile);
	void		Close();
	//���Ұ��ڵ����ļ�
	bool		FindElemFile(unsigned long uId, XPackElemFileRef& ElemRef);
	//���Ұ��ڵ����ļ�
	bool		FindElemFile(const char* pszFileName, XPackElemFileRef& ElemRef);
	//��ȡ���ڵ����ļ�
	int			ElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize);
	//��ȡspr�ļ�ͷ��������spr
	SPRHEAD*	GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable);
	//��ȡ��֡ѹ����spr��һ֡������
	SPRFRAME*	GetSprFrame(int nPackIndex, SPRHEAD* pSprHeader, int nFrame);
private:
	unsigned long FileNameToId(const char* pszFileName);
private:
	#define MAX_PAK		32
	XPackFile*			m_PakFilePtrList[MAX_PAK];
	long				m_nPakNumber;
};

extern ENGINE_API KPakList* g_pPakList;

#endif

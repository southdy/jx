//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcResList.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------

#pragma once

#ifndef _SERVER

#include "KList.h"
#include "KNpcResNode.h"
#include "KPlayerMenuState.h"

class KNpcResList : public KList
{
public:
	CActionName				m_cActionName;
	CActionName				m_cNpcAction;
	CStateMagicTable		m_cStateTable;
	KPlayerMenuStateGraph	m_cMenuState;

private:

	// ����ͬ���� NpcResNode �����ؽڵ�
	KNpcResNode*		FindNpcRes(char *lpszNpcName);

public:
    KNpcResList();
    ~KNpcResList();
	
    // ��ʼ�� ActionName
	BOOL				Init();
	// ���һ�� NpcResNode �����ؽڵ㣬����Ѵ��ڣ�ֱ�ӷ��ؽڵ�
	KNpcResNode*		AddNpcRes(char *lpszNpcName);
};
#ifndef _SERVER
extern KNpcResList	g_NpcResList;
#endif

#endif
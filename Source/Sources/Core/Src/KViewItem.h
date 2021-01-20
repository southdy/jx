//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KViewItem.h
// Date:	2003.07.28
// Code:	�߳�����
// Desc:	KViewItem Class
//---------------------------------------------------------------------------

#ifndef _KVIEWITEM_H
#define _KVIEWITEM_H

#ifndef _SERVER

class KViewItem
{
private:
	DWORD		m_dwNpcID;				// �Է���ҵ� npc �� id
	int			m_nLevel;				// �Է���ҵĵȼ�
	char		m_szName[32];			// �Է���ҵ�����

public:
	PlayerItem	m_sItem[itempart_num];	// �Է���Ҵ������ϵ�װ���ڿͻ��� Item �����е�λ����Ϣ

public:
	KViewItem();						// ����
	void		Init();					// ��ʼ��
	void		ApplyViewEquip(DWORD dwNpcID);
	void		DeleteAll();
	void		GetData(BYTE* pMsg);
};

extern	KViewItem	g_cViewItem;

#endif


#endif

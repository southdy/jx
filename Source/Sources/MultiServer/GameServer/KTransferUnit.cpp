// KTransferUnit.cpp: implementation of the KTransferUnit class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"	// ����Ԥ����ͷ�ļ�������������#ifndef֮��Ķ���
#include "KTransferUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KTransferUnit::KTransferUnit(DWORD dwIp, DWORD dwRelayID)
{
	m_dwIp = dwIp;
	m_dwRelayID = dwRelayID;
}

KTransferUnit::~KTransferUnit()
{
}

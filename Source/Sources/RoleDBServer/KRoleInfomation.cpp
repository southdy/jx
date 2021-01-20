// KRoleInfomation.cpp: implementation of the KRoleInfomation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoleDBServer.h"
#include "KRoleInfomation.h"
#include <objbase.h>
#include <initguid.h>
#include <list>
#include <map>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace std;

//���ս�ɫ�����˳����������б������Զ������˳��
list<KRoleInfomation * > g_RoleInfoList;

//�Խ�ɫ������Ϊ�ؼ����������ҵ��б������ڿͻ��˶�ȡ�����ټ���
map<std::string , KRoleInfomation * > g_RoleInfoMap;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KRoleInfomation::KRoleInfomation()
{
	Clear();
}

KRoleInfomation::~KRoleInfomation()
{
	
}

void KRoleInfomation::Clear()
{
	m_RoleInfoBuffer[0] = 0;
	m_nRoleInfoSize = 0;
	m_nSaveCmdCount = m_nLoadCmdCount = m_nSaveDBCount = m_bModify = 0;
	m_nLastSaveTime = 0;	
	m_nUnModifyTime = 0;
}

BYTE *	KRoleInfomation::GetRoleInfoBuffer(size_t & nRoleInfoSize)
{
	nRoleInfoSize = 0;
	if (m_RoleInfoBuffer[0] == 0) return NULL;
	nRoleInfoSize = m_nRoleInfoSize;
	return m_RoleInfoBuffer ;
}

BOOL	KRoleInfomation::SetRoleInfoBuffer(BYTE * pRoleInfoBuffer, size_t nRoleInfoSize)
{
	if (pRoleInfoBuffer == NULL || nRoleInfoSize == 0 ) return FALSE;
	memcpy(&m_RoleInfoBuffer, pRoleInfoBuffer, nRoleInfoSize);
	m_nRoleInfoSize = nRoleInfoSize;
	return TRUE;
}

BOOL	KRoleInfomation::CheckRoleInfoValid(const char * szRoleName)	//��֤Buffer�����Ƿ�ΪRoleName������
{
	return TRUE;
}

BOOL	KRoleInfomation::CheckRoleInfoValid()	//��֤Buffer�����Ƿ�ΪRoleName������
{
	if (m_nRoleInfoSize <= 0) return FALSE;
	return TRUE;
}

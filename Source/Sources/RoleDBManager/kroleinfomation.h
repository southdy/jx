// KRoleInfomation.h: interface for the KRoleInfomation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KROLEINFOMATION_H__AECE97BE_EEC0_48CE_AFA1_C55FCFE87C3E__INCLUDED_)
#define AFX_KROLEINFOMATION_H__AECE97BE_EEC0_48CE_AFA1_C55FCFE87C3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_ROLEINFORMATION_SIZE 50000
#define MAXSIZE_ROLENAME 100
#include "windows.h"
#pragma warning(disable:4786)
#include "s3dbinterface.h"
#include "KroleDBHeader.h"
#include "CriticalSection.h"
#include "..\MultiServer\Common\tstring.h"

#include <list>
#include <map>
using namespace std;

struct TGetRoleInfoFromDB
{
	unsigned long	nNetId;
	char	szRoleName[MAXSIZE_ROLENAME];
	unsigned long	ulIdentity;
	unsigned char	nSaveCmd;
};

class KRoleInfomation  
{
public:
	KRoleInfomation();
	virtual ~KRoleInfomation();
	void	Clear();			//�����������
	BYTE *	GetRoleInfoBuffer(size_t & nRoleInfoSize);
	TCmd *	CreateRoleInfoTCmdData(size_t &nRoleInfoSize);
	BOOL	CopyRoleInfoBuffer(BYTE * pBuffer ,size_t & nRoleInfoSize);
	BOOL	GetBaseRoleInfo(TRoleBaseInfo * pRoleBaseInfo);
	BOOL	SetRoleInfoBuffer(BYTE * pRoleInfoBuffer, size_t nRoleInfoSize);
	BOOL	GetRoleName(char * pName, size_t& nLen);
	BOOL	CheckRoleInfoValid(const char * szRoleName);	//��֤Buffer�����Ƿ�ΪRoleName������
	BOOL	CheckRoleInfoValid();	
	BOOL	IsValid();
	size_t  GetBufferSize();
	
	int		m_nSaveCmdCount;	//�ӿͻ��˻�õ�֪ͨ�浵�Ĵ���
	int		m_nSaveDBCount;		//ʵ�������ݿ�浵�Ĵ���
	int		m_nLoadCmdCount;	//�ӿͻ��˻�õ�֪ͨȡ���Ĵ���
	DWORD	m_nLastSaveTime;	
	int		m_nUnModifyTime;
	BOOL	m_bModify;			//�Ƿ�����ˣ�Ҳ�����Ƿ���Ҫ�����ݿ��ڴ浵
private:
	BYTE	m_RoleInfoBuffer[MAX_ROLEINFORMATION_SIZE];
	size_t	m_nRoleInfoSize;	//ʵ�����ݵĳ���
	OnlineGameLib::Win32::CCriticalSection m_cs;
};


//���ս�ɫ�����˳����������б������Զ������˳��
extern list<KRoleInfomation*> g_RoleInfoList;

//�Խ�ɫ������Ϊ�ؼ����������ҵ��б������ڿͻ��˶�ȡ�����ټ���
extern map<OnlineGameLib::Win32::_tstring , KRoleInfomation * > g_RoleInfoMap;
//��ȡ�����뻥��!
#endif // !defined(AFX_KROLEINFOMATION_H__AECE97BE_EEC0_48CE_AFA1_C55FCFE87C3E__INCLUDED_)

#pragma once

#include "..\MultiServer\Common\Buffer.h"
#include "..\MultiServer\Common\Macro.h"

#pragma warning(disable:4786)
#include <map>
#include <vector>
#include "KRoleInfomation.h"
#include "..\MultiServer\Heaven\Interface\IServer.h"
#include "..\MultiServer\Heaven\Interface\HeavenInterface.h"
#include "KRoleDBHeader.h"

namespace RoleDBServer
{
	using namespace std;
	
	class	KClientUser
	{
	public:
		KClientUser();
		~KClientUser();
		char		 m_szClientName[100];
		OnlineGameLib::Win32::CPackager	m_RecvPackager;
		OnlineGameLib::Win32::CPackager	m_SendPackager;
		unsigned char m_CurrentProtocol;
		
//		KRoleBlockProcess m_SendProcess;
		unsigned long m_nCurSendNo;
		//unsigned long m_nCurSendLen;
		unsigned long m_nProcessLoadCount;
		unsigned long m_nProcessWriteCount;
		size_t		  m_ulSendBufferCount;
		size_t		  m_ulRecvBufferCount;
		unsigned long m_ID;		
		//KClientUserDataMap  m_DataMap;
	//		KRoleStreamArtery m_StreamArtery;
		//		KProcessDataSet m_DBProcessSet;		//�����ݿ�ӿڴ�������ݼ�
		//KProcessDataSet m_UserProcessSet;	//�����������������ݼ�
	};
	typedef map<unsigned long , KClientUser *> KClientUserSet;
	
	class KNetServer
	{
	public:
		int Init();
		int	Receive();
		int Send();
		int	Receive(unsigned long nId);//
		int	Send(unsigned long nId);//Send
		int	AppendData(unsigned long nId, const void * pMsgBuffer, size_t nMsgLen);
		int Service(KClientUser * pClientUser, TProcessData * pData, int nId);
		void ClearProcessData(unsigned long nId);
		void RegisterClient(unsigned long nId);
		void DestoryClient(unsigned long nId);
		int  ClearClientSend(unsigned long nId);
		IServer * m_pNetServer;
		KClientUserSet	m_ClientUserSet;	//�������ӵ���Ϸ����������
	};
	extern KNetServer g_NetServer;
	
	
	
//	typedef vector<TProcessData *> KProcessDataSet;
}


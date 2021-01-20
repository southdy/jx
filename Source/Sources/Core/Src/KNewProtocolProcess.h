//////////////////////////////////////////////////////////////////////////////////////
//	�ļ���			:	KNewProtocolProcess.h
//	������			:	������
//	����ʱ��		:	2003-7-14 13:17:29
//	�ļ�˵��		:	��Ե���Account����Э��Ĵ���
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __KNEWPROTOCOLPROCESS_H__
#define __KNEWPROTOCOLPROCESS_H__

#ifndef _STANDALONE
#include <map>
#include <list>

#include "../../MultiServer/Common/Library.h"
#include "../../MultiServer/Common/Buffer.h"
#else
#include "zport.h"
#endif

#ifdef _STANDALONE
class IClient;
#else
struct IClient;
#endif
class KNewProtocolProcess
{
protected:
	typedef void (*FUN_PROCESS_NEW_PROTOCOL)(IClient* pTransfer,
										  DWORD dwFromIP, DWORD dwFromRelayID,
										  int nPlayerIndex, BYTE *pData, int nDataSize);
	typedef std::map<WORD, FUN_PROCESS_NEW_PROTOCOL> NEW_PROTOCOL_HASH;

	typedef std::list<OnlineGameLib::Win32::CBuffer *> DATAList;
	DATAList m_theTransList;
	DATAList m_theChatList;
	DATAList m_theTongList;

	OnlineGameLib::Win32::CBuffer::Allocator	m_theAllocator;

public:
	KNewProtocolProcess();
	~KNewProtocolProcess();
//protected:
	NEW_PROTOCOL_HASH m_ProtocolTable;

	static void P_ProcessGMExecute(IClient* pTransfer,
						   DWORD dwFromIP, DWORD dwFromRelayID,
						   int nPlayerIndex, BYTE* pData, int nDataSize);
	static void P_ProcessGMFindPlayer(IClient* pTransfer,
						   DWORD dwFromIP, DWORD dwFromRelayID,
						   int nPlayerIndex, BYTE* pData, int nDataSize);

	static void P_ProcessPlayerCommExtend(IClient* pTransfer,
						   DWORD dwFromIP, DWORD dwFromRelayID,
						   int nPlayerIndex, BYTE* pData, int nDataSize);

	static void P_ProcessChatExtend(IClient* pTransfer,
						   DWORD dwFromIP, DWORD dwFromRelayID,
						   int nPlayerIndex, BYTE* pData, int nDataSize);

public:
	void ProcessNetMsg(IClient* pTransfer, DWORD dwFromIP, DWORD dwFromRelayID,
					   int nPlayerIndex, BYTE* pData, int nDataSize);

	void SendNetMsgToTransfer(IClient* pClient);
	void SendNetMsgToChat(IClient* pClient);
	void SendNetMsgToTong(IClient* pClient);

	void PushMsgInTransfer(const void * const pData, const size_t datalength);
	void PushMsgInChat(const void * const pData, const size_t datalength);
	void PushMsgInTong(const void * const pData, const size_t datalength);
	//�����з�������ÿ���˷���ĳ��Э��,���Ϸ���Relay��
	void BroadcastGlobal(const void * const pData, const size_t datalength);
	//������������ÿ���˷���ĳ��Э��,����ֱ�ӷ����ͻ���
	void BroadcastLocalServer(const void * const pData, const size_t datalength);
	//�ڱ��������ķ�Χִ��ĳ��Э��
	void ExecuteLocalServer(const void * const pData, const size_t datalength);
};

extern KNewProtocolProcess g_NewProtocolProcess;
#endif //__KNEWPROTOCOLPROCESS_H__

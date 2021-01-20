#ifndef ICLIENT_H
#define ICLIENT_H

//Ӧ��ʹ���̵߳ķ���,Startup�����̵߳ȴ��ź�,��ͬ��IOģ��ʹ�ò�ͬ���ź�֪ͨ��Щ�߳�,�����ʱ��
//���ݴ�������Ŀ�Ĳ�ͬ������ͬ��Ŀ���̺߳ͻ�����,�����쳣���
#include "zport.h"
#include "xbuffer.h"
#include "Cipher.h"

enum enumServerConnectInfo {
	enumServerConnectCreate = 0x100,
	enumServerConnectClose
};

typedef void (*CALLBACK_CLIENT_EVENT )(void *lpParam, const unsigned long &ulnEventType);

class IClient;
class IClientThread : public ZThread {
	IClient *parent;
public:
	IClientThread(IClient *the_parent) : ZThread() {
		parent = the_parent;
	}
	int action();
};

extern ZPerf g_sendPerf;
extern ZPerf g_recvPerf;

class IClient {
protected:
	IClientThread *thread;
public:
	ZPerf sendPerf;
	ZPerf recvPerf;
	bool bConnected;
	bool bStop;
	unsigned int m_uServerKey;
	unsigned int m_uClientKey;
//	int read_index;
//	int write_index;
	CALLBACK_CLIENT_EVENT call_back;
	void *call_back_param;
	int nSocket;
	ZBuffer *buffer;
	IClient(int max_send = 640 * 1024, int max_receive = 640 * 1024);
	~IClient();
	int Startup();						//����һ���̷߳���
	int Cleanup();
	int Shutdown();
	void Release() {
	}
	bool ConnectTo(const char * const &pAddressToConnectServer, unsigned short usPortToConnectServer);
	void RegisterMsgFilter(void * lpParam, CALLBACK_CLIENT_EVENT pfnEventNotify);

	bool SendPackToServer( const void * const pData, const unsigned long datalength);
	void *GetPackFromServer(unsigned int &datalength );
	void startPerf() {
		buffer->startPerf(&g_recvPerf, &g_sendPerf);
	}
	void stopPerf() {
		buffer->stopPerf();
	}
};

#endif

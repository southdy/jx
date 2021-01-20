#ifndef ZCLIENT_H
#define ZCLIENT_H

//Ӧ��ʹ���̵߳ķ���,Startup�����̵߳ȴ��ź�,��ͬ��IOģ��ʹ�ò�ͬ���ź�֪ͨ��Щ�߳�,�����ʱ��
//���ݴ�������Ŀ�Ĳ�ͬ������ͬ��Ŀ���̺߳ͻ�����,�����쳣���
#include "zport.h"
#include "zbuffer.h"
#include "Cipher.h"

#include "..\MultiServer\Rainbow\Interface\IClient.h"

/*enum enumServerConnectInfo {
	enumServerConnectCreate = 0x100,
	enumServerConnectClose
};*/

typedef void (CALLBACK *CALLBACK_CLIENT_EVENT )(void *lpParam, const unsigned long &ulnEventType);

class ZClient;
class ZClientThread : public ZThread {
	ZClient *parent;
public:
	ZClientThread(ZClient *the_parent) : ZThread() {
		parent = the_parent;
	}
	int action();
};

class ZClient {
protected:
	ZClientThread *thread;
public:
	bool bConnected;
	bool bStop;
	unsigned int m_uServerKey;
	unsigned int m_uClientKey;
	int read_index;
	int write_index;
	CALLBACK_CLIENT_EVENT call_back;
	void *call_back_param;
	int nSocket;
	ZBuffer *buffer;
	ZClient(ZBuffer *the_buffer);
	~ZClient();
	void Startup();						//����һ���̷߳���
	void Cleanup();
	void Shutdown();

	bool ConnectTo(const char * const &pAddressToConnectServer, unsigned short usPortToConnectServer);
	void RegisterMsgFilter(void * lpParam, CALLBACK_CLIENT_EVENT pfnEventNotify);

	bool SendPackToServer( const void * const pData, const unsigned long datalength);
	void *GetPackFromServer(unsigned int &datalength );
	void startPerf() {
		buffer->startPerf();
	}
	void stopPerf() {
		buffer->stopPerf();
	}
};

#endif
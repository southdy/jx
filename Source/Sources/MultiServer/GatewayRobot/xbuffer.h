#ifndef XBUFFER_H
#define XBUFFER_H
#include <stdio.h>
#include <string.h>
#include "zport.h"

#define MINIMIZE_BLOCK 32						//��С���СΪ32�ֽ�

class ZBuffer {
	
	ZMutex mutex;

	char *send_buffer;							//ʵ��ʹ�õķ��ͻ�����
	char *receive_buffer;						//ʵ��ʹ�õĽ��ջ�����
	long max_send;								//���ͻ������Ĵ�С
	long max_receive;							//���ջ������Ĵ�С

//������һЩͳ������
	bool bPerf;
//	ZPerf send_perf;							//��������ͳ��
//	ZPerf receive_perf;							//��������ͳ��

	char *receive_ptr;							//��ǰ�������ݵ�ָ��
	long receive_size;							//�������ݵĴ�С

	char *send_ptr;								//��ǰ�������ݵ�ָ��
	long send_size;								//�������ݵĴ�С
	char *packet_ptr;							//��ǰ���ݰ��Ŀ�ʼָ��
	unsigned short packet_size;					//��ǰ���ݰ��Ĵ�С

	char *recv_buffer;							//�ȴ��������ȡ����
	int recv_buffer_size;

	char *getRecvBuf(int size)
	{
#define MIN_RECV_BUF_SIZE	1080

		if (recv_buffer_size >= size)
			return recv_buffer;

		//realloc
		if (recv_buffer)
			delete recv_buffer;
		recv_buffer_size = MIN_RECV_BUF_SIZE > size ? MIN_RECV_BUF_SIZE : size;
		recv_buffer = new char[recv_buffer_size];
		return recv_buffer;
	}

public:
	ZBuffer(long the_max_send, long the_max_receive) {
		send_buffer = new char[the_max_send];
		if(send_buffer) max_send = the_max_send;
		else max_send = 0;
		receive_buffer = new char[the_max_receive];
		if(receive_buffer) max_receive = the_max_receive;
		else max_receive = 0;
		recv_buffer = 0;
		recv_buffer_size = 0;
		clear();
	}
	virtual ~ZBuffer() {
		if(send_buffer) delete[] send_buffer;
		if(receive_buffer) delete[] receive_buffer;
		if(recv_buffer) delete[] recv_buffer;
	}

//�����Ƕ����ṩ�Ľӿ�
	void stopPerf() {
		bPerf = false;
//		send_perf.stop();
//		receive_perf.stop();
	}

	void startPerf() {
		bPerf = true;
//		send_perf.start();
//		receive_perf.start();
	}

//��ȡ���õĽ������ݵĻ�����
	char *getReceiveBuffer(int &size) {
		mutex.lock();
		if(receive_size + MINIMIZE_BLOCK > max_receive) {
//������󻺳������ر�����
			mutex.unlock();
			return NULL;
		}
		if(receive_ptr + receive_size + MINIMIZE_BLOCK > receive_buffer + max_receive) {		//β�����ݲ�����
			memmove(receive_buffer, receive_ptr, receive_size);
			receive_ptr = receive_buffer;
		}
		size = max_receive - (receive_ptr - receive_buffer) - receive_size;
		mutex.unlock();
		return receive_ptr + receive_size;
	}
	void receiveData(int size) {
		mutex.lock();
		receive_size += size;
		//printf("receive data %d(%d) \n", receive_size, *(short *)(receive_ptr + receive_size - size));
		mutex.unlock();
	}
	const char *getPacket(int &size) {
		if(receive_size < sizeof(unsigned short)) return NULL;
		mutex.lock();
		size = *(unsigned short *)receive_ptr;
		if(receive_size >= size) {
			const char *result = (const char *)receive_ptr + sizeof(unsigned short);
			receive_ptr += size;
			receive_size -= size;
//			printf("receive data %d\n", receive_size);
			size -= sizeof(unsigned short);
			mutex.unlock();
			char* recvbuf = getRecvBuf(size);
			memcpy(recvbuf, result, size);
			return recvbuf;
		}
		mutex.unlock();
		return NULL;
	}

	void clear() {
		mutex.lock();
		receive_ptr = receive_buffer;
		receive_size = 0;

		packet_ptr = send_ptr = send_buffer;
		*(unsigned short *)send_ptr = 0;
		send_size = 0;
		packet_size = 0;
		mutex.unlock();
	}

//�������ݵĽӿڷֳ�����������һ���ǻ�ȡ��ǰ���͵Ļ������ͳ��ȣ�����һ���Ǵӻ��������ָ����С������
	char *getSendData(int &size) {
		mutex.lock();
		size = packet_ptr - send_ptr;
		if(size) {
			mutex.unlock();
			return send_ptr;
		}
		else {
			mutex.unlock();
			return NULL;
		}
	}
	void sendData(int size) {
		mutex.lock();
		send_ptr += size;
		send_size -= size;
		if(!send_size) {
			send_ptr = packet_ptr = send_buffer;
			*(unsigned short *)send_ptr = 0;
		}
		mutex.unlock();
	}

	bool packData(const char *data, int size) {
		mutex.lock();
		if(send_size + size + MINIMIZE_BLOCK >= max_send) {						
//�����������ر�����
			mutex.unlock();
			return false;
		}
		if(!packet_size) {
			*(unsigned short *)packet_ptr = 0;
			packet_size += sizeof(unsigned short);
			send_size += sizeof(unsigned short);
		}

		if(packet_ptr + packet_size + size + MINIMIZE_BLOCK >= send_buffer + max_send) {		//β���Ŀռ䲻����
			memmove(send_buffer, send_ptr, send_size);
			packet_ptr -= (send_ptr - send_buffer);
			send_ptr = send_buffer;
		}
		memcpy(packet_ptr + packet_size, data, size);
		packet_size += size;
		send_size += size;
		mutex.unlock();
		return true;
	}
	char *completePacket(int &datalength) {
		mutex.lock();
		if(!packet_size) {
			mutex.unlock();
			return NULL;
		}
		datalength = packet_size - sizeof(unsigned short);
		mutex.unlock();
		return packet_ptr + sizeof(unsigned short);
	}
	void sendPacket() {							//���һ�����ݰ�
		if (!packet_size)
			return;
		mutex.lock();
		*(unsigned short *)packet_ptr = packet_size;
		packet_ptr += packet_size;
		packet_size = 0;
		mutex.unlock();
	}
};

#endif
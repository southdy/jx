#ifndef XBUFFER_H
#define XBUFFER_H

#define MINIMIZE_BLOCK 32						//��С���СΪ32�ֽ�

class ZBuffer {
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
	long packet_size;							//��ǰ���ݰ��Ĵ�С

public:
	ZBuffer(long the_max_send, long the_max_receive) {
		send_buffer = new char[the_max_send];
		if(send_buffer) max_send = the_max_send;
		else max_send = 0;
		receive_buffer = new char[the_max_receive];
		if(receive_buffer) max_receive = the_max_receive;
		else max_receive = 0;
	}
	virtual ~ZBuffer() {
		if(send_buffer) delete[] send_buffer;
		if(receive_buffer) delete[] receive_buffer;
	}

//�����Ƕ����ṩ�Ľӿ�
	void stopPerf() {
		bPerf = false;
		send_perf.stop();
		receive_perf.stop();
	}

	void startPerf() {
		bPerf = true;
		send_perf.start();
		receive_perf.start();
	}

//��ȡ���õĽ������ݵĻ�����
	char *getReceiveBuffer(int &size) {
		if(receive_size + MINIMIZE_BLOCK > max_receive) {
//������󻺳������ر�����
			return NULL;
		}
		if(receive_ptr + receive_size + MINIMIZE_BLOCK > receive_buffer + max_receive) {		//β�����ݲ�����
			memmove(receive_buffer, receive_ptr, receive_size);
			receive_ptr = receive_buffer;
		}
		size = max_receive - (receive_ptr - receive_buffer) - receive_size;
		return receive_ptr + receive_size;
	}
	void receiveData(int size) {
		receive_ptr += size;
	}
	const char *getPacket(int &size) {
		if(receive_size < sizeof(unsigned short)) return NULL;
		size = *(unsigned short *)receive_ptr;
		if(receive_size >= size + sizeof(unsigned short)) {
			receive_ptr += sizeof(unsigned short);
			const char *result = (const char *)receive_ptr;
			receive_ptr += size;
			return result;
		}
		return NULL;
	}

	void clear() {
		send_ptr = send_buffer;
		*(unsigned short *)send_ptr = 0;
		send_size = 0;
		packet_size = 0;
	}

//�������ݵĽӿڷֳ�����������һ���ǻ�ȡ��ǰ���͵Ļ������ͳ��ȣ�����һ���Ǵӻ��������ָ����С������
	char *getSendData(int &size) {
		size = (int)*(unsigned short *)send_ptr;
		if(size) return send_ptr;
		else return NULL;
	}
	void sendData(int size) {
		send_ptr += size;
		send_size -= size;
		if(!send_size) {
			send_ptr = packet_ptr = send_buffer;
		}
	}

	bool packData(const char *data, int size) {
		if(send_size + size + MINIMIZE_BLOCK >= max_send) {						
//�����������ر�����
		}
		if(!packet_size) {
			packet_ptr += sizeof(unsigned short);									//ͷ�������ֽڱ�ʾ��С
			*(unsigned short *)packet_ptr = 0;
		}

		if(packet_ptr + packet_size + size + MINIMIZE_BLOCK >= send_buffer + max_send) {		//β���Ŀռ䲻����
			memmove(send_buffer, send_ptr, send_size);
			send_ptr = send_buffer;
			packet_ptr -= (send_ptr - send_buffer);
		}
		memcpy(packet_ptr + packet_size, data, size);
		packet_size += size;
		send_size += size;
	}

	char *sendPacket(int index, int &datalength) {							//���һ�����ݰ�
		*(unsigned short *)packet_ptr = packet_size;
		packet_ptr += packet_size;
		packet_size = 0;
	}

};

#endif
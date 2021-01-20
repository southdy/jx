#ifndef ZPORT_H
#define ZPORT_H

//Windows��صĴ���----------------------------------------------------------------------------------
#include <stdafx.h>
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <string.h>

#ifndef WIN32
#define WINAPI
#define BYTE unsigned char
#define DWORD  unsigned long
#define LPVOID void *

#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1
#endif
//��װ�Ļ�������
class ZMutex {
#ifdef WIN32
  CRITICAL_SECTION mutex;
#else
public:
  pthread_mutex_t mutex;
#endif
public:
  ZMutex() {
#ifdef WIN32
    InitializeCriticalSection(&mutex);
#else
    int rc = pthread_mutex_init(&mutex, NULL);
#endif                
  }
  ~ZMutex() {
#ifdef WIN32
    DeleteCriticalSection(&mutex);
#else
    int rc = pthread_mutex_destroy(&mutex);
#endif                
  }
  void lock() {
#ifdef WIN32        
    EnterCriticalSection(&mutex);
#else
    int rc = pthread_mutex_lock(&mutex);
#endif                
  }
  void unlock() {
#ifdef WIN32        
    LeaveCriticalSection(&mutex);
#else
    int rc = pthread_mutex_unlock(&mutex);
#endif                
  }
};

//��װ�Ķ�ʱ����(��ȷ������)
class ZTimer {
public:
  static inline unsigned long now() {				//���ص�ǰ�ĺ�����
#ifdef WIN32        
    return GetTickCount();
#else
    return 0;
#endif
  }
};

//��װ���߳���,�̳���������ʵ��
class ZThread {
#ifdef WIN32
  unsigned long id;
  HANDLE handle;
#else
  pthread_t p_thread;
#endif
public:
	bool bStop;
	ZThread() {
#ifdef WIN32
		id = -1;
#endif
		bStop = false;
	}
	void start();
	void stop();
	virtual int action() = 0;
};

#endif

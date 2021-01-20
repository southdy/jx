//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KThread.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	BG2132 Font Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KThread.h"
#ifdef WIN32
#include "process.h"
#endif

KThread::KThread()
{
#ifdef WIN32
	m_ThreadHandle	= NULL;
	m_ThreadId		= 0;
	m_ThreadFunc	= NULL;
	m_ThreadParam	= NULL;
#endif
}
//---------------------------------------------------------------------------
// ����:	~KThread
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KThread::~KThread()
{
#ifdef WIN32
	if (m_ThreadHandle)
	{
		CloseHandle(m_ThreadHandle);
		m_ThreadHandle	= NULL;
	}
#else

#endif
}
//---------------------------------------------------------------------------
// ����:	MyThreadProc
// ����:	�̹߳���
// ����:	lpParam		����
// ����:	void
//---------------------------------------------------------------------------
#ifdef WIN32
unsigned __stdcall MyThreadProc(LPVOID lpParam)
#else
void * MyThreadProc(LPVOID lpParam)
#endif
{
 printf("start thread %d\n", lpParam);
	KThread* pThread = (KThread*)lpParam;
#ifdef WIN32
	return pThread->ThreadFunction();
#else
	return (void *)pThread->ThreadFunction();
#endif
//	return 0;
}
//---------------------------------------------------------------------------
// ����:	ThreadFunction
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
DWORD KThread::ThreadFunction()
{
	m_ThreadFunc(m_ThreadParam);
	return 0;
}
//---------------------------------------------------------------------------
// ����:	Create
// ����:	�����߳�
// ����:	func		�̺߳���
//			arg 		�̲߳���
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KThread::Create(TThreadFunc lpFunc, void* lpParam)
{
#ifdef WIN32
	m_ThreadFunc   = lpFunc;
	m_ThreadParam  = lpParam;
	m_ThreadHandle = (HANDLE)_beginthreadex(
		NULL,			// SD
		0,				// initial stack size
		MyThreadProc,		// thread function
		this,			// thread argument
		0,				// creation option
		(unsigned*)&m_ThreadId);	// thread identifier
	return (m_ThreadHandle != NULL);
#else
     m_ThreadFunc   = lpFunc;
     int ret = pthread_create(&p_thread, NULL, MyThreadProc, this);
     printf("create thread %d return %d\n", (int)p_thread, ret);
#endif
//	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Destroy
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KThread::Destroy()
{
#ifdef WIN32
	TerminateThread(m_ThreadHandle, 0);
#endif
}
//---------------------------------------------------------------------------
// ����:	Suspend
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KThread::Suspend()
{
#ifdef WIN32
	 SuspendThread(m_ThreadHandle);
#endif
}
//---------------------------------------------------------------------------
// ����:	Resume
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KThread::Resume()
{
#ifdef WIN32
	ResumeThread(m_ThreadHandle);
#endif
}
//---------------------------------------------------------------------------
// ����:	IsRunning
// ����:	�ж��߳��Ƿ�������
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KThread::IsRunning()
{
#ifdef WIN32
	DWORD dwResult = WaitForSingleObject(m_ThreadHandle, 0);
	return (dwResult == WAIT_OBJECT_0);
#endif
//	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	WaitForExit
// ����:	�ȴ��߳̽���
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KThread::WaitForExit()
{
#ifdef WIN32
	WaitForSingleObject(m_ThreadHandle, INFINITE);
#endif
}
//---------------------------------------------------------------------------
// ����:	GetPriority
// ����:	ȡ���߳����ȼ�
// ����:	void
// ����:	int
//---------------------------------------------------------------------------
int KThread::GetPriority()
{
#ifdef WIN32
	return GetThreadPriority(m_ThreadHandle);
#endif
//	return 0;
}
//---------------------------------------------------------------------------
// ����:	SetPriority
// ����:	�����߳����ȼ�
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KThread::SetPriority(int priority)
{
#ifdef WIN32
	return SetThreadPriority(m_ThreadHandle, priority);
#endif
//	return FALSE;
}

//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KLThread.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	BG2132 Font Class
//---------------------------------------------------------------------------
#include <stdafx.h>
#include "KLThread.h"
//---------------------------------------------------------------------------
// ����:	KLThread
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KLThread::KLThread()
{
	m_ThreadHandle	= NULL;
	m_ThreadId		= 0;
	m_ThreadFunc	= NULL;
	m_ThreadParam	= NULL;
}
//---------------------------------------------------------------------------
// ����:	~KLThread
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KLThread::~KLThread()
{
	if (m_ThreadHandle)
	{
		CloseHandle(m_ThreadHandle);
		m_ThreadHandle	= NULL;
	}
}
//---------------------------------------------------------------------------
// ����:	ThreadProc
// ����:	�̹߳���
// ����:	lpParam		����
// ����:	void
//---------------------------------------------------------------------------
DWORD WINAPI KLThread::ThreadProc(LPVOID lpParam)
{
	KLThread* pThread = (KLThread*)lpParam;
	return pThread->ThreadFunction();
}
//---------------------------------------------------------------------------
// ����:	ThreadFunction
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
DWORD KLThread::ThreadFunction()
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
BOOL KLThread::Create(TThreadFunc lpFunc, void* lpParam)
{
	m_ThreadFunc   = lpFunc;
	m_ThreadParam  = lpParam;
	m_ThreadHandle = CreateThread(
		NULL,			// SD
		0,				// initial stack size
		ThreadProc,		// thread function
		this,			// thread argument
		0,				// creation option
		&m_ThreadId);	// thread identifier
	if(m_ThreadHandle)
		SetThreadPriority(m_ThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL); 
	return (m_ThreadHandle != NULL);
}
//---------------------------------------------------------------------------
// ����:	Destroy
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLThread::Destroy()
{
	TerminateThread(m_ThreadHandle, 0);
}
//---------------------------------------------------------------------------
// ����:	Suspend
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLThread::Suspend()
{
	 SuspendThread(m_ThreadHandle);
}
//---------------------------------------------------------------------------
// ����:	Resume
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLThread::Resume()
{
	ResumeThread(m_ThreadHandle);
}
//---------------------------------------------------------------------------
// ����:	IsRunning
// ����:	�ж��߳��Ƿ�������
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KLThread::IsRunning()
{
	DWORD dwResult = WaitForSingleObject(m_ThreadHandle, 0);
	return (dwResult == WAIT_OBJECT_0);
}
//---------------------------------------------------------------------------
// ����:	WaitForExit
// ����:	�ȴ��߳̽���
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLThread::WaitForExit()
{
	WaitForSingleObject(m_ThreadHandle, INFINITE);
}
//---------------------------------------------------------------------------
// ����:	GetPriority
// ����:	ȡ���߳����ȼ�
// ����:	void
// ����:	int
//---------------------------------------------------------------------------
int KLThread::GetPriority()
{
	return GetThreadPriority(m_ThreadHandle);
}
//---------------------------------------------------------------------------
// ����:	SetPriority
// ����:	�����߳����ȼ�
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KLThread::SetPriority(int priority)
{
	return SetThreadPriority(m_ThreadHandle, priority);
}
//---------------------------------------------------------------------------

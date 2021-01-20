//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTimer.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Timer Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KTimer.h"
// flying add this macro.
// must pay more attention in the platform porting job.
#ifdef __linux
#include <sys/time.h>
#endif

KTimer::KTimer()
{
#ifdef WIN32
	m_nFrequency.QuadPart = 200 * 1024 * 1024;
	m_nTimeStart.QuadPart = 0;
	m_nTimeStop.QuadPart = 0;
	m_nFPS = 0;
	QueryPerformanceFrequency(&m_nFrequency);
#else
    //m_nFrequency = CLOCKS_PER_SEC;
#endif
}
//---------------------------------------------------------------------------
// ����:	Start
// ����:	��ʼ��ʱ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTimer::Start()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_nTimeStart);
#else
    gettimeofday(&m_nTimeStart, NULL);
#endif
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ��ʱ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTimer::Stop()
{
#ifdef WIN32
	QueryPerformanceCounter(&m_nTimeStop);
#else
	gettimeofday(&m_nTimeStop, NULL);
#endif
}
//---------------------------------------------------------------------------
// ����:	GetElapse
// ����:	����ӿ�ʼ��ʱ�������Ѿ�����ʱ��
// ����:	void
// ����:	DWORD in ms
//---------------------------------------------------------------------------
DWORD KTimer::GetElapse()
{
#ifdef WIN32
	LARGE_INTEGER nTime;
	QueryPerformanceCounter(&nTime);
	return (DWORD)((nTime.QuadPart - m_nTimeStart.QuadPart) 
		* 1000 / m_nFrequency.QuadPart);
#else
	timeval tv;
	gettimeofday(&tv, NULL);
    return (tv.tv_sec - m_nTimeStart.tv_sec) * 1000 + tv.tv_usec / 1000;
#endif
}
//---------------------------------------------------------------------------
// ����:	GetElapseFrequency
// ����:	����ӿ�ʼ��ʱ�������Ѿ�����ʱ��
// ����:	void
// ����:	DWORD in frequency
//---------------------------------------------------------------------------
DWORD KTimer::GetElapseFrequency()
{
#ifdef WIN32
	LARGE_INTEGER nTime;
	QueryPerformanceCounter(&nTime);
	return (DWORD)(nTime.QuadPart - m_nTimeStart.QuadPart);
#endif
	return 0;
}
//---------------------------------------------------------------------------
// ����:	GetInterval
// ����:	ȡ�ôӿ�ʼ��ֹ֮ͣ���ʱ�������Ժ���Ϊ��λ
// ����:	void
// ����:	����ֵ
//---------------------------------------------------------------------------
DWORD KTimer::GetInterval()
{
#ifdef WIN32
	return (DWORD)((m_nTimeStop.QuadPart - m_nTimeStart.QuadPart) 
		* 1000 / m_nFrequency.QuadPart);
#endif
	return 0;
}
//---------------------------------------------------------------------------
// ����:	Passed
// ����:	���Ƿ����nTime����
// ����:	nTime	����
// ����:	TRUE	�Ѿ�����
//			FALSE	��û�й�
//---------------------------------------------------------------------------
BOOL KTimer::Passed(int nTime)
{

	if (GetElapse() >= (DWORD)nTime)
	{
		Start();
		return TRUE;
	}
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	GetFPS
// ����:	ȡ��֡����
// ����:	nFPS	֡����
// ����:	TRUE	�ɹ�
//			FALSE	ʧ��
//---------------------------------------------------------------------------
BOOL KTimer::GetFPS(int *nFPS)
{
	if (GetElapse() >= 1000)
	{
		*nFPS = m_nFPS;
		m_nFPS = 0;
		Start();
		return TRUE;
	}
	m_nFPS++;
	return FALSE;
}

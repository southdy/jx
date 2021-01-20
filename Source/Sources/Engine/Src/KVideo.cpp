//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KVideo.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Video Stream Play Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KVideo.h"
//---------------------------------------------------------------------------
// ����:	KVideo
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KVideo::KVideo()
{
	m_hEvent[0]		= 0;
	m_hEvent[1]		= 0;
	m_nX			= 0;
	m_nY			= 0;
	m_nWidth		= 0;
	m_nHeight		= 0;
	m_pVideo		= NULL;
	m_pFrame		= NULL;
	m_nFrameSize	= 0;
	m_nFrameRate	= 20;
	m_nFrameTime	= 50;
	m_uTimeID		= 0;
	m_bZoomToWin	= FALSE;
}
//---------------------------------------------------------------------------
// ����:	~KVideo
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KVideo::~KVideo()
{
	// Close();
}
//---------------------------------------------------------------------------
// ����:	ThreadFunction
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KVideo::ThreadFunction(void* lpParam)
{
	KVideo* pVideo = (KVideo*)lpParam;
	while (TRUE)
	{
		if (!pVideo->HandleNotify())
			break;
	}
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	����Ƶ�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KVideo::Open(LPSTR FileName)
{
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	��ʼ��
// ����:	
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KVideo::Init()
{
	// create event handle
	if (!InitEventHandle())
		return FALSE;

	// create play thread
	m_Thread.Create(ThreadFunction, this);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Close
// ����:	�ر��ļ����ͷ���Դ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KVideo::Close()
{
	// �����ȵ��߳̽��������ͷ��ڴ棬��ֹ�Ƿ�����
	Stop();
	SetEvent(m_hEvent[1]);
	m_Thread.WaitForExit();
	FreeEventHandle();
}
//---------------------------------------------------------------------------
// ����:	InitEventHandle
// ����:	�����¼����
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KVideo::InitEventHandle()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_hEvent[i] == NULL)
			m_hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hEvent[i] == NULL)
		{
			g_MessageBox("KVideo : Create event failed");
			return FALSE;
		}
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FreeEventHandle
// ����:	�ͷ��¼����
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
void KVideo::FreeEventHandle()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_hEvent[i])
		{
			CloseHandle(m_hEvent[i]);
			m_hEvent[i] = NULL;
		}
	}
}
//---------------------------------------------------------------------------
// ����:	HandleNotify
// ����:	�������ֲ����е�ͨ����Ϣ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KVideo::HandleNotify()
{
	DWORD dwEvent;
	
	// wait for event[0..3] to be signaled
	dwEvent = WaitForMultipleObjects(
		2,				// How many possible events
		m_hEvent,		// Location of handles
		FALSE,			// Wait for all?
		INFINITE);		// How long to wait
	
    // WAIT_OBJECT_0 == 0 but is properly treated as an arbitrary
    // index value assigned to the first event, therefore we subtract
    // it from dwEvent to get the zero-based index of the event.
	dwEvent -= WAIT_OBJECT_0;
	
	// If event[0] was set then goto next frame
	if (dwEvent == 0)
		return NextFrame();

	// If the event[1] was set then exit thread
	return (dwEvent != 1);
}
//---------------------------------------------------------------------------
// ����:	NextFrame
// ����:	��һ֡
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KVideo::NextFrame()
{
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Play
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KVideo::Play(int nX, int nY, int nZoom)
{
	if (m_pVideo == NULL)
		return;
	if (m_uTimeID == 0)
	{
		m_uTimeID = timeSetEvent(
			m_nFrameTime,	// delay time in ms
			1,	// resolution
			(LPTIMECALLBACK)m_hEvent[0],
			0,	// user data
			TIME_PERIODIC | TIME_CALLBACK_EVENT_SET);
		m_Timer.Start();
		m_nX = nX;
		m_nY = nY;
		m_bZoomToWin = nZoom;
	}
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KVideo::Stop()
{
	if (m_uTimeID)
	{
		timeKillEvent(m_uTimeID);
		m_uTimeID = 0;
		m_Timer.Stop();
	}
}
//---------------------------------------------------------------------------
// ����:	IsPlaying
// ����:	�Ƿ��ڲ���
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KVideo::IsPlaying()
{
	return (m_uTimeID != 0);
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ƶ���ǰ֡
// ����:	nPercent	�ٷֱ�0-100
// ����:	void
//---------------------------------------------------------------------------
void KVideo::Seek(int nPercent)
{
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KVideo::Rewind()
{
}
//---------------------------------------------------------------------------

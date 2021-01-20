//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KLMusic.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Wave Stream Music Class
//---------------------------------------------------------------------------
#include <stdafx.h>
#include "KLMusic.h"
//---------------------------------------------------------------------------
// ����:	KLMusic
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KLMusic::KLMusic()
{
	m_pDS		   = NULL;
	m_pSoundBuffer = NULL;
	m_pSoundNotify = NULL;
	m_hEvent[0]    = NULL;
	m_hEvent[1]    = NULL;
	m_hEvent[2]    = NULL;
	m_dwBufferSize = 0;
	m_bLoop        = FALSE;
	m_hWnd		   = NULL;
}
//---------------------------------------------------------------------------
// ����:	~KLMusic
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KLMusic::~KLMusic()
{
	Close();
}
//---------------------------------------------------------------------------
// ����:	ThreadFunction
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::ThreadFunction(void* lpParam)
{
	KLMusic* pMusic = (KLMusic*)lpParam;
	while (TRUE)
	{
		if (!pMusic->HandleNotify())
			break;
	}
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	�������ļ�
// ����:	FileName	WAV�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::Open(LPSTR FileName)
{
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	��ʼ��
// ����:	void
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::Init()
{
	if(!InitDSound())
		return FALSE;

	// init sound format
	InitSoundFormat();

	// init sound buffer
	if (!InitSoundBuffer())
		return FALSE;

	// create event handle
	if (!InitEventHandle())
		return FALSE;

	// create sound notify
	if (!InitSoundNotify())
		return FALSE;

	// fill buffer with silence
	FillBufferWithSilence();

	// set buffer play position
	m_pSoundBuffer->SetCurrentPosition(0);

	// �޸ģ�����һ���������ݣ��������ô���������ӳ�0.5��
	FillBufferWithSound(1);

	// create play thread
	m_Thread.Create(ThreadFunction, this);

	return TRUE;
}

bool KLMusic::InitDSound()
{
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;
	
    SAFE_RELEASE( m_pDS );
	
    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
    {
		return FALSE;
	}
	
    // Set DirectSound coop level 
    if( FAILED( hr = m_pDS->SetCooperativeLevel( m_hWnd, DSSCL_PRIORITY )))
    {
		return FALSE;
	}
	
    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
	
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
	{
		return FALSE;
	}
	
    if( FAILED( hr = pDSBPrimary->SetFormat(&m_WaveFormat) ) )
	{
		return FALSE;
	}
	
    SAFE_RELEASE( pDSBPrimary );
	
    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	Close
// ����:	�ͷ�����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::Close()
{
	// �����ȵ��߳̽��������ͷ��ڴ棬��ֹ�Ƿ�����
	Stop();
	SetEvent(m_hEvent[2]);
	m_Thread.WaitForExit();
	FreeSoundNotify();
	FreeSoundBuffer();
	FreeEventHandle();

	SAFE_RELEASE( m_pDS ); 
}
//---------------------------------------------------------------------------
// ����:	InitSoundFormat
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::InitSoundFormat()
{
}
//---------------------------------------------------------------------------
// ����:	InitSoundBuffer
// ����:	��������Buffer
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::InitSoundBuffer()
{
	DSBUFFERDESC		dsbdesc;
	HRESULT				hres;
	
	// get direct sound
	if (m_pDS == NULL)
		return FALSE;
	
	// close any open file and release interfaces
	if (m_pSoundBuffer)
		FreeSoundBuffer();
	
	// set streaming sound buffer desc
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY // ����λ��֪ͨ
			| DSBCAPS_GETCURRENTPOSITION2 // ����ȡ�õ�ǰλ��
			| DSBCAPS_CTRLVOLUME // ʹ�������Կ���
			| DSBCAPS_LOCSOFTWARE // ʹ���������
			| DSBCAPS_STICKYFOCUS; // �����ں�̨����
	
	// The size of the buffer is arbitrary, but should be at least
	// two seconds, to keep data writes well ahead of the play position.
	dsbdesc.dwBufferBytes = m_dwBufferSize;
	dsbdesc.lpwfxFormat = &m_WaveFormat;
	
	// create streaming sound buffer
	hres = m_pDS->CreateSoundBuffer(&dsbdesc, &m_pSoundBuffer, NULL);
	if (hres != DS_OK)
	{
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FreeSoundBuffer
// ����:	�ͷ�����Buffer
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::FreeSoundBuffer()
{
	if (m_pDS == NULL)
		return;
	SAFE_RELEASE(m_pSoundBuffer);
}
//---------------------------------------------------------------------------
// ����:	InitEventHandle
// ����:	�����¼����
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::InitEventHandle()
{
	for (int i = 0; i < NUMEVENTS; i++)
	{
		if (m_hEvent[i] == NULL)
			m_hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hEvent[i] == NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FreeEventHandle
// ����:	�����¼����
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
void KLMusic::FreeEventHandle()
{
	for (int i = 0; i < NUMEVENTS; i++)
	{
		if (m_hEvent[i])
		{
			CloseHandle(m_hEvent[i]);
			m_hEvent[i] = NULL;
		}
	}
}
//---------------------------------------------------------------------------
// ����:	InitSoundNotify
// ����:	����SoundNotify
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::InitSoundNotify()
{
	HRESULT		hres;
	
	// check sound buffer
	if (m_pSoundBuffer == NULL)
		return FALSE;
	
	// setup pos notify
	m_PosNotify[0].dwOffset		= 0;
	m_PosNotify[0].hEventNotify	= m_hEvent[0];
	m_PosNotify[1].dwOffset		= m_dwBufferSize / 2;
	m_PosNotify[1].hEventNotify	= m_hEvent[1];
	
	// query interface
	hres = m_pSoundBuffer->QueryInterface(IID_IDirectSoundNotify, (void**)&m_pSoundNotify);
	if (hres != S_OK)
	{
		return FALSE;
	}
	
	// set notify position
	hres = m_pSoundNotify->SetNotificationPositions(2, m_PosNotify);
	if (hres != DS_OK)
	{
		return FALSE;
	}
	
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FreeSoundNotify
// ����:	�ͷŲ���ͨ��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::FreeSoundNotify()
{
	if (m_pDS == NULL)
		return;
	SAFE_RELEASE(m_pSoundNotify);
}
//---------------------------------------------------------------------------
// ����:	HandleNotify
// ����:	�������ֲ����е�ͨ����Ϣ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KLMusic::HandleNotify()
{
	DWORD dwEvent;
	
	// wait for event[0..3] to be signaled
	dwEvent = WaitForMultipleObjects(
		NUMEVENTS,		// How many possible events
		m_hEvent,		// Location of handles
		FALSE,			// Wait for all?
		INFINITE);		// How long to wait
	
    // WAIT_OBJECT_0 == 0 but is properly treated as an arbitrary
    // index value assigned to the first event, therefore we subtract
    // it from dwEvent to get the zero-based index of the event.
	dwEvent -= WAIT_OBJECT_0;
	
	// If the event was set by the buffer,
	// there's input to process.
	if ((dwEvent == 0) || (dwEvent == 1))
	{
		FillBufferWithSound(dwEvent);
	}

	// If the event[2] was set, exit thread
	return (dwEvent != 2);
}
//---------------------------------------------------------------------------
// ����:	FillBufferWithSound
// ����:	�������Buffer
// ����:	dwPos	λ��
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::FillBufferWithSound(DWORD dwPos)
{
	DWORD		dwStartOfs;
	LONG		lNumToWrite;
	PVOID		lpvPtr1;
	PVOID		lpvPtr2;
	DWORD		dwBytes1;
	DWORD		dwBytes2;
	DWORD		dwBytesRead;
	HRESULT		hres;

	// check sound buffer
	if (m_pSoundBuffer == NULL)
		return FALSE;

	// decide start offset in sound buffer
	if (dwPos == 0)
		dwStartOfs = m_PosNotify[1].dwOffset;
	else
		dwStartOfs = m_PosNotify[0].dwOffset;

	// decide number bytes to write to buffer
	lNumToWrite = m_dwBufferSize / 2;

	// lock buffer to obtain buffer pointer
	hres = m_pSoundBuffer->Lock(
			dwStartOfs,       // Offset of lock start
			lNumToWrite,      // Number of bytes to lock
			&lpvPtr1,         // Address of lock start
			&dwBytes1,        // Count of bytes locked
			&lpvPtr2,         // Address of wrap around
			&dwBytes2,        // Count of wrap around bytes
			0);               // Flags
	
	// check return value
	if (hres != DS_OK)
		return FALSE;

	// Read wave data from the file into the buffer
	dwBytesRead = ReadWaveData((PBYTE)lpvPtr1, dwBytes1);

	// if reached end of file
	if (dwBytesRead < dwBytes1)
	{
		memset((PBYTE)lpvPtr1 + dwBytesRead, 0,
			dwBytes1 - dwBytesRead);
	}
	
	// unlock sound buffer
	m_pSoundBuffer->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
	
	// if reached end of file
	if (dwBytesRead < dwBytes1)
	{
		Rewind();
		if (!m_bLoop)
			Stop();
	}

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FillMusicBufferWithSilence
// ����:	�ÿհ��������Buffer
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMusic::FillBufferWithSilence()
{
	PVOID   pb1, pb2;
	DWORD   cb1, cb2;
	HRESULT	hres;
	
	// check sound buffer
	if (m_pSoundBuffer == NULL)
		return FALSE;
	
	hres = m_pSoundBuffer->Lock(0, m_dwBufferSize, &pb1, &cb1, &pb2, &cb2, 0);
	if (hres != DS_OK)
		return FALSE;
	
	memset(pb1, 0, cb1);
	m_pSoundBuffer->Unlock(pb1, cb1, pb2, cb2);
	
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	ReadWaveData
// ����:	��ȡ��������
// ����:	lpBuf	����
//			dwLen	����	
// ����:	DWORD	ʵ�ʶ�ȡ�ĳ���
//---------------------------------------------------------------------------
DWORD KLMusic::ReadWaveData(LPBYTE lpBuf, DWORD dwLen)
{
	return 0;
}
//---------------------------------------------------------------------------
// ����:	Play
// ����:	��������
// ����:	bLoop		�Ƿ�ѭ������
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::Play(BOOL bLoop)
{
	if (m_pSoundBuffer)
	{
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
		m_bLoop = bLoop;
	}
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ���ֲ���
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::Stop()
{
	if (m_pSoundBuffer)
	{
		m_pSoundBuffer->Stop();
	}
}
//---------------------------------------------------------------------------
// ����:	SetVolume
// ����:	�������ֵ�����
// ����:	lVolume		���� [0, -10000]
//					  	DSBVOLUME_MAX = 0
//						DSBVOLUME_MIN = -10000
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::SetVolume(LONG lVolume)
{
	if (m_pSoundBuffer)
	{
		m_pSoundBuffer->SetVolume(lVolume);
	}
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	��ͷ��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMusic::Rewind()
{
}
//---------------------------------------------------------------------------

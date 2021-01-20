//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KWavSound.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Wav Sound Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KDSound.h"
#include "KDError.h"
#include "KWavFile.h"
#include "KWavSound.h"
//---------------------------------------------------------------------------
// ����:	KWavSound
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KWavSound::KWavSound()
{
	g_MemZero(m_Buffer, sizeof(m_Buffer));
}
//---------------------------------------------------------------------------
// ����:	~KWavSound
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KWavSound::~KWavSound()
{
	Free();
}
//---------------------------------------------------------------------------
// ����:	Load
// ����:	������Ч
// ����:	FileName	�ļ���
// ����:	��Ч����
//---------------------------------------------------------------------------
BOOL KWavSound::Load(LPSTR FileName)
{
	LPDIRECTSOUND	pDirectSound;
	KWavFile		WavFile;
	DSBUFFERDESC	desc;
	HRESULT 		hres;
	WAVEFORMATEX	PcmWavFmt;

	// get direct sound
	pDirectSound = g_pDirectSound->GetDirectSound();
	if (pDirectSound == NULL)
		return FALSE;

	// open the file and get ready to read
	if (!WavFile.Open(FileName))
		return FALSE;

	// setup ddes structure
	g_MemZero(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | 
		DSBCAPS_LOCSOFTWARE;
	desc.dwBufferBytes = WavFile.GetWaveSize();
	WavFile.GetPcmWavFormat(&PcmWavFmt);
	desc.lpwfxFormat = &PcmWavFmt;

	// create the secondry sound buffer
	hres = pDirectSound->CreateSoundBuffer(&desc, &m_Buffer[0], NULL);
	if (hres != DS_OK)
		return FALSE;

	LPVOID	lpAudio;
	DWORD	dwBytes;
	// lock the sound buffer to get buffer pointer
	hres = m_Buffer[0]->Lock(0, WavFile.GetWaveSize(),
		&lpAudio, &dwBytes, NULL, NULL, 0);
	if (hres != DS_OK)
		return FALSE;

	// read the data from the file into the buffer
	WavFile.Read((PBYTE)lpAudio, dwBytes);

	// unlock the sound buffer
	hres = m_Buffer[0]->Unlock(lpAudio, dwBytes, NULL, 0);
	if (hres != DS_OK)
		return FALSE;

	// duplicate sound buffer
	for (int i = 1; i < BUFFER_COUNT; i++)
	{
		pDirectSound->DuplicateSoundBuffer(m_Buffer[0], &m_Buffer[i]);
	}

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Free
// ����:	�ͷ���Ч
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KWavSound::Free()
{
	if (g_pDirectSound->GetDirectSound() == NULL)
		return;
	for (int i = BUFFER_COUNT - 1; i >= 0; i--)
	{
		SAFE_RELEASE(m_Buffer[i]);
	}
}
//---------------------------------------------------------------------------
// ����:	GetFreeBuffer
// ����:	ȡ�ÿ��е�SoundBuffer
// ����:	void
// ����:	Sound Buffer Index
//---------------------------------------------------------------------------
int KWavSound::GetFreeBuffer()
{
	DWORD	Status = 0;
	int 	i;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		if (m_Buffer[i] == NULL)
			continue;
		
		m_Buffer[i]->GetStatus(&Status);

		if (Status & DSBSTATUS_BUFFERLOST)
			//m_Buffer[i]->Restore();
			return BUFFER_COUNT;

		if ((Status & DSBSTATUS_PLAYING) == 0)
			break;
	}
	return i;
}
//---------------------------------------------------------------------------
// ����:	GetPlayBuffer
// ����:	ȡ�����ڲ��ŵ�SoundBuffer
// ����:	void
// ����:	Sound Buffer Index
//---------------------------------------------------------------------------
int KWavSound::GetPlayBuffer()
{
	DWORD	Status = 0;
	int 	i;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		if (m_Buffer[i] == NULL)
			continue;
		
		m_Buffer[i]->GetStatus(&Status);

		if (Status & DSBSTATUS_BUFFERLOST)
			//m_Buffer[i]->Restore();
			return BUFFER_COUNT;

		if (Status & DSBSTATUS_PLAYING)
			break;
	}
	return i;
}
//---------------------------------------------------------------------------
// ����:	Play
// ����:	������Ч
// ����:	nPan		���� [-10000,10000]
//				 		SBPAN_LEFT = -10000
//						DSBPAN_CENTER = 0
//						DSBPAN_RIGHT = 10000
//			nVolume		���� [0, -10000]
//					  	DSBVOLUME_MAX = 0
//						DSBVOLUME_MIN = -10000
//			bLoop		ѭ�� [0,1]
//						0 : ��ѭ������
//						1 : ѭ������
// ����:	void
//---------------------------------------------------------------------------
void KWavSound::Play(int nPan, int nVolume, BOOL bLoop)
{
	int i = GetFreeBuffer();
	if (i >= BUFFER_COUNT)
		return;
	m_Buffer[i]->SetCurrentPosition(0);
	m_Buffer[i]->SetPan(nPan);
	m_Buffer[i]->SetVolume(nVolume);
	m_Buffer[i]->Play(0, 0, (bLoop)? DSBPLAY_LOOPING : 0);
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ������Ч
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KWavSound::Stop()
{
	int i = GetPlayBuffer();
	if (i >= BUFFER_COUNT)
		return;
	m_Buffer[i]->Stop();
}
//---------------------------------------------------------------------------
// ����:	SetPan
// ����:	��������
// ����:	nPan		���� [-10000,10000]
//				 		SBPAN_LEFT = -10000
//						DSBPAN_CENTER = 0
//						DSBPAN_RIGHT = 10000
// ����:	void
//---------------------------------------------------------------------------
void KWavSound::SetPan(int nPan)
{
	int i = GetPlayBuffer();
	if (i >= BUFFER_COUNT)
		return;
	m_Buffer[i]->SetPan(nPan);
}
//---------------------------------------------------------------------------
// ����:	SetVolume
// ����:	��������
// ����:	nVolume		���� [0, -10000]
//					  	DSBVOLUME_MAX = 0
//						DSBVOLUME_MIN = -10000
// ����:	void
//---------------------------------------------------------------------------
void KWavSound::SetVolume(int nVolume)
{
	int i = GetPlayBuffer();
	if (i >= BUFFER_COUNT)
		return;
	m_Buffer[i]->SetVolume(nVolume);
}
//---------------------------------------------------------------------------
// ����:	IsPlaying
// ����:	�Ƿ��ڲ���
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KWavSound::IsPlaying()
{
	int i = GetPlayBuffer();
	return (i < BUFFER_COUNT);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KWavMusic.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Wave Stream Music Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KWavMusic.h"
//---------------------------------------------------------------------------
// ����:	Open
// ����:	�������ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KWavMusic::Open(LPSTR FileName)
{
	if (m_pSoundBuffer)
		Close();

	if (!m_WavFile.Open(FileName))
		return FALSE;

	return Init();
}
//---------------------------------------------------------------------------
// ����:	InitSoundFormat
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KWavMusic::InitSoundFormat()
{
	// get wave format pointer
	m_WavFile.GetPcmWavFormat(&m_WaveFormat);

	// one second sound buffer
	m_dwBufferSize = m_WaveFormat.nAvgBytesPerSec;

	// size align for IMA-ADPCM format
	if (m_WavFile.GetFormatTag() == WAVE_FORMAT_IMA_ADPCM)
	{
		int nBlockSize = (m_WavFile.GetBlockAlign() - 7) * 4;
		m_dwBufferSize = (m_dwBufferSize + nBlockSize - 1)
			/ nBlockSize * nBlockSize;
	}

	// two second sound buffer
	m_dwBufferSize *= 2;
}
//---------------------------------------------------------------------------
// ����:	ReadWaveData
// ����:	��ȡ��������
// ����:	lpBuf	����
//			dwLen	����	
// ����:	DWORD	ʵ�ʶ�ȡ�ĳ���
//---------------------------------------------------------------------------
DWORD KWavMusic::ReadWaveData(LPBYTE lpBuf, DWORD dwLen)
{
	return m_WavFile.Read(lpBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	��ͷ��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KWavMusic::Rewind()
{
	m_WavFile.Rewind();
}
//---------------------------------------------------------------------------

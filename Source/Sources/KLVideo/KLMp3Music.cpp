//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KLMp3Music.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	MP3 Stream Music Class
//---------------------------------------------------------------------------
#include <stdafx.h>
#include "mp3lib.h"
#include "KLMp3Music.h"
//---------------------------------------------------------------------------
#define MP3_BUFSIZE 		60000	// mp3 data buffer
//---------------------------------------------------------------------------
// ����:	Open
// ����:	�������ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMp3Music::Open(LPSTR FileName)
{
	if (m_pSoundBuffer)
		Close();

	if (!Mp3FileOpen(FileName))
		return FALSE;

	if (!Mp3Init())
		return FALSE;

	return Init();
}

void KLMp3Music::Close()
{
	KLMusic::Close();
	SAFE_DELETE_ARRAY(mp3_buffer);
}

//---------------------------------------------------------------------------
// ����:	InitSoundFormat
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp3Music::InitSoundFormat()
{
	// align buffer size with mp3 frame 4608
	m_dwBufferSize = m_WaveFormat.nAvgBytesPerSec;
	m_dwBufferSize = (m_dwBufferSize + 4607) / 4608 * 4608;

	// 2 second sound buffer
	m_dwBufferSize *= 2;
}
//---------------------------------------------------------------------------
// ����:	ReadWaveData
// ����:	��ȡ��������
// ����:	lpBuf	����
//			dwLen	����	
// ����:	DWORD	ʵ�ʶ�ȡ�ĳ���
//---------------------------------------------------------------------------
DWORD KLMp3Music::ReadWaveData(LPBYTE lpBuf, DWORD dwLen)
{
	return Mp3Decode(lpBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp3Music::Seek(int nPercent)
{
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	��ͷ��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp3Music::Rewind()
{
	Mp3FileSeek(0);
	mp3_bufbytes = 0;
}
//---------------------------------------------------------------------------
// ����:	Mp3FileOpen
// ����:	
// ����:	FileName
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KLMp3Music::Mp3FileOpen(LPSTR FileName)
{
	return m_Mp3File.Open(FileName);
}
//---------------------------------------------------------------------------
// ����:	Mp3FileRead
// ����:	
// ����:	pBuf
//			dwlen
// ����:	void
//---------------------------------------------------------------------------
DWORD KLMp3Music::Mp3FileRead(PBYTE pBuf, DWORD dwLen)
{
	return m_Mp3File.Read(pBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Mp3FileSeek
// ����:	
// ����:	lOffset
// ����:	void
//---------------------------------------------------------------------------
DWORD KLMp3Music::Mp3FileSeek(LONG lOffset)
{
	return m_Mp3File.Seek(lOffset, FILE_BEGIN);
}
//---------------------------------------------------------------------------
// ����:	Mp3Init
// ����:	��ʼ��MP3�ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KLMp3Music::Mp3Init()
{
	MPEG_HEAD mpeg_head;
	DEC_INFO  dec_info;
	
	// reduction_code:	sample rate reduction code
	//		0 = full rate
	//		1 = half rate
	//		2 = quarter rate
	int reduction_code = 0;
	
	// transform_code:	ignored
	int transform_code = 0;

	// convert_code:	channel conversion
	//		0 = two chan output
	//		1 = convert two chan to mono
	//		2 = convert two chan to left chan
	//		3 = convert two chan to right chan
	int convert_code = 0;
	
	// freq_limit:	limits bandwidth of pcm output to specified
	//		frequency. Special use. Set to 24000 for normal use.
	int freq_limit = 24000;

	// alloc mp3 stream buffer
	mp3_buffer = new BYTE[MP3_BUFSIZE];
	if(!mp3_buffer)
		return FALSE;

	// init mp3 buffer
	mp3_bufbytes = 0;
	mp3_bufptr = mp3_buffer;
	mp3_trigger = 2500;

	// seek to begin of mp3 file
	Mp3FileSeek(0);

	// fill mp3 buffer
	if (!Mp3FillBuffer())
		return FALSE;

	// parse mpeg header
	mp3_frmbytes = mp3_decode_head(mp3_buffer, &mpeg_head);
	if (mp3_frmbytes == 0)
	{
		return FALSE;
	}

	// init mp3 decoder
	if (!mp3_decode_init(&mpeg_head, mp3_frmbytes,
		reduction_code, transform_code, convert_code, freq_limit))
	{
		return FALSE;
	}

	// get mp3 info
	mp3_decode_info(&dec_info);

	// set pcm wave format
	m_WaveFormat.wFormatTag		 = WAVE_FORMAT_PCM;
	m_WaveFormat.nChannels		 = dec_info.channels;
	m_WaveFormat.nSamplesPerSec	 = dec_info.samprate;
	m_WaveFormat.nAvgBytesPerSec = (dec_info.channels * dec_info.samprate * dec_info.bits + 7) / 8;
	m_WaveFormat.nBlockAlign	 = (dec_info.channels * dec_info.bits + 7) / 8;
	m_WaveFormat.wBitsPerSample	 = dec_info.bits;
	m_WaveFormat.cbSize			 = 0;

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Mp3Decode
// ����:	����MP3
// ����:	lpPcmBuf		PCM buffer pointer
//			dwBufLen		buffer size in bytes
// ����:	void
//---------------------------------------------------------------------------
DWORD KLMp3Music::Mp3Decode(PBYTE lpPcmBuf, DWORD dwBufLen)
{
	PBYTE	pcm_buffer = lpPcmBuf;
	int		pcm_trigger = dwBufLen;
	int		pcm_bufbytes = 0;
	IN_OUT	res = {0, 0};

	while (TRUE)
	{
		// fill mp3 buffer
		if (!Mp3FillBuffer())
			break;
		
		// end of mp3 file
		if (mp3_bufbytes < mp3_frmbytes)
			break;

		// decode one frame to pcm buffer
		res = mp3_decode_frame(mp3_bufptr, pcm_buffer);
		if (res.in_bytes <= 0)
		{
			break;
		}

		// update buffer pointer and buffer size
		mp3_bufptr += res.in_bytes;
		mp3_bufbytes -= res.in_bytes;
		pcm_buffer += res.out_bytes;
		pcm_bufbytes += res.out_bytes;

		// is pcm buffer full
		if (pcm_bufbytes >= pcm_trigger)
			break;
	}
	return pcm_bufbytes;
}
//---------------------------------------------------------------------------
// ����:	Mp3FillBuffer
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KLMp3Music::Mp3FillBuffer()
{
	DWORD dwRead;
	
	// signed var could be negative
	if (mp3_bufbytes < 0)
		mp3_bufbytes = 0;
	
	// read mp3 data to buffer
	if (mp3_bufbytes < mp3_trigger)
	{
		memmove(mp3_buffer, mp3_bufptr, mp3_bufbytes);
		dwRead = Mp3FileRead(mp3_buffer + mp3_bufbytes, 
			MP3_BUFSIZE - mp3_bufbytes);
		mp3_bufbytes += dwRead;
		mp3_bufptr = mp3_buffer;
	}
	
	return TRUE;
}
//---------------------------------------------------------------------------

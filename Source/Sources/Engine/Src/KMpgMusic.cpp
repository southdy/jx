//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMpgMusic.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	MP3 Stream Music Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KFilePath.h"
#include "KMpgMusic.h"
//---------------------------------------------------------------------------
#define MP3_FRM_SIZE	4608
#define MP3_BUF_SIZE 	MP3_FRM_SIZE * 10// mp3 file buffer = 10 frame
#define SBLIMIT			32
#define SSLIMIT			18
#define MAXFRAMESIZE	1792
//---------------------------------------------------------------------------
struct frame {
	int lsf;// 0=MPEG1.0 1=MPEG2.0 
	int mpeg25;// 1=MPEG2.5
	int layer;// 1=LayerI 2=LayerII 3=LayerIII
	int error_protection;// error protection  0=yes, 1=no
	int br_index;// bit rate index
	int sr_index;// sampling rate index
	int padding;// padding
	int extension;// extension
	int mode;// 0=stereo 1=joint stereo 2=dual channel 3=single channel
	int mode_ext;// 0=MPG_MD_LR_LR 1=MPG_MD_LR_I 2=MPG_MD_MS_LR 3=MPG_MD_MS_I
	int copyright;// 0=no 1=yes
	int original;// 0=no 1=yes
	int emphasis;// 0=none 1=50/15 microsecs 2=reserved 3=CCITT J 17
	int framesize;// frame size
	int bitrate;// bit rate
	int samprate;// sample rate
	int channels;// channel num
};
//---------------------------------------------------------------------------
struct mpstr {
	int framesize;
	int bsnum;
	struct frame fr;
	unsigned char bsspace[2][MAXFRAMESIZE+512]; /* MAXFRAMESIZE */
	float hybrid_block[2][2][SBLIMIT*SSLIMIT];
	int hybrid_blc[2];
	float synth_buffs[2][2][0x110];
	int synth_bo;
};
//---------------------------------------------------------------------------
typedef long (*mp3_decode_init)(struct mpstr *mp);
typedef long (*mp3_decode_head)(struct mpstr *mp,unsigned char *in);
typedef long (*mp3_decode_data)(struct mpstr *mp,unsigned char *in,unsigned char *out);
//---------------------------------------------------------------------------
static HMODULE	m_hDll = NULL;
static mp3_decode_init Mp3DecodeInit = NULL;
static mp3_decode_head Mp3DecodeHead = NULL;
static mp3_decode_data Mp3DecodeData = NULL;
static BOOL LoadMpgLib();
static void FreeMpgLib();
//---------------------------------------------------------------------------
// ����:	Load Mp3 Lib
// ����:	���� Mp3Lib.dll
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL LoadMpgLib()
{
	if (m_hDll)
		return TRUE;
	char szPathName[MAXPATH];
	g_GetFullPath(szPathName, "\\mpglib.dll");
	m_hDll = LoadLibrary(szPathName);
	if (m_hDll == NULL)
	{
		g_DebugLog("Can't load mpglib.dll!");
		return FALSE;
	}
	Mp3DecodeInit = (mp3_decode_init)GetProcAddress(m_hDll, "mp3_decode_init");
	Mp3DecodeHead = (mp3_decode_head)GetProcAddress(m_hDll, "mp3_decode_head");
	Mp3DecodeData = (mp3_decode_data)GetProcAddress(m_hDll, "mp3_decode_data");
	if ((!Mp3DecodeInit)||(!Mp3DecodeHead)||(!Mp3DecodeData))
	{
		g_DebugLog("There are errors in Mpglib.dll!");
		FreeMpgLib();
		return FALSE;
	}
	g_DebugLog("Mpglib.dll load Ok!");
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	FreeMp3Lib
// ����:	
// ����:	void
//---------------------------------------------------------------------------
void FreeMpgLib()
{
	if (m_hDll)
		FreeLibrary(m_hDll);
	m_hDll = NULL;
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	�������ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KMpgMusic::Open(LPSTR FileName)
{
	if (m_pSoundBuffer)
		Close();

	if (!Mp3FileOpen(FileName))
		return FALSE;

	if (!Mp3Init())
		return FALSE;

	return Init();
}
//---------------------------------------------------------------------------
// ����:	InitSoundFormat
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMpgMusic::InitSoundFormat()
{
	// align buffer size with mp3 frame size
	m_dwBufferSize = m_WaveFormat.nAvgBytesPerSec;
	m_dwBufferSize = (m_dwBufferSize + MP3_FRM_SIZE - 1) / 
		MP3_FRM_SIZE * MP3_FRM_SIZE;
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
DWORD KMpgMusic::ReadWaveData(LPBYTE lpBuf, DWORD dwLen)
{
	return Mp3Decode(lpBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMpgMusic::Seek(int nPercent)
{
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	��ͷ��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMpgMusic::Rewind()
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
BOOL KMpgMusic::Mp3FileOpen(LPSTR FileName)
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
DWORD KMpgMusic::Mp3FileRead(PBYTE pBuf, DWORD dwLen)
{
	return m_Mp3File.Read(pBuf, dwLen);
}
//---------------------------------------------------------------------------
// ����:	Mp3FileSeek
// ����:	
// ����:	lOffset
// ����:	void
//---------------------------------------------------------------------------
DWORD KMpgMusic::Mp3FileSeek(LONG lOffset)
{
	return m_Mp3File.Seek(lOffset, FILE_BEGIN);
}
//---------------------------------------------------------------------------
// ����:	Mp3Init
// ����:	��ʼ��MP3�ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KMpgMusic::Mp3Init()
{
	struct mpstr *mp;

	// load mpglib.dll
	if (!LoadMpgLib())
		return FALSE;

	// alloc mp3 struct buffer
	if (!m_Mp3Struct.Alloc(sizeof(struct mpstr)))
		return FALSE;
	mp = (struct mpstr *)m_Mp3Struct.GetMemPtr();

	// alloc mp3 stream buffer
	if (!m_Mp3Buffer.Alloc(MP3_BUF_SIZE))
		return FALSE;
	mp3_buffer = (PBYTE)m_Mp3Buffer.GetMemPtr();

	// init mp3 buffer
	mp3_bufbytes = 0;
	mp3_bufptr = mp3_buffer;

	// seek to begin of mp3 file
	Mp3FileSeek(0);

	// fill mp3 buffer
	Mp3FillBuffer();

	// init mp3 decoder
	Mp3DecodeInit(mp);
	
	// parse mp3 header
	if (!Mp3DecodeHead(mp, mp3_bufptr))
	{
		g_MessageBox("KMpgMusic: mp3 decode head fail");
		return FALSE;
	}

	// set pcm wave format
	int channels = mp->fr.channels;
	int samprate = mp->fr.samprate;
	int bits = mp->fr.channels * 8;
	m_WaveFormat.wFormatTag		 = WAVE_FORMAT_PCM;
	m_WaveFormat.nChannels		 = channels;
	m_WaveFormat.nSamplesPerSec	 = samprate;
	m_WaveFormat.nAvgBytesPerSec = (channels * samprate * bits + 7) / 8;
	m_WaveFormat.nBlockAlign	 = (channels * bits + 7) / 8;
	m_WaveFormat.wBitsPerSample	 = bits;
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
DWORD KMpgMusic::Mp3Decode(PBYTE lpPcmBuf, DWORD dwBufLen)
{
	PBYTE	pcm_buffer = lpPcmBuf;
	int		pcm_trigger = dwBufLen;
	int		pcm_bufbytes = 0;
	int     pcm_bytes = 0;

	struct mpstr *mp;
	mp = (struct mpstr *)m_Mp3Struct.GetMemPtr();

	while (TRUE)
	{
		Mp3FillBuffer();
		
		if (mp3_bufbytes < mp->framesize + 4)
			break;

		pcm_bytes = Mp3DecodeData(mp, mp3_bufptr, pcm_buffer);
		if (pcm_bytes == 0)
		{
			g_DebugLog("KMpgMusic: bad sync in mp3 file");
			break;
		}

		mp3_bufptr += (mp->framesize + 4);
		mp3_bufbytes -= (mp->framesize + 4);
		pcm_buffer += pcm_bytes;
		pcm_bufbytes += pcm_bytes;

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
BOOL KMpgMusic::Mp3FillBuffer()
{
	unsigned long readbytes;
	
	// signed var could be negative
	if (mp3_bufbytes < 0)
		mp3_bufbytes = 0;
	
	// read mp3 data to buffer
	if (mp3_bufbytes < 2500)
	{
		if (mp3_bufbytes)
			memmove(mp3_buffer, mp3_bufptr, mp3_bufbytes);
		readbytes = Mp3FileRead(mp3_buffer + mp3_bufbytes, 
			MP3_BUF_SIZE - mp3_bufbytes);
		mp3_bufbytes += readbytes;
		mp3_bufptr = mp3_buffer;
	}
	
	return TRUE;
}
//---------------------------------------------------------------------------

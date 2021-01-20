//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KLMp4Video.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Mpeg4 Video Class
//---------------------------------------------------------------------------
#include <stdafx.h>
#include "decore.h"
#include "KLTimer.h"
#include "KLMp4Video.h"
#include "KLVideo.h"


//---------------------------------------------------------------------------
// ����:	KLMp4Video
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KLMp4Video::KLMp4Video()
{
	m_dwHandle = 0;
	
	m_hEvent[0]		= 0;
	m_hEvent[1]		= 0;
	m_hMutex		= NULL;
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
	m_bWait			= false;
}
//---------------------------------------------------------------------------
// ����:	~KLMp4Video
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KLMp4Video::~KLMp4Video()
{
	Close();
}
//---------------------------------------------------------------------------
// ����:	ThreadFunction
// ����:	�����߳�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::ThreadFunction(void* lpParam)
{
	KLMp4Video* pVideo = (KLMp4Video*)lpParam;
	while (TRUE)
	{
		if (!pVideo->HandleNotify())
			break;
	}
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	��AVI�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMp4Video::Open(LPSTR FileName)
{
	if (m_pVideo)
		Close();

	if (!m_AviFile.Open(FileName))
		return FALSE;

	if (!m_AviFile.GetVideoIndex())
		return FALSE;

	if (m_dwHandle)
		DivXExit();

	if (!DivXInit())
		return FALSE;

	return Init();
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	��ʼ��
// ����:	
// ����:	TRUE���ɹ�	FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMp4Video::Init()
{
	// create event handle
	if (!InitEventHandle())
		return FALSE;
	
	// create play thread
	m_Thread.Create(ThreadFunction, this);
	
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	InitEventHandle
// ����:	�����¼����
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KLMp4Video::InitEventHandle()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_hEvent[i] == NULL)
			m_hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hEvent[i] == NULL)
		{
			return FALSE;
		}
	}

	if(m_hMutex == NULL)
	m_hMutex = CreateMutex(NULL,			// no security attributes
						   FALSE,			// initially not owned
						   NULL);			// name of mutex
	
	if (m_hMutex == NULL) 
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	FreeEventHandle
// ����:	�ͷ��¼����
// ����:	void
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
void KLMp4Video::FreeEventHandle()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_hEvent[i])
		{
			CloseHandle(m_hEvent[i]);
			m_hEvent[i] = NULL;
		}
	}

	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}
//---------------------------------------------------------------------------
// ����:	HandleNotify
// ����:	�������ֲ����е�ͨ����Ϣ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KLMp4Video::HandleNotify()
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
	{
		m_bWait = true;
		BOOL bRet = NextFrame();
		m_bWait = false;
		return bRet;
	}	
	
	// If the event[1] was set then exit thread
	return (dwEvent != 1);
}

//---------------------------------------------------------------------------
// ����:	Close
// ����:	�ر��ļ����ͷ���Դ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::Close()
{
	// �����ȵ��߳̽��������ͷ��ڴ棬��ֹ�Ƿ�����
	Stop();
	SetEvent(m_hEvent[1]);
	m_Thread.WaitForExit();
	FreeEventHandle();
	DivXExit();
}

//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ƶ���ǰ֡
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::Seek(int nPercent)
{
	m_AviFile.VideoSeek(nPercent);
}
//---------------------------------------------------------------------------
// ����:	Rewind
// ����:	
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::Rewind()
{
	m_AviFile.Rewind();
}
//---------------------------------------------------------------------------
// ����:	NextFrame
// ����:	��һ֡
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KLMp4Video::NextFrame()
{
	BOOL bRender = TRUE;

	// read next video frame
	m_nFrameSize = m_AviFile.NextFrame(m_pVideo);

	// drop this frame if necessary
	if ((DWORD)m_AviFile.CurrentFrame() * 1000 < m_Timer.GetElapse() * m_nFrameRate)
	{
		bRender = FALSE;
		SetEvent(m_hEvent[0]);
	}

	// if reach the video end
	if (m_nFrameSize >= 0)
	{	
		DivXDecode(bRender);
		return TRUE;
	}
	else
	{
		//�������	
		Stop();
		return FALSE;
	}
}

inline void RGB32TO555(LPBYTE pDes, LPBYTE pSrc, uint32 nCount)
{
	WORD *p = (WORD*)pDes;
	BYTE a,r,g,b;
	for(uint32 i=0; i<nCount; i++)
	{
		b = *(pSrc), g = *(pSrc+1), r = *(pSrc+2), a = *(pSrc+3), 
		*p++ = (((WORD)(b&0xf8))>>3) + (((WORD)(g&0xf8))<<2) + (((WORD)(r&0xf8))<<7);
		pSrc += 4;
	}
}

inline void RGB32TO565(LPBYTE pDes, LPBYTE pSrc, uint32 nCount)
{
	WORD *p = (WORD*)pDes;
	BYTE a,r,g,b;
	for(uint32 i=0; i<nCount; i++)
	{
		b = *(pSrc), g = *(pSrc+1), r = *(pSrc+2), a = *(pSrc+3), 
		*p++ = (((WORD)(b&0xf8))>>3) + (((WORD)(g&0xfc))<<3) + (((WORD)(r&0xf8))<<8);
		pSrc += 4;
	}
}

inline void RGB32TO32(LPBYTE pDes, LPBYTE pSrc, uint32 nCount)
{
	DWORD *ps = (DWORD*)pSrc;
	DWORD *pd = (DWORD*)pDes;
	for(uint32 i=0; i<nCount; i++)
	{
		*pd++ = *ps++;
	}
}

inline void RGB32TO32R(LPBYTE pDes, LPBYTE pSrc, uint32 nCount)
{
	for(uint32 i=0; i<nCount; i++)
	{
		*pDes++ = *(pSrc+3);
		*pDes++ = *(pSrc+2);
		*pDes++ = *(pSrc+1);
		*pDes++ = *pSrc;
		pSrc += 4;
	}
}

typedef void (*COLORCONVERTFN)(LPBYTE pDes, LPBYTE pSrc, uint32 nCount);


void KLMp4Video::VideoCopyToBuffer(void *buf,uint32 left,uint32 top,uint32 Pitch,
								uint32 destheight,uint32 Flags)
{
	WaitForSingleObject( m_hMutex, INFINITE);

	u32 nBytePerPixel;
	COLORCONVERTFN ColorConvertFn;

	if(Flags == KLVIDEOSURFACE555)
	{
		nBytePerPixel = 2;
		ColorConvertFn = RGB32TO555;
	}
	else if(Flags == KLVIDEOSURFACE565)
	{
		nBytePerPixel = 2;
		ColorConvertFn = RGB32TO565;
	}
	else if(Flags == KLVIDEOSURFACE32)
	{
		nBytePerPixel = 4;
		ColorConvertFn = RGB32TO32;
	}
	else if(Flags == KLVIDEOSURFACE32R)
	{
		nBytePerPixel = 4;
		ColorConvertFn = RGB32TO32R;
	}

	u32 i;
	LPBYTE pByte;
	LPBYTE pFrame;
	u32 nLineWidth;


	_ASSERT(left * nBytePerPixel < Pitch);
	nLineWidth = Pitch / nBytePerPixel - left;
	if(nLineWidth > (uint32)m_nWidth) nLineWidth = m_nWidth;

	if(destheight > (uint32)m_nHeight)
		destheight = m_nHeight;

	pByte = ((BYTE*)buf) + Pitch * top + left * nBytePerPixel;
	pFrame = m_pFrame + (m_nHeight-1)*m_nWidth*4;
	for(i=0 ; i<destheight ; i++)
	{
		ColorConvertFn(pByte, pFrame, nLineWidth);
		pByte += Pitch;
		pFrame -= m_nWidth*4;
	}

	ReleaseMutex(m_hMutex);
}

//---------------------------------------------------------------------------
// ����:	SetPos
// ����:	
// ����:	
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::SetPos(int nX, int nY)
{
	m_nX = nX;
	m_nY = nY;
}

//---------------------------------------------------------------------------
// ����:	DivXInit
// ����:	��ʼ��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
BOOL KLMp4Video::DivXInit()
{
	char				szMethod[8];
	BITMAPINFOHEADER	BmpInfoHead;

	// check avi compress method
	m_AviFile.GetCompressMethod(szMethod);
	if (memcmp(szMethod, "divx", 4) != 0)
	{
		return FALSE;
	}

	// get frame delay
	m_nFrameRate = m_AviFile.FrameRate();
	m_nFrameTime = (1000 + m_nFrameRate - 1) / m_nFrameRate;

	// get bitmap info header
	m_AviFile.GetBitmapInfoHeader(&BmpInfoHead);

	// check compress method
	if (memcmp(&BmpInfoHead.biCompression, "DIVX", 4) != 0)
	{
		return FALSE;
	}
	
	// get frame width an height
	m_nWidth = BmpInfoHead.biWidth;
	m_nHeight = BmpInfoHead.biHeight;
	if(m_nWidth < m_nHeight)
		return FALSE;

	// get video buffer
	if(!m_pVideo)
		m_pVideo = (LPBYTE)new BYTE[m_nWidth * m_nHeight * 4];
	if(!m_pVideo)
		return FALSE;

	// get frame buffer
	if(!m_pFrame)
		m_pFrame = (LPBYTE)new BYTE[m_nWidth * m_nHeight * 4];
	if(!m_pFrame)
	{
		delete[] m_pVideo;
		m_pVideo = NULL;
		return FALSE;
	}

	// get a unique decore handle
	if (m_dwHandle == 0)
		m_dwHandle = GetTickCount();

	// init decore
	DEC_PARAM dec_param;

	dec_param.x_dim         = m_nWidth;
	dec_param.y_dim         = m_nHeight;
	dec_param.color_depth   = NULL;
	dec_param.output_format = RGB32;

	if (decore(m_dwHandle, DEC_OPT_INIT, &dec_param, NULL) != DEC_OK)
	{
		return FALSE;
	}

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	DivXDecode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::DivXDecode(BOOL bRender)
{
	WaitForSingleObject( m_hMutex, INFINITE);  

	DEC_FRAME dec_frame;

	dec_frame.length      = m_nFrameSize;
	dec_frame.bitstream   = m_pVideo;
	dec_frame.bmp         = m_pFrame;
	dec_frame.render_flag = bRender;
	
	decore(m_dwHandle, 0, &dec_frame, NULL);

	ReleaseMutex(m_hMutex);
}
//---------------------------------------------------------------------------
// ����:	DivXExit
// ����:	�ͷŽ�����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::DivXExit()
{
	if (m_dwHandle)
	{
		decore(m_dwHandle, DEC_OPT_RELEASE, NULL, NULL);
		m_dwHandle = 0;
	}
	if(m_pVideo)
	{
		delete[] m_pVideo;
		m_pVideo = NULL;
	}
	if(m_pFrame)
	{
		delete[] m_pFrame;
		m_pFrame = NULL;
	}
}

//---------------------------------------------------------------------------
// ����:	Play
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::Play(BOOL bZoom)
{
	if (m_pVideo == NULL)
		return;
	NextFrame();
	if (m_uTimeID == 0)
	{
		m_uTimeID = timeSetEvent(
			m_nFrameTime,	// delay time in ms
			1,	// resolution
			(LPTIMECALLBACK)m_hEvent[0],
			0,	// user data
			TIME_PERIODIC | TIME_CALLBACK_EVENT_SET);
		m_Timer.Start();
		m_bZoomToWin = bZoom;
	}
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KLMp4Video::Stop()
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
BOOL KLMp4Video::IsPlaying()
{
	return (m_uTimeID != 0);
}

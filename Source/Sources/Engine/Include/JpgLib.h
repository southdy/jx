//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KJpegLib.h
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KJpegLib_H
#define KJpegLib_H
//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif 
//---------------------------------------------------------------------------
// Jpeg file info
typedef struct {
	int mode;		// ɫ��ģʽ
	int width;		// ���
	int height;		// �߶�
} JPEG_INFO;

// interface
BOOL	jpeg_decode_init(BOOL bRGB555, BOOL bMMXCPU);
BOOL	jpeg_decode_info(PBYTE pJpgBuf, JPEG_INFO* pInfo);
BOOL	jpeg_decode_data(PWORD pBmpBuf, JPEG_INFO* pInfo);
BOOL	jpeg_decode_dataEx(PWORD pBmpBuf, int pitch, JPEG_INFO* pInfo);

//---------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif 
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KBmpFile24.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	24bit BMP file class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemClass.h"
#include "KColors.h"
#include "KPakFile.h"
#include "KBmpFile24.h"
//---------------------------------------------------------------------------
// ����:	Load
// ����:	��BMP�Ŀ�
// ����:	lpFileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KBmpFile24::Load(LPSTR lpFileName, KBitmap16* lpBitmap)
{
	BITMAPFILEHEADER	FileHeader;
	BITMAPINFOHEADER	InfoHeader;
	KPakFile			File;
	KMemClass			Buffer;

	// open the file
	if (!File.Open(lpFileName))
		return FALSE;

	// read file head
	File.Read(&FileHeader, sizeof(FileHeader));

	// check BMP flag "BM"
	if (FileHeader.bfType != 0x4d42)
		return FALSE;

	// read info head
	File.Read(&InfoHeader, sizeof(InfoHeader));

	// check compression
	if (InfoHeader.biCompression != 0)
		return FALSE;

	// check bit count
	if (InfoHeader.biBitCount != 24)
		return FALSE;

	// get width and height 
	m_nWidth  = InfoHeader.biWidth;
	m_nHeight = InfoHeader.biHeight;

	// byte per line % 4 must = 0
	m_nBytesPerLine = m_nWidth * 3;
	if ((m_nBytesPerLine % 4) != 0)
		m_nBytesPerLine = m_nBytesPerLine + 4 - (m_nBytesPerLine % 4);

	// alloc buffer
	if (!Buffer.Alloc(m_nHeight * m_nBytesPerLine))
		return FALSE;

	// int bitmap with width & height
	if (!lpBitmap->Init(m_nWidth, m_nHeight))
		return FALSE;

	// set file pointer to data begin
	File.Seek(FileHeader.bfOffBits, FILE_BEGIN);

	// read bitmap bits data
	File.Read(Buffer.GetMemPtr(), m_nBytesPerLine * m_nHeight);

	// decode bitmap
	Decode(Buffer.GetMemPtr(), lpBitmap->GetBitmap());

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Save
// ����:	����BMP�ļ�
// ����:	lpFileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KBmpFile24::Save(LPSTR lpFileName, KBitmap16* lpBitmap)
{
	BITMAPFILEHEADER	FileHeader;
	BITMAPINFOHEADER	InfoHeader;
	KFile				File;
	DWORD				OffBits;
	KMemClass			Buffer;

	// get width and height 
	m_nWidth  = lpBitmap->GetWidth();
	m_nHeight = lpBitmap->GetHeight();

	// byte per line % 4 must = 0
	m_nBytesPerLine = m_nWidth * 3;
	if ((m_nBytesPerLine % 4) != 0)
		m_nBytesPerLine = m_nBytesPerLine + 4 - (m_nBytesPerLine % 4);

	// alloc buffer
	if (!Buffer.Alloc(m_nHeight * m_nBytesPerLine))
		return FALSE;

	// offset in bytes
	OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// file header
	FileHeader.bfType          = 0x4d42; // "BM"
	FileHeader.bfSize          = m_nBytesPerLine * m_nHeight + OffBits;
	FileHeader.bfReserved1     = 0;
	FileHeader.bfReserved2     = 0;
	FileHeader.bfOffBits       = OffBits;

	// info header
	InfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
	InfoHeader.biWidth         = m_nWidth;
	InfoHeader.biHeight        = m_nHeight;
	InfoHeader.biPlanes        = 1;
	InfoHeader.biBitCount      = 24;
	InfoHeader.biCompression   = 0;
	InfoHeader.biSizeImage     = 0;
	InfoHeader.biXPelsPerMeter = 0xb40;
	InfoHeader.biYPelsPerMeter = 0xb40;
	InfoHeader.biClrUsed       = 0;
	InfoHeader.biClrImportant  = 0;

	// crete the file
	if (!File.Create(lpFileName))
		return FALSE;

	// write file head
	File.Write(&FileHeader, sizeof(FileHeader));

	// write info head
	File.Write(&InfoHeader, sizeof(InfoHeader));

	// encode bitmap
	Encode(Buffer.GetMemPtr(), lpBitmap->GetBitmap());

	// write bitmap bits data
	File.Write(Buffer.GetMemPtr(), m_nBytesPerLine * m_nHeight);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Decode
// ����:	����
// ����:	pBuffer		24λɫλͼ
//			pBitmap		16λɫλͼ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KBmpFile24::Decode(PVOID pBuffer, PVOID pBitmap)
{
	LPWORD lpDes = (LPWORD)pBitmap;
	LPBYTE lpSrc = (LPBYTE)pBuffer;

	if (lpDes == NULL || lpSrc == NULL)
		return FALSE;

	lpSrc += (m_nHeight - 1) * m_nBytesPerLine;
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			lpDes[j] = g_RGB(lpSrc[2], lpSrc[1], lpSrc[0]);
			lpSrc += 3;
		}
		lpSrc -= 3 * m_nWidth;
		lpSrc -= m_nBytesPerLine;
		lpDes += m_nWidth;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Encode
// ����:	����
// ����:	pBuffer		24λɫλͼ
//			pBitmap		16λɫλͼ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KBmpFile24::Encode(PVOID pBuffer, PVOID pBitmap)
{
	LPBYTE lpDes = (LPBYTE)pBuffer;
	LPWORD lpSrc = (LPWORD)pBitmap;

	if (lpDes == NULL || lpSrc == NULL)
		return FALSE;

	lpDes += (m_nHeight - 1) * m_nBytesPerLine;
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			lpDes[0] = g_Blue(lpSrc[j]);
			lpDes[1] = g_Blue(lpSrc[j]);
			lpDes[2] = g_Blue(lpSrc[j]);
			lpDes += 3;
		}
		lpDes -= 3 * m_nWidth;
		lpDes -= m_nBytesPerLine;
		lpSrc += m_nWidth;
	}
	return TRUE;
}
//---------------------------------------------------------------------------

BOOL KBmpFile24::SaveBuffer24(LPSTR lpFileName, PVOID pBitmap, int nPitch, int nWidth, int nHeight)
{
	return SaveBuffer(lpFileName, pBitmap, nPitch, nWidth, nHeight, true);
}

BOOL KBmpFile24::SaveBuffer32(LPSTR lpFileName, PVOID pBitmap, int nPitch, int nWidth, int nHeight)
{
	return SaveBuffer(lpFileName, pBitmap, nPitch, nWidth, nHeight, false);
}

//---------------------------------------------------------------------------
// ����:	BOOL SaveBuffer(LPSTR lpFileName, PVOID pBitmap, int nWidth, int nHeight, bool bIs24Bit)
// ����:	����
// ����:	lpFileName	�ļ���
//			pBitmap		16λɫλͼ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KBmpFile24::SaveBuffer(LPSTR lpFileName, PVOID pBitmap, int nPitch, int nWidth, int nHeight, bool bIs24Bit)
{
	if(!pBitmap)
		return FALSE;

	BITMAPFILEHEADER	FileHeader;
	BITMAPINFOHEADER	InfoHeader;
	KFile				File;
	DWORD				OffBits;
	KMemClass			Buffer;

	// byte per line % 4 must = 0
	int nBytesPerLine = nWidth * 3;
	if ((nBytesPerLine % 4) != 0)
		nBytesPerLine = nBytesPerLine + 4 - (nBytesPerLine % 4);

	// alloc buffer
	if (!Buffer.Alloc(nHeight * nBytesPerLine))
		return FALSE;

	// offset in bytes
	OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// file header
	FileHeader.bfType          = 0x4d42; // "BM"
	FileHeader.bfSize          = nBytesPerLine * nHeight + OffBits;
	FileHeader.bfReserved1     = 0;
	FileHeader.bfReserved2     = 0;
	FileHeader.bfOffBits       = OffBits;

	// info header
	InfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
	InfoHeader.biWidth         = nWidth;
	InfoHeader.biHeight        = nHeight;
	InfoHeader.biPlanes        = 1;
	InfoHeader.biBitCount      = 24;
	InfoHeader.biCompression   = 0;
	InfoHeader.biSizeImage     = 0;
	InfoHeader.biXPelsPerMeter = 0xb40;
	InfoHeader.biYPelsPerMeter = 0xb40;
	InfoHeader.biClrUsed       = 0;
	InfoHeader.biClrImportant  = 0;

	// crete the file
	if (!File.Create(lpFileName))
		return FALSE;

	// write file head
	File.Write(&FileHeader, sizeof(FileHeader));

	// write info head
	File.Write(&InfoHeader, sizeof(InfoHeader));

	// encode bitmap
	LPBYTE lpDes = (LPBYTE)Buffer.GetMemPtr();
	LPBYTE lpSrc = (LPBYTE)pBitmap;

	int nLineAdd = nPitch - nWidth * 4;
	lpDes += (nHeight - 1) * nBytesPerLine;
	if(bIs24Bit)
	{
		for (int i = 0; i < nHeight; i++)
		{
			for (int j = 0; j < nWidth; j++)
			{
				lpDes[0] = lpSrc[0];
				lpDes[1] = lpSrc[1];
				lpDes[2] = lpSrc[2];
				lpDes += 3;
				lpSrc += 3;
			}
			lpDes -= 3 * nWidth;
			lpDes -= nBytesPerLine;
		}
	}
	else
	{
		for (int i = 0; i < nHeight; i++)
		{
			for (int j = 0; j < nWidth; j++)
			{
				lpDes[0] = lpSrc[0];
				lpDes[1] = lpSrc[1];
				lpDes[2] = lpSrc[2];
				lpDes += 3;
				lpSrc += 4;
			}
			lpDes -= 3 * nWidth;
			lpDes -= nBytesPerLine;
			lpSrc += nLineAdd;
		}
	}

	// write bitmap bits data
	File.Write(Buffer.GetMemPtr(), nBytesPerLine * nHeight);

	return TRUE;
}

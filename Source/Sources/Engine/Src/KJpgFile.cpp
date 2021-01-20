//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KJpgFile.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Gif file read class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDDraw.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KPakFile.h"
#include "JpgLib.h"
#include "KJpgFile.h"
//---------------------------------------------------------------------------
// ����:	Load
// ����:	����һ��JPEG�ļ�
// ����:	FileName	�ļ���
//			lpBitmap	λͼָ��
//          uRGBMask16  ָ�� 16 λɫ��MASKֵ�������-1, ��ʾʹ�ô�DirectDraw��ȡֵ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KJpgFile::Load(LPSTR FileName, KBitmap16* lpBitmap, unsigned uRGBMask16)
{
	KPakFile	File;
	PBYTE		pJpg;
	BOOL		bRGB555;
	JPEG_INFO	JpegInfo;

	// open the file
	if (!File.Open(FileName))
		return FALSE;

	// allocate mem buffer for file
	if (!m_Buffer.Alloc(File.Size()))
		return FALSE;
	pJpg = (PBYTE)m_Buffer.GetMemPtr();
	
	// read file into mem buffer
	File.Read(pJpg, File.Size());

    if (uRGBMask16 == ((unsigned)-1))
    {
    	bRGB555 = (g_pDirectDraw->GetRGBBitMask16() == RGB_555) ? TRUE : FALSE;
    }
    else
    {
        bRGB555 = (uRGBMask16 == RGB_555) ? TRUE : FALSE;
    }

	// decode init
	if (!jpeg_decode_init(bRGB555, TRUE))
		return FALSE;

	// decode info
	if (!jpeg_decode_info(pJpg, &JpegInfo))
		return FALSE;

	// create bitmap
	if (!lpBitmap->Init(JpegInfo.width, JpegInfo.height))
		return FALSE;

	// decode frame
	if (!jpeg_decode_data((PWORD)lpBitmap->GetBitmap(), &JpegInfo))
		return FALSE;

	return TRUE;
}
//---------------------------------------------------------------------------

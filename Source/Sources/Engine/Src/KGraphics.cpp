//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2002 by Kingsoft
//
// File:	KGraphics.cpp
// Date:	2002.03.06
// Code:	Spe
// Desc:	��ͬ��KBitmap֮��Ĵ���
//---------------------------------------------------------------------------
#include "kwin32.h"
#include "KEngine.h"
#include "KBitmap.h"
#include "KGraphics.h"
//---------------------------------------------------------------------------
// ����:	g_RemoveSamePixel
// ����:	ȥ��Ŀ��ͼ����Դͼ��ͬ�ĵ�
// ����:	*DesBitmap, *SrcBitmap
// ����:	�Ƿ�ɹ�����
//---------------------------------------------------------------------------
BOOL g_RemoveSamePixel(KBitmap *DesBitmap, KBitmap *SrcBitmap)
{
	BYTE	*pDesBuffer, *pSrcBuffer;
	int		nHeight, nWidth, i, j;
	// Դͼ��Ŀ��ͼ��߲�ͬ���޷��Ƚ�ȥ��
	if (DesBitmap->GetHeight() != SrcBitmap->GetHeight()
		|| DesBitmap->GetWidth() != SrcBitmap->GetWidth())
		return FALSE;

	nHeight	= DesBitmap->GetHeight();
	nWidth	= DesBitmap->GetWidth();
	pDesBuffer = (BYTE *)DesBitmap->GetBitmap();
	pSrcBuffer = (BYTE *)SrcBitmap->GetBitmap();

	for (i = 0; i < nHeight; i++)
	{
		for (j = 0; j < nWidth; j++)
		{
			if (pDesBuffer[i * nWidth + j] == pSrcBuffer[i * nWidth + j])
				pDesBuffer[i * nWidth + j] = 0;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	g_RemoveDiffPixel
// ����:	ȥ��Ŀ��ͼ����Դͼ��ͬ�ĵ�
// ����:	*DesBitmap, *SrcBitmap
// ����:	�Ƿ�ɹ�����
//---------------------------------------------------------------------------
BOOL g_RemoveDiffPixel(KBitmap *DesBitmap, KBitmap *SrcBitmap)
{
	BYTE	*pDesBuffer, *pSrcBuffer;
	int		nHeight, nWidth, i, j;
	// Դͼ��Ŀ��ͼ��߲�ͬ���޷��Ƚ�ȥ��
	if (DesBitmap->GetHeight() != SrcBitmap->GetHeight()
		|| DesBitmap->GetWidth() != SrcBitmap->GetWidth())
		return FALSE;
	
	nHeight	= DesBitmap->GetHeight();
	nWidth	= DesBitmap->GetWidth();
	pDesBuffer = (BYTE *)DesBitmap->GetBitmap();
	pSrcBuffer = (BYTE *)SrcBitmap->GetBitmap();
	
	for (i = 0; i < nHeight; i++)
	{
		for (j = 0; j < nWidth; j++)
		{
			if (pDesBuffer[i * nWidth + j] != pSrcBuffer[i * nWidth + j])
				pDesBuffer[i * nWidth + j] = 0;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	g_RemoveNoneZeroPixel
// ����:	ȥ��Ŀ��ͼ��Դͼλ��Ϊ�����ĵ�
// ����:	*DesBitmap, *SrcBitmap
// ����:	�Ƿ�ɹ�����
//---------------------------------------------------------------------------
BOOL g_RemoveNoneZeroPixel(KBitmap *DesBitmap, KBitmap *SrcBitmap)
{
	BYTE	*pDesBuffer, *pSrcBuffer;
	int		nHeight, nWidth, i, j;
	KPAL32	*pPal32;
	// Դͼ��Ŀ��ͼ��߲�ͬ���޷��Ƚ�ȥ��
	if (DesBitmap->GetHeight() != SrcBitmap->GetHeight()
		|| DesBitmap->GetWidth() != SrcBitmap->GetWidth())
		return FALSE;
	
	nHeight	= DesBitmap->GetHeight();
	nWidth	= DesBitmap->GetWidth();
	pDesBuffer = (BYTE *)DesBitmap->GetBitmap();
	pSrcBuffer = (BYTE *)SrcBitmap->GetBitmap();
	pPal32 = SrcBitmap->GetPal32();

	for (i = 0; i < nHeight; i++)
	{
		for (j = 0; j < nWidth; j++)
		{
			if (pPal32[pSrcBuffer[i * nWidth + j]].Red != 0
				|| pPal32[pSrcBuffer[i * nWidth + j]].Green != 0
				|| pPal32[pSrcBuffer[i * nWidth + j]].Blue != 0)
				pDesBuffer[i * nWidth + j] = 0;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	g_RemoveZeroPixel
// ����:	ȥ��Ŀ��ͼ��Դͼλ��Ϊ���ĵ�
// ����:	*DesBitmap, *SrcBitmap
// ����:	�Ƿ�ɹ�����
//---------------------------------------------------------------------------
BOOL g_RemoveZeroPixel(KBitmap *DesBitmap, KBitmap *SrcBitmap)
{
	BYTE	*pDesBuffer, *pSrcBuffer;
	int		nHeight, nWidth, i, j;
	KPAL32	*pPal32;
	// Դͼ��Ŀ��ͼ��߲�ͬ���޷��Ƚ�ȥ��
	if (DesBitmap->GetHeight() != SrcBitmap->GetHeight()
		|| DesBitmap->GetWidth() != SrcBitmap->GetWidth())
		return FALSE;
	
	nHeight	= DesBitmap->GetHeight();
	nWidth	= DesBitmap->GetWidth();
	pDesBuffer = (BYTE *)DesBitmap->GetBitmap();
	pSrcBuffer = (BYTE *)SrcBitmap->GetBitmap();
	pPal32 = SrcBitmap->GetPal32();
	
	for (i = 0; i < nHeight; i++)
	{
		for (j = 0; j < nWidth; j++)
		{
			if (pPal32[pSrcBuffer[i * nWidth + j]].Red == 0
				&& pPal32[pSrcBuffer[i * nWidth + j]].Green == 0
				&& pPal32[pSrcBuffer[i * nWidth + j]].Blue == 0)
				pDesBuffer[i * nWidth + j] = 0;
		}
	}
	return TRUE;
}

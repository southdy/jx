//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KSpriteCodec.cpp
// Date:	2001.09.18
// Code:	Wangwei(Daphnis)
// Desc:	Convert Bitmap to Sprite Image Format
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KSpriteCodec.h"
//---------------------------------------------------------------------------
// ����:	KSpriteCodec
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KSpriteCodec::KSpriteCodec()
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nLength = 0;
	m_pBitmap = NULL;
	m_pAlpha  = NULL;
}
//---------------------------------------------------------------------------
// ����:	Convert
// ����:	��ʽת��
// ����:	pBitmap		λͼָ��
//			pAlpha		ALPHAͨ��
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KSpriteCodec::Convert(KBitmap* pBitmap, KBitmap* pAlpha)
{
	// check bitmap memory
	if (pBitmap->GetBitmap() == NULL)
		return FALSE;

	// check alpha memory
	if (pAlpha->GetBitmap() == NULL)
		return FALSE;

	// get bitmap info
	m_pBitmap = pBitmap;
	m_nWidth  = m_pBitmap->GetWidth();
	m_nHeight = m_pBitmap->GetHeight();
	m_pAlpha  = pAlpha;
	
	// allocate sprite buffer
	if (!m_Sprite.Alloc(m_nWidth * m_nHeight * 3))
		return FALSE;

	// convert image rect
	RECT rc;
	GetMiniRect(&rc);
	ConvertRect(&rc);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	ConvertRect
// ����:	ת��һ����������
// ����:	nX			X����
//			nY			Y����
//			nWidth		���
//			nHeight		�߶�
//			pBuffer		����
// ����:	ת����Ĵ�С(in bytes)
//---------------------------------------------------------------------------
void KSpriteCodec::ConvertRect(LPRECT lpRect)
{
	long nX = lpRect->left;
	long nY = lpRect->top;
	long nWidth = lpRect->right - lpRect->left;
	long nHeight = lpRect->bottom - lpRect->top;
	PBYTE pBuffer = (PBYTE)m_Sprite.GetMemPtr();
	PWORD pHead = (PWORD)pBuffer;
	long nLen, i;

	m_nLength = 8;
	pHead[0] = (WORD)nWidth;
	pHead[1] = (WORD)nHeight;
	pHead[2] = (WORD)nX;
	pHead[3] = (WORD)nY;
	pBuffer += 8;
	for (i = 0; i < nHeight; i++)
	{
		nLen = ConvertLine(nX, nY, nWidth, pBuffer);
		pBuffer += nLen;
		m_nLength += nLen;
		nY++;
	}
}
//---------------------------------------------------------------------------
// ����:	ConvertLine
// ����:	ת��һ��ͼ������
// ����:	nX			X����
//			nY			Y����
//			nWidth		���
//			pBuffer		����
// ����:	ת����Ĵ�С(in bytes)
//---------------------------------------------------------------------------
long KSpriteCodec::ConvertLine(int nX, int nY, int nWidth, PBYTE pBuffer)
{
	BYTE  byAlpha = 0;
	BYTE  byCount = 0;
	PBYTE pSprite = pBuffer;

	while (nWidth > 0)
	{
		// get alpha value
		byAlpha = m_pAlpha->GetPixel(nX, nY);

		// get pixel count
		byCount = GetAlphaCount(nX, nY, nWidth);
		
		// set pixel count
		*pSprite = byCount;
		pSprite++;
	
		// set alpha value
		*pSprite = byAlpha;
		pSprite++;

		// if alpha > 0 copy pixel
		if (byAlpha)
		{
			nWidth -= byCount;
			while (byCount-- > 0)
			{
				*pSprite = m_pBitmap->GetPixel(nX, nY);
				pSprite++;
				nX++;
			}
		}
		else
		{
			nWidth -= byCount;
			nX += byCount;
		}
	}
	return (long)(pSprite - pBuffer);
}
//---------------------------------------------------------------------------
// ����:	GetAlphaCount
// ����:	ȡ��ALPHAֵ��ͬ�ĸ���
// ����:	nX			X����
//			nY			Y����
//			nWidth		���
// ����:	ALPHA��ͬ���صĸ���
//---------------------------------------------------------------------------
BYTE KSpriteCodec::GetAlphaCount(int nX, int nY, int nWidth)
{
	BYTE byAlpha = m_pAlpha->GetPixel(nX, nY);
	BYTE byCount = 0;
	
	while (nWidth-- > 0)
	{
		if (byAlpha != m_pAlpha->GetPixel(nX++, nY))
			break;
		if (++byCount >= 255)
			break;
	}
	return byCount;
}
//---------------------------------------------------------------------------
// ����:	GetMiniRect
// ����:	ȡ�ð���ͼ�ε���С����
// ����:	lpRect
// ����:	void
//---------------------------------------------------------------------------
void KSpriteCodec::GetMiniRect(LPRECT lpRect)
{
	long x, y;

	// left
	for (x = 0; x < m_nWidth; x++)
	{
		for (y = 0; y < m_nHeight; y++)
		{
			if (m_pAlpha->GetPixel(x, y))
				break;
		}
		if (y == m_nHeight)
			continue;
		if (m_pAlpha->GetPixel(x, y))
			break;
	}
	lpRect->left = x;

	// right
	for (x = m_nWidth - 1; x >= 0; x--)
	{
		for (y = 0; y < m_nHeight; y++)
		{
			if (m_pAlpha->GetPixel(x, y))
				break;
		}
		if (y == m_nHeight)
			continue;
		if (m_pAlpha->GetPixel(x, y))
			break;
	}
	lpRect->right = x + 1;

	// top
	for (y = 0; y < m_nHeight; y++)
	{
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pAlpha->GetPixel(x, y))
				break;
		}
		if (x == m_nWidth)
			continue;
		if (m_pAlpha->GetPixel(x, y))
			break;
	}
	lpRect->top = y;

	// bottom
	for (y = m_nHeight - 1; y >= 0; y--)
	{
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pAlpha->GetPixel(x, y))
				break;
		}
		if (x == m_nWidth)
			continue;
		if (m_pAlpha->GetPixel(x, y))
			break;
	}
	lpRect->bottom = y + 1;
	if (lpRect->left >= lpRect->right || lpRect->top >= lpRect->bottom)
	{
		lpRect->left = 0;
		lpRect->right = 1;
		lpRect->top = 0;
		lpRect->bottom = 1;
	}
}
//---------------------------------------------------------------------------

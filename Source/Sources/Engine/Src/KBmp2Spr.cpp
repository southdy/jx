//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KBmp2Spr.cpp
// Date:	2001.09.18
// Code:	Wangwei(Daphnis)
// Desc:	Convert Bitmap to Sprite Image Format
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KBmp2Spr.h"
//---------------------------------------------------------------------------
// ����:	KBmp2Spr
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KBmp2Spr::KBmp2Spr()
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nLength = 0;
	m_bTransp = FALSE;
	m_pBitmap = NULL;
}
//---------------------------------------------------------------------------
// ����:	Convert
// ����:	��ʽת��
// ����:	pBitmap		λͼָ��
//			bTransp		�Ƿ�͸��
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KBmp2Spr::Convert(KBitmap* pBitmap, BOOL bTransp)
{
	// check bitmap memory
	if (pBitmap->GetBitmap() == NULL)
		return FALSE;

	// get bitmap info
	m_pBitmap = pBitmap;
	m_nWidth  = m_pBitmap->GetWidth();
	m_nHeight = m_pBitmap->GetHeight();
	m_bTransp = bTransp;
	if (m_bTransp)
		m_ColorKey = m_pBitmap->GetPixel(0, 0);

	// allocate sprite buffer
	m_nLength = m_nWidth * m_nHeight + 64 * m_nHeight;
	if (!m_Sprite.Alloc(m_nLength))
		return FALSE;

	// convert image rect
	RECT rc;
	GetMiniRect(&rc);//ȡ�ð���ͼ�ε���С����
	m_nLength = ConvertRect(&rc, (PBYTE)m_Sprite.GetMemPtr());
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
long KBmp2Spr::ConvertRect(LPRECT lpRect, PBYTE pBuffer)
{
	long nX = lpRect->left;
	long nY = lpRect->top;
	long nWidth = lpRect->right - lpRect->left;
	long nHeight = lpRect->bottom - lpRect->top;
	PWORD pHead = (PWORD)pBuffer;
	long nSize = 8;
	long nLen, i;

	pHead[0] = (WORD)nWidth;
	pHead[1] = (WORD)nHeight;
	pHead[2] = (WORD)nX;
	pHead[3] = (WORD)nY;
	pBuffer += 8;
	for (i = 0; i < nHeight; i++)
	{
		nLen = ConvertLine(nX, nY, nWidth, pBuffer);
		pBuffer += nLen;
		nSize += nLen;
		nY++;
	}
	return nSize;
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
long KBmp2Spr::ConvertLine(int nX, int nY, int nWidth, PBYTE pBuffer)
{
	long  nTCount = 0;
	long  nDCount = 0;
	PBYTE pSprite = pBuffer;

	while (nWidth > 0)
	{
		nTCount = GetTransCount(nX, nY, nWidth);
		nDCount = GetPixelCount(nX, nY, nWidth);
		if (nTCount > 0)
		{
			*pSprite = (BYTE)nTCount;// pixel count
			pSprite++;
			*pSprite = 0;// alpha value
			pSprite++;
			nWidth -= nTCount;
			nX += nTCount;
			continue;
		}
		if (nDCount > 0)
		{
			*pSprite = (BYTE)nDCount;// pixel count
			pSprite++;
			*pSprite = 255;// alpha value
			pSprite++;
			nWidth -= nDCount;
			while (nDCount-- > 0)// copy pixel
			{
				*pSprite = m_pBitmap->GetPixel(nX, nY);
				pSprite++;
				nX++;
			}
			continue;
		}
	}
	return (long)(pSprite - pBuffer);
}
//---------------------------------------------------------------------------
// ����:	GetTransCount
// ����:	ȡ��͸�����صĸ���
// ����:	nX			X����
//			nY			Y����
//			nWidth		���
// ����:	͸�����صĸ���
//---------------------------------------------------------------------------
long KBmp2Spr::GetTransCount(int nX, int nY, int nWidth)
{
	BYTE Color;
	long nCount = 0;
	
	if (!m_bTransp)
		return 0;
	
	while (nWidth > 0)
	{
		Color = m_pBitmap->GetPixel(nX, nY);
		if (Color != m_ColorKey)
			break;
		nX++;
		nWidth--;
		nCount++;
		if (nCount >= 255)
			break;
	}
	return nCount;
}
//---------------------------------------------------------------------------
// ����:	GetPixelCount
// ����:	ȡ�ò�͸�����صĸ���
// ����:	nX			X����
//			nY			Y����
//			nWidth		���
// ����:	��͸�����صĸ���
//---------------------------------------------------------------------------
long KBmp2Spr::GetPixelCount(int nX, int nY, int nWidth)
{
	BYTE Color;
	long nCount = 0;
	
	while (nWidth > 0)
	{
		Color = m_pBitmap->GetPixel(nX, nY);
		if (m_bTransp && (Color == m_ColorKey))
			break;
		nX++;
		nWidth--;
		nCount++;
		if (nCount >= 255)
			break;
	}
	return nCount;
}
//---------------------------------------------------------------------------
// ����:	GetMiniRect
// ����:	ȡ����Сͼ������
// ����:	lpRect
// ����:	void
//---------------------------------------------------------------------------
void KBmp2Spr::GetMiniRect(LPRECT lpRect)
{
	long x, y;

	// left
	for (x = 0; x < m_nWidth; x++)
	{
		for (y = 0; y < m_nHeight; y++)
		{
			if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
				break;
		}
		if (y == m_nHeight)
			continue;
		if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
			break;
	}
	lpRect->left = x;

	// right
	for (x = m_nWidth - 1; x >= 0; x--)
	{
		for (y = 0; y < m_nHeight; y++)
		{
			if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
				break;
		}
		if (y == m_nHeight)
			continue;
		if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
			break;
	}
	lpRect->right = x + 1;

	// top
	for (y = 0; y < m_nHeight; y++)
	{
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
				break;
		}
		if (x == m_nWidth)
			continue;
		if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
			break;
	}
	lpRect->top = y;

	// bottom
	for (y = m_nHeight - 1; y >= 0; y--)
	{
		for (x = 0; x < m_nWidth; x++)
		{
			if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
				break;
		}
		if (x == m_nWidth)
			continue;
		if (m_pBitmap->GetPixel(x, y) != m_ColorKey)
			break;
	}
	lpRect->bottom = y + 1;
}
//---------------------------------------------------------------------------

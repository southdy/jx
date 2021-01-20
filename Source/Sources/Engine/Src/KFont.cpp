//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2002 by Kingsoft
// File:	KFont.h
// Date:	2000.08.08
// Code:	Daniel Wang, Wooy
// Desc:	Header File
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KPakFile.h"
#include "KColors.h"
#include "KCanvas.h"
#include "KBmpFile.h"
#include "KFont.h"

#define	CHARASET_GBK_INDEX(cH, cL)		(((cH) - 0x81) * 190 + ((cL) - 0x40) - ((cL) >> 7))
#define	CHARASET_GB2132_INDEX(cH, cL)	(((cH) - 0xa1) *  94 + ((cL) - 0xa1))

//---------------------------------------------------------------------------
// ����:	KFont
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KFont::KFont()
{
	m_dwSize	= 0;
	m_dwColor	= 0;
	m_nCount	= 0;
	m_nFontW	= 16;
	m_nFontH	= 16;
	m_nLineW	= 160;
	m_nLineH	= 20;
	m_TextStyle = FONT_ALPHA;
	m_pdwOffs	= NULL;
	m_nOffsetSpace = 0;
	m_CharaSet	= CHARASET_GBK;
}

//---------------------------------------------------------------------------
// ����:	��������
//---------------------------------------------------------------------------
KFont::~KFont()
{
	Clear();
}

//---------------------------------------------------------------------------
// ����:	�ͷ�������Դ
//---------------------------------------------------------------------------
void KFont::Clear()
{
	m_Font.Free();
	if (m_pdwOffs)
	{
		free(m_pdwOffs);
		m_pdwOffs = NULL;
	}
	m_nOffsetSpace = 0;	
	m_nCount = 0;
	m_dwSize = 0;	
}

//---------------------------------------------------------------------------
// ����:	���õ�ǰʹ�õı������
//---------------------------------------------------------------------------
void KFont::SetCharaset(CHARASET CharaSet/* = CHARASET_GBK*/)
{
	m_CharaSet = CharaSet;
}

//---------------------------------------------------------------------------
// ����:	Load
// ����:	��������
// ����:	FileName	�����ļ���
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KFont::Load(LPSTR FileName, CHARASET CharaSet/*= CHARASET_GBK*/)
{
	KPakFile	File;
	TFontHead	Header;

	Clear();
	m_CharaSet = CharaSet;
	g_DebugLog("KFont::Load(%s)...", FileName);

	// open font file
	if (!File.Open(FileName))
		return FALSE;
	
	// read file head
	File.Read(&Header, sizeof(Header));

	// check font id
	if (!g_StrCmp(Header.Id, "ASF"))
		return FALSE;

	// get font info
	m_dwSize = Header.Size;
	m_nOffsetSpace = m_nCount = Header.Count;
	m_nFontW = Header.Width;
	m_nFontH = Header.Height;

	// alloc font buffer
	if ((m_pdwOffs = (DWORD*)malloc(sizeof(DWORD) * m_nOffsetSpace)) == NULL ||
		!m_Font.Alloc(m_dwSize))
	{
		Clear();
		return FALSE;
	}

	// read offs table
	File.Read(m_pdwOffs, m_nCount * sizeof(DWORD));

	// read font data
	File.Read(m_Font.GetMemPtr(), m_dwSize);

	// close file
	File.Close();

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Save
// ����:	��������
// ����:	FileName	�����ļ���
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KFont::Save(LPSTR FileName)
{
	KFile		File;
	TFontHead	Header;

	g_DebugLog("KFont::Save(%s)...", FileName);

	// open font file
	if (!File.Create(FileName) || m_nCount <= 0)
		return FALSE;

	// set font head
	g_StrCpy(Header.Id, "ASF");
	Header.Size		= m_dwSize;
	Header.Count	= m_nCount;
	Header.Width	= m_nFontW;
	Header.Height	= m_nFontH;

	// write file head
	File.Write(&Header, sizeof(Header));

	// write offs table
	File.Write(m_pdwOffs, m_nCount * 4);

	// write font data
	File.Write(m_Font.GetMemPtr(), m_dwSize);

	// close file
	File.Close();

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	DrawFont
// ����:	����һ����
// ����:	nX	:	������
//			nY	:	������
//			nH	:	����
//			nL	:	����
// ����:	void
//---------------------------------------------------------------------------
void KFont::DrawFont(int nIndex, int nX, int nY)
{
	PBYTE lpData = (PBYTE)m_Font.GetMemPtr();
	if (lpData && nIndex >= 0 && nIndex < m_nCount && m_pdwOffs[nIndex])
	{
		lpData += m_pdwOffs[nIndex];
		if(m_TextStyle==FONT_ALPHA)
		 g_pCanvas->DrawFont(nX, nY, m_nFontW, m_nFontH, m_dwColor, 31,lpData);
		else
		 g_pCanvas->DrawFontSolid(nX, nY, m_nFontW, m_nFontH, m_dwColor, 31,lpData);
	}
}

//---------------------------------------------------------------------------
// ����:	DrawFontAlpha
// ����:	��Alpha����һ����
// ����:	nX	:	������
//			nY	:	������
//			nH	:	����
//			nL	:	����
// ����:	void
//---------------------------------------------------------------------------
void KFont::DrawFontAlpha(int nIndex, int nX, int nY, int nAlpha)
{
	PBYTE lpData = (PBYTE)m_Font.GetMemPtr();
	if (lpData && nIndex >= 0 && nIndex < m_nCount  && m_pdwOffs[nIndex])
	{
		lpData += m_pdwOffs[nIndex];
		if(m_TextStyle==FONT_ALPHA)
		 g_pCanvas->DrawFont(nX, nY, m_nFontW, m_nFontH, m_dwColor, nAlpha,lpData);
		else
		 g_pCanvas->DrawFontSolid(nX, nY, m_nFontW, m_nFontH, m_dwColor, nAlpha,lpData);
	}
}

//---------------------------------------------------------------------------
// ����:	DrawWord
// ����:	����һ����
// ����:	nX, nY	:	����
//			nH, nL	:	����
//---------------------------------------------------------------------------
void KFont::DrawWord(int nX, int nY, int nH, int nL)
{
	int nIndex = -1;
	if (m_CharaSet == CHARASET_GBK)
	{
		if (nH > 0x80)
			nIndex = CHARASET_GBK_INDEX(nH, nL);
		else if (nL > 0x20 && nL < 0x5F)
			nIndex = CHARASET_GBK_INDEX(0xa3, 0x20 + nL);
		else if (nL >= 0x5F && nL < 0x7F)
			nIndex = CHARASET_GBK_INDEX(0xa3, 0x21 + nL);
	}
	else if (m_CharaSet == CHARASET_GB2132)
	{
		if (nH > 0xa0)
			nIndex = CHARASET_GB2132_INDEX(nH, nL);
		else if ((nL >= 33) && (nL <= 127))
			nIndex = CHARASET_GB2132_INDEX(0xa3, 0x80 + nL);
	}
	DrawFont(nIndex, nX, nY);
}
//---------------------------------------------------------------------------
// ����:	DrawText
// ����:	����һ���ֶ�
// ����:	nX, nY	:	����
//			lpText	:	�ֶ�
// ����:	void
//---------------------------------------------------------------------------
void KFont::DrawText(int nX, int nY, char* lpText)
{
	if (lpText == NULL)
		return;

	LPBYTE lpByte = (LPBYTE) lpText;
	int		nDx = 0;
	int		nIndex;
	int		nH, nL;
	if (m_CharaSet == CHARASET_GBK)
	{
		while (*lpByte)
		{
			if (*lpByte > 0x80)
			{
				nH = *lpByte++;
				nL = *lpByte++;
				nIndex = CHARASET_GBK_INDEX(nH, nL);
				DrawFont(nIndex, nX + nDx, nY);
				nDx += m_nFontW;
			}
			else
			{				
				nL = *lpByte++;				
				if (nL > 0x20 && nL < 0x7F)
				{
					if (nL < 0x5F)
						nL += 0x20;
					else
						nL += 0x21;
					nIndex = CHARASET_GBK_INDEX(0xa3, nL);
					DrawFont(nIndex, nX + nDx, nY);
				}
				else if (nL == 0x0a)
					nDx = m_nLineW;	//����
				nDx += m_nFontW / 2;
			}
			if (nDx >= m_nLineW)
			{
				nDx = 0;
				nY += m_nLineH;
			}
		}
	}
	else if (m_CharaSet == CHARASET_GB2132)
	{
		while (*lpByte)
		{
			if (*lpByte > 0xa0)
			{
				nH = *lpByte++;
				nL = *lpByte++;
				nIndex = CHARASET_GB2132_INDEX(nH, nL);
				DrawFont(nIndex, nX + nDx, nY);
				nDx += m_nFontW;
			}
			else
			{
				nL = *lpByte++;
				if ((nL >= 0x21) && (nL < 0x80))
				{
					nIndex = CHARASET_GB2132_INDEX(0xa3, 0x80 + nL);
					DrawFont(nIndex, nX + nDx, nY);
				}
				else if (nL == 0x0a)
					nDx = m_nLineW;	//����
				nDx += m_nFontW / 2;
			}
			if (nDx >= m_nLineW)
			{
				nDx = 0;
				nY += m_nLineH;
			}
		}
	}
}

//---------------------------------------------------------------------------
// ����:	DrawTextAlpha
// ����:	��Alpha����һ���ֶ�
// ����:	nX, nY	:	����
//			lpText	:	�ֶ�
// ����:	void
//---------------------------------------------------------------------------
void KFont::DrawTextAlpha(int nX, int nY, int nAlpha,char* lpText)
{
	if (lpText == NULL)
		return;

	LPBYTE lpByte = (LPBYTE) lpText;
	int		nDx = 0;
	int		nIndex;
	int		nH, nL;
	if (m_CharaSet == CHARASET_GBK)
	{
		while (*lpByte)
		{
			if (*lpByte > 0x80)
			{
				nH = *lpByte++;
				nL = *lpByte++;
				nIndex = CHARASET_GBK_INDEX(nH, nL);
				DrawFontAlpha(nIndex, nX + nDx, nY, nAlpha);
				nDx += m_nFontW;
			}
			else
			{				
				nL = *lpByte++;				
				if (nL > 0x20 && nL < 0x7F)
				{
					if (nL < 0x5F)
						nL += 0x20;
					else
						nL += 0x21;
					nIndex = CHARASET_GBK_INDEX(0xa3, nL);
					DrawFontAlpha(nIndex, nX + nDx, nY, nAlpha);
				}
				else if (nL == 0x0a)
					nDx = m_nLineW;	//����
				nDx += m_nFontW / 2;
			}
			if (nDx >= m_nLineW)
			{
				nDx = 0;
				nY += m_nLineH;
			}
		}
	}
	else if (m_CharaSet == CHARASET_GB2132)
	{
		while (*lpByte)
		{
			if (*lpByte > 0xa0)
			{
				nH = *lpByte++;
				nL = *lpByte++;
				nIndex = CHARASET_GB2132_INDEX(nH, nL);
				DrawFontAlpha(nIndex, nX + nDx, nY, nAlpha);
				nDx += m_nFontW;
			}
			else
			{
				nL = *lpByte++;
				if ((nL >= 0x21) && (nL < 0x80))
				{
					nIndex = CHARASET_GB2132_INDEX(0xa3, 0x80 + nL);
					DrawFontAlpha(nIndex, nX + nDx, nY, nAlpha);
				}
				else if (nL == 0x0a)
					nDx = m_nLineW;	//����
				nDx += m_nFontW / 2;
			}
			if (nDx >= m_nLineW)
			{
				nDx = 0;
				nY += m_nLineH;
			}
		}
	}
}

//---------------------------------------------------------------------------
// ����:	SetColor
// ����:	����������ɫ
// ����:	Red		����ɫ����(0 - 255)
//			Green	����ɫ����(0 - 255)
//			Blue	����ɫ����(0 - 255)
// ����:	void
//---------------------------------------------------------------------------
void KFont::SetColor(BYTE Red,BYTE Green,BYTE Blue)
{
	m_dwColor = g_RGB(Red,Green,Blue);
}
//---------------------------------------------------------------------------
// ����:	SetLine
// ����:	�����п���и�
// ����:	nLineW	: �п�
//			nLineH	: �и�
// ����:	void
//---------------------------------------------------------------------------
void KFont::SetLine(int nLineW, int nLineH)
{
	m_nLineW = nLineW;
	m_nLineH = nLineH;
}
//---------------------------------------------------------------------------
// ����:	MakeText
// ����:	���������ı��ļ�
// ����:	FileName	���ļ���
// ����:	void
//---------------------------------------------------------------------------
void KFont::MakeText(LPSTR FileName)
{
	KFile File;

	if (!File.Create(FileName))
		return;

	WORD  zbuf[200];

	if (m_CharaSet ==  CHARASET_GBK)
	{
		zbuf[190] = 0x0a0d;
		// write font text
		for (int i = 0x81; i <= 0xFE; i++)
		{
			for (int j = 0x40; j <= 0x7E; j++)
				zbuf[j - 0x40] = (WORD)( (j << 8) + i);
			for (int j = 0x80; j <= 0xFE; j++)
				zbuf[j - 0x41] = (WORD)( (j << 8) + i);
			File.Write(zbuf, 191 * 2);
		}
	}
	else if (m_CharaSet ==  CHARASET_GB2132)
	{
		zbuf[94] = 0x0a0d;
		// write font text
		for (int i = 0; i < 94; i++)
		{
			for (int j = 0; j < 94; j++)
				zbuf[j] = (WORD)((j + 0xa1) * 256 + i + 0xa1);
			File.Write(zbuf, 95 * 2);
		}
	}
}
//---------------------------------------------------------------------------
// ����:	MakeFont
// ����:	���������ļ�
// ����:	FileName:	BMP�ļ���
//			nWidth:		������
//			nHeight:	����߶�	
// ����:	void
//---------------------------------------------------------------------------
BOOL KFont::MakeFont(LPSTR FileName, int nWidth, int nHeight)
{
	KBmpFile	BmpFile;
	KBitmap		Bitmap;
	BYTE		Buffer[32 * 32];	//������ֲ������Ӵ��������ֿ��ļ���������ƣ��Լ�GetFontLine����ľ��ޡ�
	int			nSize;
	int			nX, nY;

	if (!BmpFile.Load(FileName, &Bitmap) ||
		nWidth < 0 || nWidth > 32 || nHeight < 0 || nHeight > 32)
		return FALSE;

	Clear();

	m_dwSize = 0;
	m_nOffsetSpace = (Bitmap.GetWidth() / nWidth) * (Bitmap.GetHeight() / nHeight);
	m_nFontW = nWidth;
	m_nFontH = nHeight;

	if ((m_pdwOffs = (DWORD*)malloc(sizeof(DWORD) * m_nOffsetSpace)) == NULL ||
		!m_Font.Alloc(Bitmap.GetWidth() * Bitmap.GetHeight() + 2))
	{
		Clear();
		return FALSE;
	}
	memset(m_pdwOffs, 0, sizeof(sizeof(DWORD) * m_nOffsetSpace));

	((LPBYTE)m_Font.GetMemPtr())[0] = 0;
	m_dwSize = 1;

	for (nY = 0; nY < Bitmap.GetHeight(); nY += m_nFontH)
	{
		for (nX = 0; nX < Bitmap.GetWidth(); nX += m_nFontW)
		{
			nSize = GetFontRect(nX, nY, &Bitmap, Buffer);
			if (!AddWord(nSize, Buffer))
				break;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	AddWord
// ����:	���ֿ�������һ����
// ����:	nSize:	�ִ�С
//			lpData:	������
// ����:	void
//---------------------------------------------------------------------------
BOOL KFont::AddWord(int nSize, LPBYTE lpData)
{
	if (m_nCount >= m_nOffsetSpace || m_dwSize + nSize >= m_Font.GetMemLen())
	{
		g_DebugLog("KFont::AddWord() Failed : Size overflow");
		return FALSE;
	}
	if (nSize > m_nFontH)
	{
		LPBYTE lpBuf = (LPBYTE)m_Font.GetMemPtr() + m_dwSize;
		CopyMemory(lpBuf, lpData, nSize);
		m_pdwOffs[m_nCount] = m_dwSize;
		m_dwSize += nSize;
	}
	else
		m_pdwOffs[m_nCount] = 0;
	m_nCount++;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	GetFontRect
// ����:	��λͼȡ������
// ����:	nX,nY	:	����	
//			pBmp	:	λͼָ��
//			pBuf	:	���建��
// ����:	Size of Rect
//---------------------------------------------------------------------------
int KFont::GetFontRect(int nX, int nY, KBitmap* pBmp, PBYTE pBuf)
{
	int nSize = 0;
	int nLen = 0;

	for (int i = 0; i < m_nFontH; i++)
	{
		nLen = GetFontLine(nX, nY, pBmp, pBuf);
		pBuf += nLen;
		nSize += nLen;
		nY++;
	}
	return nSize;
}
//---------------------------------------------------------------------------
// ����:	GetFontLine
// ����:	��λͼȡ������
// ����:	nX,nY	:	����	
//			pBmp	:	λͼָ��
//			pBuf	:	���建��
// ����:	Size of Line
//---------------------------------------------------------------------------
int KFont::GetFontLine(int nX, int nY, KBitmap* pBmp, PBYTE pBuf)
{
	int  nWidth = m_nFontW;
	int  nSize = 0;
	BYTE byPixel = 0;
	BYTE byTemp = 0;
	BYTE nCount = 0;

	while (nWidth > 0)
	{
		nCount = 1;
		byPixel = pBmp->GetPixel(nX, nY);
		while (--nWidth > 0)
		{
			nX++;
			byTemp = pBmp->GetPixel(nX, nY);
			if (byPixel != byTemp)
				break;
			nCount++;
		}
		byPixel &= 0xE0;	//�µĴ��봦��256�ף�ɫ����λͼ���ڵװ��֣�ɫ����ɫ�ɺڵ��ס�
//		byPixel <<= 5;		//�ɵĴ����Ǵ���8�ף�ɫ����λͼ���׵׺��֣�ɫ����ɫ�ɰ׵��ڡ�
		byPixel |= nCount;
		*pBuf = byPixel;
		pBuf++;
		nSize++;
	}
	return nSize;
}
//---------------------------------------------------------------------------

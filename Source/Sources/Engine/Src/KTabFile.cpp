//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTabFile.cpp
// Date:	2002.02.20
// Code:	Huyi(Spe)
// Desc:	Tab File Operation Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KStrBase.h"
#include "KFile.h"
#include "KFilePath.h"
#include "KPakFile.h"
#ifndef _SERVER
#include "KCodec.h"
#endif
#include "KTabFile.h"
#include <string.h>
//---------------------------------------------------------------------------
// ����:	KTabFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KTabFile::KTabFile()
{
	m_Width		= 0;
	m_Height	= 0;
}
//---------------------------------------------------------------------------
// ����:	~KTabFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KTabFile::~KTabFile()
{
	Clear();
}
//---------------------------------------------------------------------------
// ����:	Load
// ����:	����һ��Tab�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KTabFile::Load(LPSTR FileName)
{
	KPakFile	File;
	DWORD		dwSize;
	PVOID		Buffer;

	// check file name
	if (FileName[0] == 0)
		return FALSE;

	if (!File.Open(FileName))
	{
		g_DebugLog("Can't open tab file : %s", FileName);
		return FALSE;
	}

	dwSize = File.Size();

	Buffer = m_Memory.Alloc(dwSize);

	File.Read(Buffer, dwSize);

	CreateTabOffset();

	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	CreateTabOffset
// ����:	������������ָ��ļ���ƫ�Ʊ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTabFile::CreateTabOffset()
{
	int		nWidth, nHeight, nOffset, nSize;
	BYTE	*Buffer;
	TABOFFSET *TabBuffer;

	nWidth	= 1;
	nHeight	= 1;
	nOffset = 0;

	Buffer	= (BYTE *)m_Memory.GetMemPtr();
	nSize	= m_Memory.GetMemLen();

	// ����һ�о����ж�����
	while (*Buffer != 0x0d && *Buffer != 0x0a)
	{
		if (*Buffer == 0x09)
		{
			nWidth++;
		}
		Buffer++;
		nOffset++;
	}
	if (*Buffer == 0x0d && *(Buffer + 1) == 0x0a)
	{
		Buffer += 2;	// 0x0a����		
		nOffset += 2;	// 0x0a����
	}
	else
	{
		Buffer += 1;	// 0x0a����		
		nOffset += 1;	// 0x0a����
	}
	while(nOffset < nSize)
	{
		while (*Buffer != 0x0d && *Buffer != 0x0a)
		{
			Buffer++;
			nOffset++;
			if (nOffset >= nSize)
				break;
		}
		nHeight++;
		if (*Buffer == 0x0d && *(Buffer + 1) == 0x0a)
		{
			Buffer += 2;	// 0x0a����		
			nOffset += 2;	// 0x0a����
		}
		else
		{
			Buffer += 1;	// 0x0a����		
			nOffset += 1;	// 0x0a����
		}
	}
	m_Width		= nWidth;
	m_Height	= nHeight;

	TabBuffer = (TABOFFSET *)m_OffsetTable.Alloc(m_Width * m_Height * sizeof (TABOFFSET));
	Buffer = (BYTE *)m_Memory.GetMemPtr();

	nOffset = 0;
	int nLength;
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			TabBuffer->dwOffset = nOffset;	
			nLength = 0;
			while(*Buffer != 0x09 && *Buffer != 0x0d && *Buffer != 0x0a && nOffset < nSize)
			{
				Buffer++;
				nOffset++;
				nLength++;
			}
			Buffer++;	// 0x09��0x0d��0x0a(linux)����
			nOffset++;
			TabBuffer->dwLength = nLength;
			TabBuffer++;
			if (*(Buffer - 1) == 0x0a || *(Buffer - 1) == 0x0d)	//	�����Ѿ������ˣ���Ȼ����û��nWidth //for linux modified [wxb 2003-7-29]
			{
				for (int k = j+1; k < nWidth; k++)
				{
					TabBuffer->dwOffset = nOffset;
					TabBuffer->dwLength = 0;
					TabBuffer++;					
				}
				break;
			}
		}

		//modified for linux [wxb 2003-7-29]
		if (*(Buffer - 1) == 0x0d && *Buffer == 0x0a)
		{
			Buffer++;				// 0x0a����	
			nOffset++;				// 0x0a����	
		}
	}
}

//---------------------------------------------------------------------------
// ����:	Str2Column
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szColumn
// ����:	�ڼ���
//---------------------------------------------------------------------------
int KTabFile::Str2Col(LPSTR szColumn)
{
	int	nStrLen = g_StrLen(szColumn);
	char	szTemp[4];

	g_StrCpy(szTemp, szColumn);
	g_StrUpper(szTemp);
	if (nStrLen == 1)
	{
		return (szTemp[0] - 'A');
	}
	return ((szTemp[0] - 'A' + 1) * 26 + szTemp[1] - 'A') + 1;
}

//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			nColomn			��
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(int nRow, LPSTR szColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize, BOOL bColumnLab)
{
	int nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return TRUE;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szRow			��	���ؼ��֣�
//			szColomn		��	���ؼ��֣�
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(LPSTR szRow, LPSTR szColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize)
{
	int nRow, nColumn;

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return TRUE;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��		��1��ʼ
//			nColomn			��		��1��ʼ
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(int nRow, int nColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize)
{
	if (GetValue(nRow - 1, nColumn - 1,  lpRString, dwSize))
		return TRUE;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger(int nRow, LPSTR szColumn, int nDefault, int *pnValue, BOOL bColumnLab)
{
	char	Buffer[32];
	int		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return TRUE;
	}
	else
	{
		*pnValue = nDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger(LPSTR szRow, LPSTR szColumn, int nDefault, int *pnValue)
{
	int		nRow, nColumn;
	char	Buffer[32];

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return TRUE;
	}
	else
	{
		*pnValue = nDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��		��1��ʼ
//			nColomn			��		��1��ʼ
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger(int nRow, int nColumn, int nDefault, int *pnValue)
{
	char	Buffer[32];

	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return TRUE;
	}
	else
	{
		*pnValue = nDefault;
		return TRUE;
	}
}
//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetFloat(int nRow, LPSTR szColumn, float fDefault, float *pfValue, BOOL bColumnLab)
{
	char	Buffer[32];
	int		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return TRUE;
	}
	else
	{
		*pfValue = fDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetFloat(LPSTR szRow, LPSTR szColumn, float fDefault, float *pfValue)
{
	int		nRow, nColumn;
	char	Buffer[32];

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return TRUE;
	}
	else
	{
		*pfValue = fDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��		��1��ʼ
//			nColomn			��		��1��ʼ
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetFloat(int nRow, int nColumn, float fDefault, float *pfValue)
{
	char	Buffer[32];
	
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return TRUE;
	}
	else
	{
		*pfValue = fDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// ����:	GetValue
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			nColomn			��
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetValue(int nRow, int nColumn, LPSTR lpRString, DWORD dwSize)
{
	if (nRow >= m_Height || nColumn >= m_Width || nRow < 0 || nColumn < 0)
		return FALSE;

	TABOFFSET	*TempOffset;
	LPSTR		Buffer;

	Buffer = (LPSTR)m_Memory.GetMemPtr();
	TempOffset = (TABOFFSET *)m_OffsetTable.GetMemPtr();
	TempOffset += nRow * m_Width + nColumn;

	ZeroMemory(lpRString, dwSize);
	Buffer += TempOffset->dwOffset;
	if (TempOffset->dwLength == 0)
	{
		return FALSE;
	}
	if (dwSize > TempOffset->dwLength)
	{
		memcpy(lpRString, Buffer, TempOffset->dwLength);
		lpRString[TempOffset->dwLength] = 0;
	}
	else
	{
		memcpy(lpRString, Buffer, dwSize);
		lpRString[dwSize] = 0;
	}

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Clear
// ����:	���TAB�ļ�������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTabFile::Clear()
{
	m_Memory.Free();
	m_OffsetTable.Free();
}
//---------------------------------------------------------------------------
// ����:	FindRow
// ����:	�����йؼ���
// ����:	szRow���йؼ��֣�
// ����:	int
//---------------------------------------------------------------------------
int KTabFile::FindRow(LPSTR szRow)
{
	char	szTemp[128];
	for (int i = 0; i < m_Height; i++)	// ��1��ʼ��������һ�е��ֶ���
	{
		GetValue(i, 0, szTemp, g_StrLen(szRow));
		if (g_StrCmp(szTemp, szRow))
			return i + 1; //�Ķ��˴�Ϊ��һ by Romandou,��������1Ϊ���ı��
	}
	return -1;
}
//---------------------------------------------------------------------------
// ����:	FindColumn
// ����:	�����йؼ���
// ����:	szColumn���йؼ��֣�
// ����:	int
//---------------------------------------------------------------------------
int KTabFile::FindColumn(LPSTR szColumn)
{
	char	szTemp[128];
	for (int i = 0; i < m_Width; i++)	// ��1��ʼ��������һ�е��ֶ���
	{
		GetValue(0, i, szTemp, g_StrLen(szColumn));
		if (g_StrCmp(szTemp, szColumn))
			return i + 1;//�Ķ��˴�Ϊ��һ by Romandou,��������1Ϊ���ı��
	}
	return -1;
}

//---------------------------------------------------------------------------
// ����:	Col2Str
// ����:	������ת���ַ���
// ����:	szColumn
// ����:	�ڼ���
//---------------------------------------------------------------------------
void KTabFile::Col2Str(int nCol, LPSTR szColumn)
{

	if (nCol < 26)
	{
		szColumn[0] = 'A' + nCol;
		szColumn[1]	= 0;
	}
	else
	{
		szColumn[0] = 'A' + (nCol / 26 - 1);
		szColumn[1] = 'A' + nCol % 26;
		szColumn[2] = 0;
	}
}

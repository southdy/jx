//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KIniFile.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Ini File Operation Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KFile.h"
#include "KFilePath.h"
#include "KPakFile.h"
#ifndef _SERVER
#include "KCodec.h"
#endif
#include "KIniFile.h"
#include <string.h>
#include "KSG_StringProcess.h"

//---------------------------------------------------------------------------
typedef struct {
	DWORD		Id;			// �ļ���ʶ = 0x4b434150 ("PACK")
	DWORD		DataLen;	// �ļ�ԭʼ�ĳ���
	DWORD		PackLen;	// �ļ�ѹ���󳤶�
	DWORD		Method;		// ʹ�õ�ѹ���㷨
} TPackHead;
//---------------------------------------------------------------------------
static int  g_nCodec = 0;// codec method = 0,1,2,3,
//---------------------------------------------------------------------------
// ����:	SetCodec
// ����:	set codec method
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void g_SetIniCodec(int nCodec)
{
	g_nCodec = nCodec;
}
//---------------------------------------------------------------------------
// ����:	KIniFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KIniFile::KIniFile()
{
	g_MemZero(&m_Header, sizeof(SECNODE));
}
//---------------------------------------------------------------------------
// ����:	~KIniFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KIniFile::~KIniFile()
{
	Clear();
}
//---------------------------------------------------------------------------
// ����:	Load
// ����:	����һ��INI�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::Load(LPCSTR FileName)
{
	KPakFile	File;
	DWORD		dwSize;
	PVOID		Buffer;
	TPackHead*	pHeader;

	// check file name
	if (FileName[0] == 0)
		return FALSE;

	if (m_Header.pNextNode)
		Clear();

	if (!File.Open((char*)FileName))
	{
		g_DebugLog("Can't open ini file : %s", FileName);
		return FALSE;
	}
#pragma message("KIniFile ��ǰ��֧�� KeyName Ϊ����! by Romandou")

	dwSize = File.Size();

	Buffer = m_MemStack.Push(dwSize + 4);

	File.Read(Buffer, dwSize);

	pHeader = (TPackHead*)Buffer;
	if (pHeader->Id == 0x4b434150) // "PACK"
	{
		File.Close();
		return LoadPack(FileName);
	}

	CreateIniLink(Buffer, dwSize);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	LoadPack
// ����:	��һ��ѹ����INI�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::LoadPack(LPCSTR FileName)
{
	KPakFile	File;
	PVOID		PackBuf;
	PVOID		DataBuf;
#ifndef _SERVER
	KCodec*		pCodec;
	TCodeInfo	CodeInfo;
#endif
	TPackHead	Header;

	// check file name
	if (FileName[0] == 0)
		return FALSE;

	// close open file
	if (m_Header.pNextNode)
		Clear();

	// open the pack ini file
	if (!File.Open((char*)FileName))
		return FALSE;

	// read pack file header
	File.Read(&Header, sizeof(Header));

	// if not a pack file read directly
	if (Header.Id != 0x4b434150) // "PACK"
		return FALSE;

	// init codec
#ifndef _SERVER
	pCodec = NULL;
	g_InitCodec(&pCodec, Header.Method);
	if (pCodec == NULL)
		return FALSE;
#endif
	// allocate buffer for pack data
	PackBuf = m_MemStack.Push(Header.PackLen);

	// allocate bufer for real data
	DataBuf = m_MemStack.Push(Header.DataLen + 2);

	// read pack data for file
	File.Read(PackBuf, Header.PackLen);

	// decompress data
#ifndef _SERVER
	CodeInfo.lpPack = (PBYTE)PackBuf;
	CodeInfo.dwPackLen = Header.PackLen;
	CodeInfo.lpData = (PBYTE)DataBuf;
	CodeInfo.dwDataLen = Header.DataLen;
	pCodec->Decode(&CodeInfo);
	g_FreeCodec(&pCodec, Header.Method);
	if (CodeInfo.dwDataLen != Header.DataLen)
	{
		g_DebugLog("Ini file unpack fail : %s", FileName);
		return FALSE;
	}
#endif
	// check data length

	// create ini link in memory
	CreateIniLink(DataBuf, Header.DataLen);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Save
// ����:	���浱ǰ��INI�ļ�
// ����:	void
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::Save(LPCSTR FileName)
{
	KFile		File;
	LPSTR		DataBuf;
	LPSTR		pBuffer;
	DWORD		dwLen;
	
	if (FileName[0] == 0)
		return FALSE;

	if (g_nCodec)
		return SavePack(FileName);
	
	dwLen = m_MemStack.GetStackSize();

	DataBuf = (LPSTR)m_MemStack.Push(dwLen);
	pBuffer = DataBuf;
	
	SECNODE* SecNode = m_Header.pNextNode;
	KEYNODE* KeyNode = NULL;
	
	// write ini file to memory buffer
	while (SecNode != NULL)
	{
		dwLen = sprintf(pBuffer, "%s\r\n", SecNode->pSection);
		pBuffer += dwLen;
		KeyNode = SecNode->pKeyNode.pNextNode;
		while (KeyNode != NULL)
		{
			dwLen = sprintf(pBuffer, "%s%c%s\r\n", KeyNode->pKey, '=',
				KeyNode->pValue);
			pBuffer += dwLen;
			KeyNode = KeyNode->pNextNode;
		}
		SecNode = SecNode->pNextNode;
		dwLen = sprintf(pBuffer, "\r\n");
		pBuffer += dwLen;
	}

	// get buffer len
	dwLen = pBuffer - DataBuf;
	
	// create ini file
	if (!File.Create((char*)FileName))
		return FALSE;

	// write ini file
	File.Write(DataBuf, dwLen);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	SavePack
// ����:	����һ��ѹ����INI�ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::SavePack(LPCSTR FileName)
{
	KFile		File;
	PVOID		DataBuf;
	PVOID		PackBuf;
#ifndef _SERVER
	KCodec*		pCodec;
	TCodeInfo	CodeInfo;
#endif
	TPackHead	Header;
	LPSTR		pBuffer;
	DWORD		dwLen;
	
	// check file name
	if (FileName[0] == 0)
		return FALSE;

	dwLen = m_MemStack.GetStackSize();
	
	// allocate realdata buffer
	DataBuf = m_MemStack.Push(dwLen);

	// allocate compress buffer
	PackBuf = m_MemStack.Push(dwLen + 256);

	// print ini file to realdata buffer
	pBuffer = (LPSTR)DataBuf;
	SECNODE* SecNode = m_Header.pNextNode;
	KEYNODE* KeyNode = NULL;

	// write ini file to memory buffer
	while (SecNode != NULL)
	{
		dwLen = sprintf(pBuffer, "%s\r\n", SecNode->pSection);
		pBuffer += dwLen;
		KeyNode = SecNode->pKeyNode.pNextNode;
		while (KeyNode != NULL)
		{
			dwLen = sprintf(pBuffer, "%s%c%s\r\n", KeyNode->pKey, '=',
					KeyNode->pValue);
			pBuffer += dwLen;
			KeyNode = KeyNode->pNextNode;
		}
		SecNode = SecNode->pNextNode;
		dwLen = sprintf(pBuffer, "\r\n");
		pBuffer += dwLen;
	}

	// set header
	Header.Id = 0x4b434150; // "PACK"
	Header.Method = g_nCodec; // CODEC_LZO = 2;

	// init codec
#ifndef _SERVER
	pCodec = NULL;
	g_InitCodec(&pCodec, Header.Method);
	if (pCodec == NULL)
		return FALSE;
#endif

	// compress the data buffer
#ifndef _SERVER
	CodeInfo.lpData = (PBYTE)DataBuf;
	CodeInfo.lpPack = (PBYTE)PackBuf;
	CodeInfo.dwDataLen = pBuffer - (LPCSTR)DataBuf;
	CodeInfo.dwPackLen = 0;
	pCodec->Encode(&CodeInfo);
	g_FreeCodec(&pCodec, Header.Method);
	Header.DataLen = CodeInfo.dwDataLen;
	Header.PackLen = CodeInfo.dwPackLen;
#endif

	// create ini file
	if (!File.Create((char*)FileName))
		return FALSE;

	// write file header
	File.Write(&Header, sizeof(Header));

	// write file data
	File.Write(PackBuf, Header.PackLen);

	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Clear
// ����:	���INI�ļ�������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::Clear()
{
	ReleaseIniLink();
}
//---------------------------------------------------------------------------
// ����:	ReadLine
// ����:	��ȡINI�ļ���һ��
// ����:	Buffer	����
//			Szie	����
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::ReadLine(LPSTR Buffer,LONG Size)
{
	if (m_Offset >= Size)
	{
		return FALSE;
	}
	while (Buffer[m_Offset] != 0x0D && Buffer[m_Offset] != 0x0A)
	{
		m_Offset++;
		if (m_Offset >= Size)
			break;
	}
	Buffer[m_Offset] = 0;
	if (Buffer[m_Offset] == 0x0D && Buffer[m_Offset + 1] == 0x0A)
		m_Offset += 2;
	else
		m_Offset += 1;	//linux [wxb 2003-7-29]
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	IsKeyChar
// ����:	�ж�һ���ַ��Ƿ�Ϊ��ĸ
// ����:	ch		Ҫ�жϵ��ַ�	
// ����:	TRUE	����ĸ
//			FALSE	������ĸ
//---------------------------------------------------------------------------
BOOL KIniFile::IsKeyChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return TRUE;
	if ((ch >= 'A') && (ch <= 'Z'))
		return TRUE;
	if ((ch >= 'a') && (ch <= 'z'))
		return TRUE;
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	CreateIniLink
// ����:	����Ini����
// ����:	pBuffer		����
//			nBufLen		����
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::CreateIniLink(LPVOID pBuffer, LONG nBufLen)
{
	LPSTR lpBuffer = (LPSTR)pBuffer;
	LPSTR lpString = NULL;
	LPSTR lpValue  = NULL;
	char  szSection[32] = "[MAIN]";

	m_Offset = 0;
	while (m_Offset < nBufLen)
	{
		lpString = &lpBuffer[m_Offset];
		if (!ReadLine(lpBuffer, nBufLen))
			break;
		if (IsKeyChar(*lpString))
		{
			lpValue = SplitKeyValue(lpString);
			SetKeyValue(szSection, lpString, lpValue);
		}
		if (*lpString == '[')
		{
			g_StrCpyLen(szSection, lpString, sizeof(szSection));
		}
	}
}
//---------------------------------------------------------------------------
// ����:	ReleaseIniLink()
// ����:	�ͷ�Ini����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::ReleaseIniLink()
{
	SECNODE* pThisSec = &m_Header;
	SECNODE* pNextSec = pThisSec->pNextNode;
	KEYNODE* pThisKey = NULL;
	KEYNODE* pNextKey = NULL;

	while (pNextSec != NULL)
	{
		pThisSec = pNextSec->pNextNode;
		pThisKey = &pNextSec->pKeyNode;
		pNextKey = pThisKey->pNextNode;
		while (pNextKey != NULL)
		{
			pThisKey = pNextKey->pNextNode;
			m_MemStack.Free(pNextKey->pKey);
			m_MemStack.Free(pNextKey->pValue);
			m_MemStack.Free(pNextKey);
			pNextKey = pThisKey;
		}
		m_MemStack.Free(pNextSec->pSection);
		m_MemStack.Free(pNextSec);
		pNextSec = pThisSec;
	}
	m_Header.pNextNode = NULL;

	m_MemStack.FreeAllChunks();
}
//---------------------------------------------------------------------------
// ����:	SplitKeyValue
// ����:	�ָ�Key��Value
// ����:	pString		Key=Value
// ����:	ָ��Value
//---------------------------------------------------------------------------
LPSTR KIniFile::SplitKeyValue(LPSTR pString)
{
	LPSTR pValue = pString;
	while (*pValue)
	{
		if (*pValue == '=')
			break;
		pValue++;
	}
	*pValue = 0;
	return pValue + 1;
}
//---------------------------------------------------------------------------
// ����:	String2Id
// ����:	�ַ���ת��32 bits ID
// ����:	pString		�ַ���
// ����:	32 bits ID
//---------------------------------------------------------------------------
DWORD KIniFile::String2Id(LPCSTR pString)
{
	DWORD Id = 0;
	for (int i=0; pString[i]; i++)
	{
		Id = (Id + (i+1) * pString[i]) % 0x8000000b * 0xffffffef;
	}
	return Id ^ 0x12345678;
}
//---------------------------------------------------------------------------
// ����:	IsSectionExist
// ����:	Section�Ƿ����
// ����:	pSection	�ڵ�����
// ����:	BOOL
//---------------------------------------------------------------------------
BOOL KIniFile::IsSectionExist(LPCSTR pSection)
{
	// setup section name
	char szSection[32] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}
	
	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
			return TRUE;
		pSecNode = pSecNode->pNextNode;
	}
	
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	EraseSection
// ����:	���һ��Section������
// ����:	pSection	�ڵ�����
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::EraseSection(LPCSTR pSection)
{
	// setup section name
	char szSection[32] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
		{
			break;
		}
		pSecNode = pSecNode->pNextNode;
	}

	// if no such section found
	if (pSecNode == NULL)
	{
		return;
	}

	// erase all key in the section
	KEYNODE* pThisKey = &pSecNode->pKeyNode;
	KEYNODE* pNextKey = pThisKey->pNextNode;
	while (pNextKey != NULL)
	{
		pThisKey = pNextKey->pNextNode;
		m_MemStack.Free(pNextKey->pKey);
		m_MemStack.Free(pNextKey->pValue);
		m_MemStack.Free(pNextKey);
		pNextKey = pThisKey;
	}
	pSecNode->pKeyNode.pNextNode = NULL;
}

//---------------------------------------------------------------------------
// ����:	EraseKey
// ����:	���Section����һ��Key������
// ����:	pSection	�ڵ�����
// ����:	void
//---------------------------------------------------------------------------
void	KIniFile::EraseKey(LPCSTR lpSection, LPCSTR lpKey)
{
	// setup section name
	char szSection[32] = "[";
	if (lpSection[0] != '[')
	{
		g_StrCat(szSection, lpSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, lpSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
		{
			break;
		}
		pSecNode = pSecNode->pNextNode;
	}

	// if no such section found
	if (pSecNode == NULL)
	{
		return;
	}

	// erase all key in the section
	KEYNODE* pThisKey = &pSecNode->pKeyNode;
	KEYNODE* pNextKey = pThisKey->pNextNode;
	dwID = String2Id(lpKey);
	while (pNextKey != NULL)
	{
		
		if (pNextKey->dwID == dwID)
		{
		pThisKey->pNextNode = pNextKey->pNextNode;
		m_MemStack.Free(pNextKey->pKey);
		m_MemStack.Free(pNextKey->pValue);
		m_MemStack.Free(pNextKey);
		return;
		}
		pThisKey = pNextKey;
		pNextKey = pNextKey->pNextNode;
	}

}



//---------------------------------------------------------------------------
// ����:	SetKeyValue
// ����:	����Key��Value
// ����:	pSection	����
//			pKey		����
//			pValue		��ֵ
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::SetKeyValue(
	LPCSTR	pSection,
	LPCSTR	pKey,
	LPCSTR	pValue)
{
	int		nLen;
	DWORD	dwID;

	// setup section name
	char szSection[32] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pThisSecNode = &m_Header;
	SECNODE* pNextSecNode = pThisSecNode->pNextNode;
	dwID = String2Id(szSection);
	while (pNextSecNode != NULL)
	{
		if (dwID == pNextSecNode->dwID)
		{
			break;
		}
		pThisSecNode = pNextSecNode;
		pNextSecNode = pThisSecNode->pNextNode;
	}

	// if no such section found create a new section
	if (pNextSecNode == NULL)
	{
		nLen = g_StrLen(szSection) + 1;
		pNextSecNode = (SECNODE *)m_MemStack.Push(sizeof(SECNODE));
		pNextSecNode->pSection = (char *)m_MemStack.Push(nLen);
		g_MemCopy(pNextSecNode->pSection, szSection, nLen);
		pNextSecNode->dwID = dwID;
		pNextSecNode->pKeyNode.pNextNode = NULL;
		pNextSecNode->pNextNode = NULL;
		pThisSecNode->pNextNode = pNextSecNode;
	}

	// search for the same key
	KEYNODE* pThisKeyNode = &pNextSecNode->pKeyNode;
	KEYNODE* pNextKeyNode = pThisKeyNode->pNextNode;
	dwID = String2Id(pKey);
	while (pNextKeyNode != NULL)
	{
		if (dwID == pNextKeyNode->dwID)
		{
			break;
		}
		pThisKeyNode = pNextKeyNode;
		pNextKeyNode = pThisKeyNode->pNextNode;
	}

	// if no such key found create a new key
	if (pNextKeyNode == NULL)
	{
		pNextKeyNode = (KEYNODE *)m_MemStack.Push(sizeof(KEYNODE));

		nLen = g_StrLen(pKey) + 1;
		pNextKeyNode->pKey = (char *)m_MemStack.Push(nLen);
		g_MemCopy(pNextKeyNode->pKey, (void*)pKey, nLen);

		nLen = g_StrLen(pValue) + 1;
		pNextKeyNode->pValue = (char *)m_MemStack.Push(nLen);
		g_MemCopy(pNextKeyNode->pValue, (void*)pValue, nLen);

		pNextKeyNode->dwID = dwID;
		pNextKeyNode->pNextNode = NULL;
		pThisKeyNode->pNextNode = pNextKeyNode;
	}
	// replace the old value with new
	else
	{
		m_MemStack.Free(pNextKeyNode->pValue);
		nLen = g_StrLen(pValue) + 1;
		pNextKeyNode->pValue = (char *)m_MemStack.Push(nLen);
		g_MemCopy(pNextKeyNode->pValue, (void*)pValue, nLen);
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	GetKeyValue
// ����:	ȡ��Key��Value
// ����:	pSection	����
//			pKey		����
//			pValue		��ֵ
// ����:	TRUE���ɹ� FALSE��ʧ��
//---------------------------------------------------------------------------
BOOL KIniFile::GetKeyValue(
	LPCSTR	pSection,
	LPCSTR	pKey,
	LPSTR	pValue,
	DWORD	dwSize)
{
	DWORD	dwID;

	// setup section name
	char szSection[32] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
		{
			break;
		}
		pSecNode = pSecNode->pNextNode;
	}

	// if no such section founded
	if (pSecNode == NULL)
	{
		return FALSE;
	}

	// search for the same key
	KEYNODE* pKeyNode = pSecNode->pKeyNode.pNextNode;
	dwID = String2Id(pKey);
	while (pKeyNode != NULL)
	{
		if (dwID == pKeyNode->dwID)
		{
			break;
		}
		pKeyNode = pKeyNode->pNextNode;
	}

	// if no such key found
	if (pKeyNode == NULL)
	{
		return FALSE;
	}

	// copy the value of the key
	g_StrCpyLen(pValue, pKeyNode->pValue, dwSize);
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	GetString
// ����:	��ȡһ���ַ���
// ����:	lpSection		����
//			lpKeyName		����
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	void
//---------------------------------------------------------------------------
BOOL KIniFile::GetString(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	LPCSTR lpDefault,		// points to default string
	LPSTR lpRString,		// points to destination buffer
	DWORD dwSize			// size of string buffer
	)
{
	if (GetKeyValue(lpSection, lpKeyName, lpRString, dwSize))
		return TRUE;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	��ȡһ������
// ����:	lpSection		����
//			lpKeyName		����
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	void
//---------------------------------------------------------------------------
BOOL KIniFile::GetInteger(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	int   nDefault,			// default value
	int   *pnValue          // points to value
	)
{
	char Buffer[32];
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
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
// ����:	GetInteger2
// ����:	��ȡ2������������֮���ö��ŷָ
// ����:	lpSection		����
//			lpKeyName		����
//			pnValue1		����ֵ1
//			pnValue2		����ֵ2
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::GetInteger2(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	int   *pnValue1,		// value 1
	int   *pnValue2			// value 2
	)
{
	char  Buffer[32];
	char* pChar = Buffer;
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		while (*pChar)
		{
			if (',' == *pChar)
			{
				*pChar++ = 0;
				break;
			}
			pChar++;
		}
		*pnValue1 = atoi(Buffer);
		*pnValue2 = atoi(pChar);
	}
}
//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	��ȡ1��������
// ����:	lpSection		����
//			lpKeyName		����
//			fDefault		ȱʡֵ
//			pfValue			����ֵ
// ����:	void
//---------------------------------------------------------------------------
BOOL KIniFile::GetFloat(
	LPCSTR	lpSection,		// points to section name
	LPCSTR	lpKeyName,		// points to key name
	float	fDefault,		// default value
	float	*pfValue        // return value
	)
{
	char Buffer[32];
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
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
// ����:	GetFloat2
// ����:	��ȡ2����������֮���ö��ŷָ
// ����:	lpSection		����
//			lpKeyName		����
//			pfValue1		����ֵ1
//			pfValue2		����ֵ2
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::GetFloat2(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	float *pfValue1,		// value 1
	float *pfValue2			// value 2
	)
{
	char  Buffer[32];
	char* pChar = Buffer;

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		while (*pChar)
		{
			if (',' == *pChar)
			{
				*pChar++ = 0;
				break;
			}
			pChar++;
		}
		*pfValue1 = (float)atof(Buffer);
		*pfValue2 = (float)atof(pChar);
	}
}
//---------------------------------------------------------------------------
// ����:	GetStruct
// ����:	��ȡһ���ṹ
// ����:	lpSection		����
//			lpKeyName		����
//			lpStruct		����
//			dwSize			�����С
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::GetStruct(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	LPVOID	lpStruct,		// pointer to buffer that contains data to add
	DWORD	dwSize			// size, in bytes, of the buffer
	)
{
	char    Buffer[512];
	LPBYTE	lpByte;
	BYTE	ah,al;

	if (!GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
		return;
	lpByte = (LPBYTE)lpStruct;
	int len = g_StrLen(Buffer);
	if (len / 2 != (int)dwSize)
		return;
	for (int i = 0; i < len; i += 2)
	{
		// get byte high
		ah = Buffer[i];
		if ((ah >= 48) && (ah <= 57))
			ah = (BYTE)(ah - 48);
		else
			ah = (BYTE)(ah - 65 + 10);
		// get byte low
		al = Buffer[i+1];
		if ((al >= 48) && (al <= 57))
			al = (BYTE)(al - 48);
		else
			al = (BYTE)(al - 65 + 10);
		// set struct bye
		*lpByte++ = (BYTE)(ah * 16 + al);
	}
}
//---------------------------------------------------------------------------
// ����:	WriteString
// ����:	д��һ���ַ���
// ����:	lpSection		����
//			lpKeyName		����
//			lpString		�ַ���
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::WriteString(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	LPCSTR	lpString		// pointer to string to add
	)
{
	SetKeyValue(lpSection, lpKeyName, lpString);
}
//---------------------------------------------------------------------------
// ����:	WriteInteger
// ����:	д��һ������
// ����:	lpSection		����
//			lpKeyName		����
//			nValue			����
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::WriteInteger(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	int 	nValue			// Integer to write
	)
{
	char Buffer[32];

	sprintf(Buffer, "%d", nValue);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// ����:	WriteInteger2
// ����:	д��2������
// ����:	lpSection		����
//			lpKeyName		����
//			nValue1			����1
//			nValue2			����2
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::WriteInteger2(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	int 	Value1,			// value 1 to write
	int		Value2			// value 2 to write
	)
{
	char Buffer[32];

	sprintf(Buffer, "%d,%d", Value1, Value2);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// ����:	WriteFloat
// ����:	д��1��������
// ����:	lpSection		����
//			lpKeyName		����
//			fValue			������
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::WriteFloat(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	float	fValue			// Integer to write
	)
{
	char Buffer[32];

	sprintf(Buffer,"%f",fValue);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// ����:	WriteFloat2
// ����:	д��2��������
// ����:	lpSection		����
//			lpKeyName		����
//			fValue1			������1
//			fValue2			������2
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::WriteFloat2(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	float 	fValue1,		// value 1 to write
	float	fValue2			// value 2 to write
	)
{
	char Buffer[32];

	sprintf(Buffer, "%f,%f", fValue1, fValue2);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// ����:	WriteFloat2
// ����:	д��һ���ṹ
// ����:	lpSection		����
//			lpKeyName		����
//			lpStruct		�ṹ
//			dwSize			�ṹ��С
// ����:	void
//---------------------------------------------------------------------------
void KIniFile::WriteStruct(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	LPVOID	lpStruct,		// pointer to buffer that contains data to add
	DWORD 	dwSize			// size, in bytes, of the buffer
	)
{
	char    Buffer[512];
	LPSTR	lpBuff = Buffer;
	LPBYTE	lpByte;

	if (dwSize * 2 >= 512)
	{
		return;
	}
	lpByte = (LPBYTE) lpStruct;
	for (DWORD i=0; i<dwSize; i++)
	{
		sprintf(lpBuff,"%02x",*lpByte);
		lpBuff++;
		lpBuff++;
		lpByte++;
	}
	g_StrUpper(Buffer);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL KIniFile::GetNextSection(LPCSTR pSection, LPSTR pNextSection)
{
	
	if (!pSection[0])	//	�������Ϊ""ʱ����ȡ��һ��Section
	{
		if (!m_Header.pNextNode)
			return FALSE;
		else
		{
			g_StrCpy(pNextSection, m_Header.pNextNode->pSection);
			return TRUE;
		}
	}
	else
	{
		char szSection[32] = "[";
		if (pSection[0] != '[')
		{
			g_StrCat(szSection, pSection);
			g_StrCat(szSection, "]");
		}
		else
		{
			g_StrCpy(szSection, pSection);
		}
		//	���Ҳ���������Section
		SECNODE* pThisSecNode = &m_Header;
		SECNODE* pNextSecNode = pThisSecNode->pNextNode;
		DWORD dwID = String2Id(szSection);
		while (pNextSecNode != NULL)
		{
			if (dwID == pNextSecNode->dwID)
			{
				break;
			}
			pThisSecNode = pNextSecNode;
			pNextSecNode = pThisSecNode->pNextNode;
		}
		//	û�����Section������ν��һ��
		if (!pNextSecNode)
		{
			return FALSE;
		}
		else
		{
			//	����������Section�Ѿ������һ����
			if (!pNextSecNode->pNextNode)
			{
				return FALSE;
			}
			else	
			{
				g_StrCpy(pNextSection, pNextSecNode->pNextNode->pSection);
				return TRUE;
			}
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL KIniFile::GetNextKey(LPCSTR pSection, LPCSTR pKey, LPSTR pNextKey)
{
	char szSection[32] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}
	//	���Ҳ���������Section
	SECNODE* pThisSecNode = &m_Header;
	SECNODE* pNextSecNode = pThisSecNode->pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pNextSecNode != NULL)
	{
		if (dwID == pNextSecNode->dwID)
		{
			break;
		}
		pThisSecNode = pNextSecNode;
		pNextSecNode = pThisSecNode->pNextNode;
	}
	//	û�����Section������νȡKey
	if (!pNextSecNode)
	{
		return FALSE;
	}

	KEYNODE* pThisKeyNode = &pNextSecNode->pKeyNode;
	KEYNODE* pNextKeyNode = pThisKeyNode->pNextNode;

	//	Key���Ĳ���Ϊ""�Ļ���ȡ��һ��Key
	if (!pKey[0])
	{
		//	û��Key������ʧ��
		if (!pNextKeyNode->pKey)
		{
			return FALSE;
		}
		else
		{
			g_StrCpy(pNextKey, pNextKeyNode->pKey);
			return TRUE;
		}
	}

	dwID = String2Id(pKey);
	while(pNextKeyNode != NULL)
	{
		if (dwID == pNextKeyNode->dwID)
		{
			break;
		}
		pThisKeyNode = pNextKeyNode;
		pNextKeyNode = pThisKeyNode->pNextNode;
	}
	//	�Ҳ���������Key������ν��һ��
	if (!pNextKeyNode)
	{
		return FALSE;
	}
	//	����������Key�Ѿ������һ����
	if (!pNextKeyNode->pNextNode)
	{
		return FALSE;
	}
	else
	{
		g_StrCpy(pNextKey, pNextKeyNode->pNextNode->pKey);
		return TRUE;
	}
}

int	KIniFile::GetSectionCount()
{
	char Section[200];
	char Section1[200];
	if (!GetNextSection("",Section)) return 0;
	int i  = 1;
	while (1)
	{
		strcpy(Section1, Section);
		if (!GetNextSection(Section1, Section)) return i;
		i++;
	}
	return i;
}

// ������ļ���û��ȡ����Ӧ��Section �� Key�� pRect�����ֵ����
void KIniFile::GetRect(LPCSTR lpSection, LPCSTR lpKeyName, RECT *pRect)
{
	char  Buffer[256];

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
        const char *pcszTemp = Buffer;
        
        pRect->left   = KSG_StringGetInt(&pcszTemp, 0);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        pRect->top    = KSG_StringGetInt(&pcszTemp, 0);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        pRect->right  = KSG_StringGetInt(&pcszTemp, 0);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        pRect->bottom = KSG_StringGetInt(&pcszTemp, 0);
		//sscanf(Buffer, "%d,%d,%d,%d", &(pRect->left), &(pRect->top), &(pRect->right), &(pRect->bottom));
	}
}

void KIniFile::GetFloat3(LPCSTR lpSection, LPCSTR lpKeyName, float* pRect)
{
	char	Buffer[256];

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%f,%f,%f", &pRect[0], &pRect[1], &pRect[2]);
	}
}

void KIniFile::GetFloat4(LPCSTR lpSection, LPCSTR lpKeyName, float* pRect)
{
	char  Buffer[256];

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%f,%f,%f,%f", &pRect[0], &pRect[1], &pRect[2], &pRect[3]);
	}
}

void KIniFile::GetBool(LPCSTR lpSection, LPCSTR lpKeyName, BOOL* pBool)
{
	char  Buffer;

	if (GetKeyValue(lpSection, lpKeyName, &Buffer, 1))
	{
		if ('T' == Buffer)
			*pBool = TRUE;
		else
			*pBool = FALSE;
	}
}

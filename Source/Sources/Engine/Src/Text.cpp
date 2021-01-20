/*****************************************************************************************
//	���ִ�����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-31
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include <string.h>
#ifndef __linux
#include <crtdbg.h>
#else
#define HRESULT	long
#define S_OK 0
#endif

#include "Text.h"
#define		MAX_ENCODED_CTRL_LEN	4	//�������Ʒ������Ʊ�ʶ+���Ʋ����������洢����


IInlinePicEngineSink* g_pIInlinePicSink = NULL;	//Ƕ��ʽͼƬ�Ĵ���ӿ�[wxb 2003-6-19]
extern "C" ENGINE_API HRESULT 
AdviseEngine(IInlinePicEngineSink* pSink)
{
	_ASSERT(NULL == g_pIInlinePicSink);	//һ�㲻��ҽ�����
	g_pIInlinePicSink = pSink;
	return S_OK;
}

extern "C" ENGINE_API HRESULT 
UnAdviseEngine(IInlinePicEngineSink* pSink)
{
	if (pSink == g_pIInlinePicSink)
		g_pIInlinePicSink = NULL;
	return S_OK;
}

//���ĳ���ַ��Ƿ�Ϊ����������׵��ַ�
#define	NUM_CHARACTER_IN_A1	11
unsigned char	s_NotAllowAtLineHeadA1Characters[NUM_CHARACTER_IN_A1] = 
{
	 //��������������������
	0xa2, 0xa3, 0xaf, 0xb1, 0xb3, 0xb5, 0xb7, 0xb9, 0xbb, 0xbd, 0xbf
};
#define	NUM_CHARACTER_IN_A3	10
unsigned char	s_NotAllowAtLineHeadA3Characters[NUM_CHARACTER_IN_A3] = 
{
	//��   ��    ��   ��     ��    ��   ��    ��    ��    ��
	0xa1, 0xa9, 0xac, 0xae, 0xba, 0xbb, 0xbe, 0xbf, 0xdd, 0xfd
};
#define	NUM_CHARACTER_IN_00	7
unsigned char	s_NotAllowAtLineHead00Characters[NUM_CHARACTER_IN_00] =
{
	//!),.:;>?
	0x21, 0x29, 0x2c, /*0x2e,*/ 0x3a, 0x3b, 0x3e, 0x3f
};

//���ĳ���ַ��Ƿ�Ϊ����������׵��ַ������������ַ��򷵻�0�����򷵻��ַ�ռ���ӽ���
extern "C" ENGINE_API
int TIsCharacterNotAlowAtLineHead(const char* pCharacter)
{
	int				i;
	unsigned char	cChar;
	cChar = (unsigned char)(*pCharacter);
	if (cChar == 0xa3)
	{
		cChar = (unsigned char)pCharacter[1];
		if (cChar >= 0xa1 && cChar <= 0xfd)
		{
			for (i = 0; i < NUM_CHARACTER_IN_A3; i++)
				if (s_NotAllowAtLineHeadA3Characters[i] == cChar)
					return 2;
		}
	}
	else if (cChar == 0xa1)
	{
		cChar = (unsigned char)pCharacter[1];
		if (cChar >= 0xa2 && cChar <= 0xbf)
		{
			for (i = 0; i < NUM_CHARACTER_IN_A1; i++)
				if (s_NotAllowAtLineHeadA1Characters[i] == cChar)
					return 2;
		}
	}
	else if (cChar >= 0x21 && cChar <= 0x3f)
	{
		for (i = 0; i < NUM_CHARACTER_IN_00; i++)
			if (s_NotAllowAtLineHead00Characters[i] == cChar)
				return 1;
	}
	return false;
}

//��ȡ���е��¸���ʾ�ַ�
extern "C" ENGINE_API
const char* TGetSecondVisibleCharacterThisLine(const char* pCharacter, int nPos, int nLen)
{
	if (pCharacter && nLen > 0)
	{
		bool bFoundFirst = false;
		while(nPos < nLen)
		{
			unsigned char cChar = (unsigned char)(pCharacter[nPos]);
			if (cChar >= 0x20)
			{
				if (bFoundFirst)
					return (pCharacter + nPos);
				bFoundFirst = true;
				if (cChar > 0x80)
					nPos += 2;
				else
					nPos ++;
				continue;
			}
			if (cChar == KTC_COLOR || cChar == KTC_BORDER_COLOR)
				nPos += 4;
			else if (cChar == KTC_INLINE_PIC)
				nPos += 1 + sizeof(WORD);
			else if (cChar == KTC_COLOR_RESTORE || cChar == KTC_BORDER_RESTORE)
				nPos++;
			break;
		};
	}
	return NULL;
}


//--------------------------------------------------------------------------
//	���ܣ�Ѱ�ҷָ��ַ����ĺ���λ��
//	������pString    --> ��Ҫ�ָ���ַ���
//		��nDesirePos --> �����ָ��λ�ã����ֽ�Ϊ��λ��
//		  bLess      --> ��������ָ��λ�ô���һ���ַ�������м�ʱ�����λ��
//					Ϊǰ�����Ǻ󿿣�0: ���; ��0: ��ǰ����
//	ע�ͣ�Chinese GBK����汾�����ַ������ַ�ȫ����Ϊ��ʾ�ַ��������������ַ�
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int TSplitString(const char* pString, int nDesirePos, int bLess)
{
	register int	nPos = 0;
	if (pString)
	{
		nDesirePos -= 2;
		while(nPos < nDesirePos)
		{
			if ((unsigned char)pString[nPos] > 0x80)
				nPos += 2;
			else if (pString[nPos])
				nPos++;
			else
				break;
		};
		nDesirePos += 2;
		while(nPos < nDesirePos)
		{
			if ((unsigned char)pString[nPos] > 0x80)
			{
				if (bLess && (nPos + 2 > nDesirePos))
					break;
				if (pString[nPos + 1] == 0)
				{//��ֹ���ֵ�BYTE�ĸ���0x80���ַ�
					nPos ++;
					break;
				}				
				nPos += 2;
			}
			else if (pString[nPos])
				nPos ++;
			else
				break;
		}
	}
	return nPos;
}

//--------------------------------------------------------------------------
//	���ܣ��ڱ����ִ�Ѱ�ҷָ��ַ����ĺ���λ��
//	������pString    --> ��Ҫ�ָ���ַ���
//		  nCount	 -->�ַ������ݵĳ��ȣ����ֽ�Ϊ��λ��
//		��nDesirePos --> �����ָ��λ�ã��Ի������洢�ֽ�Ϊ��λ��
//		  bLess      --> ��������ָ��λ�ô���һ�������ַ�������м�ʱ��
//						���λ��Ϊǰ�����Ǻ󿿣�0: ���; ��0: ��ǰ����
//	ע�ͣ�Chinese GBK����汾�����ַ����пɰ����Ѿ�����Ŀ��Ʒ�
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int	TSplitEncodedString(const char* pString, int nCount, int nDesirePos, int bLess)
{
	int	nPos = 0;
	if (pString)
	{
		if (nDesirePos <= nCount)
		{
			register unsigned char cCharacter;
			nDesirePos -= MAX_ENCODED_CTRL_LEN;
			while (nPos < nDesirePos)
			{
				cCharacter = (unsigned char)pString[nPos];
				if (cCharacter > 0x80)
					nPos += 2;
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
					nPos += 4;
				else if (cCharacter == KTC_INLINE_PIC)
					nPos += 3;//1 + sizeof(WORD);
				else
					nPos ++;
			}
			nPos += MAX_ENCODED_CTRL_LEN;
			while(nPos < nDesirePos)
			{
				cCharacter = (unsigned char)pString[nPos];
				if (cCharacter > 0x80)
				{
					if (bLess && (nPos + 2 > nDesirePos))
						break;
					if (nPos + 1 == nCount)
					{//��ֹ���ֵ�BYTE�ĸ���0x80���ַ�
						nPos = nCount;
						break;
					}
					nPos += 2;
				}				
				else if (cCharacter == KTC_INLINE_PIC)
				{
					if (bLess && nPos + 3 > nDesirePos)
						break;
					if (nPos + 3 >= nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 3;//1 + sizeof(WORD);
				}
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				{
					if (bLess && (nPos + 4 > nDesirePos))
						break;
					if (nPos + 4 >= nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 4;
				}
				else
					nPos ++;
			}

		}
		else
		{
			nPos = nCount;
		}
	}
	return nPos;
	
}


//�ַ�����������ַ���Ǳ�ʾ���ڲ�����Ķ�Ӧ�ṹ
#define	KTC_CTRL_CODE_MAX_LEN	7
typedef struct _KCtrlTable
{
	char    szCtrl[KTC_CTRL_CODE_MAX_LEN + 1];	//�ַ�����������ַ���ʾ
	short	nCtrlLen;							//�ַ�����������ַ���ʾ�ĳ���
	short   nCtrl;								//�ַ�����������ڲ�����
}KCtrlTable;

//�������б�
static	const KCtrlTable	s_CtrlTable[] =
{	
	{ "enter",	5, KTC_ENTER		},
	{ "color",	5, KTC_COLOR		},
	{ "bclr",	4, KTC_BORDER_COLOR	},
	{ "pic",	3, KTC_INLINE_PIC	},
};

//���������Ŀ
static	const int	s_nCtrlCount = sizeof(s_CtrlTable)/sizeof(KCtrlTable);

//��ɫ�ṹ
typedef struct _KColorTable
{
	char			Token[8];		//��ɫ���ַ���ʾ
	unsigned char	Red;			//��ɫ��R����
	unsigned char	Green;			//��ɫ��G����
	unsigned char	Blue;			//��ɫ��B����
}KColorTable;

//��ɫ�б�
static	const KColorTable	s_ColorTable[] =
{
	{ "Black",	0,		0,		0	},
	{ "White",	255,	255,	255	},
	{ "Red",	255,	0,		0	},
	{ "Green",	0,		255,	0	},
	{ "Blue",	100,	100,	255 },
	{ "Yellow",	255,	255,	0	},
	{ "Pink",	255,	0,		255	},
	{ "Cyan",	0,		255,	255	},
	{ "Metal",	246,	255,	117	},
	{ "Wood",	0,		255,	120	},
	{ "Water",	78,		124,	255	},
	{ "Fire",	255,	90,		0	},
	{ "Earth",	254,	207,	179	},
	{ "DBlue",	120,	120,	120 },
	{ "HBlue",	100,	100,	255 },
};

//��ɫ����Ŀ
static	const int	s_nColorCount = sizeof(s_ColorTable)/sizeof(KColorTable);

//Ƕ��ͼƬ[wxb 2003-6-19]
#define MAXPICTOKENLEN	16


static bool TEncodeCtrl(char* pBuffer, int nCount, int& nReadPos, int& nShortCount);
static int  TEncodeCtrl(int nCtrl, char* pParamBuffer, int nParamLen, char* pEncodedBuffer);

//--------------------------------------------------------------------------
//	���ܣ����ı����еĿ��Ʊ�ǽ���ת����ȥ����Ч�ַ��������ı����洢����
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int	TEncodeText(char* pBuffer, int nCount)
{
	int nShortCount = 0;
	if (pBuffer)
	{
		unsigned char	cCharacter;
		int		nReadPos = 0;		
		while(nReadPos < nCount)
		{
			cCharacter = pBuffer[nReadPos];
			if (cCharacter > 0x80)
			{
				if (nReadPos + 1 < nCount)	//�ǿ�������������
				{
					pBuffer[nShortCount++] = cCharacter;
					pBuffer[nShortCount++] = pBuffer[nReadPos+ 1];
					nReadPos += 2;
				}
				else	//����0x80�ĵ��ֽ������ַ������˵�
					break;
			}
			else if (cCharacter == 0x0d)	//����
			{
				if (nReadPos + 1 < nCount && pBuffer[nReadPos + 1] == 0x0a)
					nReadPos += 2;
				else
					nReadPos ++;
				pBuffer[nShortCount++] = 0x0a;
			}
			else if (pBuffer[nReadPos] == '<')
				TEncodeCtrl(pBuffer, nCount, nReadPos, nShortCount);
			else if((cCharacter >= 0x20 && cCharacter < 0x7F) ||
				cCharacter == 0x0a || cCharacter == 0x09)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else
				nReadPos++;
		}
		if (nShortCount <nCount)
			pBuffer[nShortCount] = 0;
	}
	return nShortCount;
}

//--------------------------------------------------------------------------
//	���ܣ����ı����еĿ��Ʊ�ǽ���ת����ȥ����Ч�ַ��������ı����洢����
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int TFilterEncodedText(char* pBuffer, int nCount)
{
	int nShortCount = 0;
	if (pBuffer)
	{
		unsigned char	cCharacter;
		int nReadPos = 0;
		while(nReadPos < nCount)
		{
			cCharacter = pBuffer[nReadPos];
			if (cCharacter > 0x80)
			{
				if (nReadPos + 1 < nCount)	//�ǿ�������������
				{
					pBuffer[nShortCount++] = cCharacter;
					pBuffer[nShortCount++] = pBuffer[nReadPos+ 1];
					nReadPos += 2;
				}
				else	//����0x80�ĵ��ֽ������ַ������˵�
				{
					nReadPos++;
					break;
				}
			}
			else if ((cCharacter >= 0x20 && cCharacter < 0x7F) ||
				cCharacter == 0x0a || cCharacter == 0x09)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
				if (nReadPos + 4 < nCount)
				{
					*(int*)(pBuffer + nShortCount) = *(int*)(pBuffer + nReadPos);
					nShortCount += 4;
					nReadPos += 4;
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				if ((int)(nReadPos + 1 + sizeof(WORD)) < nCount)
				{
					memcpy(pBuffer + nShortCount, pBuffer + nReadPos, 1 + sizeof(WORD));;
					nShortCount += 1 + sizeof(WORD);
					nReadPos += 1 + sizeof(WORD);
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else
				nReadPos ++;
		}
		if (nShortCount < nCount)
			pBuffer[nShortCount] = 0;
	}

	return nShortCount;
}

//--------------------------------------------------------------------------
//	���ܣ�����ת�����Ʒ�
//--------------------------------------------------------------------------
static bool TEncodeCtrl(char* pBuffer, int nCount, int& nReadPos, int& nShortCount)
{

	_ASSERT(pBuffer != NULL && nReadPos < nCount && nShortCount < nCount && nShortCount <= nReadPos);
	
	int nCtrlCodeSize, nEndPos, nCtrl;

	//Ѱ�ҽ�������'='��λ�û�'>'��λ��
	int nEqualPos = nReadPos + 1;
	for (; nEqualPos < nCount && nEqualPos <= nReadPos + KTC_CTRL_CODE_MAX_LEN; nEqualPos++)
		if (pBuffer[nEqualPos] == '>' || pBuffer[nEqualPos] == '=')
			break;	

	if(nEqualPos >= nCount || nEqualPos > nReadPos + KTC_CTRL_CODE_MAX_LEN)
		goto NO_MATCHING_CTRL;	//δ�ҵ�'='����'>'��������

	nCtrlCodeSize = nEqualPos - nReadPos - 1;	//����������ŵĳ���

	for (nCtrl = 0; nCtrl < s_nCtrlCount; nCtrl++)
	{
		if (nCtrlCodeSize ==  s_CtrlTable[nCtrl].nCtrlLen &&
			memcmp(pBuffer + nReadPos + 1, s_CtrlTable[nCtrl].szCtrl, nCtrlCodeSize) == 0)
			break;
	}
	if (nCtrl >= s_nCtrlCount)		//δ�ҵ�ƥ��һ�µĿ�������
		goto NO_MATCHING_CTRL;
	nCtrl = s_CtrlTable[nCtrl].nCtrl;

	nEndPos = nEqualPos;
	if (pBuffer[nEqualPos] != '>')
	{
		for(nEndPos++; nEndPos < nCount; nEndPos++)
			if (pBuffer[nEndPos] == '>')
				break;
		if (nEndPos >= nCount)
			goto NO_MATCHING_CTRL;
		nShortCount += TEncodeCtrl(nCtrl, pBuffer + nEqualPos + 1,
			nEndPos - nEqualPos - 1, pBuffer + nShortCount);
	}
	else
		nShortCount += TEncodeCtrl(nCtrl, NULL, 0, pBuffer + nShortCount);
	nReadPos = nEndPos + 1;
	return true;

NO_MATCHING_CTRL:
	pBuffer[nShortCount++] = '<';
	nReadPos++;
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ�ת�����洢������������Ʋ���
//--------------------------------------------------------------------------
static int TEncodeCtrl(int nCtrl, char* pParamBuffer, int nParamLen, char* pEncodedBuffer)
{
	_ASSERT(pEncodedBuffer && (nParamLen == 0 || pParamBuffer != NULL));

	int nEncodedSize = 0;
	static char	Color[8];
	static char	szPic[MAXPICTOKENLEN];

	switch(nCtrl)
	{
	case KTC_ENTER:
		pEncodedBuffer[nEncodedSize ++] = nCtrl;
		break;
	case KTC_INLINE_PIC:	//[wxb 2003-6-19]
		if (nParamLen == 0 && nParamLen >= MAXPICTOKENLEN)
			break;
		{
			memcpy(szPic, pParamBuffer, nParamLen);
			szPic[nParamLen] = 0;
			pEncodedBuffer[nEncodedSize] = KTC_INLINE_PIC;
			*((WORD*)(pEncodedBuffer + nEncodedSize + 1)) = atoi(szPic);
			nEncodedSize += 1 + sizeof(WORD);
		}		
		break;
	case KTC_COLOR:
		if (nParamLen == 0)
		{
			pEncodedBuffer[nEncodedSize ++] = KTC_COLOR_RESTORE;
		}
		else if (nParamLen < 8)
		{
			memcpy(Color, pParamBuffer, nParamLen);
			Color[nParamLen] = 0;
			for (int i = 0; i < s_nColorCount; i++)
			{
#ifndef __linux
				if (stricmp(Color,s_ColorTable[i].Token) == 0)
#else
				if(strcasecmp(Color,s_ColorTable[i].Token) == 0)
#endif
				{
					pEncodedBuffer[nEncodedSize] = KTC_COLOR;
					pEncodedBuffer[nEncodedSize + 1]= s_ColorTable[i].Red;
					pEncodedBuffer[nEncodedSize + 2]= s_ColorTable[i].Green;
					pEncodedBuffer[nEncodedSize + 3]= s_ColorTable[i].Blue;
					nEncodedSize += 4;
					break;
				}
			}
		}		
		break;
	case KTC_BORDER_COLOR:
		if (nParamLen == 0)
		{
			pEncodedBuffer[nEncodedSize ++] = KTC_BORDER_RESTORE;
		}
		else if (nParamLen < 8)
		{
			memcpy(Color, pParamBuffer, nParamLen);
			Color[nParamLen] = 0;
			for (int i = 0; i < s_nColorCount; i++)
			{
#ifndef __linux
				if (stricmp(Color,s_ColorTable[i].Token) == 0)
#else
				if(strcasecmp(Color,s_ColorTable[i].Token) == 0)
#endif
//				if (stricmp(Color,s_ColorTable[i].Token) == 0)
				{
					pEncodedBuffer[nEncodedSize] = KTC_BORDER_COLOR;
					pEncodedBuffer[nEncodedSize + 1]= s_ColorTable[i].Red;
					pEncodedBuffer[nEncodedSize + 2]= s_ColorTable[i].Green;
					pEncodedBuffer[nEncodedSize + 3]= s_ColorTable[i].Blue;
					nEncodedSize += 4;
					break;
				}
			}
		}
		break;
	}
	return nEncodedSize;
}

extern "C" ENGINE_API
int	TRemoveCtrlInEncodedText(char* pBuffer, int nCount)
{
	int nLen = 0;
	nCount = TFilterEncodedText(pBuffer, nCount);
	for (int nPos = 0; nPos < nCount; nPos++)
	{
		char cCharacter = pBuffer[nPos];
		if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			nPos += 3;
		else if (cCharacter == KTC_INLINE_PIC)
			nPos += sizeof(WORD);
		else if (cCharacter != KTC_COLOR_RESTORE && cCharacter != KTC_BORDER_RESTORE)
		{
			pBuffer[nLen] = cCharacter;
			nLen ++;
		}
	}
	return nLen;
}

//��ȡ�����ı�������������п�
//������pBuffer			�ı�������
//		nCount			�ı����ݵĳ���
//		nWrapCharaNum	����ÿ�в��������ַ���Ŀ
//		nMaxLineLen		���ڻ�ȡ�ı���ʵ������п��ַ���Ŀ��
//		nFontSize		��������Ĵ�С [wxb 2003-6-19]
//		nSkipLine		����ǰ������е�����
//		nNumLineLimit	�����ı���������������������Ŀ֮������ݱ����ԡ������ֵС�ڵ���0���ʾ�޴����ơ�
//���أ��ı�������
//extern "C" ENGINE_API
//int	TGetEncodedTextLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize, int nSkipLine = 0, int nNumLineLimit = 0)
extern "C" ENGINE_API
int	TGetEncodedTextLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize, int nSkipLine, int nNumLineLimit,
							 BOOL bPicSingleLine/* = FALSE*/)
{
	//��һ����ֵ��ó��� [wxb 2003-6-20]
	_ASSERT(nFontSize >= 4 && nFontSize < 64);
	nFontSize = max(4, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	nMaxLineLen = 0;
	if (pBuffer == 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);

	float fNumChars = 0;
	int nNumLine = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;
	if (nSkipLine < 0)
		nSkipLine = 0;
	if (nNumLineLimit <= 0)
		nNumLineLimit = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	int  nExtraLineForInlinePic = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)	//�����������ַ�
		{
			nPos += 2;
			if (fNumChars + 2 < nWrapCharaNum)
				fNumChars += 2;
			else if (fNumChars + 2 == nWrapCharaNum || fNumChars == 0)
			{
				bNextLine = true;
				fNumChars += 2;
			}
			else
			{
				bNextLine = true;
				fNumNextLineChar = 2;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)//��ɫ����
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			//Ƕ��ʽͼƬ���� [wxb 2003-6-19]
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
			if (nSkipLine > 0)
			{
				nSkipLine -= 1 + nExtraLineForInlinePic;

				//����ͼƬռ���е���� [wxb 2003-6-19]
				if (nSkipLine < 0)
				{
					if (fMaxLineLen < fNumChars)
						fMaxLineLen = fNumChars;
					nNumLine += (-nSkipLine);
					if (nNumLine >= nNumLineLimit)
						break;
				}
			}
			else
			{
				if (fMaxLineLen < fNumChars)
					fMaxLineLen = fNumChars;
				nNumLine += 1 + nExtraLineForInlinePic;
				if (nNumLine >= nNumLineLimit)
					break;
			}
			nExtraLineForInlinePic = 0;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}
	if (nNumLine < nNumLineLimit && fNumChars && nSkipLine == 0)
	{
		if (fMaxLineLen < fNumChars)
			fMaxLineLen = fNumChars;
		nNumLine += 1 + nExtraLineForInlinePic;
	}

	nMaxLineLen = (int)(fMaxLineLen + (float)0.9999);	//��1
	return nNumLine;
}

//���ָ���еĿ�ʼλ��
int TGetEncodeStringLineHeadPos(const char* pBuffer, int nCount, int nLine, int nWrapCharaNum, int nFontSize, BOOL bPicSingleLine)
{
	//��һ����ֵ��ó��� [wxb 2003-6-20]
	_ASSERT(nFontSize > 1 && nFontSize < 64);
	nFontSize = max(1, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	if (pBuffer == 0 || nLine <= 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);

	float fNumChars = 0;
	int  nExtraLineForInlinePic = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)	//�����������ַ�
		{
			nPos += 2;
			if (fNumChars + 2 < nWrapCharaNum)
				fNumChars += 2;
			else if (fNumChars + 2 == nWrapCharaNum || fNumChars == 0)
			{
				bNextLine = true;
				fNumChars += 2;
			}
			else
			{
				bNextLine = true;
				fNumNextLineChar = 2;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)//��ɫ����
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			//Ƕ��ʽͼƬ���� [wxb 2003-6-19]
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
//			if (nSkipLine > 0)
//			{
//				nSkipLine -= 1 + nExtraLineForInlinePic;
//
//				//����ͼƬռ���е���� [wxb 2003-6-19]
//				if (nSkipLine < 0)
//				{
//					if (fMaxLineLen < fNumChars)
//						fMaxLineLen = fNumChars;
//					nNumLine += (-nSkipLine);
//					if (nNumLine >= nNumLineLimit)
//						break;
//				}
//			}
			if ((--nLine) == 0)
				break;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}

	return nPos;
}

extern "C" ENGINE_API
//���ԭ(�������Ʒ�)�ַ������ȣ�������β���������޶��ĳ��ȣ���ض���������..��׺
const char* TGetLimitLenEncodedString(const char* pOrigString, int nOrigLen, int nFontSize,
	int nWrapCharaNum, char* pLimitLenString, int& nShortLen, int nLineLimit, int bPicPackInSingleLine/*=false*/)
{
	if (pOrigString == NULL || pLimitLenString == NULL ||
		nOrigLen <= 0 || nShortLen < 2 || nLineLimit < 1 || nWrapCharaNum < 2)
	{
		nShortLen = 0;
		return NULL;
	}

	int nPreLineEndPos = 0, nFinalLineEndPos;
	if (nLineLimit > 1)	//����ǰ�漸��
	{
		nPreLineEndPos = TGetEncodeStringLineHeadPos(pOrigString, nOrigLen, nLineLimit - 1, nWrapCharaNum, nFontSize, bPicPackInSingleLine);
		if (nPreLineEndPos > nShortLen)
		{
			nShortLen = TSplitEncodedString(pOrigString, nOrigLen, nShortLen - 2, true);
			memcpy(pLimitLenString, pOrigString, nShortLen);
			pLimitLenString[nShortLen] = '.';
			pLimitLenString[nShortLen + 1] = '.';
			nShortLen += 2;
			return pLimitLenString;
		}
	}

	if (nPreLineEndPos < nOrigLen)
	{
		nFinalLineEndPos = TGetEncodeStringLineHeadPos(pOrigString + nPreLineEndPos,
			nOrigLen - nPreLineEndPos, 1, nWrapCharaNum, nFontSize, bPicPackInSingleLine) + nPreLineEndPos;
	}
	else
		nFinalLineEndPos = nOrigLen;

	if (nFinalLineEndPos >= nOrigLen)
	{
		nShortLen = TSplitEncodedString(pOrigString, nOrigLen, nShortLen, true);
		memcpy(pLimitLenString, pOrigString, nShortLen);
		return pLimitLenString;
	}

	int nDesireLen = (nFinalLineEndPos <= nShortLen) ? nFinalLineEndPos - 2 : nShortLen - 2;

	const char* pFinalLineHead = pOrigString + nPreLineEndPos;
	int nRemainCount = nOrigLen - nPreLineEndPos;
	nDesireLen -= nPreLineEndPos;
	while(true)
	{
		nShortLen = TSplitEncodedString(pFinalLineHead, nRemainCount, nDesireLen, true);
		int nMaxLineLen;
		TGetEncodedTextLineCount(pFinalLineHead, nShortLen, 0, nMaxLineLen, nFontSize, 0, 1, FALSE);
		if (nMaxLineLen <= nWrapCharaNum - 2)
			break;
		nDesireLen --;
	}
	nShortLen += nPreLineEndPos;

   	memcpy(pLimitLenString, pOrigString, nShortLen);
	pLimitLenString[nShortLen] = '.';
	pLimitLenString[nShortLen + 1] = '.';
	nShortLen += 2;
	return pLimitLenString;
}

//--------------------------------------------------------------------------
//	���ܣ����ԭ�ַ������ȣ�������β���������޶��ĳ��ȣ���ض���������..��׺
//	������pOrigString     --> ԭ�ַ�����Ҫ��Ϊ��ָ��
//		��nOrigLen		  --> ԭ�ַ������ȣ���������β����
//		  pLimitLenString --> ���ԭ�ַ��������޳��������洢�ض̺���ַ����Ļ�������Ҫ��Ϊ��ָ��
//		  nLimitLen		  --> �޶����ȣ���ֵҪ����ڵ���3
//	���أ���ԭ�ַ����������޳����򷵻�ԭ������ָ�룬���򷵻������洢�ض̺���ַ����Ļ�������ָ��
//	ע�ͣ�Chinese GBK����汾�����ַ������ַ�ȫ����Ϊ��ʾ�ַ��������������ַ�
//--------------------------------------------------------------------------
extern "C" ENGINE_API
const char* TGetLimitLenString(const char* pOrigString, int nOrigLen, char* pLimitLenString, int nLimitLen)
{
	if (pOrigString && pLimitLenString && nLimitLen > 0)
	{
		if (nOrigLen < 0)
			nOrigLen = strlen(pOrigString);
		if (nOrigLen < nLimitLen)
			return pOrigString;
		if (nLimitLen > 2)
		{
			nOrigLen = TSplitString(pOrigString, nLimitLen - 3, true);
			memcpy(pLimitLenString, pOrigString, nOrigLen);
			pLimitLenString[nOrigLen] = '.';
			pLimitLenString[nOrigLen + 1] = '.';
			pLimitLenString[nOrigLen + 2] = 0;
		}
		else if (nLimitLen == 2)
		{
			pLimitLenString[0] = '.';
			pLimitLenString[1] = 0;
		}
		return ((nLimitLen >= 2) ? pLimitLenString : NULL);
	}
	return NULL;
}


//���Ѿ�������ı�����ָ��λ�ÿ�ʼ����ָ���Ŀ��Ʒ��ŵ�λ�ã�����-1��ʾδ�ҵ�
extern "C" ENGINE_API
int	TFindSpecialCtrlInEncodedText(const char* pBuffer, int nCount, int nStartPos, char cControl)
{
	int nFindPos = -1;
	if (pBuffer)
	{
		while(nStartPos < nCount)
		{
			unsigned char cCharacter = pBuffer[nStartPos];
			if ((unsigned char)cControl == cCharacter)
			{
				nFindPos = nStartPos;
				break;
			}
			if (cCharacter > 0x80)	//��������������
				nStartPos += 2;
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				nStartPos += 4;
			else if (cCharacter == KTC_INLINE_PIC)
				nStartPos += 3;
			else
				nStartPos ++;
		}
	}
	return nFindPos;
}

//���Ѿ�������ı���ȥ��ָ�����͵Ŀ��Ʒ�
extern "C" ENGINE_API
int	TClearSpecialCtrlInEncodedText(char* pBuffer, int nCount, char cControl)
{
	int nFinalLen = 0;
	int nReadPos = 0;
	if (pBuffer)
	{
		if ((unsigned char)(cControl) <= 0x80)
		{
			int nMatchLen = 1;
			if (cControl == KTC_COLOR || cControl == KTC_BORDER_COLOR)
				nMatchLen = 4;
			else if (cControl == KTC_INLINE_PIC)
				nMatchLen = 3;
			while(nReadPos < nCount)
			{
				unsigned char cCharacter = pBuffer[nReadPos];
				if ((unsigned char)cControl == cCharacter)
				{
					nReadPos += nMatchLen;
				}
				else if (cCharacter > 0x80)
				{
					short sTemp = *(short*)(pBuffer + nReadPos);
					*(short*)(pBuffer + nFinalLen) = sTemp;
					nReadPos += 2;
					nFinalLen += 2;
				}
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				{
					int nTemp = *(int*)(pBuffer + nReadPos);
					*(int*)(pBuffer + nFinalLen) = nTemp;
					nFinalLen += 4;
					nReadPos += 4;
				}
				else if (cCharacter == KTC_INLINE_PIC)
				{
					memmove((pBuffer + nFinalLen), (pBuffer + nReadPos), 3);
					nFinalLen += 3;
					nReadPos += 3;
				}
				else
				{
					pBuffer[nFinalLen++] = pBuffer[nReadPos++];
				}
			}
		}
	}
	return nFinalLen;
}

//���Ѿ�������ı���ָ��������ȵ��ڻ�������λ��
extern "C" ENGINE_API
int TGetEncodedTextOutputLenPos(const char* pBuffer, int nCount, int& nLen, bool bLess, int nFontSize)
{
	int nIndex = 0, nLenTemp = 0;

	_ASSERT(nFontSize >= 4);
	nFontSize = max(4, nFontSize);

    if (pBuffer)
	{
		int nWidth, nHeight;
		int nByteCount = 0, nCurCharLen = 0;
	    unsigned char cCharacter        = 0;

		while(nLenTemp < nLen)
		{
			cCharacter = pBuffer[nIndex];
			//�������ǰԪ�ص���ռ�ֽ���nByteCount������ʾ��������ռ���nCurCharLen
			if (cCharacter > 0x80)	//��������������
			{
                nByteCount  = 2;
				nCurCharLen = 2;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
			    nByteCount  = 4;
				nCurCharLen = 0;
			}
			else if (cCharacter == KTC_COLOR_RESTORE && cCharacter == KTC_BORDER_RESTORE)
			{
				nByteCount  = 1;
				nCurCharLen = 0;
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				nByteCount  = 3;
				if(SUCCEEDED(g_pIInlinePicSink->GetPicSize(
					*((unsigned short *)(pBuffer + nIndex + 1)), nWidth, nHeight)))
				{
					nCurCharLen = ((nWidth * 2 + nFontSize - 1) /  nFontSize);
				}
				else
					nCurCharLen = 0;
			}
			else
			{
				nByteCount  = 1;
				nCurCharLen = 1;
			}

			//�����������������ֹͣ��
			if(nIndex + nByteCount > nCount)
				break;
			//�����Ȼ�û����Ҫ����
			if(nLenTemp + nCurCharLen < nLen)
			{
				nLenTemp += nCurCharLen;
		        nIndex   += nByteCount;
			}
			//�����ȵ���Ҫ������
			else if(nLenTemp + nCurCharLen == nLen)
			{
				nLenTemp += nCurCharLen;
				nIndex   += nByteCount;
				break;
			}
			//������ǳ�����
			else
			{
				nLenTemp = bLess ? nLenTemp : (nLenTemp + nCurCharLen);
				nIndex   = bLess ? nIndex   : (nIndex + nByteCount);
				break;
			}
		}
	}
	nLen = nLenTemp;
	return nIndex;
}

//���Ѿ�������ı���ָ����ǰ�λ������п��Ʒ����Ժ�����������Ч��Ӱ��
extern "C" ENGINE_API
int TGetEncodedTextEffectCtrls(const char* pBuffer, int nSkipCount, KTP_CTRL& Ctrl0, KTP_CTRL& Ctrl1)
{
	int nIndex = 0;
	Ctrl0.cCtrl = Ctrl1.cCtrl = KTC_INVALID;
	if (pBuffer)
	{
		KTP_CTRL PreCtrl0, PreCtrl1;
		PreCtrl0.cCtrl = PreCtrl1.cCtrl = KTC_INVALID;

		while(nIndex < nSkipCount)
		{
			unsigned char cCharacter = pBuffer[nIndex];
			if (cCharacter == KTC_COLOR)
			{
				PreCtrl0  =  Ctrl0;
				*(int*)(&Ctrl0) = *(int*)(pBuffer + nIndex);
				nIndex += 4;				
			}
			else if (cCharacter == KTC_BORDER_COLOR)
			{
				PreCtrl1  =  Ctrl1;
				*(int*)(&Ctrl1) = *(int*)(pBuffer + nIndex);
				nIndex += 4;
			}
			else if(cCharacter == KTC_COLOR_RESTORE)
			{
				Ctrl0 = PreCtrl0;
				nIndex ++;
			}
			else if(cCharacter == KTC_BORDER_RESTORE)
			{
				Ctrl1 = PreCtrl1;
				nIndex ++;
			}
			else
				nIndex ++;
		}
	}
	return nIndex;
}
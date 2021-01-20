/*******************************************************************************
// FileName			:	KFont2.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2002-9-5
// FileDescription	:	������
// Revision Count	:	
*******************************************************************************/
#include "KFont2.h"
#include "string.h"
#include "../KRepresentUnit.h"
#include "../../../Engine/Src/KEngine.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Engine/Src/KColors.h"

/*!*****************************************************************************
// Purpose		:  ���캯��
*****************************************************************************/
KFont2::KFont2()
{
	m_nFontWidth	= 16;
	m_nFontHalfWidth[0] = m_nFontHalfWidth[1] = 8;
	m_nFontHeight	= 16;
	m_nLastPosH		= 0;
	m_nLastPosV		= 0;
	m_pDevice		= 0;
	m_bLoaded		= false;
	m_nRefCount		= 1;
//	m_nDrawBorderWithDeffColor = true;
	m_nBorderColor = 0;
	m_nOutputWidth = m_nFontWidth;
	m_nOutputHeight= m_nFontHeight;
}

/*!*****************************************************************************
// Purpose		:  ��������
*****************************************************************************/
KFont2::~KFont2()
{
	Terminate();
}

/*!*****************************************************************************
// Function		: KFont2::Init
// Purpose		: ��ʼ��
// Return		: bool �Ƿ�ɹ�
// Argumant		: void* pDrawDevice -> �ӿڵ�ʵ����ָ��
*****************************************************************************/
bool KFont2::Init(void* pDrawDevice)
{
	Terminate();
	if ((m_pDevice = pDrawDevice) == 0)
		return false;
	return true;
}

/*!*****************************************************************************
// Function		: KFont2::Terminate
// Purpose		: �������������
*****************************************************************************/
void KFont2::Terminate()
{
	m_pDevice = 0;
	m_bLoaded = false;
	m_Resources.Terminate();
}

/*!*****************************************************************************
// Function		: KFont2::Load
// Purpose		: �����ֿ�
// Return		: bool �Ƿ�ɹ�
// Argumant		: cosnt char *pszFontFile �ֿ��ļ���
*****************************************************************************/
bool KFont2::Load(const char* pszFontFile)
{
	m_Resources.Terminate();
	m_bLoaded = false;
	if (m_pDevice)
	{
		//��ʼ�������ֿ���Դ
		if (m_Resources.Load(pszFontFile))
		{
			m_Resources.GetInfo(m_nFontWidth, m_nFontHeight);
			m_bLoaded = true;
			return true;
		}
	}
	return false;
}

/*!*****************************************************************************
// Function		: KFont2::TextOut
// Purpose		: ��ʾ�ַ���
// Argumant		: cosnt char *pszText �ַ���
// Argumant		: int nCount  �ַ����ĳ���(BYTE)��Ĭ��ֵΪ-1����ʾ���ַ�������'\0'��β��
// Argumant		: int nX	  �ַ�����ʾ�������X���������ֵΪKF_FOLLOW��
//							  ����ַ����������ϴ��ַ��������λ��֮��
// Argumant		: int nY      �ַ�����ʾ�������Y, �������ֵΪKF_FOLLOW��
//							  ���ַ�����ǰһ������ַ�����ͬһ�е�λ�á�
// Argumant		: uint nColor �ַ�����ʾ��ɫ��Ĭ��Ϊ��ɫ����32bit����ARGB�ĸ�
//							  ʽ��ʾ��ɫ��ÿ������8bit��
// Argumant     : nLineWidth  �Զ����е��п����ƣ������ֵС��һ��ȫ���ַ����
//							  �����Զ����д���Ĭ��ֵΪ0���Ȳ����Զ����д���
*****************************************************************************/
void KFont2::OutputText(const char* pszText, int nCount/*= KF_ZERO_END*/,
					int nX/*=KF_FOLLOW*/, int nY/*=KF_FOLLOW*/,
					unsigned int nColor/*=0xff000000*/, int nLineWidth/*=0*/)
{
	if (!pszText || !m_bLoaded)
		return;

	unsigned char*	lpByte = (unsigned char*) pszText;
	int				nL, h;
	int				nPos = 0;
	int				nHalfIndex = 0;

	if (nCount == KRF_ZERO_END)
		nCount = strlen(pszText);
	if (nX == KRF_FOLLOW)
		nX = m_nLastPosH;
	if (nY == KRF_FOLLOW)
		nY = m_nLastPosV;

	h = 0;

	KRColor		c;
	c.Color_dw = nColor;
	int			sColor = g_RGB(c.Color_b.r, c.Color_b.g, c.Color_b.b);

	if (nLineWidth < m_nOutputWidth + m_nOutputWidth)
		nLineWidth = 0; //�����Զ����д���

	while (nPos < nCount)
	{
		//*********�ַ����ж��봦��*********
		if (lpByte[nPos] > 0x80 && nPos + 1 < nCount)
		{
			DrawCharacter(nX + h, nY, lpByte[nPos], lpByte[nPos + 1], sColor);
			nPos += 2;
			h += m_nOutputWidth;
		}
		else	//�����ַ�
		{
			nL = lpByte[nPos++];
			if (nL > 0x20 && nL < 0x5F)
				DrawCharacter(nX + h, nY, 0xa3, (0x20 + nL), sColor);
			else if (nL >= 0x5F && nL < 0x7F)
				DrawCharacter(nX + h, nY, 0xa3, (0x21 + nL), sColor);
			
			if (nL != 0x0a)
			{
				h += m_nFontHalfWidth[nHalfIndex];
				nHalfIndex ^= 1;
			}
			else
			{
				h = 0;
				nY += m_nOutputHeight;
			}
		}
		if (nLineWidth)
		{
			if (h + m_nOutputWidth > nLineWidth)
			{
				if (nPos < nCount && lpByte[nPos] == 0x0a)	//�����Զ�����λ�ú�ǡ�ý���һ�����з���
					nPos++;
				h = 0;
				nY += m_nOutputHeight;
				nHalfIndex = 0;
			}
			else if (h && nPos + 3 < nCount &&
				h + m_nOutputWidth + m_nFontHalfWidth[nHalfIndex] >= nLineWidth)
			{
				const char* pNext = TGetSecondVisibleCharacterThisLine((const char*)lpByte, nPos, nCount);
				if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				{
					h = 0;
					nY += m_nOutputHeight;
					nHalfIndex = 0;
				}
			}
		}
	}
	m_nLastPosH = nX + h;
	m_nLastPosV = nY;
}

//���û���ʱ�ַ���Ե����ɫ����alphaΪ0��ʾ�ַ���Ե����������
void KFont2::SetBorderColor(unsigned int uColor)
{
//	if (uColor & 0xff000000)
//	{		
		KRColor		c;
		c.Color_dw = uColor;
		m_nBorderColor = g_RGB(c.Color_b.r, c.Color_b.g, c.Color_b.b);
//		m_nDrawBorderWithDeffColor = true;
//	}
//	else
//	{
//		m_nDrawBorderWithDeffColor = false;
//	}
}

/*!*****************************************************************************
// Function		: KFont2::TextOut
// Purpose		: ���Ƶ����ַ�
// Argumant		: unsigned char cFirst �ַ������ǰ���ֽ�
// Argumant		: unsigned char cNext  �ַ�����ĺ���ֽ�
*****************************************************************************/
void KFont2::DrawCharacter(int x, int y, unsigned char cFirst, unsigned char cNext, int nColor) const
{
	if (m_pDevice && m_bLoaded)
	{
		//ȡ���ַ����ֿ����������ָ��
		unsigned char* pCharacterData = m_Resources.GetCharacterData(cFirst, cNext);		
		if (pCharacterData)
		{
//			if (m_nDrawBorderWithDeffColor == false)
//				((KCanvas*)m_pDevice)->DrawFont(x, y, m_nFontWidth, m_nFontHeight, nColor, 31, pCharacterData);
//			else
				((KCanvas*)m_pDevice)->DrawFontWithBorder(x, y, m_nFontWidth, m_nFontHeight, nColor, 31, pCharacterData, m_nBorderColor);
		}
	}
}

void KFont2::GetFontSize(int* pWidth, int* pHeight)
{
	if (pWidth)
		*pWidth = m_nFontWidth;
	if (pHeight)
		*pHeight = m_nFontHeight;
}

//�����ַ���������
void KFont2::SetOutputSize(int nOutputWith, int nOutputHeight)
{
	if (nOutputWith > 0)
		m_nOutputWidth = nOutputWith;
	else
		m_nOutputWidth = m_nFontWidth;
	if (nOutputHeight > 0)
		m_nOutputHeight = nOutputHeight;
	else
		m_nOutputHeight = m_nFontHeight;
	m_nFontHalfWidth[0] = m_nOutputWidth / 2;
	m_nFontHalfWidth[1] = (m_nOutputWidth + 1) / 2;
}

//��¡һ���ӿڶ���ָ��
iFont* KFont2::Clone()
{
	if (m_nRefCount < 0xffff)
	{
		m_nRefCount ++;
		return ((iFont*)this);
	}
	return NULL;
}

//�ͷŽӿڶ���
void KFont2::Release()
{
	if ((--m_nRefCount) == 0)
		delete (this);
}

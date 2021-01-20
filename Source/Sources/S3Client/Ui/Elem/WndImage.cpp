/*****************************************************************************************
//	���洰����ϵ�ṹ--ͼ�δ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "WndImage.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KWndImage::KWndImage()
{
	IR_InitUiImageRef(m_Image);
}

void KWndImage::Clone(KWndImage* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_Image	= m_Image;
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ������
//--------------------------------------------------------------------------
int KWndImage::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		int		nValue = 0;
		pIniFile->GetInteger(pSection, "Trans", 0, &nValue);
		if (nValue)
			m_Style |= WNDIMG_ES_EXCLUDE_TRANS;
		else
			m_Style &= ~WNDIMG_ES_EXCLUDE_TRANS;
		m_Image.nFlipTime = 0;
		m_Image.nInterval = 0;
		m_Image.nNumFrames = 0;
		pIniFile->GetInteger(pSection, "ImgType", 0, &nValue);
		if (nValue == 1)
		{
			m_Image.nType = ISI_T_BITMAP16;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
			m_Style &= ~WNDIMG_ES_EXCLUDE_TRANS;
		}
		else
		{
			m_Image.nType = ISI_T_SPR;
			m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			m_Image.Color.Color_b.a = 255;
		}
		m_Image.uImage = 0;
		m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
		pIniFile->GetString(pSection, "Image", "" , m_Image.szImage, sizeof(m_Image.szImage));
		if (pIniFile->GetInteger(pSection, "Frame", -1, &nValue) && nValue != -1)
			m_Image.nFrame = nValue;

		if ((m_Width == 0 || m_Height == 0) && g_pRepresentShell)
		{
			KImageParam	Param = { 0 };
			if (g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, m_Image.nType))
			{
				if (m_Width == 0)
					m_Width = Param.nWidth;
				if (m_Height == 0)
					m_Height = Param.nHeight;
			}
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ��ж�һ�����Ƿ��ڴ��ڷ�Χ��,������Ǿ�������
//--------------------------------------------------------------------------
int KWndImage::PtInWindow(int x, int y)
{
	int	bIn = KWndWindow::PtInWindow(x, y);
	
	if ((bIn &&
			(m_Style & WND_S_SIZE_WITH_ALL_CHILD) == 0) || 
		((m_Style & WND_S_VISIBLE) &&
			bIn == false &&
			(m_Style & WND_S_SIZE_WITH_ALL_CHILD)))
	{
		if ((m_Style & WNDIMG_ES_EXCLUDE_TRANS) && g_pRepresentShell)
				bIn =  g_pRepresentShell->GetImagePixelAlpha(m_Image.szImage, m_Image.nFrame, x - m_nAbsoluteLeft, y - m_nAbsoluteTop, m_Image.nType);
	}
	return bIn;
}

//--------------------------------------------------------------------------
//	���ܣ�����ͼ��֡
//--------------------------------------------------------------------------
void KWndImage::SetFrame(int nFrame)
{
	m_Image.nFrame = nFrame;
}

void KWndImage::SetImage(short nType, const char* pszImgName, bool bAdjustWndSize)
{
	m_Image.nType = nType;
	strncpy(m_Image.szImage, pszImgName, sizeof(m_Image.szImage));
	m_Image.szImage[sizeof(m_Image.szImage) - 1] = 0;
	m_Image.nNumFrames = 0;
	m_Image.uImage = 0;
	m_Image.nFlipTime = IR_GetCurrentTime();
	if (bAdjustWndSize && g_pRepresentShell)
	{
		KImageParam	Param;
		if (g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, nType))
			SetSize(Param.nWidth, Param.nHeight);
	}
}

void KWndImage::UpdateTimer()
{
	m_Image.nFlipTime = IR_GetCurrentTime();
}

//--------------------------------------------------------------------------
//	���ܣ�ͼ�λ�֡
//--------------------------------------------------------------------------
int KWndImage::NextFrame()
{
	return IR_NextFrame(m_Image);
}

//--------------------------------------------------------------------------
//	���ܣ��������
//--------------------------------------------------------------------------
void KWndImage::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (g_pRepresentShell)
	{
		m_Image.oPosition.nX = m_nAbsoluteLeft;
		m_Image.oPosition.nY = m_nAbsoluteTop;
		g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
	}
}
/*****************************************************************************************
//	���洰����ϵ�ṹ--����������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-23
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndScrollBar.h"
#include "Wnds.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KWndScrollBar::KWndScrollBar()
{
	m_Flag = 0;
	m_nMinValue = 0;
	m_nMaxValue = 0;
	m_nCurValue = 0;
//	m_nLineSize = 0;
	m_nPageSize = 0;
	m_nMinPosition  = 0;
	m_nMaxPosition  = 0;
	m_nImgRange = 0;
	AddChild(&m_SlideBtn);
}

void KWndScrollBar::Clone(KWndScrollBar* pCopy)
{
	if (pCopy)
	{
		KWndImage::Clone(pCopy);
		pCopy->m_nMinValue = m_nMinValue;
		pCopy->m_nMaxValue = m_nMaxValue;
		pCopy->m_nCurValue = m_nMinValue - 1;
		pCopy->m_nPageSize = m_nPageSize;
		pCopy->m_nMinPosition = m_nMinPosition;
		pCopy->m_nMaxPosition = m_nMaxPosition;
		pCopy->m_Flag = (m_Flag & (~WNDSCROLL_F_DRAGGING_SLIDE));
		pCopy->m_nImgRange = m_nImgRange;
		m_SlideBtn.Clone(&pCopy->m_SlideBtn);
		pCopy->SetScrollPos(m_nMinValue);
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ������
//--------------------------------------------------------------------------
int KWndScrollBar::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndImage::Init(pIniFile, pSection))
	{
		int		nValue;
		pIniFile->GetInteger(pSection, "Type", 0, &nValue);
		if (nValue)
			m_Flag |= WNDSCROLL_ES_VERTICAL;
		else
			m_Flag &= ~WNDSCROLL_ES_VERTICAL;
		pIniFile->GetInteger(pSection, "RepeatImg", 0, &nValue);
		m_nImgRange = 0;
		if (nValue && g_pRepresentShell)
		{
			KImageParam	Param;
			Param.nWidth = 0;
			Param.nHeight = 0;
			g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, m_Image.nType);
			if (m_Flag & WNDSCROLL_ES_VERTICAL)
				m_nImgRange = Param.nHeight;
			else
				m_nImgRange = Param.nWidth;
		}

		pIniFile->GetInteger(pSection, "Min", 0, &m_nMinValue);
		pIniFile->GetInteger(pSection, "Max", 0, &m_nMaxValue);
//		pIniFile->GetInteger(pSection, "Position", 0, &m_nCurValue);
//		pIniFile->GetInteger(pSection, "LineSize", 1, &m_nLineSize);
		pIniFile->GetInteger(pSection, "PageSize", 1, &m_nPageSize);
		pIniFile->GetInteger(pSection, "SlideBegin",0, &m_nMinPosition);
		pIniFile->GetInteger(pSection, "SlideEnd",  0, &m_nMaxPosition);
		char Buffer[128];
		sprintf(Buffer, "%s_Btn", pSection);
		m_SlideBtn.Init(pIniFile, Buffer);

		SetSlideBtnPos();
		
		return true;
	}	
	return false;
}

//���ô��ڴ�С
void KWndScrollBar::SetSize(int nWidth, int nHeight)
{
	if (m_Flag & WNDSCROLL_ES_VERTICAL)
		m_nMaxPosition += nHeight - m_Height;
	else
		m_nMaxPosition += nWidth - m_Height;
	
	if (m_nMaxPosition < m_nMinPosition)
		m_nMaxPosition = m_nMinPosition;

	KWndImage::SetSize(nWidth, nHeight);
	SetSlideBtnPos();
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KWndScrollBar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (IsDisable())
		return KWndImage::WndProc(uMsg, uParam, nParam);
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_MOUSEMOVE:
		if ((uParam & MK_LBUTTON) &&
			(m_Flag & WNDSCROLL_F_DRAGGING_SLIDE))
			OnDraggingSlide(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_LBUTTONUP:
		if (m_Flag & WNDSCROLL_F_DRAGGING_SLIDE)
		{
			m_Flag &= ~WNDSCROLL_F_DRAGGING_SLIDE;
			Wnd_ReleaseCapture();
		}
		break;
	case WND_N_BUTTON_CLICK:
		if (m_Flag & WNDSCROLL_F_DRAGGING_SLIDE &&
			uParam == (unsigned int)(KWndWindow*)&m_SlideBtn)
		{
			m_Flag &= ~WNDSCROLL_F_DRAGGING_SLIDE;
			Wnd_ReleaseCapture();
		}
		break;
	case WND_N_BUTTON_DOWN:
		if (uParam == (unsigned int)(KWndWindow*)&m_SlideBtn)
			OnSlideBtnPressed();
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//�������
void KWndScrollBar::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (g_pRepresentShell)
	{
		m_Image.oPosition.nX = m_nAbsoluteLeft;
		m_Image.oPosition.nY = m_nAbsoluteTop;
		if (m_nImgRange == 0)
		{
			g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
		}
		else
		{
			int*	pValue;
			int		nEnd;
			if (m_Flag & WNDSCROLL_ES_VERTICAL)
			{
				pValue = &m_Image.oPosition.nY;
				nEnd = m_nAbsoluteTop + m_Height;
			}
			else
			{
				pValue = &m_Image.oPosition.nX;
				nEnd = m_nAbsoluteLeft + m_Width;
			}

			do
			{
				g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
				(*pValue) =  (*pValue) + m_nImgRange;
			}while((*pValue) < nEnd);
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ�����ȡֵ��Χ
//--------------------------------------------------------------------------
void KWndScrollBar::SetValueRange(int nMinValue,int nMaxValue)
{
	if (nMinValue <= nMaxValue)
	{
		m_nMinValue = nMinValue;
		m_nMaxValue = nMaxValue;
		SetScrollPos(m_nCurValue);
	}
}

//--------------------------------------------------------------------------
//	���ܣ�����λ��
//--------------------------------------------------------------------------
void KWndScrollBar::SetScrollPos(int nPosition)
{
	if (nPosition != m_nCurValue)
	{
		if (nPosition < m_nMinValue)
			nPosition = m_nMinValue;
		if (nPosition > m_nMaxValue)
			nPosition = m_nMaxValue;
		if (nPosition != m_nCurValue)
		{
			m_nCurValue = nPosition;
			SetSlideBtnPos();
			if (m_pParentWnd)
			{
				m_pParentWnd->WndProc(WND_N_SCORLLBAR_POS_CHANGED, (unsigned int)(KWndWindow*)this, m_nCurValue);
			}
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ����û����鴰��λ��
//--------------------------------------------------------------------------
void KWndScrollBar::SetSlideBtnPos()
{
	int nValueRange = m_nMaxValue - m_nMinValue;
	int nSpaceRange = m_nMaxPosition - m_nMinPosition;

	if (nValueRange <= 0)
		nValueRange = 1;

	int	Left, Top;
	m_SlideBtn.GetPosition(&Left, &Top);
	int	pos = m_nMinPosition + m_nCurValue * nSpaceRange / nValueRange;
	if (pos < m_nMinPosition)
		pos  = m_nMinPosition;
	if (pos > m_nMaxPosition)
		pos = m_nMaxPosition;

	int	nWidth, nHeight;
	m_SlideBtn.GetSize(&nWidth, &nHeight);
	if ((m_Flag & WNDSCROLL_ES_VERTICAL) == 0)
		Left = pos - nWidth / 2;
	else
		Top = pos - nHeight / 2;
	m_SlideBtn.SetPosition(Left, Top);
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ����������
//--------------------------------------------------------------------------
void KWndScrollBar::OnLButtonDown(int x,int y)
{
	int	Left, Top, Width, Height;
	m_SlideBtn.GetAbsolutePos(&Left, &Top);
	m_SlideBtn.GetSize(&Width, &Height);
	if ((m_Flag & WNDSCROLL_ES_VERTICAL) == 0) // Horizontal
	{
		if (x < Left + Width / 2)
			SetScrollPos(m_nCurValue - m_nPageSize);
		else
			SetScrollPos(m_nCurValue + m_nPageSize);
	}
	else
	{
		if (y < Top + Height / 2)
			SetScrollPos(m_nCurValue - m_nPageSize);
		else
			SetScrollPos(m_nCurValue + m_nPageSize);
	}
}

int KWndScrollBar::ScrollLine(bool bPre)
{
	int nPos = m_nCurValue + (bPre ? (-1) : 1);
	SetScrollPos(nPos);
	return m_nCurValue;
}

int	KWndScrollBar::ScrollPage(bool bPre)
{
	int nPos = m_nCurValue + (bPre ? (-m_nPageSize) : m_nPageSize);
	SetScrollPos(nPos);
	return m_nCurValue;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ������ť������
//--------------------------------------------------------------------------
void KWndScrollBar::OnSlideBtnPressed()
{
	Wnd_SetCapture(this);
	m_Flag |= WNDSCROLL_F_DRAGGING_SLIDE;
}

//--------------------------------------------------------------------------
//	���ܣ������϶�������ť
//--------------------------------------------------------------------------
void KWndScrollBar::OnDraggingSlide(int x, int y)
{
	int nValueRange = m_nMaxValue - m_nMinValue;
	int nSpaceRange = m_nMaxPosition - m_nMinPosition;
	if (nSpaceRange <= 0)
		nSpaceRange = 1;
	int	nValue = m_nMinValue;
	if (nValueRange != 0)
	{
		if ((m_Flag & WNDSCROLL_ES_VERTICAL) == 0)
			nValue += (x - m_nAbsoluteLeft - m_nMinPosition + nSpaceRange / nValueRange / 2) * nValueRange / nSpaceRange;
		else
			nValue += (y - m_nAbsoluteTop - m_nMinPosition + nSpaceRange / nValueRange / 2) * nValueRange / nSpaceRange;
	}
	SetScrollPos(nValue);
}

int	KWndScrollBar::GetMinHeight()
{
	int nHeight = 0;
	m_SlideBtn.GetSize(NULL, &nHeight);
	return max(nHeight, nHeight + m_nMinPosition + m_Height - m_nMaxPosition);
}
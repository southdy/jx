// -------------------------------------------------------------------------
//	�ļ���		��	ShowChatText.cpp
//	������		��	����
//	����ʱ��	��	2002-9-16 16:22:56
//	��������	��	��ʾ��������
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "WndMessage.h"
#include "Wnds.h"
#include "WndMessageListBox.h"
#include "WndScrollBar.h"

#include "../../../Engine/src/Text.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Engine/Src/Text.h"
extern iRepresentShell*	g_pRepresentShell;

#define MSGLISTBOX_S_SELABLE			0x00008000
#define MSGLISTBOX_S_HIGHLIGHTABLE		0x00004000
#define MSGLISTBOX_S_TEXTBOTTOM			0x00002000

KWndMessageListBox::KWndMessageListBox()
{
	m_pScrollbar = NULL;
	m_pMessages = NULL;			//��Ϣ����
	m_nNumMessage = 0;			//��Ϣ����Ŀ
	m_nCapability = 0;			//��Ϣ�����������Ŀ
	m_nFontSize = 12;			//�����С
	m_nNumMaxShowLine = 0;
	m_nNumBytesPerLine = 20;	//ÿһ���ַ�����Ŀ	
	m_nStartShowMsg = 0;
	m_nStartMsgSkipLine = 0;
	m_nNumVisibleTextLine = 0;
	m_nSelMsgIndex = -1;
	m_nHLMsgIndex  = -1;
	m_uTextLineShadowColor = 0;
	m_SelMsgBgColor = 0;

	m_bHitText = FALSE;

	m_nHideNumLine = 0;
}

// -------------------------------------------------------------------------
// ����	: ��������
// -------------------------------------------------------------------------
KWndMessageListBox::~KWndMessageListBox()
{
	SetCapability(0);
}

// -------------------------------------------------------------------------
// ����	: ���ÿ�������ѶϢ����Ŀ
// -------------------------------------------------------------------------
int KWndMessageListBox::SetCapability(int nNumMessage)
{
	KOneMsgInfo** pNew = NULL;
	if (nNumMessage)
	{
		pNew = (KOneMsgInfo**)malloc(sizeof(KOneMsgInfo*) * nNumMessage);
		if (pNew == NULL)
			return false;
		memset(pNew, 0, sizeof(KOneMsgInfo*) * nNumMessage);
	}
	if (nNumMessage && m_nNumMessage)
	{
		if (m_nNumMessage <= nNumMessage)
		{
			memcpy(pNew, m_pMessages, sizeof(KOneMsgInfo*) * m_nNumMessage);
			memset(m_pMessages, 0, sizeof(KOneMsgInfo*) * m_nNumMessage);
		}
		else
		{
			memcpy(pNew, &m_pMessages[m_nNumMessage - nNumMessage], sizeof(KOneMsgInfo*) * nNumMessage);
			memset(&m_pMessages[m_nNumMessage - nNumMessage], 0, sizeof(KOneMsgInfo*) * nNumMessage);
			m_nNumMessage = nNumMessage;
		}
	}
	else
		m_nNumMessage = 0;

	if (m_pMessages)
	{
		for (int i = 0; i < m_nCapability; i++)
		{
			if (m_pMessages[i])
			{
				free(m_pMessages[i]);
				m_pMessages[i] = NULL;
			}
		}
		free(m_pMessages);
		m_pMessages = NULL;
	}

	m_pMessages = pNew;
	m_nCapability = nNumMessage;

	UpdateData();
	return true;
}

// -------------------------------------------------------------------------
// ����	: ���ù���������
// -------------------------------------------------------------------------
void KWndMessageListBox::SetScrollbar(KWndScrollBar* pScroll)
{
	if (m_pScrollbar = pScroll)
		UpdateData();
}

// -------------------------------------------------------------------------
// ����	: ����һ����Ϣ
// -------------------------------------------------------------------------
int KWndMessageListBox::AddOneMessage(const char* pText, int nLen, unsigned int uTextBKColor)
{
	if (pText == NULL)
		return 0;
	if (nLen < 0)
		nLen = strlen(pText);
	if (m_nCapability > 0 && nLen)
	{
		ClearHideLine();

		int nTo = GetItemLineCount(m_nNumMessage);
		int nStart = GetItemLineCount(m_nStartShowMsg) + m_nStartMsgSkipLine;

		if (m_nNumMessage == m_nCapability)
		{
			KOneMsgInfo* pMsg = m_pMessages[0];
			memmove(&m_pMessages[0], &m_pMessages[1],
				sizeof(KOneMsgInfo*) * (m_nNumMessage - 1));
			m_nNumMessage--;
			m_pMessages[m_nNumMessage] = pMsg;
		}
		KOneMsgInfo* pMsg = m_pMessages[m_nNumMessage];
		if (pMsg == NULL || pMsg->nMaxSize < nLen || pMsg->nMaxSize - nLen > 256)
		{
			pMsg = (KOneMsgInfo*)realloc(pMsg, sizeof(KOneMsgInfo) + nLen);
			if (pMsg == NULL)
				return false;
			m_pMessages[m_nNumMessage] = pMsg;
			pMsg->nMaxSize = nLen;
		}
		memcpy(pMsg->Msg, pText, nLen);
		pMsg->Msg[nLen] = 0;
		pMsg->nLen = nLen;
		pMsg->uTextBKColor = uTextBKColor;

		if (g_pRepresentShell)
		{
			pMsg->nLines = TGetEncodedTextLineCount(
				pMsg->Msg, nLen, m_nNumBytesPerLine, pMsg->nCharWidth, m_nFontSize, 0, 0, true);
		}
		else
		{
			pMsg->nLines = 0;
			pMsg->nCharWidth = 0;
		}

		m_nNumMessage++;

		int nTo2 = GetItemLineCount(m_nNumMessage);
		if (nTo > 0)
			SetFirstShowLine(MulDiv(nStart, nTo2, nTo));

		UpdateData();
		return true;
	}
	return false;
}

// -------------------------------------------------------------------------
// ����	: ɾ��һ����Ϣ
// -------------------------------------------------------------------------
void KWndMessageListBox::RemoveAMessage(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nNumMessage)
	{
		ClearHideLine();

		if (m_nSelMsgIndex <= nIndex)
			m_nSelMsgIndex = -1;
		else
			m_nSelMsgIndex--;
		m_nHLMsgIndex = -1;

		int nTo = GetItemLineCount(m_nNumMessage);
		int nStart = GetItemLineCount(m_nStartShowMsg) + m_nStartMsgSkipLine;
		
		KOneMsgInfo* pMsg = m_pMessages[nIndex];
		nStart -= pMsg->nLines;

		if (m_nNumMessage < m_nCapability)
		{
			memmove(&m_pMessages[nIndex], &m_pMessages[nIndex + 1],
				sizeof(KOneMsgInfo*) * (m_nNumMessage - nIndex));
			m_nNumMessage--;
			m_pMessages[m_nNumMessage] = pMsg;
		}
		else
			m_nNumMessage--;

		int nTo2 = GetItemLineCount(m_nNumMessage);
		if (nTo > 0)
			SetFirstShowLine(MulDiv(nStart, nTo2, nTo));

		UpdateData();
	}
}

// -------------------------------------------------------------------------
// ����	: ɾ��ȫ����Ϣ
// -------------------------------------------------------------------------
void KWndMessageListBox::Clear()
{
	m_nNumMessage = 0;
	m_nNumVisibleTextLine = 0;
	m_nSelMsgIndex = -1;
	m_nHLMsgIndex = -1;
	m_nStartShowMsg = 0;
	m_nStartMsgSkipLine = 0;
	if (m_pScrollbar)
	{
		m_pScrollbar->SetScrollPos(0);
		m_pScrollbar->SetValueRange(0, 0);
	}
}

void KWndMessageListBox::Clone(KWndMessageListBox* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_nFontSize = m_nFontSize;
		pCopy->m_nNumBytesPerLine	= m_nNumBytesPerLine;
		pCopy->m_nNumMaxShowLine	= m_nNumMaxShowLine;
		pCopy->m_MsgColor		= m_MsgColor;
		pCopy->m_MsgBorderColor	= m_MsgBorderColor;
		pCopy->m_SelMsgColor	= m_SelMsgColor;
		pCopy->m_SelMsgBorderColor	= m_SelMsgBorderColor;
		pCopy->m_SelMsgBgColor	= m_SelMsgBgColor;
		pCopy->m_HLMsgColor		= m_HLMsgColor;
		pCopy->m_HLMsgBorderColor	= m_HLMsgBorderColor;
		pCopy->m_uTextLineShadowColor = m_uTextLineShadowColor;
		pCopy->SetCapability(m_nCapability);
	}
}

// -------------------------------------------------------------------------
// ����	: ��ʼ��
// -------------------------------------------------------------------------
int KWndMessageListBox::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		int	nValue;
		pIniFile->GetInteger(pSection, "MaxMsgCount", 16, &nValue);
		SetCapability(nValue);
		pIniFile->GetInteger(pSection, "HitText", 0, &m_bHitText);
		//====���ִ�С====
		pIniFile->GetInteger(pSection, "Font", 16, &m_nFontSize);
		if (m_nFontSize < 4)
			m_nFontSize = 4;
		//---������ʾ���������---
		m_nNumBytesPerLine = (m_Width * 2) / m_nFontSize;
		if (m_nNumBytesPerLine < 2)
			m_nNumBytesPerLine = 2;
		m_nNumMaxShowLine = m_Height / (m_nFontSize + 1);

		char	Buff[16];
		pIniFile->GetString(pSection, "MsgColor", "", Buff, 16);
		m_MsgColor = GetColor(Buff);
		pIniFile->GetString(pSection, "MsgBorderColor", "", Buff, 16);
		m_MsgBorderColor = GetColor(Buff);

		char	Buffer[32];
		if (pIniFile->GetString(pSection, "TextLineShadow", "", Buffer, sizeof(Buffer)))
		{			
			m_uTextLineShadowColor = (GetColor(Buffer) & 0xffffff);
			pIniFile->GetInteger(pSection, "TextLineShadowAlpha", 128, &nValue);
			if (nValue > 255 || nValue < 0)
				m_uTextLineShadowColor |= 0x16000000;
			else
				m_uTextLineShadowColor |= (((255 - nValue) << 21) & 0xff000000);
		}
		else
			m_uTextLineShadowColor = 0;

		pIniFile->GetInteger(pSection, "Selable", 0, &nValue);
		if (nValue)
		{
			m_Style |= MSGLISTBOX_S_SELABLE;
			pIniFile->GetString(pSection, "SelColor", "", Buff, 16);
			m_SelMsgColor = GetColor(Buff);
			pIniFile->GetString(pSection, "SelBorderColor", "", Buff, 16);
			m_SelMsgBorderColor = GetColor(Buff);
			
			if (pIniFile->GetString(pSection, "SelBgColor", "", Buffer, sizeof(Buffer)))
			{
				m_SelMsgBgColor = (GetColor(Buffer) & 0xffffff);
				pIniFile->GetInteger(pSection, "SelBgColorAlpha", 128, &nValue);
				if (nValue > 255 || nValue < 0)
					m_SelMsgBgColor |= 0x16000000;
				else
					m_SelMsgBgColor |= (((255 - nValue) << 21) & 0xff000000);
			}
			else
				m_SelMsgBgColor = 0;

			pIniFile->GetInteger(pSection, "HighLight", 0, &nValue);
			if (nValue)
			{
				m_Style |= MSGLISTBOX_S_HIGHLIGHTABLE;
				pIniFile->GetString(pSection, "HighLightColor", "", Buff, 16);
				m_HLMsgColor = GetColor(Buff);
				pIniFile->GetString(pSection, "HighLightBorderColor", "", Buff, 16);
				m_HLMsgBorderColor = GetColor(Buff);
			}
			else
			{
				m_nHLMsgIndex = -1;
				m_Style &= ~MSGLISTBOX_S_HIGHLIGHTABLE;
			}				
		}
		else
		{
			m_nSelMsgIndex = -1;
			m_nHLMsgIndex = -1;
			m_Style &= ~(MSGLISTBOX_S_SELABLE | MSGLISTBOX_S_HIGHLIGHTABLE);
		}

		pIniFile->GetInteger(pSection, "TextBottom", 0, &nValue);
		if (nValue)
			m_Style |= MSGLISTBOX_S_TEXTBOTTOM;
		else
			m_Style &= ~MSGLISTBOX_S_TEXTBOTTOM;
		
		UpdateData();
		
		return true;
	}
	return false;
}

//���ô��ڴ�С
void KWndMessageListBox::SetSize(int nWidth, int nHeight)
{
	ClearHideLine();
	
	if (nWidth < m_nFontSize)
		nWidth = m_nFontSize + 1;
	if (nHeight < m_nFontSize)
		nHeight = m_nFontSize + 1;

	KWndWindow::SetSize(nWidth, nHeight);

	//---������ʾ���������---
	int nNumBytesPerLine = (m_Width * 2) / m_nFontSize;
	if (nNumBytesPerLine < 2)
		nNumBytesPerLine = 2;
	int nLines = m_Height / (m_nFontSize + 1);

	if (nLines != m_nNumMaxShowLine || m_nNumBytesPerLine != nNumBytesPerLine)
	{
		int nTo = 0;
		if (m_Style & MSGLISTBOX_S_TEXTBOTTOM)
		{
			nTo	 = GetItemLineCount(m_nNumMessage) - GetItemLineCount(m_nStartShowMsg) - m_nStartMsgSkipLine - m_nNumMaxShowLine;
			nTo = max(nTo, 0);
		}
		
		m_nNumMaxShowLine = nLines;
		m_nNumBytesPerLine = nNumBytesPerLine;
		for (int i = 0; i < m_nNumMessage; i++)
		{
			if (g_pRepresentShell)
			{
				m_pMessages[i]->nLines = TGetEncodedTextLineCount(
											m_pMessages[i]->Msg, m_pMessages[i]->nLen, m_nNumBytesPerLine, m_pMessages[i]->nCharWidth, m_nFontSize, 0, 0, true);
			}
			else
				m_pMessages[i]->nLines = 0;
		}

		if (m_Style & MSGLISTBOX_S_TEXTBOTTOM)
			SetFirstShowLine(GetItemLineCount(m_nNumMessage) - nTo - m_nNumMaxShowLine);

		UpdateData();
	}
}

void KWndMessageListBox::SetFontSize(int nFontSize)
{
	if (nFontSize < 12 || nFontSize > 16)
		return;

	if (m_nFontSize == nFontSize)
		return;

	m_nFontSize = nFontSize;

	SetSize(m_Width, m_Height);
}

// -------------------------------------------------------------------------
// ����	: ����������ɾ���ߴ��ڳߴ�仯������Щ���������Լ���������������
// -------------------------------------------------------------------------
void KWndMessageListBox::UpdateData()
{
	int nTotalLine = 0;
	for (int i = 0; i < m_nNumMessage; i++)
		nTotalLine += m_pMessages[i]->nLines;
	if (nTotalLine > m_nNumMaxShowLine)
	{
		if (m_pScrollbar)
		{
			m_pScrollbar->Enable(true);
			m_pScrollbar->SetValueRange(0, nTotalLine - m_nNumMaxShowLine);
		}

		m_nNumVisibleTextLine = m_nNumMaxShowLine;
	}
	else
	{
		if (m_pScrollbar)
		{
			m_pScrollbar->Enable(false);
			m_pScrollbar->SetValueRange(0, 0);
		}

		m_nNumVisibleTextLine = nTotalLine;
	}
}

// -------------------------------------------------------------------------
// ����	: ��ȡ��һ������ʾ��������ȫ�����ֵĵڼ���
// -------------------------------------------------------------------------
int KWndMessageListBox::GetFirstShowLine()
{
	int nLine = 0;
	for (int i = 0; i < m_nStartShowMsg; i++)
		nLine += m_pMessages[i]->nLines;
	return (nLine + m_nStartMsgSkipLine);
}

// -------------------------------------------------------------------------
// ����	: ���õ�һ������ʾ��������ȫ�����ֵĵڼ���
// -------------------------------------------------------------------------
void KWndMessageListBox::SetFirstShowLine(int nLine)
{
	ClearHideLine();

	if (nLine <= 0)
	{
		m_nStartShowMsg = 0;
		m_nStartMsgSkipLine = 0;
		return;
	}
	m_nStartMsgSkipLine = nLine;
	m_nStartShowMsg = 0;
	while (m_nStartShowMsg < m_nNumMessage)
	{
		if (m_pMessages[m_nStartShowMsg]->nLines < m_nStartMsgSkipLine)
		{
			m_nStartMsgSkipLine -= m_pMessages[m_nStartShowMsg]->nLines;
			m_nStartShowMsg++;
			continue;
		}
		if (m_pMessages[m_nStartShowMsg]->nLines == m_nStartMsgSkipLine)
		{
			m_nStartMsgSkipLine = 0;
			m_nStartShowMsg++;
		}
		break;
	}

	m_nNumVisibleTextLine = 0;
	int nMsg = m_nStartShowMsg;
	if (nMsg < m_nNumMessage)
	{
		m_nNumVisibleTextLine += m_pMessages[nMsg]->nLines - m_nStartMsgSkipLine;
		nMsg++;
		for (; nMsg < m_nNumMessage; nMsg++)
			m_nNumVisibleTextLine += m_pMessages[nMsg]->nLines;
		if (m_nNumVisibleTextLine >= m_nNumMaxShowLine)
			m_nNumVisibleTextLine = m_nNumMaxShowLine;
	}
}

int KWndMessageListBox::GetItemLineCount(int nIndex)
{
	int nLine = 0;
	for (int i = min(nIndex, m_nNumMessage) - 1; i >= 0; i--)
		nLine += m_pMessages[i]->nLines;
	return nLine;
}

// -------------------------------------------------------------------------
// ����	: ���ƴ���
// -------------------------------------------------------------------------
void KWndMessageListBox::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (g_pRepresentShell == NULL || m_nNumMessage == 0)
		return;

	if (m_nHideNumLine >= m_nNumMaxShowLine)//��ȫ������,�Ͳ��û���
		return;
	
	if (m_nHideNumLine >= m_nNumVisibleTextLine)	//�ɼ�����ȫ������,�Ͳ��û���
		return;

	int	nCurMsg = m_nStartShowMsg;
	int	nViewLines = m_nNumMaxShowLine;
	int nSkipLine = m_nStartMsgSkipLine;
	int nNumVisibleTextLine = 0;

	int	y = m_nAbsoluteTop + GetOffsetTextHeight();

	if (m_Style & MSGLISTBOX_S_TEXTBOTTOM)
	{
		nNumVisibleTextLine = m_nNumVisibleTextLine - m_nHideNumLine;
		y += m_nHideNumLine * (m_nFontSize + 1);
	}
	else
	{
		nNumVisibleTextLine = m_nNumVisibleTextLine - m_nHideNumLine;
	}

	KRUShadow	bg;
	if (m_uTextLineShadowColor)
	{
		bg.Color.Color_dw = m_uTextLineShadowColor;
		bg.oPosition.nX = m_nAbsoluteLeft;
		bg.oEndPos.nX = bg.oPosition.nX + m_Width;
		bg.oPosition.nY = y;
		bg.oEndPos.nY = bg.oPosition.nY + nNumVisibleTextLine * (m_nFontSize + 1);
		g_pRepresentShell->DrawPrimitives(1, &bg, RU_T_SHADOW, true);
	}

	while (nViewLines > 0 && nCurMsg < m_nNumMessage)
	{
		KOneMsgInfo* pCurMsg = m_pMessages[nCurMsg];
		KOutputTextParam	Param;
		Param.nSkipLine = nSkipLine;
		Param.nNumLine = pCurMsg->nLines - nSkipLine;
		if (Param.nNumLine > nViewLines)
			Param.nNumLine = nViewLines;

		Param.nX = m_nAbsoluteLeft;
		Param.nY = y;
		Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;

		int nOffset = 0;
		
		bool bDraw = (nViewLines + m_nHideNumLine <= m_nNumMaxShowLine);

		if (bDraw)
		{
			if (nCurMsg == m_nSelMsgIndex && (m_Style & MSGLISTBOX_S_SELABLE))
			{
				//���Ʊ���ɫ
				if (m_SelMsgBgColor)
				{
					bg.Color.Color_dw = m_SelMsgBgColor;
					bg.oPosition.nX = Param.nX;
					bg.oPosition.nY = Param.nY;
					bg.oEndPos.nX = bg.oPosition.nX + m_Width;
					bg.oEndPos.nY = bg.oPosition.nY + Param.nNumLine * (m_nFontSize + 1);
					g_pRepresentShell->DrawPrimitives(1, &bg, RU_T_SHADOW, true);
				}
				Param.Color = m_SelMsgColor;
				Param.BorderColor = m_SelMsgBorderColor;
			}
			else if (nCurMsg == m_nHLMsgIndex)
			{
				Param.Color = m_HLMsgColor;
				Param.BorderColor = m_HLMsgBorderColor;
			}
			else
			{
				Param.Color = m_MsgColor;
				Param.BorderColor = m_MsgBorderColor;
				//nOffset = TSplitString(pCurMsg->Msg, 5, false);
			}

			if (pCurMsg->uTextBKColor)	//�������ֳĵ�
			{
				bg.Color.Color_dw = pCurMsg->uTextBKColor;
				bg.oPosition.nX = Param.nX;
				bg.oPosition.nY = Param.nY;
				bg.oEndPos.nX = bg.oPosition.nX + (pCurMsg->nCharWidth * m_nFontSize / 2);
				bg.oEndPos.nY = bg.oPosition.nY + Param.nNumLine * (m_nFontSize + 1);
				g_pRepresentShell->DrawPrimitives(1, &bg, RU_T_SHADOW, true);
			}
		
			Param.bPicPackInSingleLine = true;
			g_pRepresentShell->OutputRichText(m_nFontSize, &Param, pCurMsg->Msg + nOffset, pCurMsg->nLen - nOffset, m_Width);

			y += Param.nNumLine * (m_nFontSize + 1);
		}

		nSkipLine = 0;
		nViewLines -= Param.nNumLine;

		nCurMsg++;
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KWndMessageListBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		if (m_Style & MSGLISTBOX_S_HIGHLIGHTABLE)
			OnMouseMove(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_LBUTTONDOWN:
		if (m_Style & MSGLISTBOX_S_SELABLE)
			OnLButtonDown(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_LBUTTONDBLCLK:
		if (m_Style & MSGLISTBOX_S_SELABLE)
			OnLButtonDClick(LOWORD(nParam), HIWORD(nParam));
		break;
	case WM_MOUSEWHEEL:
		{
			if (m_pScrollbar && !m_pScrollbar->IsDisable())
			{
				int zDelta = short(HIWORD(uParam));
				int nPos = m_pScrollbar->GetScrollPos();
				nPos += (-zDelta / WHEEL_DELTA);
				m_pScrollbar->SetScrollPos(nPos);
			}
		}
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}

	return 0;
}

int	KWndMessageListBox::GetMsgAtPoint(int x, int y)
{
	int nSelLine = 0;

	nSelLine = (y - m_nAbsoluteTop - GetOffsetTextHeight()) / (m_nFontSize + 1);

	if (nSelLine < 0 || nSelLine >= m_nNumVisibleTextLine)
		return -1;

	int nMsg = m_nStartShowMsg;
	int nViewLine = m_nNumMaxShowLine;
	int nSkipLine = m_nStartMsgSkipLine;
	int nNumLine = 0;
	while (nViewLine > 0 && nMsg < m_nNumMessage)
	{
		KOneMsgInfo* pCurMsg = m_pMessages[nMsg];
		nNumLine += pCurMsg->nLines - nSkipLine;
		if (nNumLine > nViewLine)
		{
			if (nSelLine < nViewLine)
				return nMsg;
			break;
		}
		if (nSelLine < nNumLine)
			return nMsg;
		nSkipLine = 0;
		nMsg++;
	}
	return -1;
}

int KWndMessageListBox::HitTextAtPoint(int x, int y)
{
	int n = GetMsgAtPoint(x, y);
	if (n >= 0 && n < m_nNumMessage)
	{
		if (m_bHitText)
		{
			x -= m_nAbsoluteLeft;
			if (x > 0 && x <= (m_pMessages[n]->nCharWidth / 2 * m_nFontSize))
			{
				int nPos = TFindSpecialCtrlInEncodedText(m_pMessages[n]->Msg, 
					m_pMessages[n]->nLen, 0, ':');
				if (nPos >= 0)
				{
					int nNameLen = 0;
					TGetEncodedTextLineCount(m_pMessages[n]->Msg, nPos, m_nNumBytesPerLine,
						nNameLen, m_nFontSize, 0, 0, true);
					if (x <= (nNameLen  * m_nFontSize / 2))
					{
						return n;
					}
				}
			}
		}
		else
			return n;
	}

	return -1;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�������ڴ˰���
//--------------------------------------------------------------------------
void KWndMessageListBox::OnLButtonDown(int x, int y)
{	
	int nSel = HitTextAtPoint(x, y);
	if (nSel >= 0 && m_pParentWnd)
		m_pParentWnd->WndProc(WND_N_LIST_ITEM_ACTIVE, (unsigned int)(KWndWindow*)this, m_nSelMsgIndex);
	SetCurSel(nSel);
}

//��Ӧ������double click
void KWndMessageListBox::OnLButtonDClick(int x, int y)
{
	int nSel = HitTextAtPoint(x, y);
	if (nSel >= 0 && m_pParentWnd)
	{
		m_pParentWnd->WndProc(WND_N_LIST_ITEM_D_CLICK,
			(unsigned int)(KWndWindow*)this, nSel);
	}
}


int	KWndMessageListBox::SetCurSel(int nIndex)
{
	ClearHideLine();

	if (nIndex != m_nSelMsgIndex)
	{
		m_nSelMsgIndex = nIndex;
	}

	if (m_pParentWnd)
		m_pParentWnd->WndProc(WND_N_LIST_ITEM_SEL, (unsigned int)(KWndWindow*)this, m_nSelMsgIndex);

	return m_nSelMsgIndex;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ����ƶ�
//--------------------------------------------------------------------------
void KWndMessageListBox::OnMouseMove(int x, int y)
{
	int nSel = HitTextAtPoint(x, y);
	if (nSel != m_nHLMsgIndex)
	{
		m_nHLMsgIndex = nSel;
		if (m_pParentWnd)
		{
			m_pParentWnd->WndProc(WND_N_LIST_ITEM_HIGHLIGHT, (unsigned int)(KWndWindow*)this, m_nHLMsgIndex);
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ȡһ����Ϣ������
//  ������nIndex  --> ��Ϣ�����������ֵС��0����-n(n>0)��ʾ������n����Ϣ
//        pBuffer --> ������Ϣ���ݵĻ�����
//        nLen    --> pBuffer��ָ�������Ĵ�С
//        bExcludeCtrl --> �Ƿ�ȥ����Ϣ�еĿ��Ʒ�
//  ���أ��������ֵ>=0��ʾȡ����ָ������Ϣ�����ݣ�����ֵΪ�浽pBuffer����������Ϣ�ĳ���
//        �������ֵ<0��ʾ����ʧ�ܣ�ԭ�����Ϊ������ָ����������Ϣ��Ŀ�����߻�������С�����ԭ��
//--------------------------------------------------------------------------
int KWndMessageListBox::GetOneMessage(int nIndex, char* pBuffer, int nLen, bool bExcludeCtrl)
{
	if (pBuffer)
	{
		if (nIndex < 0)
			nIndex = m_nNumMessage + nIndex;
		if (nIndex >= 0 && nIndex < m_nNumMessage)
		{
			int nReturn = m_pMessages[nIndex]->nLen;
			if (nLen >= nReturn)
			{
				memcpy(pBuffer, m_pMessages[nIndex]->Msg, nReturn);
				if (bExcludeCtrl)
					nReturn = TRemoveCtrlInEncodedText(pBuffer, nReturn);
				if (nLen > nReturn)
					pBuffer[nReturn] = 0;
				return nReturn;
			}
		}
	}
	return -1;
}

//--------------------------------------------------------------------------
//	���ܣ��Ӵ��ڷ��������
//	���أ����ڵ����ݵľ����
//	��ע���˷��ؾ���������������BindDataʹ�������ûش��ڻ����FreeData�ͷ�����
//			���������ڴ��й©��
//--------------------------------------------------------------------------
unsigned int KWndMessageListBox::SplitData()
{
	KMessageListData* pData = NULL;
	if (m_nCapability)
	{
		pData = (KMessageListData*)malloc(sizeof(KMessageListData));
		if (pData)
		{
			pData->nCapability = m_nCapability;
			pData->nNumMessage = m_nNumMessage;
			pData->nSelMsgIndex = m_nSelMsgIndex;
			pData->nStartMsgSkipLine = m_nStartMsgSkipLine;
			pData->nStartShowMsg = m_nStartShowMsg;
			pData->pMessages = m_pMessages;

			m_nNumMessage = 0;
			m_nSelMsgIndex = -1;
			m_nStartMsgSkipLine = 0;
			m_nStartShowMsg = 0;
			m_pMessages = (KOneMsgInfo**)malloc(sizeof(KOneMsgInfo*) * m_nCapability);
			if (m_pMessages)
				memset(m_pMessages, 0, sizeof(KOneMsgInfo*) * m_nCapability);
			else
				m_nCapability = 0;

			if (m_pScrollbar)
			{
				m_pScrollbar->Enable(false);
				m_pScrollbar->SetScrollPos(0);
			}
		}
	}
	return ((unsigned int)pData);
}

//--------------------------------------------------------------------------
//	���ܣ���������������
//  ������hData --> Ҫ���󵽴��ڵ����ݵľ�����˾��Ϊ֮ǰ����SplitData��
//					BindData������õķ���ֵ��
//	���أ��������֮ǰ�����ڵ����ݵľ����
//	��ע���˷��ؾ���������������BindDataʹ�������ûش��ڻ����FreeData�ͷ�����
//			���������ڴ��й©��
//--------------------------------------------------------------------------
unsigned int KWndMessageListBox::BindData(unsigned int hData)
{
	if (hData == NULL)
		return SplitData();

	KMessageListData* pData = (KMessageListData*)hData;
	KMessageListData NewData = *pData;

	pData->nCapability = m_nCapability;
	pData->nNumMessage = m_nNumMessage;
	pData->nSelMsgIndex = m_nSelMsgIndex;
	pData->nStartMsgSkipLine = m_nStartMsgSkipLine;
	pData->nStartShowMsg = m_nStartShowMsg;
	pData->pMessages = m_pMessages;

	m_nCapability = NewData.nCapability;
	m_nNumMessage = NewData.nNumMessage;
	m_nSelMsgIndex = NewData.nSelMsgIndex;
	m_nStartMsgSkipLine = NewData.nStartMsgSkipLine;
	m_nStartShowMsg = NewData.nStartShowMsg;
	m_pMessages = NewData.pMessages;

	if (m_pScrollbar)
	{
		int i, nTotalLine = 0;
		for(i = 0; i < m_nStartShowMsg; i++)
			nTotalLine += m_pMessages[i]->nLines;
		int nFirstLine = nTotalLine + m_nStartMsgSkipLine;
		for (;i < m_nNumMessage; i++)
			nTotalLine += m_pMessages[i]->nLines;

		if (nTotalLine > m_nNumMaxShowLine)
		{
			m_nNumVisibleTextLine = m_nNumMaxShowLine;
			m_pScrollbar->SetValueRange(0, nTotalLine - m_nNumMaxShowLine);
			m_pScrollbar->SetScrollPos(nFirstLine);
			m_pScrollbar->Enable(true);
		}
		else
		{
			m_nNumVisibleTextLine = nTotalLine;
			m_pScrollbar->Enable(false);
			m_pScrollbar->SetScrollPos(0);
		}
	}
	return hData;
}

//--------------------------------------------------------------------------
//	���ܣ��ͷŴ������ݾ��
//  ������hData --> Ҫ�ͷŵĴ��ڵ����ݵľ�����˾��Ϊ֮ǰ����SplitData��
//					BindData������õķ���ֵ��
//--------------------------------------------------------------------------
void KWndMessageListBox::FreeData(unsigned int	hData)
{
	if (hData)
	{
		if (((KMessageListData*)hData)->pMessages)
			free (((KMessageListData*)hData)->pMessages);
		free ((void*)hData);
	}
}

unsigned int KWndMessageListBox::GetOffsetTextHeight()
{
	if (m_Style & MSGLISTBOX_S_TEXTBOTTOM)
	{
		if (m_nNumVisibleTextLine < m_nNumMaxShowLine)
			return (m_nNumMaxShowLine - m_nNumVisibleTextLine) * (m_nFontSize + 1);
	}
	return 0;
}

int	KWndMessageListBox::PtInWindow(int x, int y)
{
	if (m_bHitText)
	{
		if (HitTextAtPoint(x, y) >= 0)
			return 1;
		else
			return 0;
	}
	else
		return KWndWindow::PtInWindow(x, y);
}

void KWndMessageListBox::ClearHideLine()
{
	m_nHideNumLine = 0;
}

void KWndMessageListBox::HideNextLine()
{
	int	nCurMsg = m_nStartShowMsg;
	if (m_pMessages && m_nHideNumLine < m_nNumMaxShowLine && m_nNumMessage > 0 && nCurMsg < m_nNumMessage)
	{
		int nNewHideNumLine = m_pMessages[nCurMsg]->nLines - m_nStartMsgSkipLine;
		while (nNewHideNumLine <= m_nHideNumLine)
		{
			nCurMsg++;
			if (nCurMsg < m_nNumMessage)
			{
				nNewHideNumLine += m_pMessages[nCurMsg]->nLines;
			}
			else
				break;
		}
		m_nHideNumLine = min(nNewHideNumLine, m_nNumMaxShowLine);
	}
}

void KWndMessageListBox::HideAllLine()
{
	m_nHideNumLine = m_nNumMaxShowLine;
}

int KWndMessageListBox::GetMinHeight()
{
	return max(m_nFontSize + 1, 0);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

KScrollMessageListBox::KScrollMessageListBox()
{
	m_nMinLineCount = 2;
	m_nMaxLineCount = 30;
	m_nCurrentLineCount = 0;
	m_nLineHeight = 12;
	
	AddChild(&m_MsgList);
	AddChild(&m_Scroll);
}

int KScrollMessageListBox::Init(KIniFile* pIniFile, const char* pSection)//��ʼ��
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		char Buffer[128];
		sprintf(Buffer, "%s_List", pSection);
		m_MsgList.Init(pIniFile, Buffer);
		pIniFile->GetInteger(Buffer, "Font", 12, &m_nLineHeight);
		m_nLineHeight++;

		sprintf(Buffer, "%s_Scroll", pSection);
		m_Scroll.Init(pIniFile, Buffer);

		int nMinLineCount, nMaxLineCount;
		pIniFile->GetInteger2(pSection, "MsgLineCount", &nMinLineCount, &nMaxLineCount);
		InitMinMaxLineCount(nMinLineCount, nMaxLineCount);

		m_MsgList.SetScrollbar(&m_Scroll);	//�ߴ�ȫ��������Ϻ��ټӹ�ϵ

		RECT rc;
		if (m_Style & WND_S_SIZE_WITH_ALL_CHILD)
		{
			GetAllChildLayoutRect(&rc);
			m_Width = rc.right - rc.left;;
			m_Height = rc.bottom - rc.top;
		}
		return true;
	}
	return false;
}

//���ں���
int KScrollMessageListBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_SCORLLBAR_POS_CHANGED:
		if (uParam == (unsigned int)(KWndWindow*)&m_Scroll)
		{
			m_MsgList.SetFirstShowLine(nParam);
		}
		break;
	case WND_N_LIST_ITEM_SEL:
		if (nParam >= 0)
		{
			int nLine = m_MsgList.GetItemLineCount(nParam);
			int nTop = m_MsgList.GetFirstShowLine();
			int nBottom = nTop + m_MsgList.GetMaxShowLine();
			if (nLine < nTop)
				m_Scroll.SetScrollPos(nLine);
			else if (nLine >= nBottom)
				m_Scroll.SetScrollPos(nTop + nLine - nBottom + 1);
		}

		if (m_pParentWnd)
			m_pParentWnd->WndProc(uMsg, (unsigned int)(KWndWindow*)this, nParam);
		break;
	case WND_N_LIST_ITEM_D_CLICK:
		if (m_pParentWnd)
			m_pParentWnd->WndProc(uMsg, (unsigned int)(KWndWindow*)this, nParam);
		break;
	case WND_M_POPUPMENU:
		if (m_pParentWnd)
		{
			m_pParentWnd->WndProc(WND_M_POPUPMENU, (unsigned int)(KWndWindow*)this, nParam);
		}
		break;
	default:
		if (m_pParentWnd)
			return m_pParentWnd->WndProc(uMsg, uParam, nParam);
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KScrollMessageListBox::InitMinMaxLineCount(int nMin, int nMax)
{
	m_nMinLineCount = nMin;
	m_nMaxLineCount = nMax;
	if (m_nMinLineCount < 0)
		m_nMinLineCount = 0;
	if (m_nMaxLineCount <= m_nMinLineCount)
		m_nMaxLineCount = m_nMinLineCount + 1;
}

int KScrollMessageListBox::HeightToLineHeight(int nHeight)
{
	if (m_nLineHeight <= 0)
		return 0;

	return nHeight / m_nLineHeight * m_nLineHeight;
}

int KScrollMessageListBox::WidthToCharWidth(int nWidth)
{
	if (m_nLineHeight <= 0)
		return 0;

	return nWidth / m_nLineHeight * m_nLineHeight;
}

int KScrollMessageListBox::GetMinHeight()
{
	return max(m_MsgList.GetMinHeight(), m_Scroll.GetMinHeight());
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
//	���ܣ���ini��ȡ���ݵ�KWndMessageListBoxʵ��
//  ������pBox ->  KWndMessageListBoxʵ��ָ��
//		  pFile -> Ini�����ļ�ָ��
//		  pszSection -> Ini�д洢��Ҫ��ȡ���ݵĶεı�ʶ
//--------------------------------------------------------------------------
bool MsgListBox_LoadContent(KWndMessageListBox* pBox, KIniFile* pFile, const char* pszSection)
{
	if (pBox == NULL || pFile == NULL || pszSection == NULL)
		return false;
	pBox->Clear();
	char	Content[516] = "", szIndex[8];
	int		nIndex = 0;
	int		nCapability = 0;
	while(true)
	{
		itoa(nIndex, szIndex, 10);
		if (!pFile->GetString(pszSection, szIndex, "", Content, sizeof(Content)))
			break;
		int	nLen = strlen(Content);
		nLen = TEncodeText(Content, nLen);
		if (nLen <= 0)
		{
			Content[0] = ' ';
			Content[1] = 0;
			nLen = 1;
		}
		if (nIndex == nCapability)
		{
			nCapability += 16;
			pBox->SetCapability(nCapability);
		}
		pBox->AddOneMessage(Content, nLen);
		nIndex++;
	};
	return true;
}

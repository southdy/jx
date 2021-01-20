/*****************************************************************************************
//	���洰����ϵ�ṹ--��������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "WndButton.h"
#include "Wnds.h"
#include "MouseHover.h"

#include "../UiSoundSetting.h"

#include "../../../Engine/src/Text.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

KWndButton*	 KWndButton::m_pPressedDownBtn = NULL;
unsigned int KWndButton::ms_uBtnTipTextColor = 0xffffffff;	//��ť��ʾ�������ֵ���ɫ
int			 KWndButton::ms_nDisableBtnTip   = false;		//�Ƿ�ֹ��ť����ʾ����

//���ð�ť��ʾ�������ֵ���ɫ
void KWndButton::SetAllButtonTipTextColor(unsigned int uColor)
{
	ms_uBtnTipTextColor = uColor;
}

//��ֹ/����ť����ʾ����
void KWndButton::EnableAllButtonTip(int bEnable)
{
	ms_nDisableBtnTip = !bEnable;
}

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KWndButton::KWndButton()
{
	m_Flag		 = 0;
	m_nUpFrame   = 0;
	m_nDownFrame = 0;
	m_nCheckOverFrame =0;
	m_nOverStartFrame = 0;
	m_nDisableFrame = -1;
	m_szTip[0] = 0;
	m_nTipLen = 0;
}

void KWndButton::Clone(KWndButton* pCopy)
{
	if (pCopy)
	{
		KWndImage::Clone(pCopy);
		pCopy->m_nUpFrame		= m_nUpFrame;
		pCopy->m_nDownFrame		= m_nDownFrame;
		pCopy->m_nDisableFrame	= m_nDisableFrame;
		pCopy->m_nOverStartFrame= m_nOverStartFrame;
		pCopy->SetFrame(m_nUpFrame);
		pCopy->m_Flag = m_Flag & WNDBTN_ES_FILTER;
		memcpy(pCopy->m_szTip, m_szTip, sizeof(m_szTip));
		pCopy->m_nTipLen = m_nTipLen;
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ������
//--------------------------------------------------------------------------
int KWndButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndImage::Init(pIniFile, pSection))
	{
		int	nValue;
		pIniFile->GetInteger(pSection, "Up",  0, &nValue);
		m_nUpFrame = nValue;
		pIniFile->GetInteger(pSection, "Down",1, &nValue);
		m_nDownFrame = nValue;
		SetFrame((m_Flag & WNDBTN_F_DOWN) ? m_nDownFrame : m_nUpFrame);

		pIniFile->GetInteger(pSection, "DisableFrame", -1, &nValue);
		m_nDisableFrame = nValue;
		if (m_nDisableFrame >= 0 && IsDisable())
			SetFrame(m_nDisableFrame);

		int	bBool = 0;
		pIniFile->GetInteger(pSection, "NoOverSound", 0, &bBool);
		if (bBool)
			m_Flag |= WNDBTN_ES_NO_OVERSOUND;
		else
			m_Flag &= ~WNDBTN_ES_NO_OVERSOUND;

		pIniFile->GetInteger(pSection, "Over", 0, &bBool);
		if (bBool)
		{
			m_Flag |= WNDBTN_ES_ANIMATION;
			pIniFile->GetInteger(pSection, "OverFrame", 2, &nValue);
			m_nOverStartFrame = nValue;
		}
		else
			m_Flag &= ~WNDBTN_ES_ANIMATION;
		pIniFile->GetInteger(pSection, "CheckBox", 0, &nValue);
		if (nValue)
		{
			m_Flag |= WNDBTN_ES_CHECKBOX;
			pIniFile->GetInteger(pSection, "CheckOver", 0, &nValue);
			m_nCheckOverFrame = nValue;
		}
		else
			m_Flag &= ~WNDBTN_ES_CHECKBOX;

		pIniFile->GetInteger(pSection, "SendHoldMsg", 0, &nValue);
		if (nValue)
			m_Flag |= WNDBTN_ES_SEND_HOLD_MSG;
		else
			m_Flag &= ~WNDBTN_ES_SEND_HOLD_MSG;

		m_nTipLen = 0;
		if (pIniFile->GetString(pSection, "Tip", "", m_szTip, sizeof(m_szTip)))
		{
			m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
		}

		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ���ť�Ƿ���ڰ���״̬
//--------------------------------------------------------------------------
int KWndButton::IsButtonChecked()
{
	return (m_Flag & WNDBTN_F_CHECKED);
}

//--------------------------------------------------------------------------
//	���ܣ����ð�ť�İ���״̬
//--------------------------------------------------------------------------
void KWndButton::CheckButton(int bChecked)
{
	if (bChecked)
	{
		m_Flag |= WNDBTN_F_CHECKED;
		SetFrame(m_nDownFrame);
	}
	else
	{
		m_Flag &= ~WNDBTN_F_CHECKED;
		SetFrame(m_nUpFrame);
	}
}

int	KWndButton::IsButtonActive()
{
	return (m_Flag & WNDBTN_F_OVER);
}

//--------------------------------------------------------------------------
//	���ܣ����ں�����������Ϣ��
//--------------------------------------------------------------------------
int KWndButton::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (IsDisable())
		return KWndWindow::WndProc(uMsg, uParam, nParam);

	switch(uMsg)
	{
	case WM_LBUTTONUP:
		OnLBtnUp();
		break;
	case WM_LBUTTONDOWN:
		OnLBtnDown(false);
		break;
	case WND_M_MOUSE_ENTER:
	case WM_MOUSEMOVE:
		if (uParam & MK_LBUTTON)
			OnLBtnDownMove();
		if ((m_Flag & WNDBTN_F_OVER) == 0 && m_pPressedDownBtn != this)
		{
			if (m_pParentWnd)
				m_pParentWnd->WndProc(WND_N_BUTTON_OVER, (unsigned int)(KWndWindow*)this, 0);
			m_Flag |= WNDBTN_F_OVER;

			if ((m_Flag & WNDBTN_ES_NO_OVERSOUND) == 0)
				UiSoundPlay(UI_SI_OVER_OUTGAME_WND_BTN);
			if (m_Flag & WNDBTN_ES_ANIMATION)
			{
				if ((m_Flag & WNDBTN_F_DOWN) == 0)
					SetFrame(m_nOverStartFrame);
				else if (m_Flag & WNDBTN_ES_CHECKBOX)
					SetFrame(m_nCheckOverFrame);
			}
		}
		break;
	case WND_M_MOUSE_LEAVE:
		m_Flag &= ~WNDBTN_F_OVER;
		if (m_pPressedDownBtn == this && m_Flag & WNDBTN_F_DOWN)
		{
			m_Flag &= ~WNDBTN_F_DOWN;
			SetFrame(m_nUpFrame);
		}
		else if (m_Flag & WNDBTN_ES_ANIMATION)
			SetFrame((m_Flag & WNDBTN_F_DOWN) ? m_nDownFrame : m_nUpFrame);
		if (g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
			g_MouseOver.CancelMouseHoverInfo();
		KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	case WM_MOUSEHOVER:
		if (m_pPressedDownBtn == this &&
			(m_Flag & WNDBTN_ES_SEND_HOLD_MSG) && m_pParentWnd)
		{
			m_pParentWnd->WndProc(WND_N_BUTTON_HOLD, (unsigned int)(KWndWindow*)this, 0);
		}
		else if (ms_nDisableBtnTip == false && m_szTip[0] && !g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
		{
			char szTip[64];
			
			int nTipLen = GetToolTipInfo(szTip, 63);
			szTip[nTipLen] = 0;
			
			g_MouseOver.SetMouseHoverInfo((void*)(KWndWindow*)this, 0,
				LOWORD(nParam), HIWORD(nParam), false, true);
			g_MouseOver.SetMouseHoverTitle(szTip, nTipLen, ms_uBtnTipTextColor);
		}
		break;
	case WM_LBUTTONDBLCLK:
		OnLBtnDown(true);
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

int	KWndButton::GetToolTipInfo(char* szTip, int nMax)
{
	int nTipLen = min(m_nTipLen, nMax);
	memcpy(szTip, m_szTip, nTipLen);
	const char* pKey = GetShortKey();
	if (pKey && pKey[0] != 0 && (nTipLen + (int)strlen(pKey)) < nMax)
	{
		szTip[nTipLen++] =  '(';
		strcpy(szTip + nTipLen, pKey);
		nTipLen += strlen(pKey);
		szTip[nTipLen++] =  ')';
	}

	return nTipLen;
}

int KWndButton::SetToolTipInfo(char* szTip, int nMax)
{
	m_nTipLen = min(nMax, sizeof(m_szTip) - 1);
	memcpy(m_szTip, szTip, m_nTipLen);
	m_szTip[m_nTipLen] = 0;
	m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
	return m_nTipLen;
}


//--------------------------------------------------------------------------
//	���ܣ��������
//--------------------------------------------------------------------------
void KWndButton::PaintWindow()
{
	if ((m_Flag & (WNDBTN_F_OVER | WNDBTN_ES_ANIMATION | WNDBTN_ES_CHECKBOX)) == (WNDBTN_F_OVER | WNDBTN_ES_ANIMATION))
	{
		NextFrame();
		if (m_Image.nFrame < m_nOverStartFrame)
			SetFrame(m_nOverStartFrame);
	}
	KWndImage::PaintWindow();
#ifdef _DEBUG
	if (WND_SHOW_DEBUG_FRAME_TEXT && g_pRepresentShell &&
		m_Image.szImage[0] == 0 && (m_Flag & WNDBTN_F_DOWN) &&
		(WNDBTN_ES_CHECKBOX & m_Flag))
	{
		int x = m_nAbsoluteLeft + strlen(m_Caption) * 8;
		g_pRepresentShell->OutputText(16, "[C]", 3, x + 1, m_nAbsoluteTop + 1, 0xFF000000, 0);
		g_pRepresentShell->OutputText(16, "[C]", 3, x, m_nAbsoluteTop, 0xFFFFFFFF, 0);
	}
#endif
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�������ڴ˰���
//--------------------------------------------------------------------------
void KWndButton::OnLBtnDown(bool bDoubleClick)
{
	m_Flag &= ~WNDBTN_F_OVER;
	if ((m_Flag & WNDBTN_ES_CHECKBOX) == 0)
		m_pPressedDownBtn = this;
	else
		m_pPressedDownBtn = NULL;
	
	if ((m_Flag & WNDBTN_ES_CHECKBOX) == 0)
	{
		m_Flag |= WNDBTN_F_DOWN;
		SetFrame(m_nDownFrame);
		if (m_pParentWnd)
		{
			m_pParentWnd->WndProc(bDoubleClick ? WND_N_BUTTON_DB_CLICK : WND_N_BUTTON_DOWN,
				(unsigned int)(KWndWindow*)this, 0);
		}
	}
	else
	{
		if (m_Flag & WNDBTN_F_CHECKED)
		{
			m_Flag &= ~WNDBTN_F_CHECKED;
			SetFrame((m_Flag & WNDBTN_ES_ANIMATION) ? m_nOverStartFrame : m_nUpFrame);
		}
		else
		{
			m_Flag |= WNDBTN_F_CHECKED;
			SetFrame((m_Flag & WNDBTN_ES_ANIMATION) ? m_nCheckOverFrame : m_nDownFrame);
		}
		OnButtonClick();
		if (m_pParentWnd)
			m_pParentWnd->WndProc(WND_N_BUTTON_CLICK,
				(unsigned int)(KWndWindow*)this, (m_Flag & WNDBTN_F_CHECKED));
	}
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�������ڴ˷ſ�
//--------------------------------------------------------------------------
void KWndButton::OnLBtnUp()
{
	if (m_pPressedDownBtn == this && (m_Flag & WNDBTN_F_DOWN))
	{
		m_Flag &= ~WNDBTN_F_DOWN;
		SetFrame(m_nUpFrame);
		OnButtonClick();
		if (m_pParentWnd)
			m_pParentWnd->WndProc(WND_N_BUTTON_CLICK, (unsigned int)(KWndWindow*)this, 0);
	}
	m_pPressedDownBtn = NULL;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ����������ʱ���ƶ�
//--------------------------------------------------------------------------
void KWndButton::OnLBtnDownMove()
{
	if (m_pPressedDownBtn == this)
	{
		if ((m_Flag & WNDBTN_F_DOWN) == 0)
		{
			m_Flag |= WNDBTN_F_DOWN;
			SetFrame(m_nDownFrame);
		}
	}
}

//��ֹ��������ʹ���ڱ�����
void KWndButton::Enable(int bEnable)
{
	KWndWindow::Enable(bEnable);
	if (m_nDisableFrame >= 0)
	{
		if (bEnable)
			SetFrame((m_Flag & WNDBTN_F_DOWN) ? m_nDownFrame : m_nUpFrame);
		else
			SetFrame(m_nDisableFrame);
	}
}

///////////////////////////////////////////////////////////////////////////////////

KWndImageTextButton::KWndImageTextButton()
{
	bPart = true;
}

int	KWndImageTextButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Part", 1, &bPart);
		char szName[32];
		sprintf(szName, "%s_Image", pSection);
		if (bPart)
			m_ImagePart.Init(pIniFile, szName);
		else
			m_Image.Init(pIniFile, szName);

		sprintf(szName, "%s_Text", pSection);
		m_Text.Init(pIniFile, szName);
		
		if (bPart)
			AddChild(&m_ImagePart);
		else
			AddChild(&m_Image);
		AddChild(&m_Text);

		m_Text.Enable(false);
		m_Image.Enable(false);
		m_ImagePart.Enable(false);
		return true;
	}
	return false;
}

void KWndImageTextButton::Set2IntText(int nNumber1, int nNumber2, char Separator)
{
	m_Text.Set2IntText(nNumber1, nNumber2, Separator);
}

void KWndImageTextButton::SetIntText(int nNumber, char Separator)
{
	m_Text.SetIntText(nNumber, Separator);
}

void KWndImageTextButton::SetText(const char* pText, int nLen)	//�����ı�����
{
	m_Text.SetText(pText, nLen);
}

int	KWndImageTextButton::GetText(char* pBuffer, int nSize)		//��ȡ�ַ�������
{
	return m_Text.GetText(pBuffer, nSize);
}

void KWndImageTextButton::Set2IntValue(int nNumber1, int nNumber2)
{
	if (bPart)
		m_ImagePart.SetPart(nNumber1, nNumber2);
	else
		m_Image.SetMoveValue(nNumber1, nNumber2);
}

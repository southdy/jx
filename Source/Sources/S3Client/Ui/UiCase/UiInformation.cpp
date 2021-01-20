/*****************************************************************************************
//	����--��Ϣ����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-14
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"

#define	SCHEME_INI	"��ʾ.ini"

KUiInformation	g_UiInformation;

void UIMessageBox(const char* pMsg,
				  KWndWindow* pCaller /*= 0*/,
				  const char* pszFirstBtnText /*= "ȷ��"*/,
				  const char* pszSecondBtnText /*= 0*/,				  
				  unsigned int uParam /*= 0*/)
{
	g_UiInformation.Show(pMsg, pszFirstBtnText, pszSecondBtnText, pCaller, uParam);
}

void UiCloseMessageBox()
{
	g_UiInformation.Close();
}

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KUiInformation::KUiInformation()
{
	m_pCallerWnd = NULL;
	m_nOrigFirstBtnXPos = 0;
	m_nCentreBtnXPos = 0;
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiInformation::Initialize()
{
	m_pCallerWnd = NULL;
	AddChild(&m_Information);
	AddChild(&m_FirstBtn);
	AddChild(&m_SecondBtn);
	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiInformation::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_Information .Init(&Ini, "Info");
		m_FirstBtn .Init(&Ini, "FirstBtn");
		m_SecondBtn.Init(&Ini, "SecondBtn");
		m_FirstBtn.GetPosition(&m_nOrigFirstBtnXPos, 0);
		m_FirstBtn.GetSize(&m_nCentreBtnXPos, 0);
		m_nCentreBtnXPos = (m_Width - m_nCentreBtnXPos) / 2;
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʾ����
//--------------------------------------------------------------------------
void KUiInformation::Show(const char* pInformation,
				const char* pszFirstBtnText,
				const char* pszSecondBtnText /*= 0*/,
				KWndWindow* pCallerWnd /*= 0*/,
				unsigned int uParam /*= 0*/,
                int nInformationLen /*= -1*/)
{
	if (pInformation)
	{
		m_pCallerWnd = pCallerWnd;
		m_uCallerParam = uParam;
		m_Information.SetText(pInformation, nInformationLen);

		if (pszSecondBtnText && pszFirstBtnText == NULL)
		{
			pszFirstBtnText = pszSecondBtnText;
			pszSecondBtnText = NULL;
		}

		if (pszFirstBtnText)
		{
			m_FirstBtn.SetText(pszFirstBtnText);
			m_FirstBtn.Show();
			int y;
			m_FirstBtn.GetPosition(0, &y);
			if (pszSecondBtnText == NULL)
				m_FirstBtn.SetPosition(m_nCentreBtnXPos, y);
			else
				m_FirstBtn.SetPosition(m_nOrigFirstBtnXPos, y);
		}
		else
		{
			m_FirstBtn.Hide();
		}

		if (pszSecondBtnText)
		{
			m_SecondBtn.SetText(pszSecondBtnText);
			m_SecondBtn.Show();
		}
		else
		{
			m_SecondBtn.Hide();
		}
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		BringToTop();
		KWndShowAnimate::Show();
		Wnd_SetExclusive((KWndWindow*)this);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ش���
//--------------------------------------------------------------------------
void KUiInformation::Hide(int nBtnIndex)
{
	if (m_pCallerWnd)
	{
		m_pCallerWnd->WndProc(WND_M_OTHER_WORK_RESULT,
			m_uCallerParam, WND_OPER_RESULT(nBtnIndex));
		m_pCallerWnd = NULL;
	}
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndShowAnimate::Hide();
}

//�رմ��ڣ���ͨ���ô���
void KUiInformation::Close()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndShowAnimate::Hide();
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KUiInformation::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_FirstBtn)
			Hide(0);
		else if (uParam == (unsigned int)(KWndWindow*)&m_SecondBtn)
			Hide(1);
		break;
	case WM_KEYDOWN:
		if (uParam  == VK_RETURN || uParam == VK_SPACE || uParam == VK_ESCAPE)
		{
			if (!m_SecondBtn.IsVisible())
			{
				Hide(m_FirstBtn.IsVisible() ? 0 : -1);
				nRet = true;
			}
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}
// -------------------------------------------------------------------------
//	�ļ���		��	UiGetMoney.cpp
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-1-7
//	��������	��	ȡǮ����
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiGetMoney.h"
#include "../UiSoundSetting.h"

#define	SCHEME_INI 	"ȡǮ����.ini"


KUiGetMoney* KUiGetMoney::m_pSelf = NULL;

KUiGetMoney::KUiGetMoney()
{
	m_pRequester = NULL;
	m_nMaxMoney = 0;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiGetMoney* KUiGetMoney::OpenWindow(int nMoney, int nMaxMoney,
				KWndWindow* pRequester, unsigned int uParam, KWndWindow* pMoneyWnd)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiGetMoney;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_pRequester = pRequester;
		m_pSelf->m_uRequesterParam = uParam;
		if (nMaxMoney > 0)
			m_pSelf->m_nMaxMoney = nMaxMoney;
		else
			m_pSelf->m_nMaxMoney = 0;
		m_pSelf->m_Money.SetIntText(nMoney);
		m_pSelf->Show(pMoneyWnd);
		m_pSelf->BringToTop();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��ر����ٴ���
//--------------------------------------------------------------------------
void KUiGetMoney::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_pRequester = NULL;
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

KUiGetMoney*	KUiGetMoney::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//��ʾ����
void KUiGetMoney::Show(KWndWindow* pMoneyWnd)
{
	int Left, Top;
	if (pMoneyWnd)
	{
		pMoneyWnd->GetAbsolutePos(&Left, &Top);
		int	nWidth;
		pMoneyWnd->GetSize(&nWidth, NULL);
		Left += nWidth / 2;
		ALW_GetWndPosition(Left, Top, m_Width, m_Height, true);
	}
	else
		ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_Money);
	Wnd_SetExclusive((KWndWindow*)this);
}

//���ش���
void KUiGetMoney::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KUiGetMoney::Initialize()
{
	AddChild(&m_Money);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiGetMoney::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Money.Init(&Ini, "MoneyInput");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		}
	}	
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϣ����
//--------------------------------------------------------------------------
int KUiGetMoney::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_Money)
		{
			OnOk();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ���ȷ�ϰ�ť
//--------------------------------------------------------------------------
void KUiGetMoney::OnOk()
{
	OnCheckInput();
	int nMoney = m_Money.GetIntNumber();
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, WND_OPER_RESULT(nMoney));
	CloseWindow(true);
}

void KUiGetMoney::OnCheckInput()
{
	int nMoney = m_Money.GetIntNumber();
	if (nMoney < 0)
		nMoney = 0;
	else if (nMoney > m_nMaxMoney)
		nMoney = m_nMaxMoney;
	char	szBuff1[16], szBuff2[16];
	itoa(nMoney, szBuff1, 10);
	m_Money.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_Money.SetIntText(nMoney);
}

void KUiGetMoney::OnCancel()
{
	if (m_pRequester)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, WND_OPER_RESULT(-1));
	CloseWindow(true);
}

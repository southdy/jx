//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2002 by Kingsoft
//
// File:	KIme.cpp
// Date:	2002.07.18
// Code:	��ѧ��,wooy
// Desc:	KIme Class
//---------------------------------------------------------------------------
#include "kwin32.h"
#include "KEngine.h"
#include "KIme.h"
#include "KStrBase.h"
#include "KWin32Wnd.h"

ENGINE_API KIme* g_pIme = NULL;
//----------------------------------------------------------------------------
//���캯������ʼ����Ա
//----------------------------------------------------------------------------
KIme::KIme()
{
    g_pIme = this;
	m_bIsWorking=TRUE;
	m_bNoLanguageChange=FALSE;
    m_bCloseFlag = false; 
    m_bCloseFlagForCanChange = false;
    m_dwConversion = 0;
    m_dwSentence = 0;
    m_bCloseIMESimHotKey = false;
    m_nCaretX = 0;
    m_nCaretY = 0;
}

//----------------------------------------------------------------------------
//�����������ͷ��ڴ�
//----------------------------------------------------------------------------
KIme::~KIme()
{
	g_pIme=NULL;
}

//----------------------------------------------------------------------------
//��λIME
//----------------------------------------------------------------------------
void KIme::ResetIme(void)
{
	m_bIsWorking=TRUE;
	m_bNoLanguageChange=FALSE;
}

BOOL KIme::IsIme()
{
	HIMC hImc = ImmGetContext(g_GetMainHWnd());
	BOOL bRet = FALSE;
	if (hImc)
	{
		bRet = (ImmGetOpenStatus(hImc));
		ImmReleaseContext(g_GetMainHWnd(), hImc);
	}
	return bRet;
//	return (ImmIsIME( GetKeyboardLayout(0)));
}

//----------------------------------------------------------------------------
//�����������������״̬
//----------------------------------------------------------------------------
void KIme::OpenIME()
{
	HIMC hImc = ImmGetContext(g_GetMainHWnd());
	if (hImc)
	{
		if (ImmGetOpenStatus(hImc) == FALSE)
		{
			bool bOld = m_bNoLanguageChange;
			m_bNoLanguageChange = false;
			ImmSetOpenStatus(hImc, TRUE);
			m_bNoLanguageChange = bOld;
			HWND hWnd = ImmGetDefaultIMEWnd(::g_GetMainHWnd());
			::SendMessage(hWnd, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0);
		}
        if (m_bCloseFlag)
        {
            ImmSetConversionStatus(hImc, m_dwConversion, m_dwSentence);
        }

        int nScreenX = GetSystemMetrics(SM_CXSCREEN);
        int nScreenY = GetSystemMetrics(SM_CYSCREEN);

        POINT ptWindowPos = {0, 0}; 
        //ImmGetStatusWindowPos(hImc, &ptWindowPos);

		//ImmNotifyIME(hImc, NI_CLOSECANDIDATE, 0, 0);
		//ImmNotifyIME(hImc, NI_OPENCANDIDATE, 0, 0);


//        if (
//            !(
//                (ptWindowPos.x >= 0) &&
//                (ptWindowPos.x <= nScreenX) &&
//                (ptWindowPos.y >= 0) &&
//                (ptWindowPos.y <= nScreenY)
//            )
//        )
        {
            ptWindowPos.x =  nScreenX - 100;
            ptWindowPos.y =  nScreenY - 50;
            //ImmSetStatusWindowPos(hImc, &ptWindowPos);
        }

		ImmReleaseContext(g_GetMainHWnd(), hImc);
	}

	if (m_bCloseIMESimHotKey)
    {
        //  ���ַ����ڴ���Ǳ�׼��ʱ�����������
	    if(!ImmIsIME( GetKeyboardLayout( 0 )))
	    {
		    ImmSimulateHotKey(g_GetMainHWnd(),IME_CHOTKEY_IME_NONIME_TOGGLE);
	    }

        m_bCloseIMESimHotKey = false;
    }

    m_bCloseFlag = false;
}

//----------------------------------------------------------------------------
//�ر������������Ӣ��״̬
//----------------------------------------------------------------------------
void KIme::CloseIME()
{
    m_bCloseFlag = true;
    m_bCloseFlagForCanChange = true;

    HIMC hImc = ImmGetContext(g_GetMainHWnd());
	if (hImc)
	{
		if (ImmGetOpenStatus(hImc))
		{
			bool bOld = m_bNoLanguageChange;
			m_bNoLanguageChange = false;
			ImmSetOpenStatus(hImc, FALSE);
			m_bNoLanguageChange = bOld;
			HWND hWnd = ImmGetDefaultIMEWnd(::g_GetMainHWnd());
			//::SendMessage(hWnd, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0);
			ImmGetConversionStatus(hImc, &m_dwConversion, &m_dwSentence);
		}
        ImmSetConversionStatus(hImc, IME_CMODE_CHARCODE, IME_SMODE_NONE);

		ImmReleaseContext(g_GetMainHWnd(), hImc);
	}

//  ���ַ����ڴ���Ǳ�׼���뷨ʱ���������`
	//while(ImmIsIME( GetKeyboardLayout( 0 )))
	//	ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
	//if(ImmIsIME( GetKeyboardLayout( 0 )))
	//{
	//	ImmSimulateHotKey(g_GetMainHWnd(),IME_CHOTKEY_IME_NONIME_TOGGLE);
    //    m_bCloseIMESimHotKey = true;
	//}
}

//----------------------------------------------------------------------------
//ֹͣ����Ϣ����Ӧ����ͣ����
//----------------------------------------------------------------------------
void KIme::TurnOff()
{
	m_bIsWorking=FALSE;
}

//----------------------------------------------------------------------------
//�ָ�����Ϣ����Ӧ����ʼ����
//----------------------------------------------------------------------------
void KIme::TurnOn()
{
	m_bIsWorking=TRUE;
}

//----------------------------------------------------------------------------
//���������л�
//----------------------------------------------------------------------------
void KIme::EnableLanguageChange()
{
	m_bNoLanguageChange = false;
    m_bCloseFlagForCanChange = false;
}

//----------------------------------------------------------------------------
//��ֹ�����л�
//----------------------------------------------------------------------------
void KIme::DisableLanguageChange()
{
	m_bNoLanguageChange=TRUE;
}

void KIme::SetCaretPos(int X, int Y)
{
    //if (m_bCloseFlag)
    //    return;

    BOOL bRetCode = false;

    //if (
    //    (m_nCaretX == X) &&
    //    (m_nCaretY == Y)
    //)
    //    return;

    m_nCaretX = X;
    m_nCaretY = Y;

	HIMC hImc = ImmGetContext(g_GetMainHWnd());
	if (hImc)
	{
        COMPOSITIONFORM cf;

        //cf.dwStyle = CFS_POINT;

        //bRetCode = ImmGetCompositionWindow(hImc, &cf);
        //if (bRetCode)
        //{
            cf.dwStyle = CFS_POINT;
            cf.ptCurrentPos.x = m_nCaretX;
            cf.ptCurrentPos.y = m_nCaretY;
            ImmSetCompositionWindow(hImc, &cf);

			//CANDIDATEFORM cdf;
            //
			//cdf.dwStyle = CFS_CANDIDATEPOS;
			//cdf.dwIndex = 0;
			//cdf.ptCurrentPos.x = m_nCaretX;
			//cdf.ptCurrentPos.y = m_nCaretY;
			//ImmSetCandidateWindow(hImc, &cdf);

        //}

        ImmReleaseContext(g_GetMainHWnd(), hImc);
    }


}

//----------------------------------------------------------------------------
//��Ϣ������
//----------------------------------------------------------------------------
int	KIme::WndMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_bIsWorking)
	{
		if (uMsg == WM_INPUTLANGCHANGEREQUEST)
        {
            //if (m_bCloseFlag)
            //{
            //    return true;

                //if (m_bCloseFlagForCanChange)
                //{
                //    m_bCloseFlagForCanChange = false;
                //    return true;
                //}
                //
                //return false;
            //}

            if (!m_bNoLanguageChange)
            {
				// ��������л����뷨
                DefWindowProc(hWnd, uMsg, wParam, lParam);
                
				SetCaretPos(m_nCaretX, m_nCaretY);

				return true;
            }

			return m_bNoLanguageChange;
        }

		else if (uMsg == WM_IME_NOTIFY)
        {
            if (
                (wParam == IMN_SETOPENSTATUS) &&
                m_bNoLanguageChange
            )
            {
                CloseIME();
                return true;
            }
            else if (wParam  == IMN_SETCONVERSIONMODE)
            {
                
                if (m_bNoLanguageChange)
                {
					static int s_nInFlag = false;

					if (s_nInFlag)
						return false;
					
					s_nInFlag = true;

                    DWORD dwConversion = IME_CMODE_CHARCODE;
                    DWORD dwSentence = IME_SMODE_NONE;
                    HIMC hImc = ImmGetContext(g_GetMainHWnd());
	                if (hImc)
	                {
                        ImmGetConversionStatus(hImc, &dwConversion, &dwSentence);
                        
                        //if (
                        //    (dwConversion != IME_CMODE_CHARCODE) || 
                        //    (dwSentence != IME_SMODE_NONE)
                        //)
                        //{
                        //    ImmSetConversionStatus(hImc, IME_CMODE_CHARCODE, IME_SMODE_NONE);
                        //}

		                ImmReleaseContext(g_GetMainHWnd(), hImc);
	                }

					s_nInFlag = false;
                }
                return false;
            }

		}
	}
	return FALSE;
}


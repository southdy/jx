// -------------------------------------------------------------------------
//	�ļ���		��	UiSelDlg.cpp
//	������		��	����
//	����ʱ��	��	2002-9-10 11:27:04
//	��������	��	ѡ��Ի���
//
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiSelDlg.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI_SELDLG	"����ѡ�����.ini"

KUiSelDlg	g_UiSelDlg;

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KUiSelDlg::Initialize()
{
	AddChild(&m_IlluminateTxt);
	for (int i = 0; i < KUISEL_MAX_OPTION; i++)
		AddChild(&m_OptionBtn[i]);	
	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this);
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiSelDlg::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SELDLG);
	if (Ini.Load(Buff))
	{
		KWndImage::    Init(&Ini, "Main");
		for (int i = 0; i < KUISEL_MAX_OPTION; i++)
		{
			sprintf(Buff, "Option_%d", i);
			m_OptionBtn[i].Init(&Ini, Buff);
		}
		m_IlluminateTxt.Init(&Ini, "InfoText");
	}
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϣ����
//--------------------------------------------------------------------------
int KUiSelDlg::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg != WND_N_BUTTON_CLICK)
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	OnClickButton((KWndWindow*)uParam);
	return 0;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����ť
//--------------------------------------------------------------------------
void KUiSelDlg::OnClickButton(KWndWindow* pWnd)
{
	for (int i = 0; i < KUISEL_MAX_OPTION; i++)
		if (pWnd == (KWndWindow*)&m_OptionBtn[i])
		{
			Wnd_ReleaseExclusive(this);
			Hide();
			g_pCoreShell->OperationRequest(GOI_QUESTION_CHOOSE, 0, i);			
			break;
		}
}

//--------------------------------------------------------------------------
//	���ܣ���ʾ����
//--------------------------------------------------------------------------
void KUiSelDlg::Show(KUiQuestionAndAnswer* pContent)
{
	if (pContent && pContent->AnswerCount >= 0)
	{
		m_IlluminateTxt.SetText(pContent->Question, pContent->QuestionLen);
		int i = 0;
		if (pContent->AnswerCount == 0)
		{
			m_OptionBtn[i ].SetLabel("����", 4);
			m_OptionBtn[i++].Show();
		}
		else
		for (i = 0; i < pContent->AnswerCount && i < KUISEL_MAX_OPTION; i++)
		{
			m_OptionBtn[i].SetLabel(pContent->Answer[i].AnswerText, pContent->Answer[i].AnswerLen);
			m_OptionBtn[i].Show();
		}
		while (i < KUISEL_MAX_OPTION)
			m_OptionBtn[i++].Hide();
	}
	KWndImage::Show();
	Wnd_SetExclusive(this);
}
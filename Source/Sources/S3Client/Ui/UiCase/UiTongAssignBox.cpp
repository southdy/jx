/*******************************************************************************
File        : UiTongAssignBox.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : �������ְλ�Ľ���
********************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"

#include "../elem/wnds.h"
#include "../elem/wndmessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "../../../Engine/src/KFilePath.h"
#include "../../Core/Src/GameDataDef.h"

#include "UiTongAssignBox.h"
#include "UiInformation.h"
#include "UiTongGetString.h"


#define TONG_ASSIGN_BOX_INI "���ְλ����.ini"


extern iCoreShell *g_pCoreShell;

KUiTongAssignBox* KUiTongAssignBox::ms_pSelf = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiTongAssignBox::KUiTongAssignBox()
{
	m_pMain = NULL;
	m_nSelectFigure = -1;
	m_szTargetPlayerName[0] = 0;
	m_szTargetPlayerAgName[0] = 0;
}

KUiTongAssignBox::~KUiTongAssignBox()
{

}


/*********************************************************************
* ���ܣ��򿪴���
**********************************************************************/
KUiTongAssignBox* KUiTongAssignBox::OpenWindow()
{
	if(g_pCoreShell)
	{
    	if (ms_pSelf == NULL)
    	{
		    ms_pSelf = new KUiTongAssignBox;
		    if (ms_pSelf)
    			ms_pSelf->Initialize();
    	}
    	if (ms_pSelf)
    	{
		    UiSoundPlay(UI_SI_WND_OPENCLOSE);
			ms_pSelf->BringToTop();
			ms_pSelf->Show();
	    }
	}
	return ms_pSelf;
}


/*********************************************************************
* ���ܣ��������������ʾ���򷵻�ʵ��ָ��
**********************************************************************/
KUiTongAssignBox* KUiTongAssignBox::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* ���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
**********************************************************************/
void KUiTongAssignBox::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
		ms_pSelf->Hide();
		if (bDestory)
		{
			ms_pSelf->Destroy();
			ms_pSelf = NULL;
		}
	}
}


/*********************************************************************
* ���ܣ���ʼ��
**********************************************************************/
void KUiTongAssignBox::Initialize()
{
	AddChild(&m_BtnPilgarlic);
	AddChild(&m_TargetName);
	AddChild(&m_BtnCaptain);
	AddChild(&m_BtnConfirm);
	AddChild(&m_BtnCancel);
	AddChild(&m_BtnElder);
	AddChild(&m_TextError);

//	m_BtnPilgarlic.Hide();
//	m_TargetName.Hide();
//	m_BtnCaptain.Hide();
//	m_BtnConfirm.Hide();
//	m_BtnCancel.Hide();
//	m_BtnElder.Hide();
//	m_BtnConfirm.Enable(FALSE);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


/*********************************************************************
* ���ܣ�������淽��
**********************************************************************/
void KUiTongAssignBox::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, TONG_ASSIGN_BOX_INI);

		if(Ini.Load(Buff))
		{
			ms_pSelf->Init(&Ini, "Main");

			ms_pSelf->m_BtnPilgarlic.Init(&Ini, "BtnMember");
			ms_pSelf->m_BtnCaptain.Init(&Ini, "BtnManager");
			ms_pSelf->m_BtnConfirm.Init(&Ini, "BtnConfirm");
			ms_pSelf->m_BtnElder.Init(&Ini, "BtnDirector");
			ms_pSelf->m_BtnCancel.Init(&Ini, "BtnCancel");
			ms_pSelf->m_TargetName.Init(&Ini, "PlayerName");
			ms_pSelf->m_TextError.Init(&Ini, "ErrorBox");

			ms_pSelf->m_BtnElder.SetText("����");
			ms_pSelf->m_BtnCaptain.SetText("�ӳ�");
			ms_pSelf->m_BtnPilgarlic.SetText("����");

			Ini.GetString("Main", "AssignString", "", ms_pSelf->m_szAssign, sizeof(ms_pSelf->m_szAssign));
			Ini.GetString("Main", "CancelString", "", ms_pSelf->m_szCancel, sizeof(ms_pSelf->m_szCancel));
			Ini.GetString("Main", "ConfirmString", "", ms_pSelf->m_szConfirm, sizeof(ms_pSelf->m_szConfirm));
			Ini.GetString("Main", "ErrorFigure", "��ѡ��ְλ", ms_pSelf->m_szErrorNotSelectFigure, sizeof(ms_pSelf->m_szErrorNotSelectFigure));
		}
	}
}


/*********************************************************************
* ���ܣ����ں���
**********************************************************************/
int KUiTongAssignBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_BtnConfirm)
		{
			OnConfirm();
		}
		else if(uParam == (unsigned int)&m_BtnCancel)
		{
			if(m_pMain)
			{
    			m_pMain->Show();
	    		m_pMain = NULL;
			}
			CloseWindow();
		}
		else if(uParam == (unsigned int)&m_BtnElder)
		{
			UpdateCheckButton(enumTONG_FIGURE_DIRECTOR);
		}
		else if(uParam == (unsigned int)&m_BtnCaptain)
		{
			UpdateCheckButton(enumTONG_FIGURE_MANAGER);
		}
		else if(uParam == (unsigned int)&m_BtnPilgarlic)
		{
			UpdateCheckButton(enumTONG_FIGURE_MEMBER);
		}
		break;

	case WND_M_OTHER_WORK_RESULT:
		if(uParam == RESULT_T_ASSIGN)
		{
			if(nParam && g_pCoreShell)
			{
				KTongOperationParam Param;
    			KTongMemberItem Member;

			    Param.eOper = TONG_ACTION_ASSIGN;
			    Param.nData[0] = m_nCurrentFigure;
			    Param.nData[2] = m_nSelectFigure;
			    Param.nData[1] = 0;//m_nCurrentPostion;
			    Param.nData[3] = 0;

			    memset(&Member, 0, sizeof(KTongMemberItem));
			    strcpy(Member.Name, (char *)nParam);

 			    g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (unsigned int)&Param, (int)&Member);
			}
			if(m_pMain)
			{
    			m_pMain->Show();
	    		m_pMain = NULL;
			}
			CloseWindow();
		}
		break;

	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}

	return 1;
}


/*********************************************************************
* ���ܣ�������������������Ĺ���
**********************************************************************/
void KUiTongAssignBox::LinkToMainWindow(KWndWindow *pMain)
{
	if(pMain && ms_pSelf)
	{
		ms_pSelf->m_pMain = pMain;
		ms_pSelf->m_pMain->Hide();
	}
}


/*********************************************************************
* ���ܣ�Ϊ����������д��������
**********************************************************************/
void KUiTongAssignBox::ArrangeData(int nType, char* szTarName, char* szTarAgName, int nCurrentFigure, int nCurrentPosition)
{
	if((nType == enumTONG_FIGURE_DIRECTOR || nType == enumTONG_FIGURE_MASTER) &&
		szTarName && szTarName[0] && nCurrentFigure >= enumTONG_FIGURE_MEMBER &&
		nCurrentFigure <= enumTONG_FIGURE_DIRECTOR && ms_pSelf)
	{
		ms_pSelf->m_nType = nType;
		strcpy(ms_pSelf->m_szTargetPlayerName, szTarName);
		ms_pSelf->m_TargetName.SetText(szTarName);

		if(szTarAgName && szTarAgName[0])
		{
			strcpy(ms_pSelf->m_szTargetPlayerAgName, szTarAgName);
		}
		ms_pSelf->m_nCurrentFigure = nCurrentFigure;
		ms_pSelf->m_nCurrentPostion = nCurrentPosition;

		if(nType == enumTONG_FIGURE_DIRECTOR)
		{
			ms_pSelf->m_BtnElder.Hide();
			ms_pSelf->m_BtnCaptain.Show();
			ms_pSelf->m_BtnPilgarlic.Show();
		}
		else if(nType == enumTONG_FIGURE_MASTER)
		{
			ms_pSelf->m_BtnElder.Show();
			ms_pSelf->m_BtnCaptain.Show();
			ms_pSelf->m_BtnPilgarlic.Show();
		}
		if(nCurrentFigure == enumTONG_FIGURE_DIRECTOR)
		{
			ms_pSelf->m_BtnElder.Hide();
		}
		else if(nCurrentFigure == enumTONG_FIGURE_MANAGER)
		{
			ms_pSelf->m_BtnCaptain.Hide();
		}
		else if(nCurrentFigure == enumTONG_FIGURE_MEMBER)
		{
			ms_pSelf->m_BtnPilgarlic.Hide();
		}
	}
}


/*********************************************************************
* ���ܣ�Check������
**********************************************************************/
void KUiTongAssignBox::UpdateCheckButton(int nFigure)
{
	if(nFigure >= enumTONG_FIGURE_MEMBER && nFigure <= enumTONG_FIGURE_DIRECTOR)
	{
		m_BtnElder.CheckButton(FALSE);
		m_BtnCaptain.CheckButton(FALSE);
		m_BtnPilgarlic.CheckButton(FALSE);

		m_nSelectFigure = nFigure;
		switch(nFigure)
		{
		case enumTONG_FIGURE_MEMBER:
			m_BtnPilgarlic.CheckButton(TRUE);
			break;

		case enumTONG_FIGURE_MANAGER:
			m_BtnCaptain.CheckButton(TRUE);
			break;

		case enumTONG_FIGURE_DIRECTOR:
			m_BtnElder.CheckButton(TRUE);
			break;
		}
		m_BtnConfirm.Enable(TRUE);
	}
	else
	{
		m_BtnConfirm.Enable(FALSE);
	}
}


/*********************************************************************
* ���ܣ�����ȷ�ϴ���
**********************************************************************/
void KUiTongAssignBox::PopupConfirmWindow(const char* pszInfo, unsigned int uHandleID)
{
    KUiTongGetString::OpenWindow(pszInfo, m_szTargetPlayerName, this, uHandleID, 2, 32);
	Hide();
}


/*********************************************************************
* ���ܣ���Ӧȷ�ϲ���
**********************************************************************/
void KUiTongAssignBox::OnConfirm()
{
	if(m_nSelectFigure != -1)
	{
		PopupConfirmWindow(m_szAssign, RESULT_T_ASSIGN);
	}
	else
	{
		m_TextError.SetText(m_szErrorNotSelectFigure);
	}
}

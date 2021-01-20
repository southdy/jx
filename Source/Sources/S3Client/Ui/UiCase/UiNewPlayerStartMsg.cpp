/*****************************************************************************************
//	����--���ֽ������ʾ����
//	Copyright : Kingsoft 2003
//	Author	:   Freeway Chen
//	CreateTime:	2003-7-8
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "../UiShell.h"
#include "UiInit.h"
#include "UiNewPlayerStartMsg.h"
#include "UiHelper.h"
//#include "../UiSoundSetting.h"
#include <crtdbg.h>

#define	SCHEME_INI_NEWENTERINFO			"���ֽ���ĳ�ʼ����.ini"
#define	NEWENTERINFO_INI				"\\Ui\\NewEnterInfo.ini"

KUiNewPlayerStartMsg* KUiNewPlayerStartMsg::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KUiNewPlayerStartMsg::KUiNewPlayerStartMsg()
{
}

KUiNewPlayerStartMsg::~KUiNewPlayerStartMsg()
{
}


//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiNewPlayerStartMsg* KUiNewPlayerStartMsg::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiNewPlayerStartMsg;
		m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		//UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
		m_pSelf->Show();
        Wnd_SetExclusive(m_pSelf);
	}

	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ���
//--------------------------------------------------------------------------
void KUiNewPlayerStartMsg::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
        Wnd_ReleaseExclusive(m_pSelf);
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			m_pSelf->Hide();
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiNewPlayerStartMsg::Initialize()
{
	AddChild(&m_List);
	AddChild(&m_AdvanceList);
	AddChild(&m_NewerBtn);
	AddChild(&m_AdvanceBtn);
	AddChild(&m_Ok);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	LoadList();

	m_NewerBtn.CheckButton(true);
	m_AdvanceBtn.CheckButton(false);
	m_AdvanceList.Hide();
   
	Wnd_AddWindow(this, WL_TOPMOST);
}

void KUiNewPlayerStartMsg::LoadList()
{
	KIniFile	File;
	if (!File.Load(NEWENTERINFO_INI))
		return;

    MsgListBox_LoadContent(m_List.GetMessageListBox(), &File, "List");
    MsgListBox_LoadContent(m_AdvanceList.GetMessageListBox(), &File, "AdvanceList");
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiNewPlayerStartMsg::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_NEWENTERINFO);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_List              .Init(&Ini, "List");
		m_AdvanceList       .Init(&Ini, "List");
        m_NewerBtn          .Init(&Ini, "NewerBtn");
        m_AdvanceBtn        .Init(&Ini, "AdvanceNewerBtn");
		m_Ok                .Init(&Ini, "Ok");
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KUiNewPlayerStartMsg::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndButton*)(KWndWindow*)uParam);
		break;
	case WM_KEYDOWN:
		nRet = OnKeyDown(uParam);
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}

int KUiNewPlayerStartMsg::OnKeyDown(unsigned int uKey)
{
	int	nRet = 1;
	switch (uKey)
	{
	case VK_RETURN:
	case VK_ESCAPE:
		OnOk();
		break;
//	case VK_UP:
//		nRet = m_List.GetCurSel();
//		if (nRet  > 0)
//			m_List.SetCurSel(nRet - 1);
//		else if (nRet < 0)
//			m_List.SetCurSel(0);
//		m_ScrollBar.SetScrollPos(m_List.GetTopItemIndex());
//		nRet = 1;
//		break;
//	case VK_DOWN:
//		nRet = m_List.GetCurSel();
//		if (nRet < m_List.GetCount() - 1)
//			m_List.SetCurSel(nRet + 1);
//		else if (nRet < 0)
//			m_List.SetCurSel(0);
//		m_ScrollBar.SetScrollPos(m_List.GetTopItemIndex());
//		nRet = 1;
//		break;
	default:
		nRet = 0;
		break;
	}
	return nRet;
}

void KUiNewPlayerStartMsg::OnClickButton(KWndButton* pBtn)
{
	if(pBtn == &m_Ok)
		OnOk();
	else if (pBtn == &m_NewerBtn)
	{
		m_List.Show();
		m_AdvanceList.Hide();
        m_NewerBtn.CheckButton(true);
        m_AdvanceBtn.CheckButton(false);
	}
	else if (pBtn == &m_AdvanceBtn)
	{
		m_List.Hide();
		m_AdvanceList.Show();
        m_NewerBtn.CheckButton(false);
        m_AdvanceBtn.CheckButton(true);
	}
}

//--------------------------------------------------------------------------
//	���ܣ�ȡ��
//--------------------------------------------------------------------------
void KUiNewPlayerStartMsg::OnOk()
{
	CloseWindow(true);
    KUiHelper::OpenWindow(true);
}

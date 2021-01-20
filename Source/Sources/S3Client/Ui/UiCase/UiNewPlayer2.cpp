// -------------------------------------------------------------------------
//	�ļ���		��	UiNewPlayer2.cpp
//	������		��	����
//	����ʱ��	��	2002-9-10 14:25:24
//	��������	��	��ɫ�½����棭2
//
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiNewPlayer2.h"
#include "UiNewPlayer.h"
#include "UiInformation.h"
#include "UiSelPlayer.h"
#include "UiMsgCentrePad.h"
#include "../UiShell.h"
#include "../../Login/Login.h"

#include "../UiBase.h"
#include "../../../core/src/gamedatadef.h"

#define	SCHEME_INI_NEWPLAYER2 	"�½���ɫѡ����.ini"
#define countof(array)			(sizeof(array)/sizeof(array[0]))

#define LOGIN_CANCEL_OPER	1

KUiNewPlayer2*  KUiNewPlayer2::m_pSelf=NULL;

typedef struct tagPROPTYPEINFO
{
	OBJ_ATTRIBYTE_TYPE	type;					//����
	KWndButton*			pBtn;
	char				propertyShow[128];		//����˵��
}PROPTYPEINFO;

// ��һ����������������˵�����Ӧ�ı�������ִ��switch-case�������������Ժ��������
static PROPTYPEINFO propTypeInfoTable[] = 
{
	{series_metal,	0, "�������˵��!"},
	{series_wood,	0, "ľ������˵��!"},
	{series_water,	0, "ˮ������˵��!"},
	{series_fire,	0, "�������˵��!"},
	{series_earth,	0, "��������˵��!"},
};

KUiNewPlayer2::KUiNewPlayer2()
{
	memset(&m_Info, 0, sizeof(m_Info));
	m_Info.Attribute = series_metal;
	propTypeInfoTable[0].pBtn = &m_Gold;
	propTypeInfoTable[1].pBtn = &m_Wood;
	propTypeInfoTable[2].pBtn = &m_Water;
	propTypeInfoTable[3].pBtn = &m_Fire;
	propTypeInfoTable[4].pBtn = &m_Earth;
	m_bCreating = false;
}

KUiNewPlayer2::~KUiNewPlayer2()
{
	for (int i = 0; i < countof(propTypeInfoTable); i++)
		propTypeInfoTable[i].pBtn = 0;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiNewPlayer2* KUiNewPlayer2::OpenWindow(const char* pName, int bMale)
{
	if (m_pSelf == NULL && pName)
	{
		m_pSelf = new KUiNewPlayer2;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->Show();
		strcpy(m_pSelf->m_Info.Name, pName);
		m_pSelf->m_Info.Gender = bMale ? OBJ_G_MALE : OBJ_G_FEMALE;
		m_pSelf->UpdateProperty();
	}
	return m_pSelf;
}

void KUiNewPlayer2::Initialize()
{
	AddChild(&m_PropertyShow);
	AddChild(&m_Gold);
	AddChild(&m_Wood);
	AddChild(&m_Water);
	AddChild(&m_Fire);
	AddChild(&m_Earth);
	AddChild(&m_OK);
	AddChild(&m_Cancel);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	m_pSelf->LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiNewPlayer2::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (m_pSelf->m_bCreating)
		{
			KSelSavedCharacter* pTool = KLogin::GetCharacterModule();
			if(pTool)
				pTool->CancelOperaton();
			m_pSelf->m_bCreating = false;
		}

		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}


//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiNewPlayer2::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_NEWPLAYER2);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_PropertyShow.	Init(&Ini, "PropertyShow");
		m_Gold.			Init(&Ini, "Gold");
		m_Wood.			Init(&Ini, "Wood");
		m_Water.		Init(&Ini, "Water");
		m_Fire.			Init(&Ini, "Fire");
		m_Earth.		Init(&Ini, "Earth");
		m_OK.			Init(&Ini, "OK");
		m_Cancel.		Init(&Ini, "Cancel");
	}	
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϣ����
//--------------------------------------------------------------------------
int KUiNewPlayer2::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndWindow*)uParam);
		break;
	case WM_KEYDOWN:
		if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		else if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (uParam == LOGIN_CANCEL_OPER)
		{
			g_NetConnectAgent.SendUndispatchedToGameSpace(false);
			KSelSavedCharacter* pTool = KLogin::GetCharacterModule();
			if (pTool)
				pTool->CancelOperaton();
			OnCancel();
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ����ء���ɫ�½����棭1��
//--------------------------------------------------------------------------
void KUiNewPlayer2::OnCancel()
{
	CloseWindow(false);
	KUiNewPlayer::OpenWindow();
}

//--------------------------------------------------------------------------
//	���ܣ���ɣ�Ҫ�������½���ɫ��Ϣ��
//--------------------------------------------------------------------------
void KUiNewPlayer2::OnOk()
{
	if (m_bCreating == false)
	{
		KSelSavedCharacter* pTool = KLogin::GetCharacterModule();
		if(pTool)
		{
			m_bCreating = true;
			g_NetConnectAgent.SendUndispatchedToGameSpace(true);
			pTool->NewCharacter(&m_Info);
			UIMessageBox("�����½���ɫ...���Ժ�", this, "ȡ��", 0, LOGIN_CANCEL_OPER);
			KUiMsgCentrePad::OpenWindow();
			KUiMsgCentrePad::CloseWindow(false);
		}
	}
}

// -------------------------------------------------------------------------
// ����	: ���ڵĳ�����Ϊ
// -------------------------------------------------------------------------
void KUiNewPlayer2::Breathe()
{
	if (m_bCreating)
	{
		KSelSavedCharacter* pTool = KLogin::GetCharacterModule();
		if (pTool)
		{
			int nResult = pTool->GetLastActionResult();
			switch(nResult)
			{
			case SSC_R_START_GAME:
				m_bCreating = false;
				UiCloseMessageBox();
				CloseWindow(true);
				KUiSelPlayer::CloseWindow(true);
				UiStartGame();
				break;
			}
		}
	}
}
//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����ť
//--------------------------------------------------------------------------
void KUiNewPlayer2::OnClickButton(KWndWindow* pWnd)
{
	if (pWnd == (KWndWindow*)&m_OK)
		OnOk();
	else if (pWnd == (KWndWindow*)&m_Cancel)
		OnCancel();
	else if (pWnd == (KWndWindow*)&m_Gold)
	{
		m_Info.Attribute = series_metal;
		UpdateProperty();
	}
	else if (pWnd == (KWndWindow*)&m_Wood)
	{
		m_Info.Attribute = series_wood;
		UpdateProperty();
	}
	else if (pWnd == (KWndWindow*)&m_Water)
	{
		m_Info.Attribute = series_water;
		UpdateProperty();
	}
	else if (pWnd == (KWndWindow*)&m_Fire)
	{
		m_Info.Attribute = series_fire;
		UpdateProperty();
	}
	else if (pWnd == (KWndWindow*)&m_Earth)
	{
		m_Info.Attribute = series_earth;
		UpdateProperty();
	}
}

//--------------------------------------------------------------------------
//	���ܣ���������˵��
//--------------------------------------------------------------------------
void KUiNewPlayer2::UpdateProperty()
{
	int nSize = countof(propTypeInfoTable);
	for (int i = 0; i < nSize; i++)
	{
		if (propTypeInfoTable[i].type == m_Info.Attribute)
		{
			m_PropertyShow.SetText(propTypeInfoTable[i].propertyShow);
			propTypeInfoTable[i].pBtn->CheckButton(true);
		}
		else
			propTypeInfoTable[i].pBtn->CheckButton(false);
	}
}
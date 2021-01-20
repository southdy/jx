/*****************************************************************************************
//	����--������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-3
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "uimanage.h"
#include "../UiBase.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define MANAGE_INI_SHEET			"����������.ini"
#define MANAGE_INI_CLIQUE			"���ɹ���.ini"
#define MANAGE_INI_CONFRATERNITY	"������.ini"

//KUiManage	g_UiManage;
KUiManage* KUiManage::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KUiCliqueManage::Init()
{
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiCliqueManage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, MANAGE_INI_CLIQUE);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int	KUiCliqueManage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KUiConfraternityManage::Init()
{
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiConfraternityManage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, MANAGE_INI_CONFRATERNITY);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int	KUiConfraternityManage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)//���ں���
{
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

//--------------------------------------------------------------------------
//	���ܣ��������������ʾ���򷵻�ʵ��ָ��
//--------------------------------------------------------------------------
KUiManage* KUiManage::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiManage* KUiManage::OpenWindow()
{
	if (m_pSelf)
		m_pSelf->Show();
	else
	{
		m_pSelf = new KUiManage;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiManage::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
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
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiManage::Initialize()
{
	AddChild(&m_Close);
	AddChild(&m_LeaderShipLevel);
	AddChild(&m_LeaderShipExperience);
	
	m_TeamPad.Init();
	AddPage(&m_TeamPad, &m_TeamPadBtn);
	
	m_CliquePad.Init();
	AddPage(&m_CliquePad, &m_CliquePadBtn);

	m_ConfraternityPad.Init();
	AddPage(&m_ConfraternityPad, &m_ConfraternityPadBtn);

	//����������ɹ�������ݲ����š�
	m_CliquePadBtn.Enable(false);
	m_ConfraternityPadBtn.Enable(false);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	UpdateLeaderData();

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiManage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, MANAGE_INI_SHEET);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Close			.Init(&Ini, "CloseBtn");

		m_pSelf->m_TeamPadBtn	.Init(&Ini, "TeamBtn");
		m_pSelf->m_CliquePadBtn	.Init(&Ini, "CliqueBtn");
		m_pSelf->m_ConfraternityPadBtn.Init(&Ini, "ConfraternityBtn");

		m_pSelf->m_LeaderShipLevel     .Init(&Ini, "LeadLevel");
		m_pSelf->m_LeaderShipExperience.Init(&Ini, "LeadExp");

		Ini.Clear();

		m_pSelf->m_TeamPad.LoadScheme(pScheme);
		m_pSelf->m_CliquePad.LoadScheme(pScheme);
		m_pSelf->m_ConfraternityPad.LoadScheme(pScheme);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KUiManage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK &&
		uParam == (unsigned int)(KWndWindow*)&m_Close)
	{
		Hide();
		return 0;
	}
	else
		return KWndPageSet::WndProc(uMsg, uParam, nParam);
}

//--------------------------------------------------------------------------
//	���ܣ��������ݱ��
//--------------------------------------------------------------------------
void KUiManage::UpdateTeamData(int nID)
{
	m_TeamPad.UpdateDataTeam(nID);
}

//--------------------------------------------------------------------------
//	���ܣ������Ա���
//--------------------------------------------------------------------------
void KUiManage::UpdateTeamChange(const KUiPlayerItem* playerItem,int bAdd)
{
	m_TeamPad.UpdateIncrementUpdateDataTeam(playerItem,bAdd);
}

//--------------------------------------------------------------------------
//	���ܣ���������б�ı��
//--------------------------------------------------------------------------
void KUiManage::UpdateToJoinChange(const KUiPlayerItem* playerItem,int bAdd)
{
	m_TeamPad.UpdateIncrementUpdateDataJoinList(playerItem,bAdd);
}

//--------------------------------------------------------------------------
//	���ܣ������ֵ���
//--------------------------------------------------------------------------
void KUiManage::UpdateLeaderData()
{
	KUiPlayerLeaderShip	Leader = { 0 };	
	g_pCoreShell->OperationRequest(GDI_PLAYER_LEADERSHIP, (unsigned int)&Leader, 0);
	m_LeaderShipLevel.SetIntText(Leader.nLeaderShipLevel);

	m_LeaderShipExperience.SetPart(Leader.nLeaderShipExperience,
		Leader.nLeaderShipExperienceFull);
}
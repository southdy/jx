// -------------------------------------------------------------------------
//	�ļ���		��	UiEquip.cpp
//	Author		��	����, Wooy(Wu yue)
//	����ʱ��	��	2002-9-10 16:22:58
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../elem/wndmessage.h"
#include "uiequip.h"
#include "../../../core/src/coreshell.h"
#include "../../../Core/Src/GameDataDef.h"
#include "../../../Engine/Src/Text.h"
#include "../UiBase.h"
#include <crtdbg.h>
#include "../UiSoundSetting.h"

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_EQUIP	"װ��.ini"
#define EQUIP_INI_PAGE		"װ����ҳ.ini"
#define HORSE_INI_PAGE		"װ�����ҳ.ini"

// -------------------------------------------------------------------------
// ---> �����ؼ���UIEP_*�Լ��ɽ�����Ʒ�����͵Ķ�Ӧ��ϵ
static struct UE_CTRL_MAP
{
	int				nPosition;
	const char*		pIniSection;
}CtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		},	//װ��-ñ��
	{ UIEP_HAND,		"Weapon"	},	//װ��-����
	{ UIEP_NECK,		"Necklace"	},	//װ��-����
	{ UIEP_FINESSE,		"Bangle"	},	//װ��-����
	{ UIEP_BODY,		"Cloth"		},	//װ��-�·�
	{ UIEP_WAIST,		"Sash"		},	//װ��-����
	{ UIEP_FINGER1,		"Ring1"		},	//װ��-��ָ
	{ UIEP_FINGER2,		"Ring2"		},	//װ��-��ָ
	{ UIEP_WAIST_DECOR,	"Pendant"	},	//װ��-��׺
	{ UIEP_FOOT,		"Shoes"		}	//װ��-Ь��
};

KUiEquip* KUiEquip::m_pSelf = NULL;

void KUiHorsePage::Initialize()
{
	AddChild(&m_HorseBox);
	AddChild(&m_UpDownHorseBtn);
	AddChild(&m_HorseDesc);
}

//������淽��
// -------------------------------------------------------------------------
// ����	: ������淽��
// -------------------------------------------------------------------------
void KUiHorsePage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, HORSE_INI_PAGE);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_HorseBox.Init(&Ini, "Horse");
		m_UpDownHorseBtn.Init(&Ini, "UpDownHorse");
		m_HorseDesc.Init(&Ini, "HorseDesc");
	}
}

//--------------------------------------------------------------------------
//	���ܣ�װ���仯����
//--------------------------------------------------------------------------
void KUiHorsePage::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip && pEquip->Region.v == UIEP_HORSE)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
		if (bAdd && pEquip->Obj.uGenre != CGOG_NOTHING)
		{
			m_HorseBox.HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
				pEquip->Region.Width, pEquip->Region.Height);
			m_UpDownHorseBtn.Enable(true);
			char	Desc[1024];
			int		nLen = 0;
			g_pCoreShell->GetGameData(GDI_GAME_OBJ_DESC, (unsigned int)pEquip, (int)&Desc);
			if (Desc[0])
				nLen = TEncodeText(Desc, strlen(Desc));
			m_HorseDesc.SetText(Desc, nLen);
		}
		else
		{
			Clear();
		}
	}
}

//��������״̬
void KUiHorsePage::UpdateRideStatus(int bRiding)
{
	m_UpDownHorseBtn.CheckButton(bRiding);
}

void KUiHorsePage::Clear()
{
	m_HorseBox.HoldObject(CGOG_NOTHING, 0, 0, 0);
	m_UpDownHorseBtn.CheckButton(false);
	m_HorseDesc.SetText("", 0);
}

// -------------------------------------------------------------------------
// ����	: ���ں���
// -------------------------------------------------------------------------
int	KUiHorsePage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	if (uMsg == WND_N_ITEM_PICKDROP)
	{
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
				OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
	}
	else if (uMsg == WND_N_BUTTON_CLICK && (KWndWindow*)uParam == (KWndWindow*)&m_UpDownHorseBtn)
	{
		g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, PA_RIDE, 0);
	}
	else
	{
		nRet = KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����������װ���ĸı�
//--------------------------------------------------------------------------
void KUiHorsePage::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);
		m_HorseBox.GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.Region.v = UIEP_HORSE;
		Pick.eContainer = UOC_EQUIPTMENT;

		if (eStatus == UIS_S_TRADE_REPAIR)
		{
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
				(unsigned int)(&Pick), 0);
			return;
		}
		else if (eStatus == UIS_S_TRADE_SALE)
		{
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
				(unsigned int)(&Pick), 0);
			return;
		}
		else if (eStatus == UIS_S_TRADE_NPC || eStatus == UIS_S_TRADE_BUY)
		{			
			return;
		}
	}
	else if (pDropPos == NULL)
	{
		return;
	}

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.Region.v = UIEP_HORSE;
		Drop.eContainer = UOC_EQUIPTMENT;
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}

// -------------------------------------------------------------------------
// ����	: ��ʼ��
// -------------------------------------------------------------------------
void KUiEquipPage::Initialize()
{
	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_EquipBox[i]);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
void KUiEquipPage::Clear()
{
	for (int i = 0; i < _ITEM_COUNT; i ++)
		m_EquipBox[i].Celar();
}

//--------------------------------------------------------------------------
//	���ܣ�װ���仯����
//--------------------------------------------------------------------------
void KUiEquipPage::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; i++)
		{
			if (CtrlItemMap[i].nPosition == pEquip->Region.v)
			{
				if (bAdd)
					m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}

// -------------------------------------------------------------------------
// ����	: ������淽��
// -------------------------------------------------------------------------
void KUiEquipPage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, EQUIP_INI_PAGE);
	if (Ini.Load(Buff))
	{
		if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, 0))
			KWndPage::Init(&Ini, "Male");
		else
			KWndPage::Init(&Ini, "Female");
		for (int i = 0; i < _ITEM_COUNT; i ++)
		{
			m_EquipBox[i].Init(&Ini, CtrlItemMap[i].pIniSection);
		}
	}
}

// -------------------------------------------------------------------------
// ����	: ���ں���
// -------------------------------------------------------------------------
int	KUiEquipPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����������װ���ĸı�
//--------------------------------------------------------------------------
void KUiEquipPage::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_EQUIPTMENT;
		pWnd = pPickPos->pWnd;

		if (eStatus == UIS_S_TRADE_REPAIR)
		{
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
				(unsigned int)(&Pick), 0);
			return;
		}
		else if (eStatus == UIS_S_TRADE_SALE)
		{
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
				(unsigned int)(&Pick), 0);
			return;
		}
		else if (eStatus == UIS_S_TRADE_NPC)
		{
			
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
			return;
	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_EQUIPTMENT;
	}

	for (int i = 0; i < _ITEM_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_EquipBox[i])
		{
			Drop.Region.v = Pick.Region.v = CtrlItemMap[i].nPosition;
			break;
		}
	}
	_ASSERT(i < _ITEM_COUNT);

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ---> װ������
//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiEquip* KUiEquip::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiEquip;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_EquipPage.Clear();
		m_pSelf->m_HorsePage.Clear();
		
		KUiObjAtRegion	Equips[_ITEM_COUNT + 1];
		int nCount = g_pCoreShell->GetGameData(GDI_EQUIPMENT, (unsigned int)&Equips, 0);
		for (int i = 0; i < nCount; i++)
		{
			if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			{
				if (Equips[i].Region.v != UIEP_HORSE)
					m_pSelf->m_EquipPage.UpdateEquip(&Equips[i], true);
				else
					m_pSelf->m_HorsePage.UpdateEquip(&Equips[i], true);
			}
		}
		m_pSelf->Show();		
		m_pSelf->BringToTop();
	}

	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��������������ʾ���򷵻�ʵ��ָ��
//--------------------------------------------------------------------------
KUiEquip* KUiEquip::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiEquip::CloseWindow(bool bDestroy)
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

// -------------------------------------------------------------------------
// ����	: ��ʼ��
// -------------------------------------------------------------------------
void KUiEquip::Initialize()
{
	m_EquipPage.Initialize();
	AddPage(&m_EquipPage, &m_Equip1Btn);
	m_HorsePage.Initialize();
	AddPage(&m_HorsePage, &m_Equip2Btn);
	
	AddChild(&m_Close);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

// -------------------------------------------------------------------------
// ����	: ������淽��
// -------------------------------------------------------------------------
void KUiEquip::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_EQUIP);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");	// ���ڱ�������
		m_pSelf->m_Close.Init(&Ini, "Close");
		m_pSelf->m_Equip1Btn.Init(&Ini, "Equip1Btn");
		m_pSelf->m_Equip2Btn.Init(&Ini, "Equip2Btn");
		m_pSelf->m_EquipPage.LoadScheme(pScheme);
		m_pSelf->m_HorsePage.LoadScheme(pScheme);
	}
}

// -------------------------------------------------------------------------
// ����	: ���ں���
// -------------------------------------------------------------------------
int	KUiEquip::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK && (KWndWindow*)uParam == (KWndWindow*)&m_Close)
	{
		Hide();	// �ر�װ����
		return 0;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}

// -------------------------------------------------------------------------
// ����	: װ���仯����
// -------------------------------------------------------------------------
void KUiEquip::UpdateEquip(KUiObjAtRegion* pEquipt, int bAdd)
{
	if (pEquipt)
	{
		if (pEquipt->Region.v != UIEP_HORSE)
			m_EquipPage.UpdateEquip(pEquipt, bAdd);
		else
			m_HorsePage.UpdateEquip(pEquipt, bAdd);
	}
}

//��������״̬
void KUiEquip::UpdateRideStatus(int bRiding)
{
	if (m_pSelf)
		m_pSelf->m_HorsePage.UpdateRideStatus(bRiding);
}

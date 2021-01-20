// -------------------------------------------------------------------------
//	�ļ���		��	����ȷ�Ͽ�
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-5-28
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiTradeConfirmWnd.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"����ȷ��.ini"

KUiTradeConfirm* KUiTradeConfirm::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiTradeConfirm* KUiTradeConfirm::OpenWindow(KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTradeConfirm;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_BuyImg.Hide();
		m_pSelf->m_SaleImg.Hide();
		m_pSelf->m_RepairImg.Hide();
		if (eAction == TCA_SALE)
		{
			m_pSelf->m_SaleImg.Show();
			m_pSelf->m_OkBtn.Enable(true);
			m_pSelf->m_Money.SetTextColor(m_pSelf->m_uNormalPriceColor);
		}
		else
		{
			if (eAction == TCA_BUY)
				m_pSelf->m_BuyImg.Show();
			else
				m_pSelf->m_RepairImg.Show();
			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
		}
		m_pSelf->m_ItemInfo = *pObj;
		m_pSelf->m_PriceInfo = *pPriceInfo;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiTradeConfirm::KUiTradeConfirm()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}

//�������������ʾ���򷵻�ʵ��ָ��
KUiTradeConfirm* KUiTradeConfirm::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��ر����ٴ���
//--------------------------------------------------------------------------
void KUiTradeConfirm::CloseWindow(bool bDestroy)
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


//��ʾ����
void KUiTradeConfirm::Show()
{
	m_ItemName.SetText(m_PriceInfo.szItemName);
	m_Money.SetIntText(m_PriceInfo.nPrice);
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

//���ش���
void KUiTradeConfirm::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KUiTradeConfirm::Initialize()
{
	AddChild(&m_ItemName);
	AddChild(&m_Money);
	AddChild(&m_BuyImg);
	AddChild(&m_SaleImg);
	AddChild(&m_RepairImg);
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
void KUiTradeConfirm::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ItemName.Init(&Ini, "ItemName");

		m_Money.Init(&Ini, "Price");			
		Ini.GetString("Price", "Color", "", Buff, sizeof(Buff));
		m_uNormalPriceColor = GetColor(Buff);
		Ini.GetString("Price", "CantBuyColor", "", Buff, sizeof(Buff));
		m_uNotEnoughMoneyPriceColor = GetColor(Buff);

		m_BuyImg.Init(&Ini, "BuyImg");
		m_SaleImg.Init(&Ini, "SaleImg");
		m_RepairImg.Init(&Ini, "RepairImg");
		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_Money.SetIntText(m_PriceInfo.nPrice);
	}
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϣ����
//--------------------------------------------------------------------------
int KUiTradeConfirm::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{
			if (m_BuyImg.IsVisible())
				OnBuy();
			else if (m_SaleImg.IsVisible())
				OnSale();
			else
				OnRepair();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			if (m_SaleImg.IsVisible())
			{
				OnSale();
			}
			else if (m_BuyImg.IsVisible())
			{
				if (m_BuyImg.IsDisable())
					OnCancel();
				else
					OnBuy();
			}
			else if (m_RepairImg.IsDisable())
			{
				OnCancel();
			}
			else
			{
				OnRepair();
			}
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����ť
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnBuy()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�������ť
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnSale()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ������ť
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnRepair()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}


void KUiTradeConfirm::OnCancel()
{
	CloseWindow(false);
}

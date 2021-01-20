/*******************************************************************************
File        : UiTrembleItem.cpp
Creator     : Fyt(Fan Zhanpeng)
create data : 09-08-2003(mm-dd-yyyy)
Description : ���������Ľ���
********************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiTrembleItem.h"

#define TREMBLE_ITEM_INI "������Ʒ.ini"


KUiTrembleItem* KUiTrembleItem::ms_pSelf = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiTrembleItem::KUiTrembleItem()
{
	m_nStatus = STATUS_WAITING_MATERIALS;
}

KUiTrembleItem::~KUiTrembleItem()
{

}


/*********************************************************************
* ���ܣ��򿪴���
**********************************************************************/
KUiTrembleItem* KUiTrembleItem::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTrembleItem;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->m_nStatus = STATUS_WAITING_MATERIALS;
		ms_pSelf->BringToTop();
		ms_pSelf->Show();
	}
	return ms_pSelf;
}


/*********************************************************************
* ���ܣ��������������ʾ���򷵻�ʵ��ָ��
**********************************************************************/
KUiTrembleItem* KUiTrembleItem::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* ���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
**********************************************************************/
void KUiTrembleItem::CloseWindow(bool bDestory)
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
void KUiTrembleItem::Initialize()
{
	AddChild(&m_GemSpirit);
	AddChild(&m_GemWater);
	AddChild(&m_GemEarth);
	AddChild(&m_GemLevel);
	AddChild(&m_GemFire);
	AddChild(&m_GemGold);
	AddChild(&m_GemWood);
	AddChild(&m_Item);
	AddChild(&m_Confirm);
	AddChild(&m_Cancel);
	AddChild(&m_TrembleEffect);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


/*********************************************************************
* ���ܣ�������淽��
**********************************************************************/
void KUiTrembleItem::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
		char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, TREMBLE_ITEM_INI);
    	if (Ini.Load(Buff))
    	{
			ms_pSelf->Init(&Ini, "Main");

			ms_pSelf->m_Item.Init(&Ini, "Item");
			ms_pSelf->m_GemGold.Init(&Ini, "GemGold");
			ms_pSelf->m_GemWood.Init(&Ini, "GemWood");
			ms_pSelf->m_GemWater.Init(&Ini, "GemWater");
			ms_pSelf->m_GemFire.Init(&Ini, "GemFire");
			ms_pSelf->m_GemEarth.Init(&Ini, "GemEarth");
			ms_pSelf->m_GemLevel.Init(&Ini, "GemLevel");
			ms_pSelf->m_GemSpirit.Init(&Ini, "GemSpirit");
			ms_pSelf->m_TrembleEffect.Init(&Ini, "Effect");

			ms_pSelf->m_Confirm.Init(&Ini, "Assemble");
			ms_pSelf->m_Cancel.Init(&Ini, "Close");
		}
	}
}


/*********************************************************************
* ���ܣ�������淽��
**********************************************************************/
int KUiTrembleItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Confirm)
		{
		}
		else if(uParam == (unsigned int)&m_Cancel)
		{
			CloseWindow();
		}
		break;

	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;
}


/*********************************************************************
* ���ܣ������
**********************************************************************/
void KUiTrembleItem::Breathe()
{
	if(m_nStatus == STATUS_BEGIN_TREMBLE)
	{
		m_TrembleEffect.Show();
		m_TrembleEffect.SetFrame(0);
		m_nStatus = STATUS_TREMBLING;
	}
	else if(m_nStatus == STATUS_TREMBLING)
	{
		if(!PlayEffect())
		{
			m_nStatus = STATUS_CHANGING_ITEM;
			m_TrembleEffect.Hide();
		}
	}
	else if(m_nStatus == STATUS_CHANGING_ITEM)
	{
		UpdateResult();
		m_nStatus = STATUS_FINISH;
	}
}


/*********************************************************************
* ���ܣ�������Ч
**********************************************************************/
int KUiTrembleItem::PlayEffect()
{
	if(m_TrembleEffect.GetMaxFrame() == 0 ||
	   m_TrembleEffect.GetMaxFrame() >= m_TrembleEffect.GetCurrentFrame() - 1)
	{
		m_TrembleEffect.SetFrame(0);
		return 0;
	}
	else
	{
		m_TrembleEffect.NextFrame();
		return 1;
	}
}


/*********************************************************************
* ���ܣ��ѽ�����µ�������
**********************************************************************/
void KUiTrembleItem::UpdateResult()
{
}





int KCanGetNumImage::GetMaxFrame()
{
	return m_Image.nNumFrames;
}


int KCanGetNumImage::GetCurrentFrame()
{
	return m_Image.nFrame;
}
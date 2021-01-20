/*****************************************************************************************
//	����--��Ϸ��ʾ�����Ϣ������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-16
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include <crtdbg.h>
#include "UiPlayerBar.h"
#include "UiStatus.h"
#include "UiSkillTree.h"
#include "UiEscDlg.h"
#include "UiSelColor.h"
#include "UiMsgCentrePad.h"
#include "UiFaceSelector.h"
#include "UiMiniMap.h"
#include "UiHelper.h"
#include "../ShortcutKey.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../core/src/MsgGenreDef.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../../TextCtrlCmd/TextCtrlCmd.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Engine/src/Text.h"

#include "../../../Headers/KRelayProtocol.h"
#include "../../../Headers/KProtocolDef.h"
#include "../../../Headers/KProtocol.h"
#include "malloc.h"

#include "UiChatCentre.h"

#include "../UiChatPhrase.h"

#include "../../Login/Login.h"

extern KUiChatPhrase g_UiChatPhrase;

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI			"�����Ϣ������.ini"
#define	SCHEME_INI_MINI		"�����Ϣ��������С��.ini"
#define GAME_LOGO			"������Ե�������"
#define	SWITCH_LOGO_INTERVAL	5000

#define		SEL_CHANNEL_MENU		1
#define		SEL_PHRASE_MENU			2


const char*		s_TimeName[12] =
{
	"ҹ��",
	"����",
	"ƽ��",
	"�ճ�",
	"ʳʱ",
	"����",
	"����",
	"�Օi",
	"��ʱ",
	"����",
	"�ƻ�",
	"�˶�",
};

const char*		s_TimeStone[12] = 
{
	"��",
	"��",
	"��",
	"î",
	"��",
	"��",
	"��",
	"δ",
	"��",
	"��",
	"��",
	"��"
};

const char*		s_TimeLine[4] = { "һ", "��", "��", "��" };

int GetFormatedTimeString(int nTime, char* pString)
{
	nTime += 60;
	return sprintf(pString, "%s%sʱ%s��",
		s_TimeName[(nTime / 120) % 12], s_TimeStone[(nTime / 120) % 12],
		s_TimeLine[(nTime % 120) / 30]);
	//return sprintf(pString, "%sʱ%s��",
	//	s_TimeStone[(nTime / 120) % 12], s_TimeLine[(nTime % 120) / 30]);
}

#include "time.h"

int GetFormatedTimeString(struct tm* ptm, char* pString)
{
	return sprintf(pString, "%d��%d�� %d:%02d",
			ptm->tm_mon + 1,
			ptm->tm_mday,
			ptm->tm_hour,
			ptm->tm_min);
	//return sprintf(pString, "[%04d/%02d/%02d %02d:%02d:%02d]",
	//		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

GameWorld_DateTime::GameWorld_DateTime()
{
	m_bTrueWorld = true;
	m_bLogo    = false;
	m_uLastSwitchTime = IR_GetCurrentTime();
}

IMPLEMENT_COMCLASS(GameWorld_DateTime)
void GameWorld_DateTime::OnButtonClick()
{
	m_bTrueWorld = !m_bTrueWorld;
	UpdateData();
}

int	GameWorld_DateTime::Init(KIniFile* pIniFile, const char* pSection)
{
	if(KWndLabeledButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "SmoothPing", 0, &m_nSmoothPing);
		pIniFile->GetInteger(pSection, "CrowdPing", 0, &m_nCrowdPing);
		pIniFile->GetInteger(pSection, "BlockPing", 0, &m_nBlockPing);

		pIniFile->GetString(pSection, "SmoothMsg", "", m_szSmoothMsg, sizeof(m_szSmoothMsg));
		pIniFile->GetString(pSection, "CrowdMsg", "", m_szCrowdMsg, sizeof(m_szCrowdMsg));
		pIniFile->GetString(pSection, "BlockMsg", "", m_szBlockMsg, sizeof(m_szBlockMsg));
		pIniFile->GetString(pSection, "GameLogo", GAME_LOGO, m_szLogo, sizeof(m_szLogo));

		char Buff[16];
		pIniFile->GetString(pSection, "Color", "0,0,0", Buff, sizeof(Buff));
		m_uDefaultColor = GetColor(Buff);
		pIniFile->GetString(pSection, "SmoothColor", "0,0,0", Buff, sizeof(Buff));
		m_uSmoothColor = GetColor(Buff);
		pIniFile->GetString(pSection, "CrowdColor", "0,0,0", Buff, sizeof(Buff));
		m_uCrowdColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BlockColor", "0,0,0", Buff, sizeof(Buff));
		m_uBlockColor = GetColor(Buff);
	}
	return 1;
}

void GameWorld_DateTime::UpdateData()
{
	char szTime[64];
	if(!m_bLogo && !m_bNetStatus)
	{
        if (!m_bTrueWorld && g_pCoreShell)
	    {
    		KUiSceneTimeInfo Info;
		    memset(&Info, 0, sizeof(KUiSceneTimeInfo));
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Info, 0);
		    int nLen = GetFormatedTimeString(Info.nGameSpaceTime, szTime);
	    }
	    else
	    {		
    		time_t curtm = ::time(NULL);
		    struct tm* ptm = localtime(&curtm);
		    int nLen = GetFormatedTimeString(ptm, szTime);
	    }
		SetLabelColor(m_uDefaultColor);
		SetLabel(szTime);
	}

	if (IR_IsTimePassed(SWITCH_LOGO_INTERVAL, m_uLastSwitchTime))
	{
		if(m_bLogo)
		{
			m_bLogo = false;
			UpdateNetStatus();
			m_bNetStatus = true;
		}
		else if(m_bNetStatus)
		{
			m_bNetStatus = false;
		}
		else
		{
			SetLabelColor(m_uDefaultColor);
			SetLabel(m_szLogo);
			m_bLogo = true;
		}
	}

	if (m_Flag & WNDBTN_F_OVER)
	{
		m_bLogo = false;
		m_bNetStatus = false;
		m_uLastSwitchTime = IR_GetCurrentTime();
	}

	NextFrame();
}

void GameWorld_DateTime::UpdateNetStatus()
{
	int i, nPing;
	char Number[32], Buff[32];
	unsigned int uDefaultColor;

	if(g_pCoreShell) nPing = g_pCoreShell->GetPing();
	itoa(nPing, Number, 10);

	if(nPing >= 0 && nPing <= m_nSmoothPing)
	{
		i = strlen(m_szSmoothMsg);
		memcpy(Buff, m_szSmoothMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uSmoothColor;
	}
	else if(nPing > m_nSmoothPing && nPing <= m_nCrowdPing)
	{
		i = strlen(m_szCrowdMsg);
		memcpy(Buff, m_szCrowdMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uCrowdColor;
	}
	else
	{
		i = strlen(m_szBlockMsg);
		memcpy(Buff, m_szBlockMsg, i);
		Buff[i] = ' ';
		memcpy(Buff + i + 1, Number, strlen(Number) + 1);
		uDefaultColor = m_uBlockColor;
	}
	SetLabelColor(uDefaultColor);
	SetLabel(Buff);
}

int	KImmediaItem::Init(KIniFile* pIniFile, const char* pSection)
{
	int nRet = KWndObjectBox::Init(pIniFile, pSection);
	if (nRet)
	{
		char ChName[64];
		pIniFile->GetString(pSection, "TextColor", "255,255,0", ChName, sizeof(ChName));
		m_TextColor = GetColor(ChName);
		return true;
	}
	return nRet;
}

void KImmediaItem::PaintWindow()
{
	KWndObjectBox::PaintWindow();
	if (g_pCoreShell)
	{
		int nNum = g_pCoreShell->GetGameData(GDI_IMMEDIATEITEM_NUM, m_nIndex, 0);
		if (nNum > 1 && nNum < 1000)
		{
			int nFontSize = 12;
			char szNum[4];
			int nLen = sprintf(szNum, "%d", nNum);
			szNum[3] = 0;
			g_pRepresentShell->OutputText(nFontSize, szNum, KRF_ZERO_END,
				m_nAbsoluteLeft + m_Width - nLen * (nFontSize + 1) / 2,
				m_nAbsoluteTop + m_Height - nFontSize - 1, m_TextColor);
		}
	}
}

//--------------------------------------------------------------------------

KUiPlayerBar* KUiPlayerBar::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KUiPlayerBar::KUiPlayerBar()
{
	m_cPreMsgCounter = 0;
	m_cLatestMsgIndex= 0;
	m_bMiniMode = false;
	for (int i = 0; i < MAX_RECENT_MSG_COUNT; i++)
		m_RecentMsg[i][0] = 0;
	m_szSelfName[0] = 0;

	m_nCurChannel = -1;
	m_nRecentPlayerName = 0;

	memset(m_RecentPlayerName, 0, sizeof(m_RecentPlayerName));

	m_nExperienceFull = 0;
	m_nCurLevelExperience = 0;
	m_nExperience = 0;
}

KUiPlayerBar::~KUiPlayerBar()
{
}

//--------------------------------------------------------------------------
//	���ܣ��������������ʾ���򷵻�ʵ��ָ��
//--------------------------------------------------------------------------
KUiPlayerBar* KUiPlayerBar::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiPlayerBar* KUiPlayerBar::OpenWindow()
{
	KUiChatCentre::OpenWindow(false);

	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiPlayerBar;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->UpdateData();
		m_pSelf->Show();
	}

	m_pSelf->m_nCurChannel = -1;
	m_pSelf->m_nRecentPlayerName = 0;

	return m_pSelf;
}

//�л���С
void KUiPlayerBar::OnSwitchSize()
{
	m_bMiniMode = !m_bMiniMode;
	char Scheme[128];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateData()
{
	//_ASSERT(g_pCoreShell);
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
	strcpy(m_szSelfName, Info.Name);

	int nA;
	int nA2;
	UpdateXXXNumber(nA, nA2);

	KUiPlayerImmedItemSkill immedItemSkillInfo;
	memset(&immedItemSkillInfo, 0, sizeof(KUiPlayerImmedItemSkill));
	g_pCoreShell->GetGameData(GDI_PLAYER_IMMED_ITEMSKILL, (int)&immedItemSkillInfo, 0);
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].HoldObject(immedItemSkillInfo.ImmediaItem[i].uGenre,
			immedItemSkillInfo.ImmediaItem[i].uId, 0, 0);
	}
	
	m_ImmediaSkill[0].HoldObject(immedItemSkillInfo.IMmediaSkill[0].uGenre,
		immedItemSkillInfo.IMmediaSkill[0].uId, 0, 0);
	m_ImmediaSkill[1].HoldObject(immedItemSkillInfo.IMmediaSkill[1].uGenre,
		immedItemSkillInfo.IMmediaSkill[1].uId, 0, 0);

	int nM;
	int nL;
	UpdateRuntimeAttribute(nM, nL);
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiPlayerBar::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_cLatestMsgIndex = 0;
		m_pSelf->m_cPreMsgCounter = 0;
		for(int i = 0; i < MAX_RECENT_MSG_COUNT; i++)
			m_pSelf->m_RecentMsg[i][0] = 0;

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
//	���ܣ�������淽��
//--------------------------------------------------------------------------
void KUiPlayerBar::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s" , pScheme, m_pSelf->m_bMiniMode ? SCHEME_INI_MINI : SCHEME_INI);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini);
	}
}

unsigned int s_CanFocusColor = 0;
unsigned int s_NotCanFocusColor = 0;

void KUiPlayerBar::LoadScheme(KIniFile* pIni)
{
//	_ASSERT(pIni);
	Init(pIni, $Main);

	m_EscDlg  .Init(pIni, "Options");
	m_Face    .Init(pIni, "Face");
	m_Friend  .Init(pIni, "Friend");

	m_DateTime.Init(pIni, "DateTime");
	
	char	szBuffer[16];
	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		sprintf(szBuffer, "Item_%d", i);
		m_pSelf->m_ImmediaItem[i].Init(pIni, szBuffer);
		m_pSelf->m_ImmediaItem[i].m_nIndex = i;
	}
	m_ImmediaSkill[0].Init(pIni, "ImediaLeftSkill");
	m_ImmediaSkill[1].Init(pIni, "ImediaRightSkill");
	m_ImmediaSkill[0].EnablePickPut(false);
	m_ImmediaSkill[1].EnablePickPut(false);

	m_InputEdit	.Init(pIni, "InputEdit");
	s_CanFocusColor = m_InputEdit.GetFocusBkColor();

	if (pIni->GetString("InputEdit", "FocusNoCanBKColor", "0,0,0", szBuffer, sizeof(szBuffer)))
	{
		s_NotCanFocusColor = (GetColor(szBuffer) & 0xffffff);
		int	nValue;
		if (pIni->GetInteger("InputEdit", "FocusNoCanBKColorAlpha", 128, &nValue))
		{
			if (nValue > 255 || nValue < 0)
				s_NotCanFocusColor |= 0x16000000;
			else
				s_NotCanFocusColor |= (((255 - nValue) << 21) & 0xff000000);
		}
	}
	else
	{
		KRColor nColor;
		nColor.Color_dw = s_CanFocusColor;
		nColor.Color_b.a = 12;
		nColor.Color_b.r = 108;
		s_NotCanFocusColor = nColor.Color_dw;
	}

	m_SendBtn	.Init(pIni, "SendBtn");
	m_ChannelSwitchBtn.Init(pIni, "ChannelBtn");
	m_ChannelOpenBtn.Init(pIni, "OpenChannelBtn");

	m_SwitchBtn .Init(pIni, "SwitchSizeBtn");
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiPlayerBar::Initialize()
{
	AddChild(&m_DateTime);
	AddChild(&m_Face);
	AddChild(&m_EscDlg);
	AddChild(&m_Friend);

	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		m_ImmediaItem[i].SetObjectGenre(CGOG_ITEM);
		m_ImmediaItem[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
		AddChild(&m_ImmediaItem[i]);
		m_ImmediaItem[i].SetContainerId((int)UOC_IMMEDIA_ITEM);
	}
	AddChild(&m_ImmediaSkill[0]);
	AddChild(&m_ImmediaSkill[1]);
	m_ImmediaSkill[0].SetContainerId((int)UOC_IMMEDIA_SKILL);
	m_ImmediaSkill[1].SetContainerId((int)UOC_IMMEDIA_SKILL);

	AddChild(&m_InputEdit);
	AddChild(&m_ChannelSwitchBtn);
	AddChild(&m_SendBtn);
	AddChild(&m_SwitchBtn);
	AddChild(&m_ChannelOpenBtn);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	m_pSelf->LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KUiPlayerBar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_M_POPUPMENU:
		if (uParam == (unsigned int)(KWndWindow*)(&m_InputEdit))
		{
			PopupPhraseMenu(LOWORD(nParam), HIWORD(nParam), true);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelSwitchBtn)
		{
			int x, y;
			m_ChannelSwitchBtn.GetAbsolutePos(&x, &y);
			PopupChannelMenu(x, y);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelOpenBtn)
		{
		}
		break;
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
			OnObjPickedDropped((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Friend)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_FRIEND);
		else if (uParam == (unsigned int)(KWndWindow*)&m_EscDlg)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SYSTEM);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Face)
		{
			KUiFaceSelector::OpenWindow(this, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SendBtn)
			OnSend(false);
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelSwitchBtn)
		{
			int x, y;
			m_ChannelSwitchBtn.GetAbsolutePos(&x, &y);
			PopupChannelMenu(x, y);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChannelOpenBtn)
		{
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_SwitchBtn)
			OnSwitchSize();
		break;
	case WND_N_LEFT_CLICK_ITEM:
		if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[0])
			KUiSkillTree::OpenWindow(true);
		else if (nParam == (int)(KWndWindow*)&m_ImmediaSkill[1])
			KUiSkillTree::OpenWindow(false);
		break;
	case WND_N_RIGHT_CLICK_ITEM:
		if (g_UiBase.IsOperationEnable(UIS_O_USE_ITEM))
		{
			for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
				if (nParam == (int)(KWndWindow*)&m_ImmediaItem[i])
					OnUseItem(i);
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN)
		{
			OnSend((GetKeyState(VK_CONTROL) & 0x8000));
			Wnd_SetFocusWnd(NULL);
			nRet = 1;
		}
		else if (nParam == VK_UP || nParam == VK_DOWN)
		{
			if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				int x, y;
				m_InputEdit.GetAbsolutePos(&x, &y);
				int nCX = 0;
				m_InputEdit.GetCaretPosition(&nCX, NULL);
				PopupPhraseMenu(x + nCX, y, nParam == VK_DOWN);
			}
			else
			{
				InputRecentMsg(nParam == VK_UP);
				nRet = 1;
			}
		}
		else if (nParam == VK_PRIOR || nParam == VK_NEXT)
		{
			if (m_nCurChannel >= 0)
			{
				int nToChannel;
				if (nParam == VK_PRIOR)
					nToChannel = (m_nCurChannel - 1 >= 0) ? m_nCurChannel - 1 : KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName - 1;
				else
					nToChannel = (DWORD(m_nCurChannel + 1) < (KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName)) ? m_nCurChannel + 1 : 0;

				if (nToChannel != m_nCurChannel)
				{
					InputCurrentChannel(nToChannel, true);
				}
			}				
			nRet = 1;
		}
		else if (nParam == VK_ESCAPE)
		{
			Wnd_SetFocusWnd(NULL);
			nRet = 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		m_cPreMsgCounter = 0;
		break;
	case WND_M_MENUITEM_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == SEL_CHANNEL_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					InputCurrentChannel(LOWORD(nParam), true);
				}

				KPopupMenu::Cancel();
			}
			else if (HIWORD(nParam) == SEL_PHRASE_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					char szPhrase[64];
					int nLen = g_UiChatPhrase.GetPhrase(LOWORD(nParam), szPhrase);
					m_InputEdit.InsertString(szPhrase, nLen);
				}
			}
		}
		break;
	case WND_M_MENUITEMRIGHT_SELECTED:
		if (uParam == (unsigned int)(KWndWindow*)this)
		{
			if (HIWORD(nParam) == SEL_CHANNEL_MENU)
			{
				if (short(LOWORD(nParam)) >= 0)
				{
					if (KUiMsgCentrePad::CheckChannel(LOWORD(nParam), !KUiMsgCentrePad::GetChannelSubscribe(LOWORD(nParam))))
					{
						KPopupMenuData* p = KPopupMenu::GetMenuData();
						if (p)
						{
							PopupChannelMenu(p->nXX, p->nYY);
						}
					}
					else
						KPopupMenu::Cancel();
				}
				else
					KPopupMenu::Cancel();
			}
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (nParam)
		{
			m_InputEdit.InsertString((const char*)nParam, -1);
			Wnd_SetFocusWnd(&m_InputEdit);
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

void KUiPlayerBar::PaintWindow()
{
	KWndImage::PaintWindow();

	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName;
	if (m_nCurChannel < 0 || m_nCurChannel >= nChannelDataCount)
	{
		SetCurrentChannel(0);
	}
}

void KUiPlayerBar::PopupChannelMenu(int x, int y)
{
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nChannelDataCount + m_nRecentPlayerName <= 0)
		return;

	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nChannelDataCount + m_nRecentPlayerName));
	if (pMenuData == NULL)
	{
		return;
	}

	KPopupMenu::InitMenuData(pMenuData, nChannelDataCount + m_nRecentPlayerName);
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;
	pMenuData->usMenuFlag |= (PM_F_AUTO_DEL_WHEN_HIDE | PM_F_CANCEL_BY_CALLER);

	pMenuData->nItemTitleIndent = 0;
	pMenuData->byItemTitleUpSpace = 0;

	int i;
	pMenuData->nItemHeight = 0;
	WORD nHei = 0;
	char szText[64];
	for (i = 0; i < nChannelDataCount; i++)
	{
		WORD nPicIndex = -1;
		int nOffset = 0;
		KRColor uColor;
		KRColor uBKColor;
		short nCheckPicIndex = -1;
		if (KUiMsgCentrePad::GetChannelMenuinfo(i, &nPicIndex, &nHei, &uColor, &uBKColor, szText, &nCheckPicIndex))
		{
			pMenuData->Items[i].szData[0] = KTC_INLINE_PIC;
			*((WORD*)(pMenuData->Items[i].szData + 1)) = nPicIndex;
			pMenuData->Items[i].szData[3] = uColor.Color_b.a;
			pMenuData->Items[i].szData[4] = uColor.Color_b.r;
			pMenuData->Items[i].szData[5] = uColor.Color_b.g;
			pMenuData->Items[i].szData[6] = uColor.Color_b.b;
			nOffset = 7;
			if (nHei > pMenuData->nItemHeight)
				pMenuData->nItemHeight = nHei;
			strcpy(pMenuData->Items[i].szData + nOffset, szText);
			pMenuData->Items[i].uDataLen = strlen(szText) + nOffset;
			
			pMenuData->Items[i].szData[pMenuData->Items[i].uDataLen++] = KTC_TAB;

			if (nCheckPicIndex != -1)
			{
				pMenuData->Items[i].szData[pMenuData->Items[i].uDataLen] = KTC_INLINE_PIC;
				*((WORD*)(pMenuData->Items[i].szData + pMenuData->Items[i].uDataLen + 1)) = nCheckPicIndex;
				pMenuData->Items[i].uDataLen += 3;
			}
		}
		pMenuData->Items[i].uBgColor = uBKColor.Color_dw;
		pMenuData->usMenuFlag |= PM_F_TAB_SPLIT_ITEM_TEXT;
	}
	if (nHei > 0)
        pMenuData->nItemRightWidth = MENU_ITEM_DEFAULT_RIGHT_WIDTH;
	else
		pMenuData->nItemRightWidth = 0;

	for (; i < nChannelDataCount + m_nRecentPlayerName; i++)
	{
		if (m_RecentPlayerName[i - nChannelDataCount][0] != 0)
		{
			WORD nPicIndex = -1;
			int nOffset = 0;
			KRColor uColor;
			KRColor uBKColor;
			if (KUiMsgCentrePad::GetPeopleMenuinfo(m_RecentPlayerName[i - nChannelDataCount], &nPicIndex, &nHei, &uColor, &uBKColor))
			{
				pMenuData->Items[i].szData[0] = KTC_INLINE_PIC;
				*((WORD*)(pMenuData->Items[i].szData + 1)) = nPicIndex;
				pMenuData->Items[i].szData[3] = uColor.Color_b.a;
				pMenuData->Items[i].szData[4] = uColor.Color_b.r;
				pMenuData->Items[i].szData[5] = uColor.Color_b.g;
				pMenuData->Items[i].szData[6] = uColor.Color_b.b;
				nOffset = 7;
				if (nHei > pMenuData->nItemHeight)
					pMenuData->nItemHeight = nHei;
			}
			strcpy(pMenuData->Items[i].szData + nOffset, m_RecentPlayerName[i - nChannelDataCount]);
			pMenuData->Items[i].uDataLen = strlen(m_RecentPlayerName[i - nChannelDataCount]) + nOffset;
			pMenuData->Items[i].uBgColor = uBKColor.Color_dw;
		}
		else
		{
			break;
		}
	}
	pMenuData->nNumItem = i;

	pMenuData->nX = x;
	pMenuData->nY = y;
	pMenuData->nSelectedItem = m_nCurChannel;
	KPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_CHANNEL_MENU);
}

void KUiPlayerBar::PopupPhraseMenu(int x, int y, bool bFirstItem)
{
	int nDataCount = g_UiChatPhrase.GetPhraseCount();
	if (nDataCount <= 0)
		return;
	KPopupMenuData* pMenuData = (KPopupMenuData*)malloc(MENU_DATA_SIZE(nDataCount));
	if (pMenuData == NULL)
	{
		return;
	}

	KPopupMenu::InitMenuData(pMenuData, nDataCount);
	pMenuData->usMenuFlag &= ~PM_F_HAVE_HEAD_TAIL_IMG;
	pMenuData->usMenuFlag |= PM_F_AUTO_DEL_WHEN_HIDE;
	pMenuData->nItemTitleIndent = 0;
	pMenuData->byItemTitleUpSpace = 0;

	int i;
	for (i = 0; i < nDataCount; i++)
	{
		pMenuData->Items[i].uDataLen = g_UiChatPhrase.GetPhrase(i, pMenuData->Items[i].szData);
	}
	pMenuData->nNumItem = nDataCount;

	pMenuData->nX = x;
	pMenuData->nY = y;
	pMenuData->nSelectedItem = bFirstItem ? 0 : nDataCount - 1;
	KPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_PHRASE_MENU);
}

#include "../ChatFilter.h"
extern CChatFilter g_ChatFilter;

void KUiPlayerBar::OnSend(BOOL bDirectSend)
{
	char	Buffer[512];
	int     nName = 0;
	int	    nMsgLength = m_InputEdit.GetText(Buffer, sizeof(Buffer), true);

	if (nMsgLength <= 0)
		return;

	//====���浽���������Ϣ��¼��====
	memcpy(m_RecentMsg[m_cLatestMsgIndex], Buffer, nMsgLength);
	m_RecentMsg[m_cLatestMsgIndex][nMsgLength] = 0;
	m_cLatestMsgIndex = (m_cLatestMsgIndex + 1) % MAX_RECENT_MSG_COUNT;
	m_cPreMsgCounter = 0;

	if (!bDirectSend)
	{
		if (TextMsgFilter(Buffer, nMsgLength))	//�ͻ��˿�������ִ�й���
		{
			m_InputEdit.ClearText();
			return;
		}
	}

	if (g_pCoreShell == NULL)
		return;

	//====��ȡ�����Ӵ�ǰ�˿���ָ���е���Ϣ����Ŀ��====
	int bChannel = false;
	int nDestChannel = -1;
	char Name[32];
	Name[0] = 0;
	if (Buffer[nName] == TEXT_CTRL_CHAT_PREFIX || Buffer[nName] == TEXT_CTRL_CHANNEL_PREFIX)
	{
		bChannel = (Buffer[nName] == TEXT_CTRL_CHANNEL_PREFIX);
		while (nName < nMsgLength)
		{
			if (Buffer[nName] == ' ')
			{
				Buffer[nName] = 0;
				nName++;
				break;
			}
			nName++;
		}

		if (bChannel)
		{
			ReplaceSpecialName(Name, 31, Buffer + 1);
			nDestChannel = GetChannelIndex(Name);
		}
		else
		{
			strncpy(Name, Buffer + 1, 31);
		}
		Name[31] = 0;
	}

	if (Name[0] == 0)	//��ָ������
	{
		bChannel = true;
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		if (m_nCurChannel >= 0 && m_nCurChannel < nChannelDataCount)
		{
			nDestChannel = m_nCurChannel;
		}
		else
		{
			//�Ǹ������
			for (int n = 0; n < nChannelDataCount; n++)
			{
				if (KUiMsgCentrePad::IsChannelType(n, KUiMsgCentrePad::ch_Screen))
				{
					nDestChannel = n;
					break;
				}
			}
		}

		if (nDestChannel >= 0)
			strncpy(Name, KUiMsgCentrePad::GetChannelTitle(nDestChannel), 31);
		
		Name[31] = 0;
	}
	else	//ָ�����ֵĻָ�Buffer����
	{
		Buffer[nName - 1] = ' ';
	}

	nMsgLength -= nName;
	
	if (bChannel && !KUiMsgCentrePad::IsChannelType(nDestChannel, KUiMsgCentrePad::ch_GM))	//�ڷ�GMƵ��������GMָ��,�����ͳ�ȥ,����й��
	{
		if (nMsgLength > 3 &&
			Buffer[nName] == '?' &&
			(Buffer[nName + 1] == 'g' || Buffer[nName + 1] == 'G') &&
			(Buffer[nName + 2] == 'm' || Buffer[nName + 2] == 'M'))
		return;
	}

	if (!g_ChatFilter.IsTextPass(Buffer + nName))
	{
		char szWarning[] = "������Ҫ����Ϣ��ʹ�ò����ʵĴ���!"; 
		KUiMsgCentrePad::SystemMessageArrival(szWarning, sizeof(szWarning));
		return;
	}

	char szEvent[1024];
	if (bChannel)
		sprintf(szEvent, APP_CHAT, Name, Buffer + nName);
	else
		sprintf(szEvent, APP_SAY, Name, Buffer + nName);

	szEvent[1023] = 0;

	if (g_UiBase.NotifyEvent(szEvent) == 0)
		return;

	char	Buffer2[1536];

   	nMsgLength = KUiFaceSelector::ConvertFaceText(Buffer2, Buffer + nName, nMsgLength);
	nMsgLength = TEncodeText(Buffer2, nMsgLength);

	if (bChannel)
	{
		DWORD nChannelID = KUiMsgCentrePad::GetChannelID(nDestChannel);
		if (nChannelID != -1)
		{
			KUiMsgCentrePad::CheckChannel(nDestChannel, true);
			OnSendChannelMessage(nChannelID, Buffer2, nMsgLength);
			m_InputEdit.ClearText();
			InputCurrentChannel(nDestChannel, true);
		}
	}
	else if (!IsSelfName(Name))
	{
		OnSendSomeoneMessage(Name, Buffer2, nMsgLength);
		
		int nAdd = AddRecentPlayer(Name);
		if (nAdd >= 0)
		{
			m_InputEdit.ClearText();
			InputCurrentChannel(nAdd, true);
		}
	}
}

BOOL KUiPlayerBar::IsCanSendMessage(const char* Buffer, int nLen, char* szDestName, int nChannelID)
{
	if (m_pSelf)
	{
		if (Buffer && nLen > 0 && !g_ChatFilter.IsTextPass(Buffer))
			return FALSE;

		char szEvent[1024];
		if (nChannelID >= 0)
			sprintf(szEvent, APP_CHAT, szDestName, Buffer);
		else
			sprintf(szEvent, APP_SAY, szDestName, Buffer);

		szEvent[1023] = 0;

		if (g_UiBase.NotifyEvent(szEvent) == 0)
			return FALSE;
	}
	return TRUE;
}

void KUiPlayerBar::OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen)				//����Ƶ�����쵽������
{
	if (nChannelID != -1 && Buffer && nLen > 0)
	{
		int nLeft = KUiMsgCentrePad::PushChannelData(nChannelID, Buffer, nLen);
		if (nLeft < 0)
		{
			char szWarning[] = "ǧ�ﴫ���޷�����ʹ�ã���������ϢƬ�̡�";
			KUiMsgCentrePad::SystemMessageArrival(szWarning, sizeof(szWarning));
		}
		else if (nLeft > 0)
		{
			char szWarning[64];
			sprintf(szWarning, "��Ϣ����%d��󷢳�!", (nLeft + 999 ) / 1000);
			KUiMsgCentrePad::SystemMessageArrival(szWarning, strlen(szWarning) + 1);
		}
	}
}

void KUiPlayerBar::OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen)				//����Ƶ�����쵽������
{
	if (g_pCoreShell && nChannelID != -1 && Buffer && nLen > 0)
	{
		size_t chatsize = sizeof(CHAT_CHANNELCHAT_CMD) + nLen;
		size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

		tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
		pExHeader->ProtocolType = c2s_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_CHANNELCHAT_CMD* pCccCmd = (CHAT_CHANNELCHAT_CMD*)(pExHeader + 1);
		pCccCmd->ProtocolType = chat_channelchat;
		pCccCmd->wSize = chatsize - 1;
		pCccCmd->packageID = -1;
		pCccCmd->filter = 1;
		pCccCmd->channelid = nChannelID;
		pCccCmd->cost = cost;
		pCccCmd->sentlen = nLen;
		memcpy(pCccCmd + 1, Buffer, nLen);

		g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
	}
}

void KUiPlayerBar::OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen)				//����someone���쵽������
{
	if (g_pCoreShell && Name && Name[0] != 0 && Buffer && nLen > 0 && m_pSelf)
	{
		size_t chatsize = sizeof(CHAT_SOMEONECHAT_CMD) + nLen;
		size_t pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

		tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
		pExHeader->ProtocolType = c2s_extendchat;
		pExHeader->wLength = pckgsize - 1;

		CHAT_SOMEONECHAT_CMD* pCscCmd = (CHAT_SOMEONECHAT_CMD*)(pExHeader + 1);
		pCscCmd->ProtocolType = chat_someonechat;
		pCscCmd->wSize = chatsize - 1;
		pCscCmd->packageID = -1;
		strcpy(pCscCmd->someone, Name);
		pCscCmd->sentlen = nLen;
		memcpy(pCscCmd + 1, Buffer, nLen);

		g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
	}
}


void KUiPlayerBar::InputRecentMsg(bool bPrior)
{
	int nCounter;
	if (bPrior)
		nCounter = m_cPreMsgCounter - 1;
	else
		nCounter = m_cPreMsgCounter + 1;
	if (nCounter < 0 && nCounter >= - MAX_RECENT_MSG_COUNT)
	{
		int nIndex = m_cLatestMsgIndex + nCounter;
		if (nIndex < 0)
			nIndex += 8;
		if (m_RecentMsg[nIndex][0])
		{
			m_InputEdit.SetText(m_RecentMsg[nIndex]);
			m_cPreMsgCounter = nCounter;
		}
	}
	else if (nCounter >= 0)
	{
		m_InputEdit.ClearText();
		m_cPreMsgCounter = 0;
	}
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�������ȡ��\���¶���
//--------------------------------------------------------------------------
void KUiPlayerBar::OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.v = 0;
		Pick.eContainer = UOC_IMMEDIA_ITEM;
		pWnd = pPickPos->pWnd;
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
		Drop.Region.v = 0;
		Drop.eContainer = UOC_IMMEDIA_ITEM;
	}

	for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_ImmediaItem[i])
		{
			Drop.Region.h =	Pick.Region.h = i;
			break;
		}
	}
	_ASSERT(i < UPB_IMMEDIA_ITEM_COUNT);
	
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, 
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}

//--------------------------------------------------------------------------
//	���ܣ�ʹ����Ʒ
//--------------------------------------------------------------------------
void KUiPlayerBar::OnUseItem(int nIndex)
{
	if (m_pSelf && nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
	{
		KUiDraggedObject Obj;
		m_pSelf->m_ImmediaItem[nIndex].GetObject(Obj);
		KUiObjAtRegion Info;
		{
			Info.Obj.uGenre = Obj.uGenre;
			Info.Obj.uId = Obj.uId;
			Info.Region.h = nIndex;
			Info.Region.v = 0;
			Info.Region.Width = Info.Region.Height = 0;
		}
		g_pCoreShell->OperationRequest(GOI_USE_ITEM, (int)&Info, UOC_IMMEDIA_ITEM);
	}
}


int KUiPlayerBar::GetChannelIndex(const char* pTitle)
{
	if (pTitle)
	{
		int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
		int n = KUiMsgCentrePad::GetChannelIndex((char*)pTitle);
		if (n >= 0 && n < nChannelDataCount)
			return n;
		
		n = nChannelDataCount;
		for (; n < nChannelDataCount + m_nRecentPlayerName; n++)
		{
			if (m_RecentPlayerName[n - nChannelDataCount][0] == 0)
				break;
			if (strcmp(m_RecentPlayerName[n - nChannelDataCount], pTitle) == 0)
			{
				return n;
			}
		}
	}
	return -1;
}

void KUiPlayerBar::SetCurrentChannel(int nIndex)
{
	if (!m_pSelf)
		return;
	if (nIndex < 0)
		return;

	if (nIndex == m_pSelf->m_nCurChannel)
		return;
	
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nIndex < nChannelDataCount)
	{
		m_pSelf->m_nCurChannel = nIndex;
		WORD nPicIndex = -1;
		if (KUiMsgCentrePad::GetChannelMenuinfo(m_pSelf->m_nCurChannel, &nPicIndex))
		{
			char buffer[3];
			buffer[0] = KTC_INLINE_PIC;
			*((WORD*)(buffer + 1)) = nPicIndex;
			m_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);
		}
	}
	else if (nIndex < nChannelDataCount + m_pSelf->m_nRecentPlayerName)
	{
		m_pSelf->m_nCurChannel = nIndex;
		WORD nPicIndex = -1;
		if (KUiMsgCentrePad::GetPeopleMenuinfo(m_pSelf->m_RecentPlayerName[m_pSelf->m_nCurChannel - nChannelDataCount], &nPicIndex))
		{
			char buffer[3];
			buffer[0] = KTC_INLINE_PIC;
			*((WORD*)(buffer + 1)) = nPicIndex;
			m_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);
		}
		else
			m_pSelf->m_ChannelSwitchBtn.SetText(m_pSelf->m_RecentPlayerName[m_pSelf->m_nCurChannel - nChannelDataCount]);
	}
}

void KUiPlayerBar::InputCurrentChannel(int nIndex, bool bFocus)
{
	if (nIndex < 0)
		return;
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	if (nIndex < nChannelDataCount)
	{
		InputNameMsg(true, KUiMsgCentrePad::GetChannelTitle(nIndex), bFocus);
		SetCurrentChannel(nIndex);
	}
	else if (nIndex < nChannelDataCount + m_nRecentPlayerName)
	{
		InputNameMsg(false, m_RecentPlayerName[nIndex - nChannelDataCount], bFocus);
		SetCurrentChannel(nIndex);
	}
}

//--------------------------------------------------------------------------
//	���ܣ��л�����һ��Ƶ��
//--------------------------------------------------------------------------
void KUiPlayerBar::SwitchChannel(BOOL bUp)
{
	if(m_pSelf)
	{
		int nNewChannelID;
		int nCount = KUiMsgCentrePad::GetChannelCount();
		if(bUp)
		{
			if(m_pSelf->m_nCurChannel < 1)
		    {
			    nNewChannelID = nCount - 1;
		    }
		    else
		    {
    			nNewChannelID = m_pSelf->m_nCurChannel - 1;
		    }
		}
		else
		{
		    if(m_pSelf->m_nCurChannel > nCount - 2)
		    {
			    nNewChannelID = 0;
		    }
		    else
		    {
    			nNewChannelID = m_pSelf->m_nCurChannel + 1;
		    }
		}
		m_pSelf->InputCurrentChannel(nNewChannelID, FALSE);
	}
}


//--------------------------------------------------------------------------
//  ���ܣ��õ�PlayerBar�ﵱǰ��Ƶ��
//--------------------------------------------------------------------------
int KUiPlayerBar::GetCurChannel()
{
	if(m_pSelf)
	    return m_pSelf->m_nCurChannel;
	else
		return -1;
}


//--------------------------------------------------------------------------
//	���ܣ����³������Щ��ֵ����
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateXXXNumber(int& nMana, int& nFullMana)
{
	
	KUiPlayerRuntimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));

	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);

	m_nExperienceFull = Info.nExperienceFull;
	m_nCurLevelExperience = Info.nCurLevelExperience;
	m_nExperience = Info.nExperience;

	KUiStatus* pStatus = KUiStatus::GetIfVisible();
	if (pStatus)
		pStatus->UpdateRuntimeInfo(&Info);

	KUiSceneTimeInfo	Spot;
	g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Spot, 0);
	KUiMiniMap::UpdateSceneTimeInfo(&Spot);

	nMana = max(Info.nMana, 0);
	nFullMana = max(Info.nManaFull, 0);
}

void KUiPlayerBar::UpdateRuntimeAttribute(int& nMoney, int& nLevel)
{
	KUiPlayerAttribute	Info;
	memset(&Info, 0, sizeof(KUiPlayerAttribute));
	g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);
	KUiStatus* pStatus = KUiStatus::GetIfVisible();
	if (pStatus)
		pStatus->UpdateRuntimeAttribute(&Info);

	nMoney = Info.nMoney;
	nLevel = Info.nLevel;
}

//--------------------------------------------------------------------------
//	���ܣ������Ʒ�ڻ�
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateItem(int nIndex, unsigned int uGenre, unsigned int uId)
{
	if (nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
		m_ImmediaItem[nIndex].HoldObject(uGenre, uId, 0, 0);
	}
}

//--------------------------------------------------------------------------
//	���ܣ������ݼ���
//--------------------------------------------------------------------------
void KUiPlayerBar::UpdateSkill(int nIndex, unsigned int uGenre, unsigned int uId)
{
	if (nIndex == 0 || nIndex == 1)
	{
		m_ImmediaSkill[nIndex].HoldObject(uGenre, uId, 0, 0);
	}
}

//--------------------------------------------------------------------------
//	���ܣ���Ҫ����
//--------------------------------------------------------------------------
void KUiPlayerBar::Breathe()
{
	if (g_LoginLogic.GetStatus() != LL_S_IN_GAME)
		return;

	m_DateTime.UpdateData();
	int nMana;
	int nFullMana;
	UpdateXXXNumber(nMana, nFullMana);
	int nMoney;
	int nLevel;
	UpdateRuntimeAttribute(nMoney, nLevel);

	DWORD dwID;
	BYTE cost;
	char* Buffer;
	int nLen;
	int nUseMana = 0;
	int nUseMoney = 0;
	int nUseLevel = 0;

	while(KUiMsgCentrePad::GetChannelData(dwID, cost, Buffer, nLen))
	{
		int nRet = IsHasCost(cost, nMoney, nLevel, nMana, nFullMana,
			nUseMoney, nUseLevel, nUseMana);

		bool bSend = !nRet;
		char szSystem[256];
		szSystem[0] = 0;

		if (!bSend)
		{
			if (nRet == 2 && nUseLevel > 0)
				sprintf(szSystem, "���ĵȼ�����%d�����޷�ʹ��ǧ�ﴫ��������Ϣ��", nUseLevel);

			if (nRet == 1 && nUseMoney > 0)
				sprintf(szSystem, "������������%d�����޷�ʹ��ǧ�ﴫ��������Ϣ��", nUseMoney);

			if (nRet == 3 && nUseMana > 0)
				sprintf(szSystem, "������������%d�㣬�޷�ʹ��ǧ�ﴫ��������Ϣ��", nUseMana);
		}
		else
		{
			if (nUseMoney > 0)
				sprintf(szSystem, "��ʹ��ǧ�ﴫ��, ��������%d����", nUseMoney);
			if (nUseMana > 0)
				sprintf(szSystem, "��ʹ��ǧ�ﴫ��, ��������%d�㡣", nUseMana);
		}

		if (bSend)
			OnDirectSendChannelMessage(dwID, cost, Buffer, nLen);

		KUiMsgCentrePad::PopChannelData(dwID);
		
		if (szSystem[0])
			KUiMsgCentrePad::SystemMessageArrival(szSystem, strlen(szSystem));
	}
	
	if (m_nCurChannel >= 0 && m_nCurChannel < (int)KUiMsgCentrePad::GetChannelCount() &&
		Wnd_GetFocusWnd() == (KWndWindow*)&m_InputEdit)
	{
		cost = KUiMsgCentrePad::GetChannelCost(KUiMsgCentrePad::GetChannelID(m_nCurChannel));
		if (IsHasCost(cost, nMoney, nLevel, nMana, nFullMana,
			nUseMoney, nUseLevel, nUseMana) != 0)
			m_InputEdit.SetFocusBkColor(s_NotCanFocusColor);
		else
			m_InputEdit.SetFocusBkColor(s_CanFocusColor);
	}
}

//0 �ɹ�, �������ĵ�����������
//1 ��ΪnUseMoney,Ǯ����
//2 ��ΪnUseLevel,�ȼ�����
//3 ��ΪnUseMana,��������
int KUiPlayerBar::IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int& nUseMoney, int& nUseLevel, int& nUseMana)
{
	if (cost == 0)//���
	{
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = 0;
	}
	else if (cost == 1)//10Ԫÿ��
	{
		if (nMoney < 10)
		{
			nUseMoney = 10;
			return 1;
		}
		nUseMoney = 10;
		nUseLevel = 0;
		nUseMana = 0;
	}
	else if (cost == 2)//2: <10Lv ? ����˵ : MaxMana/2/��
	{
		if (nLevel < 10)
		{
			nUseLevel = 10;
			return 2;
		}

		if (nMana < nFullMana / 2)
		{
			nUseMana = nFullMana / 2;
			return 3;
		}
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = nFullMana / 2;
	}
	else if (cost == 3)//3: MaxMana/10/��
	{
		if (nMana < nFullMana / 10)
		{
			nUseMana = nFullMana / 10;
			return 3;
		}
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = nFullMana / 10;
	}
	else if (cost == 4)//4: <20Lv ? ����˵ : MaxMana*4/5/��
	{
		if (nLevel < 20)
		{
			nUseLevel = 20;
			return 2;
		}

		if (nMana < nFullMana * 4 / 5)
		{
			nUseMana = nFullMana * 4 / 5;
			return 3;
		}
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = nFullMana * 4 / 5;
	}
	else
	{
		nUseMoney = 0;
		nUseLevel = 0;
		nUseMana = 0;
	}
	return 0;
}

//--------------------------------------------------------------------------
//	���ܣ���������������ַ���
//--------------------------------------------------------------------------

void KUiPlayerBar::InputNameMsg(char bChannel, const char* szName, bool bFocus)
{
	if (m_pSelf == NULL)
		return;

	if (bFocus)
		Wnd_SetFocusWnd(&m_pSelf->m_InputEdit);
	
	if (szName == NULL || szName[0] == 0)	//��������Ϊ�շ���
		return;

	if (bChannel)	//������ΪƵ��ʱ,ʲô������,��Ϊ�û�����Ҫ�˹���,���Ǻ���Ĵ����ѵø���
		return;

	char Buffer[512];
	int nLen = m_pSelf->m_InputEdit.GetText(Buffer, sizeof(Buffer), false);
	if (nLen < 0)
		nLen = 0;
	Buffer[nLen] = 0;

	int nName = 0;
	if (Buffer[nName] == TEXT_CTRL_CHAT_PREFIX)
	{
		while (nName <= nLen)
		{
			if (Buffer[nName] == ' ' ||
				Buffer[nName] == 0)
			{
				Buffer[nName] = 0;
				break;
			}
			nName++;
		}

		if (nName > nLen)	//ԭ������û������
			nName = 0;
	}
	
	if (bChannel)
	{
		m_pSelf->m_InputEdit.SetText("");
		//m_pSelf->m_InputEdit.SetText("&");
		//m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
		//m_pSelf->m_InputEdit.InsertString(" ", 1);
		m_pSelf->m_InputEdit.InsertString(Buffer + nName, nLen - nName);
	}
	else
	{
		if (nName > 0 &&
			(KShortcutKeyCentre::FindCommand(Buffer + 1) >= 0 ||
			KShortcutKeyCentre::ms_FunsMap.find(Buffer + 1) != KShortcutKeyCentre::ms_FunsMap.end()
			))	//��ԭ���봮����"/"���Ǻ����ؼ���ʱ
		{	//ֻ׷��" name"
			m_pSelf->m_InputEdit.InsertString(" ", 1);
			m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
		}
		else
		{
			m_pSelf->m_InputEdit.SetText("/");
			m_pSelf->m_InputEdit.InsertString(szName, strlen(szName));
			m_pSelf->m_InputEdit.InsertString(" ", 1);
			m_pSelf->m_InputEdit.InsertString(Buffer + nName, nLen - nName);
		}
	}
}

int	KUiPlayerBar::FindRecentPlayer(const char* szName)
{
	if (szName == NULL || szName[0] == 0)
		return -1;
	for (int n = 0; n < m_pSelf->m_nRecentPlayerName; n++)
	{
		if (m_pSelf->m_RecentPlayerName[n][0] == 0)
		{
			break;
		}
		if (strcmp(m_pSelf->m_RecentPlayerName[n], szName) == 0)
		{
			return n;
		}
	}

	return -1;
}

//--------------------------------------------------------------------------
//	���ܣ�����indexȡ��ĳ����Ƶ����Ŀ�������
//--------------------------------------------------------------------------
char* KUiPlayerBar::GetRecentPlayerName(int nIndex)
{
	int nChannelCount = KUiMsgCentrePad::GetChannelCount();
	if(m_pSelf && nIndex >= nChannelCount && nIndex < m_pSelf->m_nRecentPlayerName + nChannelCount)
	{
		return m_pSelf->m_RecentPlayerName[nIndex - nChannelCount];
	}
	else
	{
		return "";
	}
}


int KUiPlayerBar::AddRecentPlayer(const char* szName)
{
	if (!m_pSelf)
		return -1;
	if (szName == NULL || szName[0] == 0)
		return -1;
	if (strcmp(m_pSelf->m_szSelfName, szName) == 0)
		return -1;
	int n = m_pSelf->GetChannelIndex(szName);
	if (n >= 0)
		return n;
	n = 0;
	int nChannelDataCount = KUiMsgCentrePad::GetChannelCount();
	for (; n < MAX_RECENTPLAYER_COUNT; n++)
	{
		if (m_pSelf->m_RecentPlayerName[n][0] == 0)
		{
			strncpy(m_pSelf->m_RecentPlayerName[n], szName, 32);
			m_pSelf->m_nRecentPlayerName++;
			return n + nChannelDataCount;
		}
	}

	if (n >= MAX_RECENTPLAYER_COUNT)
	{
		memmove(m_pSelf->m_RecentPlayerName[0], m_pSelf->m_RecentPlayerName[1], sizeof(m_pSelf->m_RecentPlayerName) - 32);
		n = MAX_RECENTPLAYER_COUNT - 1;
		strncpy(m_pSelf->m_RecentPlayerName[n], szName, 32);
		return n + nChannelDataCount;
	}

	return -1;
}

void KUiPlayerBar::ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc)
{
	KUiMsgCentrePad::ReplaceChannelName(szDest, nDestSize, szSrc);
}


bool KUiPlayerBar::GetExp(int& nFull, int& nCurrLevelExp, int& nCurrentExp)
{
	if (m_pSelf)
	{
		nFull = m_pSelf->m_nExperienceFull;
		nCurrLevelExp = m_pSelf->m_nCurLevelExperience;
		nCurrentExp = m_pSelf->m_nExperience;
		return true;
	}
	return false;
}

BOOL KUiPlayerBar::LoadPrivateSetting(KIniFile* pFile)
{
	if (pFile && g_pCoreShell)
	{
		int nName = 0;
		int nLife = 0;
		int nPK = 0;

		if (pFile->GetInteger("Player", "ShowLife", 0, (int*)(&nLife)))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_LIFE, 0, nLife);
		if (pFile->GetInteger("Player", "ShowName", 0, (int*)(&nName)))
			g_pCoreShell->OperationRequest(GOI_SHOW_PLAYERS_NAME, 0, nName);
		if (pFile->GetInteger("Player", "PK", 0, (int*)(&nPK)))
			g_pCoreShell->OperationRequest(GOI_PK_SETTING, 0, nPK);
	}
	return TRUE;
}

int	KUiPlayerBar::SavePrivateSetting(KIniFile* pFile)
{
	if (pFile && g_pCoreShell)
	{
		int nLife = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_LIFE, 0, 0);
		pFile->WriteInteger("Player", "ShowLife", nLife);
		int nName = g_pCoreShell->GetGameData(GDI_SHOW_PLAYERS_NAME, 0, 0);
		pFile->WriteInteger("Player", "ShowName", nName);
		int nPK = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
		pFile->WriteInteger("Player", "PK", nPK);
	}
	return 1;
}

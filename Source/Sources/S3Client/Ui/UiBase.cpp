/*****************************************************************************************
//	��������ϵͳ��������ģ��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-17
*****************************************************************************************/
#include "KWin32.h"
#include "KWin32Wnd.h"
#include "KIniFile.h"
#include "UiBase.h"
#include "KFilePath.h"
#include "KIme.h"
#include "../../core/src/coreshell.h"
#include "../../core/src/GameDataDef.h"
#include "Elem/MouseHover.h"
#include "Elem/WndShowAnimate.h"
#include "Elem/WndObjContainer.h"
#include "UiCase/UiInformation.h"
#include "UiCase/UiInformation2.h"
#include "UiCase/UiSelColor.h"
#include "UiCase/UiPlayerBar.h"
#include "UiCase/UiStatus.h"
#include "UiCase/UiMiniMap.h"
#include "UiCase/UiMsgCentrePad.h"
//#include "UiCase/UiManage.h"
#include "UiCase/UiOptions.h"
#include "UiCase/UiItem.h"
#include "UiCase/UiSkills.h"
#include "UiCase/UiStoreBox.h"
#include "UiCase/UiChatCentre.h"
#include "UiCase/UiChannelSubscibe.h"
#include "UiCase/UiChatStatus.h"
#include "UiCase/UiTeamManage.h"
#include "Uicase/UiShop.h"
#include "UiCase/UiHeaderControlBar.h"
#include "UiCase/UiSysMsgCentre.h"
#include "UiCase/UiMsgSel.h"
#include "UiCase/UiSelPlayerNearby.h"
#include "UiCase/UiSkillTree.h"
#include "UiCase/UiHelper2.h"
#include "UiChatPhrase.h"
#include "ShortcutKey.h"
#include "Elem/Wnds.h"
#include "Elem/PopupMenu.h"
#include "../../Represent/iRepresent/iRepresentShell.h"

extern iRepresentShell*	g_pRepresentShell;
extern iCoreShell*      g_pCoreShell;


#define	UI_INFO_FILE_PATH			"\\Ui"				//������Ϣ�ļ���ŵĸ�Ŀ¼
#define	UI_COMMON_CONFIG_FILE		"\\Ui\\Setting.ini"
#define	UI_DEF_CONFIG_FILE			"\\Ui\\DefConfig.Ini"//�������ü�¼�ļ���
#define	UI_PUBLIC_SETTING			"����.ini"			//ĳ�����淽�����趨�ļ�

#define	UI_USER_DATA_FOLDER			"\\UserData"		//������ݵĴ���Ŀ¼λ��
#define	UI_USER_DATA_TEMP_FOLDER	"\\UserData\\Temp"	//������ݵ���ʱ����Ŀ¼λ��
#define	UI_COMMON_SETTING_FILE		"\\UserData\\UiCommon.ini"//���湫�����õ��ļ�������
#define UI_PRIVATE_SETTING_FILE		"UiConfig.ini"		//����������ݵĴ洢�ļ�

#define	THEME_SECTION			"Theme"
#define	FONT_SECTION			"FontList"

const char*	$Main = "Main";

KUiBase	g_UiBase;

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
KUiBase::KUiBase()
{
	m_CurScheme[0] = 0;
	m_CurSchemePath[0] = 0;
	m_UserAccountId[0] = 0;
	m_pUiCommSettingFile = NULL;	//���湫�����õ��ļ�
	m_pUiPrivateSettingFile = NULL;	//��ǰ�˺ŵ������ļ�
	m_Status = UIS_S_IDLE;
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
KUiBase::~KUiBase()
{
}

//--------------------------------------------------------------------------
//	���ܣ��˳�����
//--------------------------------------------------------------------------
void KUiBase::Exit()
{
	m_UserAccountId[0] = 0;
	CloseCommSettingFile(false);
	ClosePrivateSettingFile(false);
	CloseCommConfigFile();
}

#ifdef _DEBUG
	extern int WND_SHOW_MOUSE_OVER_WND;
#endif

//���������ݵ���ʱ����Ŀ¼λ��
const char*	KUiBase::GetUserTempDataFolder()
{
	return UI_USER_DATA_TEMP_FOLDER;
}

//���������ݵ���ʱ����Ŀ¼�ڵ�ȫ���ļ�
void KUiBase::CleanTempDataFolder()
{
	ClearFolder(UI_USER_DATA_TEMP_FOLDER, false);
}

//�����ǰ���˽�����ݵĴ洢Ŀ¼
void KUiBase::ClearFolder(const char* pszFolder, bool bDeleteFolder)
{
	char szPathFile[MAX_PATH];
	WIN32_FIND_DATA FileData;
	HANDLE hFind;

	if (pszFolder && pszFolder[0])
	{
		g_GetFullPath(szPathFile, (char*)pszFolder);
		int		nLen = strlen(szPathFile) + 1;
		strcat(szPathFile, "\\*.*");
		hFind = FindFirstFile(szPathFile, &FileData);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				strcpy(szPathFile + nLen, FileData.cFileName);
				SetFileAttributes(szPathFile, FILE_ATTRIBUTE_NORMAL);
				DeleteFile(szPathFile);
			}while(FindNextFile(hFind, &FileData));
			FindClose(hFind);
		}
		if (bDeleteFolder)
		{
			szPathFile[nLen - 1] = 0;
			RemoveDirectory(szPathFile);
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
int KUiBase::Init()
{
	if (g_pIme)
	{
		g_pIme->DisableLanguageChange();
		g_pIme->CloseIME();
		g_pIme->TurnOn();
	}

	KIniFile* pSetting = GetCommSettingFile();
	char	SchemeName[32] = "";
	int		nInterval = 20;
	if (pSetting)
	{
		pSetting->GetString("Main", "LastUsedScheme", "", SchemeName, 32);
		pSetting->GetInteger("Main", "WndMoveInterval", 20, &nInterval);
		#ifdef _DEBUG
			int	nValue;
			pSetting->GetInteger("Main", "ShowDebugFrameText", 0, &nValue);
			WND_SHOW_DEBUG_FRAME_TEXT = nValue;
			pSetting->GetInteger("Main", "ShowMouseOverWnd", 0, &nValue);
			WND_SHOW_MOUSE_OVER_WND = nValue;
		#endif
			CloseCommSettingFile(false);
	}
	g_CreatePath(UI_USER_DATA_FOLDER);
	g_CreatePath(UI_USER_DATA_TEMP_FOLDER);
	KWndShowAnimate::SetInterval(nInterval);
	LoadScheme(SchemeName);
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ������û��˺�
//--------------------------------------------------------------------------
void KUiBase::SetUserAccount(char* pszId, char* pszRole)
{
	if (pszId == NULL || pszId[0] == 0 || pszRole == NULL || pszRole[0] == 0)
		return;

	//����(���)����id��ö�Ӧ������ַ���id
	{
		m_UserAccountId[0] = 0;
		unsigned int	uID1 =  g_FileName2Id(pszId);
		unsigned int	uID2 = ~g_FileName2Id(pszRole);
		int				nLen1 = strlen(pszId);
		int				nLen2 = strlen(pszRole);
		sprintf(m_UserAccountId, "%x%xl%x%x", uID1, nLen1, nLen2, uID2);
	}
}

//�����ǰ���˽�����ݵĴ洢Ŀ¼
void KUiBase::CleanPrivateDataFolder()
{
	char	szBuffer[128];
	sprintf(szBuffer, "%s\\%s", UI_USER_DATA_FOLDER, m_UserAccountId);
	ClearFolder(szBuffer, true);
}

//�����ǰ���˽�����ݵ�ĳ���ļ�
void KUiBase::DeletePrivateDataFile(const char* pszFileName)
{
	char	szBuffer[128], szPathFile[MAX_PATH];

	if (pszFileName && pszFileName[0])
	{
		sprintf(szBuffer, "%s\\%s\\%s", UI_USER_DATA_FOLDER, m_UserAccountId, pszFileName);
		g_GetFullPath(szPathFile, (char*)szBuffer);
		SetFileAttributes(szPathFile, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(szPathFile);
	}
}

//--------------------------------------------------------------------------
//	���ܣ��������ü�¼���綨�ƵĴ���λ��
//--------------------------------------------------------------------------
void CreateBlackListUnit();
void KUiBase::LoadPrivateConfig()
{
	CreateBlackListUnit();

	KIniFile*	pConfigFile = GetPrivateSettingFile();

	if (pConfigFile)
	{
		//----��ȡ���ڷ����趨----
		char	SchemeName[32];
		if (pConfigFile->GetString("Main", "Scheme", "", SchemeName, 32))
		{
			if (SchemeName[0] && LoadScheme(SchemeName))
			{
			//----����������������趨----
				KUiPlayerBar::LoadPrivateSetting(pConfigFile);
				KUiSkillTree::LoadConfig(pConfigFile);
				KShortcutKeyCentre::LoadPrivateSetting(pConfigFile);
				KUiChatCentre::LoadPrivateSetting(pConfigFile);	//��KShortcutKeyCentre֮��,��Ϊ���нű�����Unit
			//----����������������趨����----
			}
		}
		ClosePrivateSettingFile(false);
	}
}

//--------------------------------------------------------------------------
//	���ܣ��洢���ü�¼
//--------------------------------------------------------------------------
int KUiBase::SavePrivateConfig()
{
	if (m_UserAccountId[0] == 0)
		return true;

	KIniFile* pConfigFile = GetPrivateSettingFile();
	if (pConfigFile)
	{
		pConfigFile->Clear();

		pConfigFile->WriteString("Main", "Scheme", m_CurScheme);
		//----������ڱ��������趨----
		KUiSkillTree::SaveConfig(pConfigFile);
		KUiPlayerBar::SavePrivateSetting(pConfigFile);
		KUiChatCentre::SavePrivateSetting(pConfigFile);
		KShortcutKeyCentre::SavePrivateSetting(pConfigFile);
		//----������ڱ��������趨����----
		ClosePrivateSettingFile(true);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ���õ�ǰ����˺��ڽ���ϵͳ�е�id
//--------------------------------------------------------------------------
int KUiBase::GetUserPrivateDataFolder(char* pBuffer, int nSize)
{
	char	szBuffer[128];
	sprintf(szBuffer, "%s\\%s\\", UI_USER_DATA_FOLDER, m_UserAccountId);
	g_CreatePath(szBuffer);
	if (pBuffer && nSize > (int)(strlen(szBuffer)))
	{
		strcpy(pBuffer, szBuffer);
		return true;
	}
	return false;
}


int g_nWindowNTFlag = false;

static int _KSG_GetWindowVersion()
{  
    g_nWindowNTFlag = (GetVersion() < 0x80000000);

    return true;
}

//--------------------------------------------------------------------------
//	���ܣ�����ĳ�ַ����������(Skin)
//--------------------------------------------------------------------------
int KUiBase::LoadScheme(const char* pScheme)
{
    _KSG_GetWindowVersion();

	if (pScheme == NULL)
		return false;
	
	if (m_CurScheme[0] != 0 && strcmp(pScheme, m_CurScheme) == 0)
		return true;

	char	Buffer[128];

	sprintf(Buffer, "%s\\"UI_PUBLIC_SETTING, m_CurSchemePath);
	if (GetSchemePath(pScheme))
	{
		int			nCount, nId, i;
		KIniFile	Ini;

		char		Section[8];
		//----ж������----
		if (g_pRepresentShell && Ini.Load(Buffer) &&
			Ini.GetInteger(FONT_SECTION, "Count", 0, &nCount))
		{
			for (i = 0; i < nCount; i++)
			{
				itoa(i, Section, 10);
				if (Ini.GetInteger(FONT_SECTION, Section, 0, &nId))
					g_pRepresentShell->ReleaseAFont(nId);
			}
		}

		sprintf(Buffer, "%s\\"UI_PUBLIC_SETTING, m_CurSchemePath);
		Ini.Load(Buffer);

		//----��������----
		if (g_pRepresentShell && Ini.GetInteger(FONT_SECTION, "Count", 0, &nCount))
		{
			for (i = 0; i < nCount; i++)
			{
				itoa(i, Section, 10);
				if (Ini.GetInteger(FONT_SECTION, Section, 0, &nId))
				{
					strcat(Section, "_File");
					if (Ini.GetString(FONT_SECTION, Section, "", Buffer, sizeof(Buffer)) &&
							Buffer[0])
					{
						g_pRepresentShell->CreateAFont(Buffer, CHARACTER_CODE_SET_GBK, nId);
					}
				}
			}			
		}
		//----�����������----
		WndObjContainerInit(&Ini);

		for (i = CURSOR_NORMAL; i < CURSOR_INDEX_COUNT; i++)
		{
			itoa(i, Section, 10);
            if (g_nWindowNTFlag)
			    Ini.GetString("CursorList", Section, "", Buffer, sizeof(Buffer));
            else    // Win9x
			    Ini.GetString("CursorList98", Section, "", Buffer, sizeof(Buffer));

			Wnd_LoadCursor(i, Buffer);//�������ָ����Դ
		}
		
		LoadSchemeForEachWnd();
		
		return true;
	}
	return false;
}

void KUiBase::LoadSchemeForEachWnd()
{
	//----������ڽ��淽��----
	KUiPlayerBar::LoadScheme(m_CurSchemePath);
	KUiStatus::LoadScheme(m_CurSchemePath);
	KUiMsgCentrePad::LoadScheme(m_CurSchemePath);
//	KUiManage::LoadScheme(m_CurSchemePath);
	KUiOptions::LoadScheme(m_CurSchemePath);
	KUiItem::LoadScheme(m_CurSchemePath);
	KUiSkills::LoadScheme(m_CurSchemePath);
	KUiStoreBox::LoadScheme(m_CurSchemePath);
	KUiMsgSel::LoadScheme(m_CurSchemePath);
	KUiSelPlayerNearby::LoadScheme(m_CurSchemePath);
	KUiChatCentre::LoadScheme(m_CurSchemePath);
	KUiChatStatus::LoadScheme(m_CurSchemePath);
	KUiTeamManage::LoadScheme(m_CurSchemePath);
	KUiHeaderControlBar::LoadScheme(m_CurSchemePath);
	KUiShop::LoadScheme(m_CurSchemePath);
	KUiSysMsgCentre::LoadScheme(m_CurSchemePath);
	KUiMiniMap::LoadScheme(m_CurSchemePath);
	g_MouseOver.LoadScheme(m_CurSchemePath);
	g_UiInformation.LoadScheme(m_CurSchemePath);
	g_UiInformation2.LoadScheme(m_CurSchemePath);
	KPopupMenu::LoadTheme(m_CurSchemePath);
	KUiSelColor::LoadScheme(m_CurSchemePath);
	//----������ڽ��淽������----
}

//--------------------------------------------------------------------------
//	���ܣ���õ�ǰָ�����淽����Ŀ¼�������δ���ҵ�ָ���ķ�������ȡ��һ��
//		��������Ĭ�Ϸ�������Ϊ��ǰ����
//	������pScheme --> ָ�����淽������
//--------------------------------------------------------------------------
int KUiBase::GetSchemePath(const char* pScheme)
{
	KIniFile* pIni = NULL;
	int			nCount;
	char		Buff[32], Key[32];

	if (pScheme == NULL ||
		(pIni = GetCommConfigFile()) == NULL)
		return false;

	pIni->GetInteger(THEME_SECTION, "Count", 0, &nCount);
	for (int i = 0; i < nCount; i++)
	{
		itoa(i, Key, 10);
		if (pIni->GetString(THEME_SECTION, Key, "", Buff, 32))
		{
			if (strcmp(Buff, pScheme) == 0)
			{
				strcpy(m_CurScheme, pScheme);
				strcat(Key, "_Path");
				if (pIni->GetString(THEME_SECTION, Key, "", Buff, 32) && Buff[0])
				{
					sprintf(m_CurSchemePath, "%s\\%s", UI_INFO_FILE_PATH, Buff);
					CloseCommConfigFile();
					return true;
				}
			}
		}
	}
	if (pIni->GetString(THEME_SECTION, "0", "", m_CurScheme, 32) &&
		pIni->GetString(THEME_SECTION, "0_Path", "", Buff, 32) && Buff[0])
	{
		sprintf(m_CurSchemePath, "%s\\%s", UI_INFO_FILE_PATH, Buff);
		CloseCommConfigFile();
		return true;
	}
	CloseCommConfigFile();
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ���ý��淽������Ŀ
//--------------------------------------------------------------------------
int KUiBase::SchemeCount()
{
	int			nCount = 0;
	KIniFile*	pIni = GetCommConfigFile();
	if (pIni)
	{
		pIni->GetInteger(THEME_SECTION, "Count", 0, &nCount);
		if (nCount < 0)
			nCount = 0;
		CloseCommConfigFile();
	}
	return nCount;
}

//--------------------------------------------------------------------------
//	���ܣ����ĳ�����淽����������·��
//	������pName --> ���ڻ�ȡ���������ƣ�Ϊָ��һ�����Ȳ�С�ڵ�32�Ļ�������ָ�룬���߿�ָ�롣
//		  pPath --> ���ڻ�ȡ������·����Ϊָ��һ�����Ȳ�С�ڵ�32�Ļ�������ָ�룬���߿�ָ�롣
//--------------------------------------------------------------------------
int KUiBase::GetScheme(int nIndex, char* pName, char* pPath)
{
	KIniFile	Ini;
	int			nCount = 0;
	KIniFile*	pIni = GetCommConfigFile();
	if (pIni)
	{
		Ini.GetInteger(THEME_SECTION, "Count", 0, &nCount);
		if (nIndex >= 0 && nIndex < nCount)
		{
			char	Num[8] = "";
			itoa(nIndex, Num, 10);
			if (pName)
				Ini.GetString(THEME_SECTION, Num, "", pName, 32);
			if (pPath)
				Ini.GetString(THEME_SECTION, Num, "", pPath, 32);
			return true;
		}
		CloseCommConfigFile();
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ���õ�ǰ���淽����·��
//--------------------------------------------------------------------------
int KUiBase::GetCurSchemePath(char* pBuffer, int nSize)
{
	if (pBuffer && nSize >= (int)strlen(m_CurSchemePath) + 1)
	{
		strcpy(pBuffer, m_CurSchemePath);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪱�����湫�����õ��ļ�
//--------------------------------------------------------------------------
KIniFile*	KUiBase::GetCommSettingFile()
{
	if (m_pUiCommSettingFile == NULL)
	{
		m_pUiCommSettingFile = new KIniFile;
		if (m_pUiCommSettingFile)
			m_pUiCommSettingFile->Load((char*)UI_COMMON_SETTING_FILE);
	}
	return m_pUiCommSettingFile;
}

//--------------------------------------------------------------------------
//	���ܣ��رձ�����湫�����õ��ļ�
//--------------------------------------------------------------------------
void KUiBase::CloseCommSettingFile(bool bSave)
{
	if (m_pUiCommSettingFile)
	{
		if (bSave)
			m_pUiCommSettingFile->Save((char*)UI_COMMON_SETTING_FILE);
		delete(m_pUiCommSettingFile);
		m_pUiCommSettingFile = NULL;
	}
}

KIniFile* KUiBase::GetCommConfigFile()
{
	if (m_pUiCommConfigFile == NULL)
	{
		m_pUiCommConfigFile = new KIniFile;
		if (m_pUiCommConfigFile &&
			!m_pUiCommConfigFile->Load((char*)UI_COMMON_CONFIG_FILE))
		{
			delete m_pUiCommConfigFile;
			m_pUiCommConfigFile = NULL;
		}
	}
	return m_pUiCommConfigFile;
}

void KUiBase::CloseCommConfigFile()
{
	if (m_pUiCommConfigFile)
	{
		delete(m_pUiCommConfigFile);
		m_pUiCommConfigFile = NULL;
	}
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴򿪵�ǰ�˺ŵ������ļ�
//--------------------------------------------------------------------------
KIniFile*	KUiBase::GetPrivateSettingFile()
{	
	if (m_pUiPrivateSettingFile == NULL && m_UserAccountId[0])
	{
		m_pUiPrivateSettingFile = new KIniFile;
		if (m_pUiPrivateSettingFile)
		{
			char	FileName[128];
			sprintf(FileName, "%s\\%s\\%s", UI_USER_DATA_FOLDER, m_UserAccountId, UI_PRIVATE_SETTING_FILE);
			m_pUiPrivateSettingFile->Load(FileName);
		}
	}
	return m_pUiPrivateSettingFile;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ򿪵�ǰ�˺ŵ������ļ�
//--------------------------------------------------------------------------
void KUiBase::ClosePrivateSettingFile(bool bSave)
{
	if (m_pUiPrivateSettingFile)
	{
		if (bSave && m_UserAccountId[0])
		{			
			char	FileName[128];
			sprintf(FileName, "%s\\%s", UI_USER_DATA_FOLDER, m_UserAccountId);
			g_CreatePath(FileName);
			strcat(FileName, "\\");
			strcat(FileName, UI_PRIVATE_SETTING_FILE);			
			m_pUiPrivateSettingFile->Save(FileName);
		}
		delete(m_pUiPrivateSettingFile);
		m_pUiPrivateSettingFile = NULL;
	}
}

void KUiBase::SetStatus(UISYS_STATUS eStatus)
{
	if (m_Status == eStatus)
		return;
	m_Status = eStatus;
	Wnd_GameSpaceHandleInput(m_Status == UIS_S_IDLE);
	SetCurrentCursor();
}

//���ݵ�ǰ״̬�������ָ��ͼ��
void KUiBase::SetCurrentCursor()
{
	int	nCursorIndex;
	switch(m_Status)
	{
	case UIS_S_TRADE_PLAYER:	//����Player������
	case UIS_S_TRADE_NPC:		//����npc������
		nCursorIndex = CURSOR_NORMAL;
		break;
	case UIS_S_TRADE_SALE:		//����npc������ ��
		nCursorIndex = CURSOR_SELL;
		break;
	case UIS_S_TRADE_BUY:		//����npc������ ��
		nCursorIndex = CURSOR_BUY;
		break;
	case UIS_S_TRADE_REPAIR:	//����npc������ ����
		nCursorIndex = CURSOR_REPAIR;
		break;
	default:
		nCursorIndex = CURSOR_NORMAL;
		break;
	}
	Wnd_SwitchCursor(nCursorIndex);
}

int	KUiBase::IsOperationEnable(UISYS_OPERATION uOper)
{
	int bEnable = false;
	switch(uOper)
	{
	case UIS_O_MOVE_ITEM:
	case UIS_O_USE_ITEM:
		bEnable = (m_Status == UIS_S_IDLE);
		break;
	case UIS_O_TRADE_ITEM:
		bEnable = (m_Status == UIS_S_TRADE_PLAYER || m_Status == UIS_S_TRADE_NPC ||
			m_Status == UIS_S_TRADE_SALE || m_Status == UIS_S_TRADE_BUY);
		break;
	}
	return bEnable;
}

void KUiBase::RegisterEvent(const char* szName, HANDLE hWnd)
{
	if (hWnd == 0)
		return;

	EVENTWNDLIST::iterator i = m_EventWndList.find(szName);
	if (i == m_EventWndList.end())
	{
		m_EventWndList[szName] = hWnd;
	}
}

void KUiBase::UnregisterEvent(const char* szName, HANDLE hWnd)
{
	if (hWnd == 0)
		return;

	EVENTWNDLIST::iterator i = m_EventWndList.find(szName);
	if (i != m_EventWndList.end() && i->second == hWnd)
	{
		m_EventWndList.erase(i);
	}
}

int KUiBase::NotifyOneEvent(const char* szName, const char* szEvent)		//0Ϊ�Ѿ�����, ��Ҫ�ٴ�����, ��0Ϊδ��������
{
	if (szEvent == NULL || szEvent[0] == 0)
		return -1;

	EVENTWNDLIST::iterator i = m_EventWndList.find(szName);
	if (i == m_EventWndList.end())
		return -1;

	int nLen = strlen(szEvent) + 1;

	int nRet = -1;
	if (i->second)
	{
		COPYDATASTRUCT cp;
		cp.dwData = 0;
		cp.cbData = nLen;
		cp.lpData = (void*)szEvent;
		nRet = ::SendMessage((HWND)i->second, WM_COPYDATA, (WPARAM)g_GetMainHWnd(), (LPARAM)&cp);
	}

	return nRet;
}

int KUiBase::NotifyEvent(const char* szEvent)		//0Ϊ�Ѿ�����, ��Ҫ�ٴ�����, ��0Ϊδ��������
{
	if (szEvent == NULL || szEvent[0] == 0)
		return -1;

	int nLen = strlen(szEvent) + 1;

	int nRet = -1;
	EVENTWNDLIST::iterator i = m_EventWndList.begin();
	while(i != m_EventWndList.end())
	{
		if (i->second)
		{
			COPYDATASTRUCT cp;
			cp.dwData = 0;
			cp.cbData = nLen;
			cp.lpData = (void*)szEvent;
			nRet = ::SendMessage((HWND)i->second, WM_COPYDATA, (WPARAM)g_GetMainHWnd(), (LPARAM)&cp);
			if (!nRet)
				return nRet;
		}
		i++;
	}

	return nRet;
}

///////////////////////////////////////////////////////////////////////////////

int AddinNotify::CreateUnit(const char* Unit)
{
	if (Unit && Unit[0])
	{
		char szEvent[256];
		sprintf(szEvent, APP_CREATEUNIT, Unit);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int AddinNotify::DeleteUnit(const char* Unit)
{
	if (Unit && Unit[0])
	{
		char szEvent[256];
		sprintf(szEvent, APP_DELETEUNIT, Unit);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int AddinNotify::CreateUnitGroup(const char* Unit, const char* Name)
{
	if (Unit && Unit[0] && Name)
	{
		char szEvent[256];
		sprintf(szEvent, APP_CREATEUNITGROUP, Unit, Name);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int AddinNotify::RenameUnitGroup(const char* Unit, const char* Name, const char* NewName, const STRINGLIST& friends)
{
	if (Unit && Unit[0] && Name && NewName)
	{
		char szEvent[512];
		int nLen = sprintf(szEvent, APP_RENAMEUNITROUP, Unit, Name, NewName);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int AddinNotify::DeleteUnitGroup(const char* Unit, const char* Name, const STRINGLIST& friends)
{
	if (Unit && Unit[0] && Name)
	{
		char szEvent[512];
		int nLen = sprintf(szEvent, APP_DELETEUNITROUP, Unit, Name);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int AddinNotify::MoveUnitGroup(const char* Unit, const char* Name, const char* Name2, const STRINGLIST& friends)
{
	if (Unit && Unit[0] && Name && Name2)
	{
		char szEvent[512];
		int nLen = sprintf(szEvent, APP_MOVEUNITROUP, Unit, Name, Name2);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int	AddinNotify::CreateUnitMember(const char* Unit, const char* Name, const char* Group)
{
	if (Unit && Unit[0] && Name && Group)
	{
		char szEvent[256];
		sprintf(szEvent, APP_CREATEUNITMEMBER, Unit, Name, Group);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int AddinNotify::DeleteUnitMember(const char* Unit, const char* Name)
{
	if (Unit && Unit[0] && Name)
	{
		char szEvent[256];
		sprintf(szEvent, APP_DELETEUNITMEMBER, Unit, Name);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int	AddinNotify::MoveUnitMember(const char* Unit, const char* Name, const char* Group)
{
	if (Unit && Unit[0] && Name && Group)
	{
		char szEvent[256];
		sprintf(szEvent, APP_MOVEUNITMEMBER, Unit, Name, Group);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

int	AddinNotify::SetUnitMemberStatus(const char* Unit, const char* Name, const char* status)
{
	if (Unit && Unit[0] && Name && status)
	{
		char szEvent[256];
		sprintf(szEvent, APP_SETUNITMEMBERSTATUS, Unit, Name, status);
		return g_UiBase.NotifyEvent(szEvent);
	}

	return 1;
}

AddinNotify s_ANotify;

void CreateAddinUnit(const char* UnitName)
{
	KUiChatCentre::AddAddinUnit(UnitName, &s_ANotify);
}

void DeleteAddinUnit(const char* UnitName)
{
	KUiChatCentre::RemoveAddinUnit(UnitName);
}
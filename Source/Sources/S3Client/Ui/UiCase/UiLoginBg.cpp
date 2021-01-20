/*****************************************************************************************
//	����--login���̱�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-16
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiLoginBg.h"

#define	SCHEME_INI				"��½���̱�������.ini"

KUiLoginBackGround* KUiLoginBackGround::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiLoginBackGround* KUiLoginBackGround::OpenWindow(const char* pszConfig)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiLoginBackGround;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		SetConfig(pszConfig);
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiLoginBackGround::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
		{
			m_pSelf->Hide();
		}
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
void KUiLoginBackGround::Initialize()
{
	AddChild(&m_VersionWnd);
	m_szConfig[0] = 0;
	for (int i = 0; i < MAX_NUM_BUTTERFLY; i++)
	{
		m_uAdviceIntervalMin[i] = 0;
		m_uInterval[i] = 0;
		m_uLastSleepTime[i] = 0;
		AddChild(&m_Butterflys[i]); 
	}

	char	szVersion[80] = "";
	KIniFile* pSetting = g_UiBase.GetCommConfigFile();
	if (pSetting)
	{
		if (pSetting)
			pSetting->GetString("Main", "GameVersion", "", szVersion, sizeof(szVersion));
		g_UiBase.CloseCommConfigFile();
	}
	m_VersionWnd.SetText(szVersion);

	Wnd_AddWindow(this, WL_LOWEST);
}

//����ͼ��
void KUiLoginBackGround::SetConfig(const char* pszConfig)
{
	if (m_pSelf == NULL || pszConfig == NULL || pszConfig[0] == 0 ||
		strcmp(pszConfig, m_pSelf->m_szConfig) == 0)
	{
		return;
	}

	strncpy(m_pSelf->m_szConfig, pszConfig, sizeof(m_pSelf->m_szConfig));

	char Buff[256];
	KIniFile	Ini;
	g_UiBase.GetCurSchemePath(Buff, 256);
	strcat(Buff, "\\");
	strcat(Buff, SCHEME_INI);
	if (Ini.Load(Buff))
		m_pSelf->LoadScheme(&Ini);
}

//������淽��
void KUiLoginBackGround::LoadScheme(KIniFile* pIni)
{
	if (pIni)
	{
		char	Buff[128];
		strcpy(Buff, m_szConfig);
		Init(pIni, Buff);
		m_VersionWnd.Init(pIni, "VersionText");
		for (int i = 0; i < MAX_NUM_BUTTERFLY; i++)
		{
			sprintf(Buff, "%s_Butterfly_%d", m_szConfig, i);
			m_Butterflys[i].Init(pIni, Buff);
			m_uAdviceIntervalMin[i] = m_uAdviceIntervalMax[i] = 0;
			pIni->GetInteger2(Buff, "Interval", (int*)&m_uAdviceIntervalMin[i],
				(int*)&m_uAdviceIntervalMax[i]);
			if (m_uAdviceIntervalMin[i] && m_uAdviceIntervalMax[i] < m_uAdviceIntervalMin[i])
			{
				m_uAdviceIntervalMax[i] = m_uAdviceIntervalMin[i];
			}

			m_uInterval[i] = 0;
		}
	}
}

//�����
void KUiLoginBackGround::Breathe()
{
	for (int i = 0; i < MAX_NUM_BUTTERFLY; i++)
	{
		if (m_uAdviceIntervalMin[i])
		{
			if (m_uInterval[i] == 0)
			{
				if (m_Butterflys[i].NextFrame())
				{
					m_uLastSleepTime[i] = IR_GetCurrentTime();
					m_uInterval[i] = m_uAdviceIntervalMin[i] +
						(rand() % (m_uAdviceIntervalMax[i] - m_uAdviceIntervalMin[i]));
				}
			}
			else if (IR_IsTimePassed(m_uInterval[i], m_uLastSleepTime[i]))
			{
				m_Butterflys[i].UpdateTimer();
				m_uInterval[i] = 0;
			}
		}
		else
		{
			m_Butterflys[i].NextFrame();
		}
	}
}
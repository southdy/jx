/*****************************************************************************************
//	����--�������������
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-3-23
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#define	MAX_PHRASE_COUNT	10

class KUiChatPhrase
{
public:
	KUiChatPhrase();
	~KUiChatPhrase();
	int		GetPhrase(int nIndex, char* pszPhrase);
	int		SetPhrase(int nIndex, char* pszPhrase, int nLen);
	int		GetPhraseCount();
	int		SavePrivateSetting(KIniFile* pFile, LPCSTR lpSection, int nStart);
	void	ClearAllPhrase();

	enum EMOTE_TYPE
	{
		EMOTE_T_PHRASE = 0,
		EMOTE_T_MENU,
	};
	//�����������춯���ļ�
	int     LoadEntireEmote();
	//�������춯�����и���index�ľ͸�index�ģ�û�о͸���CmdѰ��Ҫ�滻��
	int     SetEmote(char *szCmd, char *szName, char *szStringTarget, char *szStringSelf, int nIndex = -1);
	//
	int     GetEmoteCount(EMOTE_TYPE eType);
	//���������ȡһ����������(pBuff)�������������ȣ������ǰ�����MENU��PHRASE��
	int     GetEmote(const char *szCmd, char *pBuff, int nBuffLen, int nType = 0);
	//����������ȡһ����������(pBuff)�������������ȣ��������ǰ�����MENU��PHRASE��
	int     GetEmote(int nIndex, char *pBuff, int nBuffLen, int nType = 0);
	//��ȡһ�����ڲ˵��ϵĶ�������(pBuff)�������������ȣ�����MENU
	int     GetMenuEmote(const char *szCmd, char *pBuff, int nBuffLen, int nType = 0);
	int     GetMenuEmote(int nIndex, char *pBuff, int nBuffLen, int nType = 0);
	//ȡ��ָ�������Ķ�������
	int     GetMenuEmoteName(int nIndex, char *pBuff, int nBuffLen);
	//�����ַ���
	int     ConvertEmoteString(char *szString, int nStringLen, const char *szMyName, const char *szTarName);

	//�Ѷ�ȡ����Emote��Ϣ�����ȥShortcutϵͳ�еĺ���������
	int     ConstructFunctionAlias();

private:
	struct  EMOTE_PHRASE
	{
		char    szName[16];
		int     nNameLen;
		char    szString[128];
		int     nStringLen;
		char    szStringMe[128];
		int     nStringMeLen;
		char    szCmd[64];
	} *m_pEmotePhrase, *m_pMenuEmote;
	
	int     LoadEmotePhrase(KIniFile *pIni);
	int     LoadMenuEmote(KIniFile *pIni);
	int     HandleLoad(KIniFile *pIni, EMOTE_PHRASE *pData, int nCount, const char *szSection);
	int     FindEmote(EMOTE_TYPE eType, const char *szCmd);

private:
	struct	CHAT_PHRASE
	{
		char	szString[64];
		int		nStringLen;
	}		m_PhraseList[MAX_PHRASE_COUNT];

	int m_nEmotePhraseCount;
	int m_nMenuEmoteCount;

	unsigned int m_uMyNameColor;
	unsigned int m_uTargetColor;
};

extern KUiChatPhrase	g_UiChatPhrase;
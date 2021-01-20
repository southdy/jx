/*****************************************************************************************
//	����--��Ϸ��ʾ�����Ϣ������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-16
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndValueImage.h"
#include "../Elem/WndObjContainer.h"
#include "../Elem/WndLabeledButton.h"

#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include "../../../Core/src/GameDataDef.h"
#include "UiMsgCentrePad.h"


#include "../Elem/ComWindow.h"
//ʱ��
class GameWorld_DateTime : public KWndLabeledButton
{
public:
	DECLARE_COMCLASS(GameWorld_DateTime)
	GameWorld_DateTime();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	void            UpdateData();

private:
	int             m_nSmoothPing, m_nCrowdPing, m_nBlockPing;
	bool			m_bTrueWorld;
	bool            m_bNetStatus;
	bool			m_bLogo;
	char            m_szLogo[32], m_szSmoothMsg[32], m_szCrowdMsg[32], m_szBlockMsg[32];
	unsigned int	m_uLastSwitchTime, m_uDefaultColor, m_uSmoothColor, m_uCrowdColor, m_uBlockColor;

private:
	void OnButtonClick();
	void UpdateNetStatus();
};

class KImmediaItem : public KWndObjectBox
{
public:
	int m_nIndex;
	DWORD m_TextColor;
	KImmediaItem() :m_nIndex(-1), m_TextColor(0xFFFFFF00){}
	void PaintWindow();
	int	Init(KIniFile* pIniFile, const char* pSection);
};

struct KUiDraggedObject;
struct KPopupMenuData;
class KIniFile;

class KUiPlayerBar : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiPlayerBar* OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiPlayerBar* GetIfVisible();				//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void			LoadScheme(const char* pScheme);//������淽��

	static void			InputNameMsg(char bChannel, const char* szName, bool bFocus);
	static int			FindRecentPlayer(const char* szName);
	static int          AddRecentPlayer(const char* szName);
	static char*        GetRecentPlayerName(int nIndex);//����indexȡ��ĳ����Ƶ����Ŀ�������

	static const char*	SelfName()
	{
		if (m_pSelf)
			return m_pSelf->m_szSelfName;
		return NULL;
	}
	static BOOL	IsSelfName(char* szName)
	{
		if (m_pSelf && szName)
		{
			return (strcmpi(m_pSelf->m_szSelfName, szName) == 0);
		}
		return FALSE;
	}
	static void	OnUseItem(int nIndex);				//ʹ�ÿ����Ʒ
	static BOOL	IsCanSendMessage(const char* Buffer, int nLen, char* szDestName, int nChannelID);				//�ܷ�����?
	static void	OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen);				//����Ƶ�����쵽������
	static void	OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen);				//����someone���쵽������

	void	UpdateItem(int nIndex, unsigned int uGenre, unsigned int uId);	//�����Ʒ�ڻ�
	void	UpdateSkill(int nIndex, unsigned int uGenre, unsigned int uId);	//�����ݼ���

	static	bool		GetExp(int& nFull, int& nCurrLevelExp, int& nCurrentExp);

	static BOOL			LoadPrivateSetting(KIniFile* pFile);
	static int			SavePrivateSetting(KIniFile* pFile);

	static void         SetCurrentChannel(int nIndex);
	static void         SwitchChannel(BOOL bUp = FALSE);  //�л�����һ��Ƶ���������˾��л�����һ����ѭ��
	                                                      //bUp��true�������������½�
	                                                      //e...�±���0������ĸ���
    static int          GetCurChannel();                  //�õ�PlayerBar�ﵱǰ��Ƶ��
private:
	KUiPlayerBar();
	~KUiPlayerBar();
	void	Breathe();									//�����
	void	Initialize();								//��ʼ��
	void	PaintWindow();

	void	UpdateData();								//��������
	void	UpdateXXXNumber(int& nMana, int& nFullMana);							//���³������Щ��ֵ����
	void	UpdateRuntimeAttribute(int& nMoney, int& nLevel);

	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//��Ӧ�������ȡ����¶���
	void	LoadScheme(KIniFile* pIni);//������淽��

	void	OnSend(BOOL bDirectSend);
	void	OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen);				//����Ƶ�����쵽������
	void	PopupChannelMenu(int x, int y);
	void	PopupPhraseMenu(int x, int y, bool bFirstItem);
	void	InputRecentMsg(bool bPrior);
	void	OnSwitchSize();			//�л���С

	int GetChannelIndex(const char* pTitle);
	//void SetCurrentChannel(int nIndex);
	void InputCurrentChannel(int nIndex, bool bFocus);
	//������Channel�е�����, < 0 ʧ��
	//int AddRecentPlayer(const char* szName);
	void ReplaceSpecialName(char* szDest, size_t nDestSize, char* szSrc);
	
	int IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int& nUseMoney, int& nUseLevel, int& nUseMana);

private:
	static KUiPlayerBar* m_pSelf;
private:
	KWndButton		m_Face;		//����
	KWndButton		m_Friend;	//����	
	KWndButton		m_EscDlg;	//ϵͳѡ��

	GameWorld_DateTime		m_DateTime;		//ʱ��

#define	UPB_IMMEDIA_ITEM_COUNT	3
	KImmediaItem	m_ImmediaItem[UPB_IMMEDIA_ITEM_COUNT];
	KWndObjectBox	m_ImmediaSkill[2];
	
	KWndButton	m_SendBtn;	//����
	KWndEdit512	m_InputEdit;	//�����
	KWndPureTextBtn	m_ChannelSwitchBtn;	//�л�Ƶ��
	KWndButton	m_ChannelOpenBtn;	//Ƶ����

	KWndButton	m_SwitchBtn;	//�л���С

	char		m_cPreMsgCounter;
	char		m_cLatestMsgIndex;
#define	MAX_RECENT_MSG_COUNT	8
	char		m_RecentMsg[MAX_RECENT_MSG_COUNT][512];
	int m_nCurChannel;
#define	MAX_RECENTPLAYER_COUNT	10
	char m_RecentPlayerName[MAX_RECENTPLAYER_COUNT][32];
	int m_nRecentPlayerName;

	int				m_bMiniMode;
	
	char			m_szSelfName[32];	//�Լ�������

	int				m_nExperienceFull;
	int				m_nCurLevelExperience;
	int				m_nExperience;
};

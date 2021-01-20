/*****************************************************************************************
//	��������ϵͳ
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-17
------------------------------------------------------------------------------------------
		��������ϵͳ�Խ������洰��Ԫ����ϵΪ������Ϊ������ʵ��Ӧ�ý����ʵ�֡�ÿ�����洰��
    �������iKUiWindow�ӿڡ�����ϵͳ����֧���ô����в�ͬ�����(skin)����ϵͳ�г�Ϊ����(Scheme)
	��������ϵͳ��ʵ�ֳ����ļ�����Ui��Ϊ�ļ���ǰ׺��
	    ����ϵͳ�����ÿ������ң�����id����һ�ݴ����Զ���Ӧ�������ļ���

        ����ϵͳ��ʵ����ΪKUiBase������ϵͳ��Ӧ�ã�
		Init --> ��ʼ������ϵͳ������洰�ڣ�ִ�д˲����󣬽��洰�ڿ���ִ�д��ڲ��������ˡ�
		LoadConfig --> ���봰�ڵĿɶ��Ƶ�������Ϣ���細�ڵ�λ�á�
		LoadScheme --> ���봰��ϵͳ�Ľ��淽������ִ����������󣬽��洰�ڲ�֪����ʲôͼ��
						װ���Լ���

	ע��
		����ȫ�����ڽ��淽����һ��һ�����룬����ÿ���������õ���ͼ���ڵ�һ�α���ʾ��ʱ���
	���룬��Ҫʱ���Խ�һ�������Ż�Ϊ�������ڵĽ��淽���ڵ�һ����ʾ����ʱ�����롣
*****************************************************************************************/
#pragma once

enum UISYS_STATUS
{
	UIS_S_IDLE,
	UIS_S_TRADE_PLAYER,		//����Player������
	UIS_S_TRADE_NPC,		//����npc������
	UIS_S_TRADE_SALE,		//����npc������ ��
	UIS_S_TRADE_BUY,		//����npc������ ��
	UIS_S_TRADE_REPAIR		//����npc������ ����
};

enum UISYS_OPERATION
{
	UIS_O_MOVE_ITEM,
	UIS_O_TRADE_ITEM,
	UIS_O_USE_ITEM,
};

#define APP_START	"Start()"
#define APP_EXIT	"Exit()"
#define APP_STARTGAME	"StartGame(\"%s\")"
#define APP_EXITGAME	"ExitGame()"
#define APP_SAY		"Say(\"%s\", \"%s\")"
#define APP_CHAT	"Chat(\"%s\", \"%s\")"
#define APP_CREATEUNIT	"CreateUnit(\"%s\")"
#define APP_DELETEUNIT	"DeleteUnit(\"%s\")"
#define APP_CREATEUNITGROUP	"CreateUnitGroup(\"%s\", \"%s\")"
#define APP_RENAMEUNITROUP	"RenameUnitGroup(\"%s\", \"%s\", \"%s\")"
#define APP_DELETEUNITROUP	"DeleteUnitGroup(\"%s\", \"%s\")"
#define APP_MOVEUNITROUP	"MoveUnitGroup(\"%s\", \"%s\", \"%s\")"
#define APP_CREATEUNITMEMBER	"CreateUnitMember(\"%s\", \"%s\", \"%s\")"
#define APP_DELETEUNITMEMBER	"DeleteUnitMember(\"%s\", \"%s\")"
#define APP_MOVEUNITMEMBER	"MoveUnitMember(\"%s\", \"%s\", \"%s\")"
#define APP_SETUNITMEMBERSTATUS	"SetUnitMemberStatus(\"%s\", \"%s\", \"%s\")"

class KIniFile;

extern const char*	$Main;

#pragma warning(disable:4786)
#include "map"
#include "string"

class KUiBase
{
public:
	KUiBase();
	~KUiBase();
	int		Init();											//��ʼ��
	void	Exit();											//�˳�

	void	SetUserAccount(char* pszId, char* pszRole);		//�����û��˺�
	int		GetUserPrivateDataFolder(char* pBuffer, int nSize);	//��õ�ǰ���˽�����ݵĴ洢Ŀ¼
	const char*	GetUserTempDataFolder();					//���������ݵ���ʱ����Ŀ¼λ��
	void	CleanTempDataFolder();							//���������ݵ���ʱ����Ŀ¼�ڵ�ȫ���ļ�
	void	CleanPrivateDataFolder();						//�����ǰ���˽�����ݵĴ洢Ŀ¼
	void	DeletePrivateDataFile(const char* pszFileName);	//�����ǰ���˽�����ݵ�ĳ���ļ�

	int		LoadScheme(const char* pSchemeName);			//����ĳ�ַ����������(Skin)
	int		SchemeCount();									//��ý��淽������Ŀ
	int		GetScheme(int nIndex, char* pName, char* pPath);//���ĳ�����淽����������·��
	int		GetCurSchemePath(char* pBuffer, int nSize);		//��õ�ǰ���淽����·��
	
	//�������á��趨�ļ�
	KIniFile*	GetCommSettingFile();						//�򿪱�����湫�����õ��ļ�
	void		CloseCommSettingFile(bool bSave);			//�رձ�����湫�����õ��ļ�
	KIniFile*	GetPrivateSettingFile();					//�򿪴򿪵�ǰ�˺ŵ������ļ�
	void		ClosePrivateSettingFile(bool bSave);		//�رմ򿪵�ǰ�˺ŵ������ļ�
	KIniFile*	GetCommConfigFile();
	void		CloseCommConfigFile();

	//���û�ȡ�������״̬
	UISYS_STATUS GetStatus() { return m_Status; }
	void		 SetStatus(UISYS_STATUS eStatus);
	void		 SetCurrentCursor();						//���ݵ�ǰ״̬�������ָ��ͼ��
	int			 IsOperationEnable(UISYS_OPERATION uOper);

	void		LoadPrivateConfig();						//�������ü�¼���綨�ƵĴ���λ��
	int			SavePrivateConfig();						//�洢���ü�¼

	void		RegisterEvent(const char* szName, HANDLE hWnd);
	void		UnregisterEvent(const char* szName, HANDLE hWnd);
	int			NotifyEvent(const char* szEvent);		//0Ϊ�Ѿ�����, ��Ҫ�ٴ�����, ��0Ϊδ��������
	int			NotifyOneEvent(const char* szName, const char* szEvent);		//0Ϊ�Ѿ�����, ��Ҫ�ٴ�����, ��0Ϊδ��������

private:
	void	LoadSchemeForEachWnd();
	int		GetSchemePath(const char* pScheme);				//��õ�ǰָ�����淽����Ŀ¼��
	void	ClearFolder(const char* pszFolder, bool bDeleteFolder);	//�����ǰ���˽�����ݵĴ洢Ŀ¼

private:
	UISYS_STATUS	m_Status;				//�������״̬

	char		m_CurScheme[32];			//��ǰ���淽��������
	char		m_CurSchemePath[40];		//��ǰ���淽����·��
	char		m_UserAccountId[32];		//��ǰ����˺��ڽ���ϵͳ�е�id

	KIniFile*	m_pUiCommSettingFile;		//���湫�����õ��ļ�
	KIniFile*	m_pUiPrivateSettingFile;	//��ǰ�˺ŵ������ļ�
	KIniFile*	m_pUiCommConfigFile;

	typedef std::map<std::string, HANDLE> EVENTWNDLIST;
	EVENTWNDLIST m_EventWndList;
};

extern KUiBase	g_UiBase;
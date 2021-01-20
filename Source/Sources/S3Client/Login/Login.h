/*****************************************************************************************
//	��Ϸ�ĵ�½���ӹ����߼�ģ��
//	Copyright : Kingsoft 2002-2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-13
------------------------------------------------------------------------------------------
	״̬��
    �ο�KLoginDef.h�ļ�������һЩ��½�����Ϣ��
*****************************************************************************************/
#pragma once
#include "LoginDef.h"
#include "../NetConnect/NetMsgTargetObject.h"

struct TProcessData;
struct tagNewDelRoleResponse;
struct tagNotifyPlayerLogin;

enum LOGIN_LOGIC_STATUS
{
	LL_S_IDLE = 0,					//����
	LL_S_WAIT_INPUT_ACCOUNT,		//�ȴ����˺�����
	LL_S_ACCOUNT_CONFIRMING,		//�ȴ��˺�������֤
	LL_S_WAIT_ROLE_LIST,			//�ȴ����ս�ɫ�б�����
	LL_S_ROLE_LIST_READY,			//��ɫ�б����
	LL_S_CREATING_ROLE,				//�����½���ɫ
	LL_S_DELETING_ROLE,				//����ɾ����ɫ
	LL_S_WAIT_TO_LOGIN_GAMESERVER,	//�ȴ���½��Ϸ������
	LL_S_ENTERING_GAME,				//���ڽ�����Ϸ
	LL_S_IN_GAME,					//��Ϸ����ʱ
};

enum LOGIN_LOGIC_RESULT_INFO
{
	LL_R_NOTHING,					//�޽����Ϣ
	LL_R_CONNECT_FAILED,			//����ʧ��
	LL_R_CONNECT_SERV_BUSY,			//������æ
	LL_R_CONNECT_TIMEOUT,			//���ӳ�ʱδ�����Ӧ
	LL_R_ACCOUNT_PWD_ERROR,			//�˺�/�������
	LL_R_ACCOUNT_FREEZE,			//�˺Ŷ���
	LL_R_ACCOUNT_LOCKED,			//�˺ű�����
	LL_R_INVALID_ROLENAME,			//(�½�)��ɫ�����ֲ��Ϸ�
	LL_R_SERVER_SHUTDOWN,			//��Ϸ����������������ά����
	LL_R_INVALID_PROTOCOLVERSION,	//�汾�ŽϾɣ���Ҫ�������µĿͻ���	
	LL_R_INVALID_PASSWORD,			//��ɾ����ɫʱ���ṩ���������

	LL_R_ACCOUNT_CONFIRM_SUCCESS,	//�˺���֤�ɹ�
	LL_R_CREATE_ROLE_SUCCESS,		//������ɫ�ɹ�
	LL_R_LOGIN_TO_GAMESERVER,		//��ʼ������Ϸ���������������
	LL_R_ACCOUNT_NOT_ENOUGH_POINT,	//�˺ŵ�������

};

#ifndef MAX_PLAYER_PER_ACCOUNT
	#define	MAX_PLAYER_PER_ACCOUNT	3
#endif

//=====================================
//	�½���ɫʱ��Ҫ�Ľ�ɫ�����Ϣ�ṹ
//=====================================
struct KRoleChiefInfo
{
	char				Name[32];		//����
	unsigned	char	Gender;			//�Ա�	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//��������
	union
	{
		unsigned short	NativePlaceId;	//������ID
		short			nLevel;			//�ȼ�
	};
};

struct KLoginServer
{
	char			Title[32];		//����������˵��
	unsigned char	Address[4];		//������ip��ַ
};

class KLogin : public iKNetMsgTargetObject
{
public:
	KLogin();								//���캯��
	~KLogin();								//��������

	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ

	//====��������������һ������״̬ԾǨ====
	//�루�˺ţ���������������
	int	CreateConnection(const unsigned char* pAddress);
	//�����ʺ����룬�˺ŵ�½
	int	AccountLogin(const char* pAccount, const KSG_PASSWORD& crPassword, bool bOrignPassword = true);
	//ѡ����Ϸ��ɫ
	int	SelectRole(int nIndex);
	//�����½���Ϸ��ɫ
	int	CreateRole(KRoleChiefInfo* pCreateInfo);
	//����ɾ����Ϸ��ɫ
	int	DeleteRole(int nIndex, const KSG_PASSWORD &crSupperPassword);
	//֪ͨ�ȴ����ؽ����ʱ��
	void NotifyTimeout();
	//֪ͨҪ��ʼ��Ϸ��
	void NotifyToStartGame();
	//֪ͨ�������ӣ����⣩�Ͽ���
	void NotifyDisconnect();

	//�ص�����״̬
	void ReturnToIdle();
	//ȫ���Զ�����
	void AutoLogin();
	//�ж��Ƿ����ִ��ȫ���Զ�����
	int	IsAutoLoginEnable();
	//���ü�¼���
	void SetRememberAccountFlag(bool bEnable);
	//���ü�¼���
	void SetRememberAllFlag(bool bEnable);

	//====���ݻ�ȡ����====
	//��ȡ��½�߼���ǰ��״̬
	LOGIN_LOGIC_STATUS GetStatus() { return m_Status; }
	//��ȡ�����Ľ����Ϣ
	LOGIN_LOGIC_RESULT_INFO GetResult();
	//��ȡ��ɫ����Ŀ
	int		GetRoleCount(int& nAdviceChoice);
	//��ȡĳ����ɫ����Ϣ
	int		GetRoleInfo(int nIndex, KRoleChiefInfo* pInfo);
	//�����½ѡ��
	void	SaveLoginChoice();
	//��ȡ��ǰ�ĵĵ�½ѡ��
	void	LoadLoginChoice();
	//��ȡ������������б�
	KLoginServer*	GetServerRegionList(int& nCount, int& nAdviceChoice);
	//��½�������б��ȡ
	KLoginServer*	GetServerList(int nRegion, int& nCount, int& nAdviceChoice);
	//��ȡ���飨�ɵģ���½�˺�
	bool	GetLoginAccount(char* pszAccount);
	//ȡ�õ�ǰ�˺ŵ�ʣ��ʱ��
	DWORD	GetAccountLifeTime() { return m_LeftTime; }
	//�жϵ�ǰ��ɫ�Ƿ�Ϊ�½��Ľ�ɫ
	int		IsRoleNewCreated() { return m_Choices.bIsRoleNewCreated; }
    //����ѡ�еķ���������������
    int     SetAccountServer(const KLoginServer &rcSelectServer);

private:
	//====��������������Ӧ�ķ��ش���====
	void	ProcessAccountLoginResponse(KLoginStructHead* pResponse);		//�����˺ŵ�½����Ӧ
	void	ProcessRoleListResponse(TProcessData* pResponse);				//��ɫ�б���
	void	ProcessDeleteRoleResponse(tagNewDelRoleResponse* pResponse);	//ɾ����ɫ�б����Ӧ
	void	ProcessCreateRoleResponse(tagNewDelRoleResponse* pResponse);	//�½���ɫ�б����Ӧ
	void	ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse);//׼����½��Ϸ����������Ӧ

	//������Ϣ����
	int		Request(const char* pszAccount, const KSG_PASSWORD* pcPassword, int nAction);
	//�����˺ŷ�����
	int		ConnectAccountServer(const unsigned char* pIpAddress);

	void	RegistNetAgent();
	void	UnRegistNetAgent();
	void	GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword);
	void	SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword);
	void	ClearAccountPassword(bool bAccount, bool bPassword);

private:
	LOGIN_LOGIC_STATUS			m_Status;
	LOGIN_LOGIC_RESULT_INFO		m_Result;
	
	bool						m_bInAutoProgress;				//�Ƿ��������Զ���½������
	bool						m_bReserved;
	short						m_nNumRole;						//��ɫ����Ŀ
	KRoleChiefInfo				m_RoleList[MAX_PLAYER_PER_ACCOUNT];
	DWORD						m_LeftTime;

	struct	LOGIN_CHOICE
	{
		int						nServerRegionIndex;			//��������������
		KLoginServer			AccountServer;				//��ǰʹ�õķ�����
		char					Account[32];				//��ǰ�˺�
		KSG_PASSWORD    		Password;					//��ǰ�˺ŵ�����
		char					szProcessingRoleName[32];	//��ǰ����Ľ�ɫ������
		bool					bRememberAccount;			//�Ƿ��¼��½�˺�
		bool					bRememberAll;				//�Ƿ��¼ȫ���ĵ�½ѡ��
		bool					bAutoLoginEnable;			//�Ƿ������Զ���½
		bool					bIsRoleNewCreated;			//��ǰ��ɫ�Ƿ�Ϊ�½��Ľ�ɫ
		bool					bLoaded;					//�Ƿ��Ѽ���ѡ���¼
	}							m_Choices;
};

extern	KLogin		g_LoginLogic;

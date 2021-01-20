//------------------------------------------------------------------------------------------
//	����--login����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-13
//	Modifyed by Yangxiaodong later
//------------------------------------------------------------------------------------------
#pragma once
//69280300
#define LOGIN_A_TAG						0
#define LOGIN_R_TAG						0

#define LOGIN_ACCOUNT_MIN_LEN			4
#define LOGIN_ACCOUNT_MAX_LEN			30
#define LOGIN_PASSWORD_MIN_LEN			6
#define LOGIN_PASSWORD_MAX_LEN			30
#define LOGIN_REALNAME_MIN_LEN			4
#define LOGIN_REALNAME_MAX_LEN			30
#define GAMESERVERNAME_MAX_LEN			30

#define def_DBUSERNAME_MAX_LEN			16
#define def_DBUSERNAME_MIN_LEN			6
#define def_DBPASSWORD_MAX_LEN			16
#define def_DBPASSWORD_MIN_LEN			6
//login action return value
//the folloin' value is used in login connect operaion
#define LOGIN_ACTION_FILTER				0xffff0000	//��½�����еĲ������͹�����ֵ
// Parameters from client to server
#define LOGIN_A_CONNECT					0x010000	//����
#define LOGIN_A_NEWACCOUNT				0x020000	//�½��˺�
#define LOGIN_A_SERVERLIST				0x030000	//��ȡ�������б�
#define LOGIN_A_REPORT					0x040000	//֪ͨ����������(���������������)
#define LOGIN_A_LOGIN					0x050000	//��½������(δ��ʼ��Ϸ)
#define LOGIN_A_LOGOUT					0x060000	//ע��
#define LOGIN_A_DEPOSIT					0x070000	//�㿨��ֵ
#define LOGIN_A_DROPACCOUNT				0x080000	//ɾ���˺�
#define LOGIN_A_BEGINGAME				0x090000	//ѡ����Ϸ��������ʼ����Ϸ

// ���ݿ⴫����Ϣ���Ͷ���
//------>BEGIN
#define DBLOGIN_A_LOGIN					0x0a0000	//��¼���ݿ�
#define DBLOGIN_A_LOGOUT				0x0b0000	//�����ݿ�ע��
#define DBLOGIN_A_ADDDBUSER				0x0c0000	//������ݿ����Ա
#define DBLOGIN_A_DELDBUSER				0x0d0000	//ɾ�����ݿ����Ա
#define DBLOGIN_A_QUERYUSERLIST			0x0e0000	//��ȡ���ݿ����Ա�б�
#define DBLOGIN_A_LOCKDATABASE			0x0f0000	//��ס�˺����ݿ�
#define DBLOGIN_A_ACTIVATEDATABASE		0x100000	//�����˺����ݿ�
#define DBLOGIN_A_CREATEACCOUNT			0x110000	//�������˺�
#define DBLOGIN_A_DELETEACCOUNT			0x120000	//ɾ���˺�
//<------END

// Parameters from server to client
#define LOGIN_R_CONNECT					LOGIN_A_CONNECT
#define LOGIN_R_NEWACCOUNT				LOGIN_A_NEWACCOUNT
#define LOGIN_R_SERVERLIST				LOGIN_A_SERVERLIST
#define LOGIN_R_LOGIN					LOGIN_A_LOGIN
#define LOGIN_R_REPORT					LOGIN_A_REPORT
#define LOGIN_R_LOGOUT					LOGIN_A_LOGOUT
#define LOGIN_R_DROPACCOUNT				LOGIN_A_DROPACCOUNT
#define LOGIN_R_BEGINGAME				LOGIN_A_BEGINGAME
// ���ݿ⴫����Ϣ���Ͷ���
//------>BEGIN
#define DBLOGIN_R_LOGIN					DBLOGIN_A_LOGIN
#define DBLOGIN_R_LOGOUT				DBLOGIN_A_LOGOUT
#define DBLOGIN_R_ADDDBUSER				DBLOGIN_A_ADDDBUSER
#define DBLOGIN_R_DELDBUSER				DBLOGIN_A_DELDBUSER
#define DBLOGIN_R_QUERYUSERLIST			DBLOGIN_A_QUERYUSERLIST
#define DBLOGIN_R_LOCKDATABASE			DBLOGIN_A_LOCKDATABASE
#define DBLOGIN_R_ACTIVATEDATABASE		DBLOGIN_A_ACTIVATEDATABASE
#define DBLOGIN_R_CREATEACCOUNT			DBLOGIN_A_CREATEACCOUNT
#define DBLOGIN_R_DELETEACCOUNT			DBLOGIN_A_DELETEACCOUNT
#define DBLOGIN_R_SUCCESS				0xa
#define DBLOGIN_R_FAILED				0xb
#define DBLOGIN_R_HASLOGGEDIN			0xc
#define DBLOGIN_R_HASNOENOUGHPRIORITY	0xd
#define DBLOGIN_R_USERHASEXISTED		0xe
//<------END
#define LOGIN_R_REQUEST					0x0	//when the login request is send from client to server
#define LOGIN_R_SUCCESS					0x1
#define LOGIN_R_FAILED					0x2
#define LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR	0x3
#define LOGIN_R_ACCOUNT_EXIST			0x4
#define	LOGIN_R_TIMEOUT					0x5
#define LOGIN_R_IN_PROGRESS				0x6
#define LOGIN_R_NO_IN_PROGRESS			0x7
#define LOGIN_R_VALID					0x8	//�Ϸ��û�
#define LOGIN_R_INVALID					0x9	//�Ƿ��û�
#define LOGIN_R_NOTREGISTERED			0xa //�û�δע��
#define LOGIN_R_BEDISCONNECTED			0xb //�û��Ѿ�����
#define LOGIN_R_ACCDBISLOCKED			0xc //�˺����ݿⱻ����

struct KLoginStructHead
{
	DWORD Size;		// size of the struct, if the struct is been inherit,the size is ref the the derive struct
	DWORD Param;	// be one of the LOGIN_R_* define value combin with a LOGIN_A_* value.
};
#define KLOGINSTRUCTHEADSIZE			8	// = 2 * sizeof( DWORD )

struct KLoginAccountInfo : KLoginStructHead
{
	char Account[LOGIN_ACCOUNT_MAX_LEN+2];		//account
	char Password[LOGIN_PASSWORD_MAX_LEN+2];	//password
};

struct KLoginGameServer
{
	char Title[GAMESERVERNAME_MAX_LEN+2];		//����������˵��
	DWORD Address;								//������������ַ
	short Port;									//�������˿ں�
	DWORD ID;									//������ID
};
#define KLOGINGAMESERVERSIZE			44	// = GAMESERVERNAME_MAX_LEN + 2 * sizeof( DWORD ) + sizeof( int )

struct KLoginServerList : KLoginStructHead
{
	DWORD Count;				//�˴δ��͵ķ����������Ŀ
	KLoginGameServer* Server;	//��������Ϣ(�䳤����)
};


typedef struct tag_DBLOGINSTRUCT : KLoginStructHead
{
	char szUserName[def_DBUSERNAME_MAX_LEN];
	char szPassword[def_DBPASSWORD_MAX_LEN];
}_DBLOGINSTRUCT, *_PDBLOGINSTRUCT;

/*****************************************************************************************
//	����--login����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-13
------------------------------------------------------------------------------------------
	��½�����У����пͻ��˷����������˵���Ϣ����c2s_login������һ��KLoginStructHead�ṹ��
���ݣ����пͻ��˷����������˵���Ϣ����s2c_login������һ��KLoginStructHead�ṹ�����ݡ�����
һЩ�����˵���������һ����KLoginStructHeadΪ��һ����Ա�ĸ���Ľṹ��

    һ�����������˺ŷ������������ӹ�������Ϣ�������£�
	��������c2s_login & KLoginAccountInfo; KLoginInfo::Param = LOGIN_A_LOGIN | LOGIN_R_REQUEST;
	���������ص�½�����s2c_login & KLoginAccountInfo;
		KLoginInfo::Param = LOGIN_A_LOGIN | (LOGIN_R_SUCCESS or LOGIN_R_FAILED or LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR);
	��½���̽�����

	�����˺Ź��̹�������Ϣ�������£�(ע�⣺ֻ���ڵ��԰汾�ſ����ṩ�˲�����)
	��������c2s_login & KLoginAccountInfo; KLoginInfo::Param = LOGIN_A_NEWACCOUNT | LOGIN_R_REQUEST;
	���������ص���������s2c_login & KLoginAccountInfo;
		KLoginInfo::Param = LOGIN_A_NEWACCOUNT | (LOGIN_R_SUCCESS or LOGIN_R_FAILED or LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR or LOGIN_R_ACCOUNT_EXIST);
	������̽�����

	������½������ʵ�ʵ�ִ�����໥����ܻ���ʱ���ص������Կͻ���Ҫ����KLoginInfo�е�Account
	��Password���������жϷ��������ص���Ϣ�Ƿ�Ϊ������һ�εĵ�½����������������֮������

  һ�������Ĵ��˺ŷ�������ȡ��Ϸ�������б��������Ϣ�������£�
	��������c2s_login & KLoginStructHead; 
		KLoginStructHead::Param = LOGIN_A_SERVERLIST | LOGIN_R_REAUEST;
	�����������˺��б�s2c_login & KLoginServerList; 
		KLoginServerList::Param ��ֵΪ LOGIN_A_SERVERLIST | (LOGIN_R_SUCCESS or LOGIN_R_VALID or LOGIN_R_INVALID or  LOGIN_R_FAILED);
		���б�����ķ���������Ŀ�ܶ�ʱ�����Էֶ�η������ͣ�KLoginServerList::Param ��ֵΪ
		(LOGIN_A_SERVERLIST | LOGIN_R_VALID)ʱ��ʾ�б���δ������ϣ��¸����ݰ�����������δ���б�
		KLoginServerList::Param��ֵΪLOGIN_A_SERVERLIST | LOGIN_R_SUCCESSʱ�򣬱�ʾ�б��ͽ�����
		����ͻ��˵ȴ���ָ����һ��ʱ���û���յ��������б����ݰ��������б��������
	������Ϸ�������б���̽�����

	��������������������������Ϸ�������б�Ĳ���û������½������ǰ�����ƣ������߿ɲ���������

	�������Կ��ǰѴ��͵����ݵĽṹ��Ŀռ䶨���ַ�������Ϊ�䳤������ʵ��ȷ�����ȣ�������С
	��Ҫ���紫�͵����ݵĳ��ȡ������Ĵ洢�˺�������Ľṹ�Ƿ��Ϊ�䳤��������������ܷ�ʽ��
	�Ե���ȷ�������Կ��ǰ����������������˺Ź����еĶ���˺����봫�͸�Ϊֻ�ڷ�������ʱ����
	һ�Σ�ͬʱ��������ı�ʶ��ֵ��һ�������˺���������紫�ʹ���������һ�㰲ȫ�ԣ�����Ҳ��
	��СҪ���紫�����ݵ���������
*****************************************************************************************/
#pragma once
#pragma pack(push, 1)

#include "KProtocol.h"

#define	LOGIN_ACCOUNT_MIN_LEN	6
#define LOGIN_ACCOUNT_MAX_LEN	16
#define LOGIN_PASSWORD_MIN_LEN	8
#define LOGIN_PASSWORD_MAX_LEN	16
#define LOGIN_ROLE_NAME_MIN_LEN	6
#define	LOGIN_ROLE_NAME_MAX_LEN	16

//login action return value
//the folloin' value is used in login connect operaion
#define	LOGIN_ACTION_FILTER					0xff0000	//��½�����еĲ������͹�����ֵ
#define LOGIN_A_CONNECT						0x010000	//����
#define	LOGIN_A_NEWACCOUNT					0x020000	//�½��˺ţ��˺ŷ�������
#define	LOGIN_A_SERVERLIST					0x030000	//��ȡ�������б��˺ŷ�������
#define	LOGIN_A_REPORT						0x040000	//֪ͨ����������(���������������)
#define	LOGIN_A_LOGIN						0x050000	//��½���������˺š���Ϸ��������
#define	LOGIN_A_LOGOUT						0x060000	//ע��
#define	LOGIN_A_CHARACTERLIST				0x070000	//��ȡ��ɫ�б���Ϸ��������


#define	LOGIN_R_REQUEST						0	//when the login request is send form client to server
#define	LOGIN_R_SUCCESS						1
#define	LOGIN_R_FAILED						2
#define	LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR	3
#define	LOGIN_R_ACCOUNT_EXIST				4
#define	LOGIN_R_TIMEOUT						5
#define	LOGIN_R_IN_PROGRESS					6
#define	LOGIN_R_NO_IN_PROGRESS				7
#define	LOGIN_R_VALID						8    //�Ϸ�
#define	LOGIN_R_INVALID						9    //�Ƿ�
// Add by Freeway Chen in 2003.7.1
#define LOGIN_R_INVALID_PROTOCOLVERSION     10   //Э��汾�����ݣ���� KProtocolDef.h's KPROTOCOL_VERSION
#define LOGIN_R_FREEZE						11	//������


struct KLoginStructHead
{
	unsigned short	Size;		//size of the struct, if the struct is been inherit, the size is ref the the derive struct
	int				Param;		//be one of the LOGIN_R_* define value combin with a LOGIN_A_* value.
};

struct KLoginAccountInfo : KLoginStructHead
{
	char	        Account[32];	//account
	KSG_PASSWORD    Password;
	unsigned long   nLeftTime;     //ʣ��ʱ��
    
    #ifdef USE_KPROTOCOL_VERSION
    // Add by Freeway Chen in 2003.7.1
    unsigned    ProtocolVersion;     // Э��汾����� KProtocolDef.h's KPROTOCOL_VERSION
    #endif
};


#pragma pack(pop)
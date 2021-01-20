//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KGmProtocol.h
// Date:	2003.06.02
// Code:	�߳�����
//---------------------------------------------------------------------------

#ifndef KGMPROTOCOL_H
#define KGMPROTOCOL_H

#pragma	pack(push, 1)

#define	defACCOUNT_STRING_LENGTH		32
#define defGAMEWORLD_STRING_LENGTH		32


typedef struct tagGMCmdHeader
{
	BYTE	ProtocolFamily;							// Э�������ļ���
	BYTE	ProtocolType;							// Э������
	char	AccountName[defACCOUNT_STRING_LENGTH];	// �����˺�����
}GM_HEADER;

// GM����Ϸ����ҵĸ��ֲ���
// Э��id��gm_c2s_execute  ���䳤Э�飩
typedef struct tagGMExecuteCommand : tagGMCmdHeader
{
	WORD	wExecuteID;								// ����id��== 1 ִ�нű���>= 2 ����������
	WORD	wLength;								// ��Я���ݳ��ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý����Ŷ�Ӧ����id�Ĳ����������ݣ����ݲ�ͬ�Ĳ���idȷ�����ȼ���Ӧ����
} GM_EXECUTE_COMMAND;

// ����Ϸ������趨�������ⱻ����״̬(ʵ�ʲ���ʱ�����ؾ���Э��ʵ��)
// Э��id��gm_c2s_disable  ���䳤Э�飩
typedef struct tagGMDisableActionCmd : tagGMCmdHeader
{
	BYTE	btProtocol;								// ������Э����
	WORD	wLength;								// ��Я���ݳ��ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý����Ŷ�Ӧ������Э���ŵľ���������ݣ������ɱ�����Э���ž���
} GM_DISABLE_ACTION_COMMAND;

// ����Ϸ�����ȡ���������ⱻ����״̬
// Э��id��gm_c2s_enable  ���䳤Э�飩
typedef struct tagGMEnableActionCmd : tagGMCmdHeader
{
	BYTE	btProtocol;								// ȡ��������Э����
	WORD	wLength;								// ��Я���ݳ��ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý����Ŷ�Ӧȡ��������Э���ŵľ���������ݣ�������ȡ��������Э���ž���
} GM_ENABLE_ACTION_COMMAND;

// ���ض�������ݽ��и���
// Э��id��gm_c2s_tracking  ������Э�飩
typedef struct tagTrackCmd : tagGMCmdHeader
{
	BYTE	ProtocolFamily;							//Э�������ļ���
	BYTE	ProtocolType;							// Э������
	BYTE	btTrackFlag;							// �Ƿ���٣�1���٣�0ȡ������
} GM_TRACK_COMMAND;

// �����ض���ҵĸ������ݸ�GM
// Э��id��gm_s2c_tracking  ���䳤Э�飩
typedef struct tagGMTrackSync : tagGMCmdHeader
{
	WORD	wLength;								// ��Я���ݰ����ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý�����һ�����ݰ���������ݰ�����Ϸ������������ҿͻ��˵ĸ���Э�����
} GM_TRACK_SYNC;

enum enumGMGetRoleCmds
{
	gm_role_entergame_position = 0,	//��ȡ��ҽ�����Ϸ��λ�ã����� GM_GET_ROLE_DATA_SUB_ENTER_POS
};

typedef struct tagGMGetRoleEnterGamePos
{
	int nSubWorldID;
	int	nEnterGameX;
	int nEnterGameY;
}GM_ROLE_DATA_SUB_ENTER_POS;

// �趨��ȡ�ý�ɫ�ĸ�����ֵ����ֻ�ܴ��������ݣ������Ҫ�Ƚϴ������ݣ�ֱ�Ӳ�ѯ��ɫ��������
// �趨��Э��id gm_c2s_setrole  ���䳤Э�飩
typedef struct tagGMSetRoleDataCmd : tagGMCmdHeader
{
	WORD	wSetID;									// ����ID���趨��һ������
	WORD	wLength;								// ��Я���ݳ��ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý����Ŷ�Ӧ����ID�ľ�������������ݣ����ݲ�ͬ�Ĳ���IDȷ�����ȼ���Ӧ����
} GM_SET_ROLE_DATA_COMMAND;

// ȡ�ã�Э��id gm_c2s_getrole  ���䳤Э�飩
typedef struct tagGMGetRoleDataCmd : tagGMCmdHeader
{
	WORD	wGetID;									// ����ID��ȡ����һ������
	WORD	wLength;								// ��Я���ݳ��ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý����Ŷ�Ӧ����ID�ľ�������������ݣ����ݲ�ͬ�Ĳ���IDȷ�����ȼ���Ӧ����
} GM_GET_ROLE_DATA_COMMAND;

// �ظ�����Ϸ����������������ݸ�GMϵͳ
// Э��id��gm_s2c_getrole  ���䳤Э�飩
typedef struct tagGMGetRoleDataSync : tagGMCmdHeader
{
	WORD	wGetID;									// ����ID��ȡ����һ������
	WORD	wLength;								// ��Я���ݳ��ȣ����ݴӽṹ��β����ʼ
	// ����Ӧ�ý����Ŷ�Ӧ����ID�ľ�������������ݣ����ݲ�ͬ�Ĳ���IDȷ�����ȼ���Ӧ����
} GM_GET_ROLE_DATA_SYNC;

// ��ȡ�����������Ϸ������������Э�飩
typedef struct tagGMGetPlayerAtGWCmd : tagGMCmdHeader
{
} GM_GET_PLAYER_AT_GW_CMD;


// �ظ�����Ϸ����������GW������������
typedef struct tagGMGetPlayerAtGWSync : tagGMCmdHeader
{
	char szCurrentRoleName[32];
	int nX;
	int nY;
	int nWorldID;
} GM_GET_PLAYER_AT_GW_SYNC;

// ��ȡ����*���ݿ�*��ȡ����˺Ű��������н�ɫ��(����)
typedef struct tagGMGetRoleListCmd : tagGMCmdHeader
{
} GM_GET_ROLE_LIST_CMD;

// �ظ�����Ϸ����������GW������������(�䳤)
typedef struct tagGMGetRoleListSync : tagGMCmdHeader
{
	WORD wRoleCount;	//����������� RoleBaseInfo ����Ч�ṹ�ĸ���
} GM_GET_ROLE_LIST_SYNC;

// �˺Ž���������Э�飩
typedef struct tagGMUnlockAccount : tagGMCmdHeader
{
}GM_UNLOCK_ACCOUNT;

// �㲥Э�飨�䳤��
typedef struct tagGMBroadCastChatCmd : tagGMCmdHeader
{
	WORD wSentenceLen;	//��߸��㲥���ַ���
}GM_BROADCAST_CHAT_CMD;

typedef struct tagGMExecuteSync : tagGMCmdHeader
{
	WORD wStrLength;	//��������ص��ַ���(char)���ȣ���������0
}GM_EXECUTE_SYNC;

#pragma	pack(pop)
#endif

//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KRelayProtocol.h
// Date:	2003.06.02
// Code:	xiemaopei
//---------------------------------------------------------------------------

#ifndef KPLAYERPROTOCOL_H
#define KPLAYERPROTOCOL_H

#pragma	pack(push, 1)

enum PROTOCOL_FAMILY
{
	pf_normal = 0,
	pf_relay,
	pf_gameworld,
	pf_gamemaster,
	pf_playercommunity,
	pf_chat,	//reserved
	pf_tong,
	pf_friend,

	pf_extend,
};


struct EXTEND_HEADER
{
	BYTE	ProtocolFamily;							//Э�������ļ���
	BYTE	ProtocolID;								//Э������
};

enum ROUTESERVER_METHOD			//����relay֮��ת��֮��
{
	rm_account_id = 1,			//�����ʺ�ӳ��Ϊ��Ӧ��ʶ(tagEnterGame2, tagLeaveGame2ͬ��·������)
	rm_map_id,					//���ݵ�ͼӳ��Ϊ��Ӧ��ʶ(ͨ��������ȡ��·������)
	rm_role_id,					//���ݽ�ɫӳ��Ϊ��Ӧ��ʶ(tagEnterGame2, tagLeaveGame2ͬ��·������)
	rm_gm,						//ֱ�ӷ���GM�Ķ���
};

// Э��id��relay_c2c_data  ���䳤Э�飩
// Э��id��relay_s2c_loseway  ���䳤Э�飩			nToIP ���ܹ㲥
struct RELAY_DATA : EXTEND_HEADER
{
	DWORD	nToIP;									//255.255.255.255�ǹ㲥, 0.0.0.0��Ŀ�ĵ��Ѿ�����
	DWORD	nToRelayID;								//0�����ڹ㲥, -1������㲥
	DWORD	nFromIP;								//0.0.0.0ʱ�����ս���Relay��ϵ��Relay����дnFromIP��nFromRelayID
	DWORD	nFromRelayID;
	WORD	routeDateLength;						//Ҫת�����ݳ��ȣ����ݴӽṹ��β����ʼ
	//��ӦҪת����Э�������
};


// Э��id��relay_c2c_askwaydata  ���䳤Э�飩

struct RELAY_ASKWAY_DATA : EXTEND_HEADER
{
	DWORD	nFromIP;								//0.0.0.0ʱ�����ս���Relay��ϵ��Relay����дnFromIP��nFromRelayID
	DWORD	nFromRelayID;
	DWORD	seekRelayCount;							//���Ҿ�����Relay�ĸ�������-1ʱΪ���ɴ�
	WORD	seekMethod;								//ָ�����ҷ���	
	WORD	wMethodDataLength;						//�����������ݳ��ȣ����ݴӽṹ��β����ʼ
	WORD	routeDateLength;						//Ҫת�����ݳ��ȣ����ݴӷ����������ݽ�β����ʼ
	//��Ӧ������������
	//ת������
};




////////////////////////////////////////////////////////
//pf_playercommunity

//extend protocol package's header
struct tagExtendProtoHeader
{
	BYTE ProtocolType;	//c2s_extend / s2c_extend
	WORD wLength;
	//extend pckg
};


#define __X_NAME_LEN_	32


//c2s_extend ������GameSvrʱ��GameSvr�������Ӵ˽ṹ���Ա�������Դ
struct tagPlusSrcInfo
{
	DWORD nameid;
	unsigned long lnID;
};

//playercomm_c2s_querychannelid
//��ͨChannel������'\'��ͷ������Ϊ��ͨChannel����
//����Channel�������ִ�Сд��
//���飺\Tnnnn������nnnnΪ����ID
//���ɣ�\Fnnnn������nnnnΪ����ID
//ͬ����\S
struct PLAYERCOMM_QUERYCHANNELID : EXTEND_HEADER
{
	char channel[__X_NAME_LEN_];
};

//playercomm_s2c_notifychannelid
struct PLAYERCOMM_NOTIFYCHANNELID : EXTEND_HEADER
{
	char	channel[__X_NAME_LEN_];
	DWORD	channelid;
	BYTE	cost;
};

//playercomm_c2s_freechannelid
struct PLAYERCOMM_FREECHANNELID : EXTEND_HEADER
{
	DWORD channelid;
};

//playercomm_c2s_subscribe
struct PLAYERCOMM_SUBSCRIBE : EXTEND_HEADER
{
	DWORD channelid;
	BOOL subscribe;
};

//playercomm_s2c_gmquerychannelid
struct PLAYERCOMM_GMQUERYCHANNELID : EXTEND_HEADER
{
	char channel[__X_NAME_LEN_];
	BYTE force;
};

//playercomm_s2c_gmfreechannelid
struct PLAYERCOMM_GMFREECHANNELID : EXTEND_HEADER
{
	DWORD channelid;
};

//playercomm_s2c_gmlisten
struct PLAYERCOMM_GMSUBSCRIBE : EXTEND_HEADER
{
	DWORD channelid;
	BOOL subscribe;
};


struct CHAT_MSG_EX : EXTEND_HEADER
{
	char	m_szSourceName[__X_NAME_LEN_];	// ��Դ��ҽ�ɫ����
	char	m_szAccountName[__X_NAME_LEN_];	// ��Դ����˺�����
	WORD SentenceLength;
	//��˵����
};


#pragma pack(pop)

#endif

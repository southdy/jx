#pragma once
#define MAX_BLOCK_SIZE	350
enum	enumGame2DBServerProtol
{
	PROTOL_ONLYSAVEROLE,//��Ϸ�����������ݿ������Ҫ�󱣴�����
		PROTOL_SAVEORCREATEROLE,
		PROTOL_LOADROLE,//��ý�ɫ����
		PROTOL_GETROLELIST,//���ĳ���ʺŵĽ�ɫ�б�
		PROTOL_DELETEROLE,
};

enum   enumDBServer2DBProtol
{
	PROTOL_ROLEINFO,
		PROTOL_ROLELIST,
};

struct  TRoleHeader
{
	unsigned char	nProtoId;
	size_t			nDataLen;
	//unsigned long	ulIdentity;
};

struct 	TRoleNetMsg	:TRoleHeader	//	ÿ�����ݰ��Ľṹ
{
	unsigned char	bDataState;// 1 ��ʾ���µĵ�һ�Σ�0��ʾ����һ�εİ��ļ���
	size_t			nOffset;//When First ,it means TotalLen;Later, it means offset;
	char 			pDataBuffer[1];
};


struct TProcessData: TRoleHeader			//����DB����Ϸ�������Ľṹ
{
	unsigned long   ulNetId;		//����ID
	char			pDataBuffer[1];//ʵ�ʵ�����
};

typedef TProcessData TStreamData ;
typedef TRoleNetMsg KBlock;

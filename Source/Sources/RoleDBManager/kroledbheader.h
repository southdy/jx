#pragma once
#define MAX_BLOCK_SIZE	350
#define TRACEE
#include "KProtocol.h"
#ifdef TRACEE
#include "windows.h"
#endif

extern void ShowTrace(char* pMsg);

#pragma pack(push)
#pragma	pack(1)
struct  TRoleHeader
{
	unsigned char	nProtoId;
	size_t			nDataLen;//TRoleNetMsgʱ��ʾ��Block��ʵ�����ݳ���,TProcessDataʱ��ʾStream��ʵ�����ݳ���
	unsigned long	ulIdentity;
};

struct 	TRoleNetMsg	:TRoleHeader	//	ÿ�����ݰ��Ľṹ
{
	unsigned char	bDataState;// 1 ��ʾ���µĵ�һ�Σ�0��ʾ����һ�εİ��ļ���
	size_t			nOffset;//When First ,it means TotalLen;Later, it means offset;
	char 			pDataBuffer[1];
};

/*
struct TProcessData: TRoleHeader			//����DB����Ϸ�������Ľṹ
{
	unsigned long   ulNetId;		//����ID
	char			pDataBuffer[1];//ʵ�ʵ�����
};
*/

struct TCmd 
{
	unsigned long  ulNetId;
	TProcessData ProcessData;
};

typedef TProcessData TStreamData ;
typedef TRoleNetMsg KBlock;

#pragma pack(pop)
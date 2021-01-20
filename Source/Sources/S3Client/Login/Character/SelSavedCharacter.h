/*****************************************************************************************
//	�浵��ɫѡ��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-12

//	�������ƴ�ģ�������600-800�У����蹤ʱ8-12Сʱ������
------------------------------------------------------------------------------------------
	��ģ�����ڻ�ȡ����Ѿ�ӵ�еĽ�ɫ����Ϣ�������Խ������/ɾ��/�����ɫ�Ȳ�����

	ӦΪ��ģ�齫���ӷ�����ִ�в��������Ե��÷���LoadData��NewCharacter��DeleteCharacter��
SelCharacter���������صĶ���(������ʾ�Ĳ���ֵ)��ʾ���Ͳ��������Ƿ�ɹ�(0ֵ��ʾʧ�ܣ���0ֵ
��ʾ�ɹ�)�������ǲ�����ʵ�ʽ����ͨ��GetLastActionResult��������֪�����һ�β�����ִ�н����

	���ڴ�ģ���״̬��
	1 ģ���ʼ��ʱ����'����'״̬(SSC_S_IDLE)��
	2 ����LoadData������������浵��ɫ���ݣ������ɫ����ʱ����'�����ɫ������'״̬(SSC_S_LOADING_DATA)��
	����ɹ����л�Ϊ'��ɫ�Ѿ�����'״̬(SSC_S_STANDBY)�����ʧ���򷵻�'����'״̬(SSC_S_IDLE)��
	3 ����SSC_S_STANDBY״̬��ʱ�����ִ��NewCharacter��DeleteCharacter��SelCharacter������
	4 �ɹ�ִ��NewCharacter��ת��'�����½���ɫ'״̬(SSC_S_CREATING_CHARACTER)������������
	���۳ɹ����ת��SSC_S_STANDBY״̬��������ִ�н��ͨ��GetLastActionResult������á�
	5 �ɹ�ִ��DeleteCharacter��ת��'����ɾ����ɫ'״̬(SSC_S_DELETING_CHARACTER)������������
	���۳ɹ����ת��SSC_S_STANDBY״̬��������ִ�н��ͨ��GetLastActionResult������á�
	6 �ɹ�ִ��SelCharacter��ת��'�ѽ�ɫ������Ϸ��'״̬(SSC_S_LOADING_CHARACTER)������������
	�ɹ���ת��'��ɫ�Ѿ��ɹ�������Ϸ'״̬(SSC_S_LOAD_CHARACTER_LOADED)��ʧ����ת��SSC_S_STANDBY
	״̬��
*****************************************************************************************/
#pragma once

#include "../../NetConnect/NetConnectAgent.h"

#ifndef MAX_PLAYER_PER_ACCOUNT
	#define	MAX_PLAYER_PER_ACCOUNT	3
#endif

//=====================================
//	KSelSavedCharacter��״̬����
//=====================================
enum SSC_STATUS
{
	SSC_S_IDLE	= 0,				//����״̬
	SSC_S_LOADING_DATA,				//�����ɫ������
	SSC_S_STANDBY,					//��ɫ�Ѿ�����
	SSC_S_LOADING_CHARACTER,		//�ѽ�ɫ������Ϸ��
	SSC_S_LOAD_CHARACTER_LOADED,	//��ɫ�Ѿ��ɹ�������Ϸ
	SSC_S_CREATING_CHARACTER,		//�����½���ɫ
	SSC_S_DELETING_CHARACTER,		//����ɾ����ɫ
};

//=====================================
//	KSelSavedCharacter�Ĳ������ؽ��
//=====================================
enum SSC_RESULT
{
	SSC_R_IN_PROGRESS = 0,	//����ִ����
	SSC_R_NOTHING,			//������ϣ�û�б仯
	SSC_R_UPDATE,			//����
	SSC_R_INVALID_ROLENAME,	//�½��Ľ�ɫ�����ֲ��Ϸ����Դ���
	SSC_R_CREATE_ROLE_SUCCEED,//������ɫ�ɹ�
	SSC_R_START_GAME,		//��ʼ��Ϸ
	SSC_R_SVR_DOWN,			//�Ҳ�����Ч�ķ�����
	SSC_R_FAILED,			//ʧ��
};

//=====================================
//	�½���ɫʱ��Ҫ�Ľ�ɫ�����Ϣ�ṹ
//=====================================
struct KNewCharacterInfo
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

class KSelSavedCharacter
{
public:
	KSelSavedCharacter();				//���캯��
	~KSelSavedCharacter();				//��������
	void	AcceptNetMsg(void* pMsgData);//����������Ϣ
	int		LoadData();					//��ʼ�����ɫ����
	int		GetStatus();				//����ֵΪ÷��SSC_STATUS��ȡֵ֮һ������ֵ�����뿴��ص�ֵ����
	void	SetLastActionResult(int nResult);		//�������һ�β����ķ��ؽ��
	int		GetLastActionResult();		//�õ����һ�β����ķ��ؽ��
	int		GetCharacterNum();			//��ȡ��ɫ����Ŀ
	int		GetCharacterInfo(int nIndex, KNewCharacterInfo* pInfo);	//��ȡĳ����ɫ����Ϣ
	int		NewCharacter(KNewCharacterInfo* pData);					//�����½�һ����ɫ
	int		DeleteCharacter(int nIndex, const char* pszPassword);	//����ɾ��һ����ɫ
	int		SelCharacter(int nIndex);								//ѡ��ĳ����ɫ
	void	FreeData();												//�ͷŽ�ɫ����
	void	SetCharacterBaseInfo(int nNum, const RoleBaseInfo *pInfo);
	void	SetAccountName(const char* pAccount);
private:
	SSC_STATUS			m_Status;
	RoleBaseInfo		m_BaseInfo[MAX_PLAYER_PER_ACCOUNT];
	int					m_nNumCharacter;	//��ɫ����Ŀ
	unsigned int		m_nRequestTime;		//���������ʱ��
	int					m_nLastOperResult;	//�ϴεĲ������ؽ��
	char				m_AccountName[32];
	char				m_szProcessingRoleName[32];
};

// GameStatistic.h: interface for the GameStatistic class.
//��Ϸͳ����
//By Fellow
//////////////////////////////////////////////////////////////////////

#if !defined(GAMESTATISTIC_H)
#define GAMESTATISTIC_H

#include "DBTable.h"
#include "S3DBInterface.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TGameStatistic  
{
public://ͳ���б�����ݽṹ
	struct TRoleList
	{
		char Name[32];
		//char Account[32];
		int Level;
		int Money;
		int Sect;			//����
	};
	struct TStatData
	{
		TRoleList MoneyStat[100];			//��Ǯ��������б�һ�ٸ���ң�
		TRoleList LevelStat[100];			//������������б�һ�ٸ���ң�
		TRoleList MoneyStatBySect[11][10];	//�����ɽ�Ǯ��������б�[����][�����]
		TRoleList LevelStatBySect[11][10];	//�����ɼ�����������б�[����][�����]
		int SectPlayerNum[11];				//�������ɵ������
		int SectMoneyMost[11];				//�Ƹ�����ǰһ������и�������ռ������
		int SectLevelMost[11];				//��������ǰһ������и�������ռ������
		
	};
private:
	ZDBTable* DBTable;			//���ݿ�
	
	enum StatType{stMoney, stLevel};//��Ҫ�жϵ����stMoney, stLevel��
	
	TRoleList* GetMin(			//���ҳ��б���Ǯ/���𣨻����������ٵ�һ��Ԫ��
		TRoleList* const aRoleList,	//�б�
		const int n,				//�б��С
		const StatType aType);		//��Ҫ�жϵ����stMoney, stLevel��

	void ListSort(			//���б����ض�������
		TRoleList* const aRoleList,	
		const int n,
		const StatType aType);
	void RoleDataCopy(TRoleList* Desc, TRoleData* Source);//��RoleData���õĽṹ���Ƶ�RoleList�ṹ��
public:
	TGameStatistic(){DBTable = NULL;};
	TGameStatistic(ZDBTable * aTable){DBTable = aTable;};
	virtual ~TGameStatistic(){};
	void SetDBTable(ZDBTable * aTable){DBTable = aTable;};
//����
	bool AllStat(TStatData& aStatData);//ȫ��ͳ��
};

#endif // !defined(GAMESTATISTIC_H)

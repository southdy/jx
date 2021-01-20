// GameStatistic.cpp: implementation of the GameStatistic class.
//��Ϸͳ����
//By Fellow
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameStatistic.h"

using namespace std;

TGameStatistic::TRoleList* TGameStatistic::GetMin(
					TRoleList* const aRoleList,
					const int n,
					const StatType aType)
{//���ҳ��б���Ǯ/���𣨻����������ٵ�һ��Ԫ��
	TRoleList* tmpData;
	tmpData = &aRoleList[0];
	while(true)
	{
		for(int i=0;i<n;++i)
		{
			int aSourse, aDest;
			switch(aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[i].Money;
				break;
			case stLevel:
				aDest = tmpData->Level;
				aSourse = aRoleList[i].Level;
				break;
			}
			if(aDest > aSourse)
			{
				tmpData = &aRoleList[i];
				break;
			}
		}
		if(i==n)break;
	}
	return tmpData;
}


void TGameStatistic::ListSort(
					TRoleList* const aRoleList,
					const int n,
					const StatType aType)
{//���б����ض�������
	int i,j;
	for(i=0;i<n;++i)
	{
		TRoleList* tmpData = &aRoleList[i];
		for(j=i+1;j<n;++j)
		{
			int aSourse, aDest;
			switch(aType)
			{
			case stMoney:
				aDest = tmpData->Money;
				aSourse = aRoleList[j].Money;
				break;
			case stLevel:
				aDest = tmpData->Level;
				aSourse = aRoleList[j].Level;
				break;
			}
			if( aSourse > aDest )
					swap(*tmpData, aRoleList[j]);
		}
	}
}

void TGameStatistic::RoleDataCopy(TRoleList* Desc, TRoleData* Source)
{//��RoleData���õĽṹ���Ƶ�RoleList�ṹ��
	//strcpy(Desc->Account, Source->BaseInfo.caccname);
	strcpy(Desc->Name, Source->BaseInfo.szName);
	Desc->Sect = Source->BaseInfo.nSect;
	Desc->Money = Source->BaseInfo.imoney + Source->BaseInfo.isavemoney;
	Desc->Level = Source->BaseInfo.ifightlevel;
}

bool TGameStatistic::AllStat(TStatData& aStatData)
{//ȫ��ͳ��
	if( !DBTable ) return false;
	int i;
	
	memset(&aStatData, 0, sizeof(TStatData));
	char* aBuffer;
	int size;
	TRoleData* pRoleData;

	///////���ݿ����/////////////
	aBuffer = DBTable->GetRecord(size, ZDBTable::cpFirst, 0);
	while(aBuffer)
	{
		TRoleList* tmpData;
		pRoleData = (TRoleData*)aBuffer;
		
		//////////////////////////////���ͳ��////////////////////////////////////
		//�Խ�Ǯ����
		tmpData = GetMin(aStatData.MoneyStat, 100, stMoney);
		if( tmpData->Money < pRoleData->BaseInfo.imoney + pRoleData->BaseInfo.isavemoney)
		{//�����ǰ���ݽϴ�Ͱѵ�ǰ���ݴ����б�����С��
			RoleDataCopy(tmpData, pRoleData);
		}

		//�Լ�������
		tmpData = GetMin(aStatData.LevelStat, 100, stLevel);
		if( tmpData->Level < pRoleData->BaseInfo.ifightlevel)
		{//�����ǰ���ݽϴ�Ͱѵ�ǰ���ݴ����б�����С��
			RoleDataCopy(tmpData, pRoleData);
		}
		
		//�����ɶԽ�Ǯ����
		if( (pRoleData->BaseInfo.nSect <=10) && (pRoleData->BaseInfo.nSect >= 1) )
		{
			tmpData = GetMin(aStatData.MoneyStatBySect[pRoleData->BaseInfo.nSect], 10, stMoney);
		}
		else
		{
			tmpData = GetMin(aStatData.MoneyStatBySect[0], 10, stMoney);
		}
		if( tmpData->Money < pRoleData->BaseInfo.imoney  + pRoleData->BaseInfo.isavemoney)
		{//�����ǰ���ݽϴ�Ͱѵ�ǰ���ݴ����б�����С��
			RoleDataCopy(tmpData, pRoleData);
		}

		//�����ɶԼ�������
		if( (pRoleData->BaseInfo.nSect <=10) && (pRoleData->BaseInfo.nSect >= 1) )
		{
			tmpData = GetMin(aStatData.LevelStatBySect[pRoleData->BaseInfo.nSect], 10, stMoney);
		}
		else
		{
			tmpData = GetMin(aStatData.LevelStatBySect[0], 10, stMoney);
		}
		if( tmpData->Level < pRoleData->BaseInfo.ifightlevel)
		{//�����ǰ���ݽϴ�Ͱѵ�ǰ���ݴ����б�����С��
			RoleDataCopy(tmpData, pRoleData);
		}
		

		//////////////////////////////����ͳ��////////////////////////////////////
		//�������ɵ������ͳ��
		if( (pRoleData->BaseInfo.nSect <=10) && (pRoleData->BaseInfo.nSect >= 1) )
		{
			++aStatData.SectPlayerNum[pRoleData->BaseInfo.nSect];
		}
		else
		{
			++aStatData.SectPlayerNum[0];
		}
				
		//�����ǰ���棬�α�ָ����һ����¼
		delete [] aBuffer;
		aBuffer = DBTable->GetRecord(size,ZDBTable::cpNext);
	}

	//////////////////////////////����ͳ��////////////////////////////////////
	for(i=0;i<100;++i)
	{	//�Ƹ�����ǰһ������и�������ռ������
		if( (aStatData.MoneyStat[i].Sect <=10) && (aStatData.MoneyStat[i].Sect >= 1) )
		{
			++aStatData.SectMoneyMost[aStatData.MoneyStat[i].Sect];
		}
		else
		{
			++aStatData.SectMoneyMost[0];
		}
		
		//��������ǰһ������и�������ռ������
		if( (aStatData.LevelStat[i].Sect <=10) && (aStatData.LevelStat[i].Sect >= 1) )
		{
			++aStatData.SectLevelMost[aStatData.LevelStat[i].Sect];
		}
		else
		{
			++aStatData.SectLevelMost[0];
		}
	}

	//////////////////////////////����////////////////////////////////////
	ListSort(aStatData.MoneyStat, 100, stMoney);
	ListSort(aStatData.LevelStat, 100, stLevel);
	for(i=0;i<11;++i)
	{//�����ɽ�Ǯ����
		ListSort(aStatData.MoneyStatBySect[i], 10, stMoney);
	}
	for(i=0;i<11;++i)
	{//�����ɼ�������
		ListSort(aStatData.LevelStatBySect[i], 10, stLevel);
	}
	
	return true;
}

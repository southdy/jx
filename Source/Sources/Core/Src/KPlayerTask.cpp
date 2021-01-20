//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTask.cpp
// Date:	2002.10.05
// Code:	�߳�����
// Desc:	PlayerTask Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KPlayerTask.h"

#ifdef _SERVER
int			g_TaskGlobalValue[TASKGLOBALVALUENUM]; //ȫ�ֵı��������ڷ������ű�ϵͳ

KTabFile	g_WayPointTabFile;
KTabFile	g_StationTabFile;
KTabFile	g_DockTabFile;
KTabFile	g_StationPriceTabFile;
KTabFile	g_WayPointPriceTabFile;
KTabFile	g_DockPriceTabFile;

int	*		g_pStationPriceTab = NULL;
int	*		g_pWayPointPriceTab = NULL;
int	*		g_pDockPriceTab = NULL;

#endif
//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KPlayerTask::KPlayerTask()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ����
//---------------------------------------------------------------------------
void	KPlayerTask::Release()
{
	memset(nSave, 0, sizeof(nSave));
	memset(nClear, 0, sizeof(nClear));
}

//---------------------------------------------------------------------------
//	���ܣ������ʱ���̿��Ʊ���
//---------------------------------------------------------------------------
void	KPlayerTask::ClearTempVar()
{
	memset(nClear, 0, sizeof(nClear));
}

//---------------------------------------------------------------------------
//	���ܣ��õ�����������
//---------------------------------------------------------------------------
int		KPlayerTask::GetSaveVal(int nNo)
{
	if (nNo < 0 || nNo >= MAX_TASK)
		return 0;
	return nSave[nNo];
}

//---------------------------------------------------------------------------
//	���ܣ��趨����������
//---------------------------------------------------------------------------
void	KPlayerTask::SetSaveVal(int nNo, BOOL bFlag)
{
	if (nNo < 0 || nNo >= MAX_TASK)
		return;
	nSave[nNo] = bFlag;
}

//---------------------------------------------------------------------------
//	���ܣ��õ���ʱ���̿��Ʊ���ֵ
//---------------------------------------------------------------------------
int		KPlayerTask::GetClearVal(int nNo)
{
	if (nNo < 0 || nNo >= MAX_TEMP_TASK)
		return 0;
	return nClear[nNo];
}

//---------------------------------------------------------------------------
//	���ܣ��趨��ʱ���̿��Ʊ���ֵ
//---------------------------------------------------------------------------
void	KPlayerTask::SetClearVal(int nNo, int nVal)
{
	if (nNo < 0 || nNo >= MAX_TEMP_TASK)
		return;
	nClear[nNo] = nVal;
}

//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTask.h
// Date:	2002.10.05
// Code:	�߳�����
// Desc:	PlayerTask Class
//---------------------------------------------------------------------------

#ifndef KPLAYERTASK_H
#define KPLAYERTASK_H

#define		MAX_TASK		256
#define		MAX_TEMP_TASK	2560



#ifdef _SERVER
#define TASKGLOBALVALUENUM 5000
extern int		g_TaskGlobalValue[TASKGLOBALVALUENUM];

extern KTabFile g_WayPointTabFile;
extern KTabFile g_StationTabFile;
extern KTabFile g_DockTabFile;

extern KTabFile g_StationPriceTabFile;
extern KTabFile g_WayPointPriceTabFile;
extern KTabFile g_DockPriceTabFile;

extern int	*g_pStationPriceTab;
extern int  *g_pWayPointPriceTab;
extern int  *g_pDockPriceTab;

#endif


class KPlayerTask
{
public:
	int			nSave[MAX_TASK];					// ���ڼ�¼�����Ƿ���ɣ��뱣�浽���ݿ�
	int			nClear[MAX_TEMP_TASK];				// ���ڼ�¼��������е��м䲽����������������浽���ݿ⣬������ߺ���������
public:
	KPlayerTask();									// ���캯��
	void		Release();							// ���
	void		ClearTempVar();						// �����ʱ���̿��Ʊ���
	int			GetSaveVal(int nNo);				// �õ�����������
	void		SetSaveVal(int nNo, BOOL bFlag);	// �趨����������
	int			GetClearVal(int nNo);				// �õ���ʱ���̿��Ʊ���ֵ
	void		SetClearVal(int nNo, int nVal);		// �趨��ʱ���̿��Ʊ���ֵ
};

#endif

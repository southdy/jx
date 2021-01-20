#ifndef KDATAMANAGER_H
#define KDATAMANAGER_H

#include "SynDataSets.h"
#include "Kengine.h"
#include "KThread.h"
#include "KNode.h"
#include "KList.h"

class TSynData:public KNode
{
public:
	long Id;
	long index;
};

#define MAXNUM 100

//�ͻ��� ���ã�������������ȡֵ����ֵ
class KDataManage  
{
public:
	KDataManage();
	virtual ~KDataManage();
	SynDataSets_ptr m_pCorbaDataSets;
	KList m_LockList;
	KThread *m_pLockThread;
	
	
	
	
	long Lock(CORBA::Long MapId, CORBA::Long StyleId, long id);
	long UnLock(CORBA::Long MapId, CORBA::Long StyleId, long id, long index);
	
	
	CORBA::Any* GetData(CORBA::Long MapId, CORBA::Long StyleId, long id);
	long SetData(CORBA::Long MapId, CORBA::Long StyleId,long id, const CORBA::Any& data) ;
	//������ݣ�����ס����
	CORBA::Any* GetDataWithLock(CORBA::Long MapId, CORBA::Long StyleId,long id) ;
	CORBA::Long SetDataWithLock(CORBA::Long MapId, CORBA::Long StyleId,CORBA::Long id, const CORBA::Any& data) ;
	CORBA::Any* GetDataWithOnce(CORBA::Long MapId, CORBA::Long StyleId,CORBA::Long id);
	CORBA::Long SetDataWithOnce(CORBA::Long MapId, CORBA::Long StyleId,CORBA::Long id, const CORBA::Any& data) ;
	
	char m_ServerObjName[30];
	
	void GetReqInfo(){};
	
};

#endif // KDATAMANAGER_H

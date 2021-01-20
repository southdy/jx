// KSynDataSet.h: interface for the KSynDataSet class.
//
//////////////////////////////////////////////////////////////////////
#ifndef KSYNDATASET_H
#define KSYNDATASET_H

#include "SynDataSet.h"
#include "KSynDataSet.h"
#include "KSynData.h"
#include "KSynLock.h"
#include "KNode.h"
#include "Klist.h"
class DataSeq;

#include "ReqServer.h"
#define MAX_UNITNUM 4000
class LockInfoNode:public KNode
{
public:
	long id;
	long index;
	ReqServer_ptr  pServer;
};

struct TServerNode:public KNode
{
public:
	ReqServer_ptr  pServer;
	long ServerId;
};

class KSynDataSet 
{
public:
	
	KSynDataSet();
	virtual ~KSynDataSet();
	
	
	CORBA::Long Lock(CORBA::Long id, const char* ObjName) ;
	CORBA::Long UnLock(CORBA::Long id, CORBA::Long index) ;
	CORBA::Any* GetData(CORBA::Long id) ;
	KSynData* GetDataPtr(long id){ return m_Sets[id]; };
	CORBA::Any* GetDataMember(CORBA::Long id,CORBA::Long MemberId);
	CORBA::Long SetData(CORBA::Long id, const CORBA::Any& data) ;
	CORBA::Long SetDataMember(CORBA::Long id, CORBA::Long MemberId, const CORBA::Any& data) ;
	CORBA::Any* GetDataWithLock(CORBA::Long id, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Any* GetDataMemberWithLock(CORBA::Long id, CORBA::Long MemberId, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Long SetDataWithLock(CORBA::Long id, const CORBA::Any& data, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Long SetDataMemberWithLock(CORBA::Long id, CORBA::Long MemberId, const CORBA::Any& data, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Any* GetDataWithOnce(CORBA::Long id, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Any* GetDataMemberWithOnce(CORBA::Long id, CORBA::Long MemberId, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Long SetDataWithOnce(CORBA::Long id, const CORBA::Any& data, CORBA::Long& Result, const char* ObjName) ;
	CORBA::Long SetDataMemberWithOnce(CORBA::Long id, CORBA::Long MemberId, const CORBA::Any& data, CORBA::Long& Result, const char* ObjName) ;
	
	DataSeq* SaveData();
	
	//���KSynData���ݵ�ָ������
	KSynData *  m_Sets[MAX_UNITNUM];
	
	//���KSynLock���������
	KSynLock m_LockSets[MAX_UNITNUM];
	
	//�����������б���
	BOOL AddData(KSynData * pData, long id);
//	BOOL DelData(id);
	long m_nTopIndex;
	KList m_ReqLockList;//���������б�
	KList m_ServerList;//�������б�
	
};

#endif //KSYNDATASET_H

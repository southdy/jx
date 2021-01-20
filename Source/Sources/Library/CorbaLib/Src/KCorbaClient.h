#ifndef KCorbaClient_H
#define KCorbaClient_H

#include <omniORB3/CORBA.h>
#include "KDataManage.h"
#include "KCorbaServer.h"
//��Ҫ��ɵģ�����ָ��Orb��
//ָ��m_DataManage��Զ�̻���̶���ĵ�ַ

class KCorbaClient
{
public:
	CORBA::ORB_var   m_OrbVar;
	CORBA::Object_var m_ObjVar;
	
	char  m_ServerName[20];
	char  m_ServerType[20];
	
	KCorbaClient(){m_ObjVar = NULL; m_OrbVar = NULL;};
	~KCorbaClient(){};

	void SetORB(CORBA::ORB_var Orb){m_OrbVar = Orb;};

	BOOL CreateClient(char *ServerName, char *ServerType);
	BOOL CreateClient(CORBA::String_var sior);
	BOOL CreateOrb(int argc , char **argv){m_OrbVar = CORBA::ORB_init(argc, argv, "omniORB3");};
	
};
#endif // KCorbaClient_H
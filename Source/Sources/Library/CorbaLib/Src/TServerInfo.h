#ifndef ServerInfo_H
#define ServerInfo_H
#include "SynDataSet.h"
struct ServerInfo
{
SynDataSet_var ServerRef;
CORBA::Object_var Obj;
char ServerName[20];
char ServerType[20];
char ServerObjName[40];
int From;
int To;

long ActionNum;//ÿ�η����ڷ���˵Ĵ���
long LoopNum;//�ܹ���ɶ��ٴΡ�

//CORBA::ORB::RequestSeq ReqSeq;
};

#endif
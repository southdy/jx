// GameServer.cpp : Defines the entry point for the console application.
//

/*#ifdef _STANDALONE
#ifndef __linux
//#include <winsock2.h>
#endif
#endif*/
#include "StdAfx.h"
#include "KSOServer.h"

extern KSwordOnLineSever g_SOServer;
/*#ifdef __linux
#include <unistd.h>
#include <signal.h> 
#include <sys/param.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
void init_daemon(void) { 
	int pid; 
	int i; 
	if(pid=fork()) exit(0);			//�Ǹ����̣����������� 
	else if(pid< 0) exit(1);		//forkʧ�ܣ��˳� 
//�ǵ�һ�ӽ��̣���̨����ִ�� 
	setsid();//��һ�ӽ��̳�Ϊ�µĻỰ�鳤�ͽ����鳤 
//��������ն˷��� 
	if(pid = fork()) exit(0);//�ǵ�һ�ӽ��̣�������һ�ӽ��� 
	else if(pid< 0) 
	exit(1);//forkʧ�ܣ��˳� 
//�ǵڶ��ӽ��̣����� 
//�ڶ��ӽ��̲����ǻỰ�鳤 

	for(i=0; i< NOFILE; ++i) close(i); 
	umask(0);//�����ļ�������ģ 
	return; 
} 

#endif*/
int g_nPort = 0;
int main(int argc, char* argv[])
{
	BOOL bRunning = TRUE;
	if (argc == 2)
	{
		g_nPort = atoi(argv[1]);
	}
/*#ifdef __linux
	init_daemon();
#endif*/

	if (!g_SOServer.Init())
		return 0;

	while(bRunning)
	{
		bRunning = g_SOServer.Breathe();
	}

	g_SOServer.Release();
	return 1;
}

#ifdef _STANDALONE
extern "C"
{
void lua_outerrmsg(const char * szerrmsg)
{
	fprintf(stderr, szerrmsg);
}
};
#endif
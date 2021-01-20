#include "stdafx.h"
#include "KDBProcessThread.h"
#include "kengine.h"
//ѭ�����������ݿ������������Ϣ�Ĳ��� 

DWORD WINAPI DatabaseLoadThreadFun(void * pParam)
{
	list<TGetRoleInfoFromDB *>::iterator I;
	TGetRoleInfoFromDB GetRoleInfoCmd;
	
	while(1)
	{
		try
		{
			g_LoadDBThreadMark ++;
			if (g_LoadDBThreadMark > MAX_MARK_COUNT) g_LoadDBThreadMark = 0;
			bool	bFind = false;
			
			{
				CCriticalSection::Owner locker( g_GetRoleInfoMutex );
				I = g_DBLoadThreadCmdList.begin();
				
				if (I != g_DBLoadThreadCmdList.end())
				{
					GetRoleInfoCmd.nNetId = (*I)->nNetId;
					strcpy(GetRoleInfoCmd.szRoleName, (*I)->szRoleName);
					GetRoleInfoCmd.ulIdentity = (*I)->ulIdentity;
					delete *I;
					g_DBLoadThreadCmdList.pop_front();
					bFind = true;
				}
			}
			
			if (bFind)
			{
				static unsigned char szRoleInfo[MAX_ROLEINFORMATION_SIZE];
				int nBufferSize = MAX_ROLEINFORMATION_SIZE;
				
				sprintf(g_LoadMarkString, "�������ݿ��ȡ��ɫ%s......", GetRoleInfoCmd.szRoleName);
				g_LoadDBThreadMark = MAX_MARK_COUNT + LOADMARK_NO; 
				
				if (!S3DBI_GetRoleInfo(szRoleInfo, GetRoleInfoCmd.szRoleName, nBufferSize))	continue;
				
				if (nBufferSize > MAX_ROLEINFORMATION_SIZE)
				{
					ASSERT(0);
					g_DebugLog("��õĽ�ɫ���ݳ�����");
					g_nMistakeCount ++;
				}
				
				//���ӽ�ɫ���ݺ��������б���������
				
				const char * szName = (const char *)(GetRoleInfoCmd.szRoleName);
				if (szName)
				{
					{
						CCriticalSection::Owner locker(g_RoleInfoSetMutex);	
						KRoleInfomation * pRoleInfomation =	g_RoleInfoMap[szName] ;
						
						if (!pRoleInfomation)//�¼ӽ�ɫ��
						{
							KRoleInfomation * pNewRoleInfomation = new KRoleInfomation;
							pNewRoleInfomation->SetRoleInfoBuffer(szRoleInfo, nBufferSize);
							g_RoleInfoMap[szName] = pNewRoleInfomation;
							g_RoleInfoList.push_back(pNewRoleInfomation);
						}
					}
					
					TCmd * pNewCmd		= (TCmd *)new char [sizeof(TCmd)  + strlen(szName) - 1];
					pNewCmd->ulNetId	= GetRoleInfoCmd.nNetId;
					pNewCmd->ProcessData.nProtoId	= c2s_roleserver_getroleinfo;
					pNewCmd->ProcessData.nDataLen	= strlen(szName);
					pNewCmd->ProcessData.ulIdentity = GetRoleInfoCmd.ulIdentity;
					memcpy(&pNewCmd->ProcessData.pDataBuffer[0], szName, strlen(szName));
					
					CCriticalSection::Owner locker(g_MainMsgListMutex);
					g_MainThreadCmdList.push_back(pNewCmd);
				}
				
			}
			Sleep(1);
		}
		catch(...)
		{
			g_nMistakeCount ++;
			g_DebugLog("DBLoad Throw Error!");
		}
		
	}
	return 1;
}

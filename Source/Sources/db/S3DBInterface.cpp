// S3DBInterface.cpp : Defines the entry point for the DLL application.
//

#include "S3DBInterface.h"
#include "DBTable.h"


//��Ҫ����һЩ����������
//�ڴ˹�������͸��ƵĻ������̵߳Ĺ۵㣬��Ҫһ���������߳������������?
//��־���ݿ���Ҫ��ʱ��������
ZDBLogTable *db_table;
int get_account(DB *db, const DBT *pkey, const DBT *pdata, DBT *ikey) {			//����һ��������buffer���õ�account��Ϊ����
	memset(ikey, 0, sizeof(DBT));
	TRoleData *pRoleData = (TRoleData *)pdata->data;
	ikey->data = pRoleData->BaseInfo.caccname;
	ikey->size = strlen(pRoleData->BaseInfo.caccname) + 1;
	return 0;
}

S3DBI_API BOOL S3DBI_InitDBInterface()
{
	db_table = new ZDBLogTable("database", "roledb"); 
	db_table->addIndex(get_account);
	if(db_table->open()) return TRUE;
	else return FALSE;
}

S3DBI_API void S3DBI_ReleaseDBInterface()		//�ͷ����ݿ�����
{
	db_table->close();
	delete db_table;
}

S3DBI_API void *	S3DBI_GetRoleInfo(IN OUT BYTE * pRoleBuffer, IN char * strUser, IN OUT int &nBufLen)
{
	int size;
	char *buffer = db_table->search(strUser, strlen(strUser) + 1, size);
	if(buffer) {
		nBufLen = size;
		memmove(pRoleBuffer, buffer, size);
		delete[] buffer;
	}
	else nBufLen = 0;
	return pRoleBuffer;
}

//�����ɫ����Ϣ��������ݿⲻ���ڸ���ң������Ӹ����
//bAutoInsertWhenNoExistUser ��ΪTRUEʱ��ʾ�������Ҫ����ĸ���������ݿ��в����������Զ����뵽���ݿ��У�FALSE������ֱ�ӷ��ش���
//ע��INI�ļ�ֻ���Ž���Ҫ�Ķ������ݣ�����Ķ������ݽ��Զ�����ԭ״��
S3DBI_API int	S3DBI_SaveRoleInfo(IN BYTE * pRoleBuffer, IN char * strUser, BOOL bAutoInsertWhenNoExistUser)
{
//��Ҫ����ʺ������ҵ�����
	TRoleData *pRoleData = (TRoleData *)pRoleBuffer;
	if(strUser[0] == 0) strUser = pRoleData->BaseInfo.szName;
	if(db_table->add(strUser, strlen(strUser) + 1, (char *)pRoleBuffer,  pRoleData->dwDataLen)) return 1;
	else return 0;
}

S3DBI_API void S3DBI_ShowDebugInfo(IN BYTE * pRoleBuffer, char * pShowString)
{
	return;
}

S3DBI_API int S3DBI_GetRoleListOfAccount(char * szAccountName, S3DBI_RoleBaseInfo * RoleBaseList, int nMaxCount)
{
	int size = 0;
	S3DBI_RoleBaseInfo *base_ptr = RoleBaseList;
	int count = 0;
	char *buffer = db_table->search(szAccountName, strlen(szAccountName) + 1, size, 0);
	while(buffer) {
		if(count < nMaxCount) {
			TRoleData *pRoleData = (TRoleData *)buffer;
			strncpy(base_ptr->szRoleName, pRoleData->BaseInfo.szName, 32);
			base_ptr->nSex = pRoleData->BaseInfo.bSex;
			base_ptr->nHelmType = pRoleData->BaseInfo.ihelmres;
			base_ptr->nArmorType = pRoleData->BaseInfo.iarmorres;
			base_ptr->nWeaponType = pRoleData->BaseInfo.iweaponres;
			base_ptr->nLevel = pRoleData->BaseInfo.ifightlevel;
			base_ptr++;
		}
		count++;
		buffer = db_table->next(size);
	}
	return count;
}

//ɾ����ɫ
S3DBI_API int	S3DBI_DeleteRole(char * strUser)
{
	return 1;
}

S3DBI_API void*	S3DBI_GetList(S3DBI_LISTNAME listname,  char * strUser)
{
	return 0;
}

S3DBI_API void*	S3DBI_GetList(S3DBI_LISTNAME listname,  int iid)
{
	return 0;
}



S3DBI_API int	S3DBI_GetCountOfList(S3DBI_LISTNAME listname, void * pList)
{
	return 0;
}

S3DBI_API int	S3DBI_DeleteDataAtList(S3DBI_LISTNAME listname, void * pList, int i )
{
	return 0;
}

S3DBI_API void	S3DBI_ReleaseListBuffer(S3DBI_LISTNAME listname, void * pList)
{
	return;
}

S3DBI_API int	S3DBI_ClearList(S3DBI_LISTNAME listname, void * pList)
{
	return 0;
}

S3DBI_API int	S3DBI_DeleteData(S3_DB_TABLENAME tablename, int nId)
{
	return 0;
}
S3DBI_API int	S3DBI_DeleteData(S3_DB_TABLENAME tablename, char * strUser)
{
	return 0;
}


S3DBI_API int	S3DBI_GetListData(S3DBI_LISTNAME listname, void * pList, int i , KTabFileCtrl *pTabFile)
{
	return 0;
}

S3DBI_API int	S3DBI_InsertDataAtList(S3DBI_LISTNAME listname, void * pList, KTabFileCtrl *pTabFile)
{
	return 0;
}
S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, int nId, KTabFileCtrl *pTabFile)
{
	return 0;
}
S3DBI_API int	S3DBI_GetData(S3_DB_TABLENAME tablename, char * strUser, KTabFileCtrl * pTabFile)
{
	return 0;
}
S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, int nId, KTabFileCtrl *pTabFile)
{
	return 0;
}
S3DBI_API int	S3DBI_UpdateData(S3_DB_TABLENAME tablename, char * strUser, KTabFileCtrl *pTabFile)
{
	return 0;
}
S3DBI_API int	S3DBI_InsertData(S3_DB_TABLENAME tablename,  KTabFileCtrl *pTabFile)
{
	return 0;
}


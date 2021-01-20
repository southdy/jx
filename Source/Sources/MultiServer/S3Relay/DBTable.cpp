#include "stdafx.h"
#include "DBTable.h"
#ifndef WIN32
#include <sys/stat.h>
#else
#include <direct.h>
#endif
#include <stdio.h>
#include <string.h>

ZDBTable::ZDBTable(const char *path, const char *name) {
#ifdef WIN32
		getcwd(env_path, MAX_TABLE_NAME);
#else
#endif
		strcat(env_path, "\\");
		strcat(env_path, path);
#ifdef WIN32
		int ret = mkdir(env_path);
#else
        int ret = mkdir(env_path, 0);
#endif
	dbcp = NULL;//��ʼ���α꣨by Fellow��
	
	if(!db_env_create(&dbenv, 0)) {
			dbenv->set_errpfx(dbenv, "index_db");
			if(!dbenv->open(dbenv, env_path, DB_CREATE | DB_INIT_LOG | DB_INIT_LOCK | DB_INIT_MPOOL | DB_INIT_TXN | DB_RECOVER, 0)) {
				index_number = 0;
				strcpy(table_name, name);
				return;				//�ɹ���
			}
		dbenv->close(dbenv, 0);
	}
	dbenv = NULL;
}

ZDBTable::~ZDBTable() {
	if(dbenv) dbenv->close(dbenv, 0);
}

int ZDBTable::addIndex(GetIndexFunc func, bool isUnique) {
	if(!dbenv) return -1;
	if(index_number + 1 >= MAX_INDEX) return index_number;
	get_index_funcs[index_number] = func;
	is_index_unique[index_number] = isUnique;
	return index_number++;
}

bool ZDBTable::open() {
	if(!dbenv) return false;
	char index_table_name[MAX_TABLE_NAME];
    int index;
	int ret;
	if(!db_create(&primary_db, dbenv, 0)) {
		if(!primary_db->open(primary_db, NULL, table_name, NULL, DB_BTREE, DB_CREATE|DB_AUTO_COMMIT, 0664)) {	//�������ݿ�
			for(index = 0; index < index_number; index++) {
				sprintf(index_table_name, "%s.%d", table_name, index);
				if(!db_create(&index_db[index], dbenv, 0)) {
					if(!is_index_unique[index]) {
						if(index_db[index]->set_flags(index_db[index], DB_DUP | DB_DUPSORT)) break;
					}
					if(index_db[index]->open(index_db[index], NULL, index_table_name, NULL, DB_BTREE, DB_CREATE|DB_AUTO_COMMIT, 0664)) break;
					if(ret = primary_db->associate(primary_db, NULL, index_db[index], get_index_funcs[index], DB_AUTO_COMMIT)) {
						index_db[index]->close(index_db[index], 0);
						break;
					}
				}
				else break;
			}
			if(index == index_number) return true;										//�ɹ���
			else while(--index) (index_db[index])->close(index_db[index], 0);				//�����ر�ǰ���������
			primary_db->close(primary_db, 0);
		}
	}
	return false;
}

void ZDBTable::close() {
	if(!dbenv) return;
	primary_db->close(primary_db, 0);
	for(int index = 0; index < index_number; index++) index_db[index]->close(index_db[index], 0);
}

bool ZDBTable::commit() {
/*	if(!dbenv) return false;
	int ret;
	DB_TXN *tid;
	if((ret = dbenv->txn_begin(dbenv, NULL, &tid, 0)) != 0) return false;
	if((ret = primary_db->sync(primary_db, 0)) == 0) {
		ret = tid->commit(tid, 0);
		if(!ret) return true;
	}
	else {
		tid->abort(tid);
		return false;
	}*/
	return true;
}

bool ZDBTable::add(const char *key_ptr, int key_size, const char *data_ptr, int data_size) {
	DBT data, key;
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));
	key.data = (void *)key_ptr;
	key.size = key_size;
	data.data = (void *)data_ptr;
	data.size = data_size;
	
	if(!primary_db->put(primary_db, NULL, &key, &data, DB_AUTO_COMMIT)) {
		return true;
	}
	return false;
}

bool ZDBTable::remove(const char *key_ptr, int key_size, int index) {
	DBT data, key;
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));
	key.data = (void *)key_ptr;
	key.size = key_size;
	if(!primary_db->del(primary_db, NULL, &key, DB_AUTO_COMMIT)) {
		return true;
	}
	return false;
}

char *ZDBTable::_search(bool bKey, const char *key_ptr, int key_size, int &size, int index) {
	dbcp = NULL;
	DBT key, data, pkey;
	if(index < -1 || index >= index_number) return NULL;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	key.data = (void *)key_ptr;
	key.size = key_size;
	if(index == -1) {									//��������
		if(primary_db->get(primary_db, NULL, &key, &data, 0)) return NULL;
	}
	else if(is_index_unique[index]) {					//û���ظ�����
		if(bKey) {
			if(index_db[index]->pget(index_db[index], NULL, &key, &pkey, &data, 0)) return NULL;
		}
		else {
			if(index_db[index]->get(index_db[index], NULL, &key, &data, 0)) return NULL;
		}
	}
	else {												//���α�
		if(index_db[index]->cursor(index_db[index], NULL, &dbcp, 0)) {
			dbcp = NULL;
			return NULL;
		}
		if(bKey) {
			if(dbcp->c_pget(dbcp, &key, &pkey, &data, DB_SET)) {
				dbcp->c_close(dbcp);
				dbcp = NULL;
				return NULL;
			}
		}
		else {
			if(dbcp->c_get(dbcp, &key, &data, DB_SET)) {
				dbcp->c_close(dbcp);
				dbcp = NULL;
				return NULL;
			}
		}

	}
	char *result;
	if(bKey) {
		result = new char[pkey.size];
		memmove(result, pkey.data, pkey.size);
		size = pkey.size;
	}
	else {
		result = new char[data.size];
		memmove(result, data.data, data.size);
		size = data.size;
	}
	return result;
}

char *ZDBTable::_next(bool bKey, int &size) {
	if(!dbcp) return NULL;
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	if(bKey) {
		if(dbcp->c_pget(dbcp, &key, &pkey, &data, DB_NEXT_DUP)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}
	}
	else {
		if(dbcp->c_get(dbcp, &key, &data, DB_NEXT_DUP)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}
	}
	char *result;
	if(bKey) {
		result = new char[pkey.size];
		memmove(result, pkey.data, pkey.size);
		size = pkey.size;
	}
	else {
		result = new char[data.size];
		memmove(result, data.data, data.size);
		size = data.size;
	}
	return result;
}

char *ZDBTable::GetRecord(int &size, CursorPointer cpMode, int index )
{//ȡ�ð��α�ĳһ������
	if(!dbcp)
	{//������ݿ�ָ��û�г�ʼ�����ȳ�ʼ��dbcp
		if(index_db[index]->cursor(index_db[index], NULL, &dbcp, 0))
		{//��ʼ��dbcpʧ��
			dbcp = NULL;
			return NULL;
		}
	}
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	
	if(dbcp->c_get(dbcp, &key, &data, cpMode)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}

	char *result;
	result = new char[data.size];
	memmove(result, data.data, data.size);
	size = data.size;
	
	return result;
}


char *ZDBTable::GetRecord_key(int &size, CursorPointer cpMode, int index )
{
	if(!dbcp)
	{//������ݿ�ָ��û�г�ʼ�����ȳ�ʼ��dbcp
		if(index_db[index]->cursor(index_db[index], NULL, &dbcp, 0))
		{//��ʼ��dbcpʧ��
			dbcp = NULL;
			return NULL;
		}
	}
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	
	if(dbcp->c_get(dbcp, &key, &data, cpMode)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}

	char *result;
	result = new char[key.size];
	memmove(result, key.data, key.size);
	size = key.size;
	
	return result;
}

bool ZDBTable::GetRecordEx(char* aBuffer, int &size,
						   char* aKeyBuffer, int &keysize,
						   CursorPointer cpMode, int index)
{//ȡ�ð��α�ĳһ������(�°溯��)
	if(!dbcp)
	{//������ݿ�ָ��û�г�ʼ�����ȳ�ʼ��dbcp
		if(index_db[index]->cursor(index_db[index], NULL, &dbcp, 0))
		{//��ʼ��dbcpʧ��
			dbcp = NULL;
			return false;
		}
	}
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	
	if(dbcp->c_get(dbcp, &key, &data, cpMode)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return false;
		}

	memmove(aBuffer, data.data, data.size);
	size = data.size;
	
	memmove(aKeyBuffer, key.data, key.size);
	keysize = key.size;

	return true;
}

CDBTableReadOnly::CDBTableReadOnly(const char *path, const char *name) {
#ifdef WIN32
		getcwd(env_path, MAX_TABLE_NAME);
#else
#endif
		strcat(env_path, "\\");
		strcat(env_path, path);
#ifdef WIN32
		int ret = mkdir(env_path);
#else
        int ret = mkdir(env_path, 0);
#endif
	dbcp = NULL;//��ʼ���α꣨by Fellow��

	if(!db_env_create(&dbenv, 0)) {
			dbenv->set_errpfx(dbenv, "index_db");
			if(!dbenv->open(dbenv, env_path, DB_USE_ENVIRON | DB_INIT_MPOOL, 0)) {
				strcpy(table_name, name);
				return;				//�ɹ���
			}
		dbenv->close(dbenv, 0);
	}
	dbenv = NULL;
}

CDBTableReadOnly::~CDBTableReadOnly() {
	if(dbenv) dbenv->close(dbenv, 0);
}

bool CDBTableReadOnly::open() {
	if(!dbenv) return false;
	if(!db_create(&primary_db, dbenv, 0)) {
		if(!primary_db->open(primary_db, NULL, table_name, NULL, DB_BTREE, DB_RDONLY, 0664)) {	//�������ݿ�
			return true;
		}
	}
	return false;
}	

void CDBTableReadOnly::close() {
	if(!dbenv) return;
	primary_db->close(primary_db, 0);
}

char *CDBTableReadOnly::_search(bool bKey, const char *key_ptr, int key_size, int &size) {
	dbcp = NULL;
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	key.data = (void *)key_ptr;
	key.size = key_size;
	if(primary_db->get(primary_db, NULL, &key, &data, 0)) return NULL;

	char *result;
	if(bKey) {
		result = new char[pkey.size];
		memmove(result, pkey.data, pkey.size);
		size = pkey.size;
	}
	else {
		result = new char[data.size];
		memmove(result, data.data, data.size);
		size = data.size;
	}
	return result;
}

char *CDBTableReadOnly::_next(bool bKey, int &size) {
	if(!dbcp) return NULL;
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	if(bKey) {
		if(dbcp->c_pget(dbcp, &key, &pkey, &data, DB_NEXT_DUP)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}
	}
	else {
		if(dbcp->c_get(dbcp, &key, &data, DB_NEXT_DUP)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}
	}
	char *result;
	if(bKey) {
		result = new char[pkey.size];
		memmove(result, pkey.data, pkey.size);
		size = pkey.size;
	}
	else {
		result = new char[data.size];
		memmove(result, data.data, data.size);
		size = data.size;
	}
	return result;
}

char *CDBTableReadOnly::GetRecord(int &size, CursorPointer cpMode)
{
	if(!dbcp)
	{//������ݿ�ָ��û�г�ʼ�����ȳ�ʼ��dbcp
		if(primary_db->cursor(primary_db, NULL, &dbcp, 0))
		{//��ʼ��dbcpʧ��
			dbcp = NULL;
			return NULL;
		}
	}
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	
	if(dbcp->c_get(dbcp, &key, &data, cpMode)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}

	char *result;
	result = new char[data.size];
	memmove(result, data.data, data.size);
	size = data.size;
	
	return result;
}

char *CDBTableReadOnly::GetRecord_key(int &size, CursorPointer cpMode)
{
	if(!dbcp)
	{//������ݿ�ָ��û�г�ʼ�����ȳ�ʼ��dbcp
		if(primary_db->cursor(primary_db, NULL, &dbcp, 0))
		{//��ʼ��dbcpʧ��
			dbcp = NULL;
			return NULL;
		}
	}
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	
	if(dbcp->c_get(dbcp, &key, &data, cpMode)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return NULL;
		}

	char *result;
	result = new char[key.size];
	memmove(result, key.data, key.size);
	size = key.size;
	
	return result;
}

bool CDBTableReadOnly::GetRecordEx(char* aBuffer, int &size,
						   char* aKeyBuffer, int &keysize,
						   CursorPointer cpMode)
{//ȡ�ð��α�ĳһ������(�°溯��)
	if(!dbcp)
	{//������ݿ�ָ��û�г�ʼ�����ȳ�ʼ��dbcp
		if(primary_db->cursor(primary_db, NULL, &dbcp, 0))
		{//��ʼ��dbcpʧ��
			dbcp = NULL;
			return false;
		}
	}
	DBT key, data, pkey;
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	
	if(dbcp->c_get(dbcp, &key, &data, cpMode)) {
			dbcp->c_close(dbcp);
			dbcp = NULL;
			return false;
		}

	memmove(aBuffer, data.data, data.size);
	size = data.size;
	
	memmove(aKeyBuffer, key.data, key.size);
	keysize = key.size;

	return true;
}
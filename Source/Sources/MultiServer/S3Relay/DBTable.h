#ifndef DBTABLE_H
#define DBTABLE_H
//����֧�ָ��ơ�������ģ���������¼�����ݱ���

#include "db.h"

#define MAX_INDEX			16
#define MAX_TABLE_NAME		256

typedef int (*GetIndexFunc)(DB *, const DBT *, const DBT *, DBT *);

class ZDBTable {
	DB *primary_db;												//�������-���ݵ����ݿ�
	DB *index_db[MAX_INDEX];									//�������-���������ݿ�
	GetIndexFunc get_index_funcs[MAX_INDEX];					//�������л�ö���key�ĵĺ����б�
	bool is_index_unique[MAX_INDEX];							//�����Ƿ�Ψһ
	int index_number;											//����������Ŀ
	char table_name[MAX_TABLE_NAME];
	DBC *dbcp;													//Ŀǰ���ǵ��̣߳�ʹ��һ��CURSOR
protected:
	char env_path[MAX_TABLE_NAME];
	DB_ENV *dbenv;												//���ݿ⻷��
	char *_search(bool bKey, const char *key_ptr, int key_size, int &size, int index);		//����ָ����¼
	char *_next(bool bKey, int &size);															//��һ����¼
public:
	ZDBTable(const char *path, const char *name);			//����Ŀ¼�����ݱ������
	virtual ~ZDBTable();
	
	int addIndex(GetIndexFunc func, bool isUnique = false);		//���Ӷ�������
	bool open();												//�����ݱ�
	void close();												//�ر����ݱ�
	bool commit();												//����������ύ
//������¼����
	bool add(const char *key_ptr, int key_size, const char *data_ptr, int data_size);
	char *search(const char *key_ptr, int key_size, int &size, int index = -1){
		return _search(false, key_ptr, key_size, size, index);		//����ָ����¼
	}
	char *next(int &size) {															//��һ����¼
		return _next(false, size);
	}
	char *search_key(const char *key_ptr, int key_size, int &size, int index = -1) {		//����ָ����¼����������ֵ
		return _search(true, key_ptr, key_size, size, index);		//����ָ����¼
	}
	char *next_key(int &size) {															//��һ����¼����������ֵ
		return _next(true, size);
	}
	bool remove(const char *key_ptr, int key_size, int index = -1);
	
	//������¼��¼(by Fellow)
	enum CursorPointer{cpFirst=10, cpCurrent=8, cpNext=19, cpLast=18};
	char *GetRecord(int &size, CursorPointer cpMode, int index = -1);//ȡ�ð��α�ĳһ������
	char *GetRecord_key(int &size, CursorPointer cpMode, int index = -1);	//ȡ�ð��α�ĳһ�����ݵ�Keyֵ
	
	bool GetRecordEx(char* aBuffer, int &size,
				char* aKeyBuffer, int &keysize,
				CursorPointer cpMode, int index = -1);		//ȡ�ð��α�ĳһ�����ݺ�key(�°溯��)
};


//���ݿ�����ֻࣨ����		Add By Fellow At 2003.08.14
//֧�ֲ�����ȡ
class CDBTableReadOnly  
{	
	DB *primary_db;												//�������-���ݵ����ݿ�
	DBC *dbcp;													//Ŀǰ���ǵ��̣߳�ʹ��һ��CURSOR
	char table_name[MAX_TABLE_NAME];
protected:
	char env_path[MAX_TABLE_NAME];
	DB_ENV *dbenv;												//���ݿ⻷��
	char *_search(bool bKey, const char *key_ptr, int key_size, int &size);		//����ָ����¼
	char *_next(bool bKey, int &size);							//��һ����¼
public:
	CDBTableReadOnly(const char *path, const char *name);			//����Ŀ¼�����ݱ������
	virtual ~CDBTableReadOnly();
	
	bool open();												//�����ݱ�
	void close();												//�ر����ݱ�
//������¼����
	char *search(const char *key_ptr, int key_size, int &size){
		return _search(false, key_ptr, key_size, size);		//����ָ����¼
	}
	char *next(int &size) {											//��һ����¼
		return _next(false, size);
	}

	//������¼��¼(by Fellow)
	enum CursorPointer{cpFirst=10, cpCurrent=8, cpNext=19, cpLast=18};
	char *GetRecord(int &size, CursorPointer cpMode);//ȡ�ð��α�ĳһ������
	char *GetRecord_key(int &size, CursorPointer cpMode);	//ȡ�ð��α�ĳһ�����ݵ�Keyֵ
	bool GetRecordEx(char* aBuffer, int &size,
						   char* aKeyBuffer, int &keysize,
						   CursorPointer cpMode);

	char *search_key(const char *key_ptr, int key_size, int &size) {		//����ָ����¼����������ֵ
		return _search(true, key_ptr, key_size, size);		//����ָ����¼
	}
	char *next_key(int &size) {															//��һ����¼����������ֵ
		return _next(true, size);
	}
};
#endif

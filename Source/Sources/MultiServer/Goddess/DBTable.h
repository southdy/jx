#ifndef DBTABLE_H
#define DBTABLE_H
//����֧�ָ��ơ�������ģ���������¼�����ݱ���

#include "db.h"
#include "malloc.h"

#define MAX_INDEX			16					//һ�����ݱ�����������Ŀ
#define MAX_TABLE_NAME		256					//��ı�����

typedef int (*GetIndexFunc)(DB *, const DBT *, const DBT *, DBT *);

typedef struct ZCursor {
	bool bTravel;								//�Ƿ����
	DBC *dbcp;									//��ǰ����ʹ�õ��α�
	int index;									//��ǰ������
	char *key;
	int key_size;
	char *data;									//���ص�����
	int size;									//���ݵĴ�С
}tagZCursor;

class ZDBTable {
	DB *primary_db;												//�������-���ݵ����ݿ�
	DB *index_db[MAX_INDEX];									//�������-���������ݿ�
	GetIndexFunc get_index_funcs[MAX_INDEX];					//�������л�ö���key�ĵĺ����б�
	bool is_index_unique[MAX_INDEX];							//�����Ƿ�Ψһ
	int index_number;											//����������Ŀ
	char table_name[MAX_TABLE_NAME];
protected:
	char env_path[MAX_TABLE_NAME];
	DB_ENV *dbenv;												//���ݿ⻷��
	ZCursor *_search(bool bKey, const char *key_ptr, int key_size, int index);		//����ָ����¼
	bool _next(bool bKey, ZCursor *cursor);															//��һ����¼
public:
	bool bStop;
	ZDBTable(const char *path, const char *name);			//����Ŀ¼�����ݱ������
	virtual ~ZDBTable();
	
	int addIndex(GetIndexFunc func, bool isUnique = false);		//����������
	bool open();												//�����ݱ�
	void close();												//�ر����ݱ�
	bool commit();												//����������ύ��Ŀǰʹ���Զ��ύ����ʹ��

//������¼����
	bool add(const char *key_ptr, int key_size, const char *data_ptr, int data_size);
	bool remove(const char *key_ptr, int key_size, int index = -1);

	void closeCursor(ZCursor *cursor) {
		if(!cursor) return;
		if(cursor->bTravel) {
			free(cursor->key);
		}
		free(cursor->data);

		delete cursor;
	}
	ZCursor *first() {											//�������ݿ⣬�õ���һ����¼	
		return _search(false, NULL, 0, -1);
	}
	ZCursor *search(const char *key_ptr, int key_size, int index = -1){
		return _search(false, key_ptr, key_size, index);		//����ָ����¼
	}
	bool next(ZCursor *cursor) {											//��һ����¼
		return _next(false, cursor);
	}
	ZCursor *search_key(const char *key_ptr, int key_size, int index = -1) {	//����ָ����¼����������ֵ
		return _search(true, key_ptr, key_size, index);
	}
	bool next_key(ZCursor *cursor) {															//��һ����¼����������ֵ
		return _next(true, cursor);
	}
//������һЩά���ԵĲ���
	void deadlock() {						//�������
		dbenv->lock_detect(dbenv, 0, DB_LOCK_DEFAULT, NULL);
	}
	void removeLog();						//�����־�ļ�
};

#define MAX_RETRY	16

#endif

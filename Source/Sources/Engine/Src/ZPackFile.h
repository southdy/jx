#ifndef ZPACKFILE_H
#define ZPACKFILE_H

//#include <windows.h>
//#include "ucl/ucl.h"
#include "KWin32.h"
#include "KMutex.h"
#define MINIMIZE_MAP_SIZE		65536 * 1024			//��Сӳ��Ĵ�СΪ64M

class ZMapFile {
public:
	HANDLE m_hFile;
	HANDLE m_hMap;
	char *m_Ptr;						//����ֱ�ӷ��ʵĻ�����ָ��
	unsigned long m_Size;				//�ļ��Ĵ�С
	unsigned long old_offset;
	ZMapFile(const char *file_name);
	char *map(unsigned long offset = 0, unsigned long size = 0);
	void unmap();
	~ZMapFile();
};

typedef struct {
	unsigned long index_high;
	unsigned long index_low;
	long offset; 
	long size; 
} item_info;

#define MINIMIZE_BLOCK_SIZE	16					//һ��������16���ֽ�
#define LAST_ITEM			10

class ZCache {
	char *buffer;								//ʵ�ʵĻ�����
	item_info *free_items;						//
	long cache_size;
	KMutex m_Mutex;
	item_info last_item[LAST_ITEM];
	int last;
public:
	ZCache(long size);
	virtual ~ZCache();
	char *getNode(unsigned long index_high, unsigned long index_low, long size);
	char *searchNode(unsigned long index_high, unsigned long index_low = 0xfffffffful);
};

//---------------------------------------------------------------------------------------------------------------------------------
typedef struct {		//������Ϣ
	unsigned long id;
	unsigned long offset;
	long size;
	long compress_size;
} index_info;

#define TYPE_NONE			0					//û��ѹ��
#define TYPE_UCL			1					//UCLѹ��
#define TYPE_BZIP2			2					//bzip2ѹ��
#define TYPE_FRAME			0x10				//ʹ���˶���֡ѹ��

unsigned long hash(const char *file_name);									//����ָ���ַ�����hashֵ

//һ��Pack�ļ���������Ľṹ:
//�������ĸ��ֽڵ��ļ���ͷ��־:�ַ���'PACK',Ȼ���������ĿȻ����������ʼ��ƫ����\���ݿ�ʼ��ƫ����,Ȼ����У���,Ȼ���Ǳ������ֽ�:
typedef struct {
	unsigned char signature[4];			//"PACK"
	unsigned long count;				//���ݵ���Ŀ��
	unsigned long index_offset;			//������ƫ����
	unsigned long data_offset;			//���ݵ�ƫ����
	unsigned long crc32;
	unsigned char reserved[12];
} z_pack_header;

class ZPackFile {
protected:
	ZCache		*cache;
	ZMapFile	*data_map;
	index_info	*index_list;
	z_pack_header header;
	bool _readData(int node_index, char *node);								//��ָ��node_index������ȫ������ָ���ڴ��У������Ҫ�Ļ���ɽ�ѹ��
public:
	bool opened;
	ZPackFile(const char *file_name, ZCache *the_cache);							//���ļ�
	virtual ~ZPackFile();
	int getNodeIndex(unsigned long id);
	char *getData(unsigned long index);
	unsigned long getSize(unsigned long index);
	char *getData(const char *name);												//��ȡָ���ڵ������
};

#endif
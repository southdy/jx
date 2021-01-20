#ifndef ZPACKFILE_H
#define ZPACKFILE_H

#include <windows.h>
#include "ucl/ucl.h"
//#include "KWin32.h"
//#include "KMutex.h"
#define MINIMIZE_MAP_SIZE		65536 * 1024			//��Сӳ��Ĵ�СΪ64M

typedef struct {
	long compress_size;
	long size;
} frame_info;

typedef struct {
	unsigned long	id;
	int				frame;
} frame_index_info;

#define MAX_IMAGE		0x10													//�����ʱ�ļ��ĸ���


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

//---------------------------------------------------------------------------------------------------------------------------------

typedef struct {		//������Ϣ
	unsigned long id;
	unsigned long offset;
	long size;
	long compress_size;
	FILETIME UpdateTime;
} index_info_nt;

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

enum enumSubDir{sdFont=0,sdMaps,sdScript,sdResource,sdSetting,sdSound,sdSpr,sdUi,sdLastDir};


//һ��Pack�ļ���������Ľṹ:
//�������ĸ��ֽڵ��ļ���ͷ��־:�ַ���'PACK',Ȼ���������ĿȻ����������ʼ��ƫ����\���ݿ�ʼ��ƫ����,Ȼ����У���,Ȼ���Ǳ������ֽ�:

typedef struct {
	typedef struct {
		unsigned long count;				//���ݵ���Ŀ��
		unsigned long index_count_offset;	//������Ŀ��ƫ����
		unsigned long index_offset;			//������ƫ����
		unsigned long data_offset;			//���ݵ�ƫ����
	} z_sub_header;

	unsigned char signature[4];			//"PACK"
	long count;
	z_sub_header SubHeader[sdLastDir-1];
	unsigned long crc32;
	unsigned char reserved[12];
} z_pack_header_nt;

typedef struct {
	unsigned char signature[4];			//"PACK"
	unsigned long count;				//���ݵ���Ŀ��
	unsigned long index_offset;			//������ƫ����
	unsigned long data_offset;			//���ݵ�ƫ����
	unsigned long crc32;
	unsigned char reserved[12];
} z_pack_header;

class ZCache {
	char *buffer;								//ʵ�ʵĻ�����
	item_info *free_items;						//
	long cache_size;
	item_info last_item[LAST_ITEM];
	int last;
public:
	ZCache(long size);
	virtual ~ZCache();
	char *getNode(unsigned long index_high, unsigned long index_low, long size);
	char *searchNode(unsigned long index_high, unsigned long index_low = 0xfffffffful);
};

class ZPackFile {
protected:
	ZCache		*cache;							//����
	ZMapFile	*data_map;						//�ļ�����
	index_info	*index_list;					//����ָ��
	z_pack_header header;						//���ļ�ͷ��
	bool _readData(int node_index, char *node);								//��ָ��node_index������ȫ������ָ���ڴ��У������Ҫ�Ļ���ɽ�ѹ��
public:
	bool opened;
	ZPackFile(const char *file_name, ZCache *the_cache);							//���ļ�
	virtual ~ZPackFile();
	int getNodeIndex(unsigned long id);
	char *getData(unsigned long index);
	unsigned long getSize(unsigned long index);
	char *getData(const char *name);								//��ȡָ���ڵ������
	index_info* IndexData(int index){return &index_list[index];};	//������������
	char* getOrigData(unsigned long id);				//ȡ��δ��ѹ��ԭʼ����
	int getFileCount(){return header.count;};							//ȡ���ļ���
	const z_pack_header* GetHeader() const{return &header;};			//ȡ���ļ�ͷ
};


class ZPackFileNT {
protected:
	ZCache		*cache;							//����
	ZMapFile	*data_map;						//�ļ�����
	index_info_nt	*index_list;					//����ָ��
	z_pack_header_nt header;						//���ļ�ͷ��
	bool _readData(int node_index, char *node);								//��ָ��node_index������ȫ������ָ���ڴ��У������Ҫ�Ļ���ɽ�ѹ��
public:
	bool opened;
	ZPackFileNT(const char *file_name, ZCache *the_cache);							//���ļ�
	virtual ~ZPackFileNT();
	int getNodeIndex(unsigned long id);
	char *getData(unsigned long index);
	unsigned long getSize(unsigned long index);
	char *getData(const char *name);								//��ȡָ���ڵ������
	index_info_nt* IndexData(int index){return &index_list[index];};	//������������
	char* getOrigData(unsigned long id);				//ȡ��δ��ѹ��ԭʼ����
	int getFileCount(){return header.count;};							//ȡ���ļ���
	const z_pack_header_nt* GetHeader() const{return &header;};			//ȡ���ļ�ͷ
};

#endif
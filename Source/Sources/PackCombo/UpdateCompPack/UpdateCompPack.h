
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UPDATECOMPPACK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UPDATECOMPPACK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef UPDATECOMPPACK_EXPORTS
#define UPDATECOMPPACK_API __declspec(dllexport)
#else
#define UPDATECOMPPACK_API __declspec(dllimport)
#endif

//һ��Pack�ļ���������Ľṹ:
//�������ĸ��ֽڵ��ļ���ͷ��־:�ַ���'PACK',Ȼ���������ĿȻ����������ʼ��ƫ����\���ݿ�ʼ��ƫ����,Ȼ����У���,Ȼ���Ǳ������ֽ�:
typedef struct {
	unsigned char signature[4];			//"PACK"
	unsigned long count;				//���ݵ���Ŀ��
	unsigned long index_offset;			//������ƫ����
	unsigned long data_offset;			//���ݵ�ƫ����
	unsigned long crc32;
	unsigned char reserved[12];
} t_pack_header;

typedef struct {		//������Ϣ
	unsigned long id;
	unsigned long offset;
	long size;
	long compress_size;
} t_index_info;

int getIndex(t_index_info* IndexList,int Count, unsigned long id);
void GetHeader(t_pack_header* aHeader, FILE* aFile);
void GetIndexInfo(t_index_info* aIndexInfo, const t_pack_header* aHeader, FILE* aFile);

extern "C" 
UPDATECOMPPACK_API HRESULT PackComb(const char* stdFilePath,const char* addFilePath);


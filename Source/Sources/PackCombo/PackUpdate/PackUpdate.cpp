//�����Ǵ���SPR�ļ��Ĵ���

//SPRͷ�ļ��Ķ��壬Ϊ��ʵ�ֿ���ֲ��Ŀ��
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>

#include <ucl/ucl.h>
#include <conio.h>
#include <IOSTREAM>

using namespace std;
#include "ZPackFile.h"
typedef struct {
	BYTE	Comment[4];	// ע������(SPR\0)
	WORD	Width;		// ͼƬ���
	WORD	Height;		// ͼƬ�߶�
	WORD	CenterX;	// ���ĵ�ˮƽλ��
	WORD	CenterY;	// ���ĵĴ�ֱλ��
	WORD	Frames;		// ��֡��
	WORD	Colors;		// ��ɫ��
	WORD	Directions;	// ������
	WORD	Interval;	// ÿ֡���������Ϸ֡Ϊ��λ��
	WORD	Reserved[6];// �����ֶΣ����Ժ�ʹ�ã�
} SPRHEAD;

typedef struct
{
	DWORD	Offset;		// ÿһ֡��ƫ��
	DWORD	Length;		// ÿһ֡�ĳ���
} SPROFFS;


//���¶���
#define FRAME_SIZE			800 * 1024			//800K���ϵ�SPR�ļ�ʹ�÷�֡��ѹ��
int root_length = 0;
char SubDir[8][10]={"font","maps","script","��Ϸ��Դ","settings","sound","spr","ui"};
#define MAX_FILE					2004800			//���20����ļ�
z_pack_header_nt header;							//�ļ�ͷ
index_info_nt index_list[MAX_FILE];
char *temp_buffer[MAX_FILE * sizeof(index_info_nt)];
int file_count;										//��ǰ�ļ�������

enumSubDir NowDir;
unsigned long offset;								//��ǰƫ����
#define COMPRESS_BUF_SIZE	10240000
char compress_buffer[COMPRESS_BUF_SIZE];			//10M��ѹ����������������е�֡��һ��д

ZCache* baseCache;
ZPackFileNT* basePack;
const z_pack_header_nt* baseHeader;

FILETIME GetFileLastWriteTime(const char* FilePath)
{
	HANDLE hFile;
	FILETIME aFileTime;
	FILETIME ftime;
	memset(&aFileTime,0,sizeof(FILETIME));
	hFile = CreateFile(FilePath,GENERIC_READ,					// ���ļ���Win32 API����
		FILE_SHARE_READ|FILE_SHARE_DELETE, NULL, OPEN_EXISTING,	// ֻ�����ʽ�򿪼���
		FILE_FLAG_BACKUP_SEMANTICS,NULL);						// ���ִ��Ŀ¼
	__try
	{
		if(hFile) GetFileTime((HANDLE)hFile,NULL,NULL,&aFileTime);
		
		FileTimeToLocalFileTime(&aFileTime, &ftime); // ת���ɱ���ʱ��
	}
	__finally
	{
		CloseHandle(hFile);
	}
	return ftime;
}
//����һ���ļ��������ļ��У�����ƫ����
bool addFile(FILE *output, const char *file_name) {
	char full_name[MAX_PATH];
	getcwd(full_name, MAX_PATH);
	strcat(full_name, "\\");
	strcat(full_name, file_name);
	char *ptr = full_name;
	while(*ptr) {
		if(*ptr >= 'A' && *ptr <= 'Z') *ptr += 'a' - 'A';
		ptr++;
	}

	unsigned long id = hash(full_name + root_length);//ȡ�����ݵ�Ψһid
	
	//�����ɰ����Ƿ�����ͬ��id
	int baseIndex = basePack->getNodeIndex(id);
	FILETIME fTime = GetFileLastWriteTime(file_name);
	if(baseIndex >= 0)//����ھɰ����ҵ���ͬid����Ҫ�Ƚ�ʱ��
	{
		index_info_nt* IndexInfo = basePack->IndexData(baseIndex);
		if(CompareFileTime(&fTime,&IndexInfo->UpdateTime) != 1)
		{//�������ʱ��û��ԭ���µĻ����Ͳ�����
			cout<<"["<<SubDir[NowDir]<<"] "<<"Skip File:"<<file_name<<endl;
			return true;
		}
	}

	ZMapFile map(file_name);
	map.map();
	if(baseIndex >= 0)
	{
		bool bSPR = false;									//�Ƿ�ΪSPR�ļ�
		const char *ext = file_name + strlen(file_name) - 3;
		if(*ext == 's' && *(ext + 1) == 'p' && *(ext + 2) == 'r') bSPR = true;
		
		char* aInnerBuffer = basePack->getData(id);
		if(aInnerBuffer && ((!bSPR) || map.m_Size < FRAME_SIZE))
		{
			if (memcmp(map.m_Ptr,aInnerBuffer , map.m_Size) == 0)
			{
				cout<<"["<<SubDir[NowDir]<<"] "<<"Skip File(Date New):"<<file_name<<endl;
				map.unmap();
				return true;
			}
		}
		
	}

	int index;

	for(index = 0; index < file_count; index++)
	{
		if(index_list[index].id == id)
		{
			cout<<"Error: Same ID["<<id<<"] -- "<<full_name + root_length<<endl;
			getch();
			return false;
		}
	}

	index=file_count;
	
	file_count++;
	++(header.SubHeader[NowDir].count);

	int compress_size = 0;

	unsigned long compress_type = TYPE_UCL;				//ʹ��UCLѹ��

	bool bSPR = false;									//�Ƿ�ΪSPR�ļ�
	const char *ext = file_name + strlen(file_name) - 3;
	if(*ext == 's' && *(ext + 1) == 'p' && *(ext + 2) == 'r') bSPR = true;

	int r;
	unsigned int size = 0;

	if(map.m_Ptr) {
		//�������
		index_list[index].id = id;
		index_list[index].offset = offset;
		index_list[index].size = map.m_Size;
		index_list[index].UpdateTime = fTime;//�ļ�����޸�����
		
		ptr = compress_buffer;
		
		if(!bSPR || map.m_Size < FRAME_SIZE) {
			if(compress_type == TYPE_UCL) {
				r = ucl_nrv2b_99_compress((BYTE *)map.m_Ptr, map.m_Size, (BYTE *)ptr, (unsigned int *)&index_list[index].compress_size, NULL, 5, NULL, NULL);
			}
			else if(compress_type == TYPE_BZIP2) {
//				index_list[index].compress_size = COMPRESS_BUF_SIZE;
//				r = BZ2_bzBuffToBuffCompress(ptr, (unsigned int *)&index_list[index].compress_size, map.m_Ptr, map.m_Size, 9, 0, 30);
			}
			if(r) return false;
			fwrite(compress_buffer, 1, index_list[index].compress_size, output);
			offset += index_list[index].compress_size;
			printf("%s [%d]->[%d]\n", full_name + root_length, map.m_Size, index_list[index].compress_size);
			index_list[index].compress_size |= (compress_type << 24);
		}
		else {								//ÿ֡����ѹ��
			SPRHEAD *head;
			head = (SPRHEAD *)map.m_Ptr;
			memmove(ptr, head, sizeof(SPRHEAD) + head->Colors * 3);			//ǰ������ݲ�ѹ��
			ptr += sizeof(SPRHEAD) + head->Colors * 3;
			frame_info *compress_frame_info = (frame_info *)ptr;					//ѹ����ÿһ֡������
			ptr += head->Frames * sizeof(SPROFFS);
	
			SPROFFS *frame_info = (SPROFFS *)(map.m_Ptr + sizeof(SPRHEAD) + head->Colors * 3);		//ԭ��ÿһ֡������
			char *frame_data = (char *)frame_info + head->Frames * sizeof(SPROFFS);
			int frame_index;

			int frame_offset = 0;
			for(frame_index = 0; frame_index < head->Frames; frame_index++) {
//ѹ��ÿһ֡������
				if(frame_info[frame_index].Length >= 256) {				//С��256�ֽڵĲ�ѹ��
					if(compress_type == TYPE_UCL) {
						r = ucl_nrv2b_99_compress((BYTE *)frame_data + frame_info[frame_index].Offset, frame_info[frame_index].Length, (BYTE *)ptr, &size, NULL, 10, NULL, NULL);
					}
					else if(compress_type == TYPE_BZIP2) {
//						size = COMPRESS_BUF_SIZE;
//						r = BZ2_bzBuffToBuffCompress(ptr, &size, frame_data + frame_info[frame_index].Offset, frame_info[frame_index].Length, 9, 0, 30);
					}
					if(r) return false;
					compress_frame_info[frame_index].size = frame_info[frame_index].Length;		//��¼ԭ���Ĵ�С
				}
				else {
					size = frame_info[frame_index].Length;
					memmove(ptr, (BYTE *)frame_data + frame_info[frame_index].Offset, size);
					compress_frame_info[frame_index].size = -(long)frame_info[frame_index].Length;		//��¼ԭ���Ĵ�С
				}
				compress_size += size;
				compress_frame_info[frame_index].compress_size = size;
				frame_offset += size;
				ptr += size;		
			}
			fwrite(compress_buffer, 1, ptr - compress_buffer, output);
			offset += ptr - compress_buffer;
			printf("[frame] %s old size = %d, compressed size = %d\n", full_name + root_length, map.m_Size, compress_size);
			index_list[index].compress_size = (ptr - compress_buffer) | ((compress_type | TYPE_FRAME) << 24);
		}
	}
	map.unmap();
	return true;
}

void addDirectory(FILE *output, const char *rootDir, const char *subDir = NULL) {
	char szRealDir[MAX_PATH];
	if(subDir) sprintf(szRealDir, "%s\\%s", rootDir, subDir);
	else {
		strcpy(szRealDir, rootDir);
		root_length = strlen(rootDir);
		while(rootDir[root_length] != '\\') root_length--;
	}
	if(chdir(szRealDir)) return;
	_finddata_t FindData;
	long dir = _findfirst("*.*", &FindData);
	while(dir != -1) {
		if(strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0)	{
			if(_findnext(dir, &FindData)) break;
			continue;
		}
		if(FindData.attrib & _A_SUBDIR)
		{
			addDirectory(output, szRealDir, FindData.name);
		}
		else
		{	
			if(!addFile(output, FindData.name)) return;
		}
		if(_findnext(dir, &FindData)) break;
	} 
	_findclose(dir);
	chdir(rootDir);
}

bool pack(const char *dir, const char *pack_name = 0) {
	char name_buffer[MAX_PATH];
	file_count = 0;
	offset = 0;
	if(!pack_name) {
		strcpy(name_buffer, dir);
		strcat(name_buffer, ".pak");
		pack_name = name_buffer;
	}

	FILE * output = NULL;
	output = fopen(pack_name, "wb");
	memset(&header, 0, sizeof(z_pack_header_nt));
	fwrite(&header, 1, sizeof(z_pack_header_nt), output);
	offset += sizeof(header);

	int i;
	for(i=sdFont;i<sdLastDir;++i)
	{//�Ը����Ӱ����в���
		NowDir = (enumSubDir)i;

		header.SubHeader[NowDir].count = 0;
		header.SubHeader[NowDir].index_count_offset = file_count;
		char szRealDir[MAX_PATH];
		sprintf(szRealDir, "%s\\%s", dir, SubDir[i]);
		if(chdir(szRealDir)) continue;

		addDirectory(output,szRealDir);
	}
	
	memcpy(header.signature, "PACK", 4);
	for(i=sdFont;i<sdLastDir;++i)
	{
		int IndexOf =header.SubHeader[i].index_count_offset;
		header.SubHeader[i].index_offset = offset + sizeof(index_info_nt) * IndexOf;
		header.SubHeader[i].data_offset = index_list[IndexOf].offset;
		
		cout<<SubDir[i]<<" Index Count:"<<header.SubHeader[i].count<<endl;
	}
	header.count = file_count;
	int result = fwrite(&index_list[0], 1, file_count * sizeof(index_info_nt), output);
	fseek(output, 0, SEEK_SET);
	fwrite(&header, 1, sizeof(header), output);
	fclose(output);

	return true;
}

int main(int argc, char **argv)
{

	if(argc < 4) {
		printf("usage: packUpdate [base pack file name] [pack directory] [pack file name]");
		return 0;
	}
	

	if (ucl_init() != UCL_E_OK) return 0;

	//argv[1] ---- f:\\temp\\PackBase.pac
	//argv[2] ---- D:\\����\\sword_a1_small
	//argv[3] ---- F:\\temp\\PackUpdate1.pac

	char* baseFile = argv[1];
	//char* baseFile = "f:\\temp\\PackBase.pac";//argv[1];
	baseCache =new ZCache(65536 * 1024);
	basePack = new ZPackFileNT(baseFile, baseCache);
	baseHeader = basePack->GetHeader();

	pack(argv[2],argv[3]);
	//pack("D:\\����\\sword_a2_small"/*argv[2]*/,"F:\\temp\\PackUpdate1.pac"/*argv[3]*/);
	if(argc == 4) return 0;

	cout<<"Press any key to begin test..."<<endl;
	getch();
	ZCache aaCache(65536 * 1024);
	ZPackFileNT aaPack(argv[3], &aaCache);
	int i;
	const z_pack_header_nt* header = aaPack.GetHeader();
	cout<<"Pack File Count:"<<header->SubHeader[1].count<<endl; 
	getch();
	
	for(i=0;i<header->SubHeader[1].count;++i)
	{
		int IndexOf = header->SubHeader[1].index_count_offset;
		index_info_nt* IndexInfo=aaPack.IndexData(IndexOf + i);
		int fileSize = IndexInfo->compress_size & 0x00FFFFFF;
		cout<<"["<<i<<"]||"
			<<IndexInfo->id<<"||"
			<<IndexInfo->offset<<"||"
			<<IndexInfo->size<<"||"
			<<fileSize<<"||";
		char* aa = aaPack.getOrigData(IndexInfo->id);
		if(aa)
			cout<<"OK"<<endl;
		else
		{
			cout<<"Error"<<endl;
			getch();
		}
	}
	getch();
	return 0;
}
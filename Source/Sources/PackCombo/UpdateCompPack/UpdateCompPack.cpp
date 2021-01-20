// UpdateCompPack.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "UpdateCompPack.h"

#define MAX_FILE					2004800			//���20����ļ�

char temp_buffer[MAX_FILE] = {0};


int getIndex(t_index_info* IndexList,int Count, unsigned long id)
{//���ַ��ҵ�ָ��������
	int nBegin, nEnd, nMid;
	nBegin = 0;
	nEnd = Count;
	while (nBegin <= nEnd) {
		nMid = (nBegin + nEnd) / 2;
		if (id == IndexList[nMid].id) break;
		if (id < IndexList[nMid].id) nEnd = nMid - 1;
		else nBegin = nMid + 1;
	}
	if(id != IndexList[nMid].id) return -1;								//�����ļ�����Ҳû��
	return nMid;
	
}
void GetHeader(t_pack_header* aHeader, FILE* aFile)
{
	fseek(aFile,0,SEEK_SET);
	fread(aHeader,1,sizeof(t_pack_header),aFile);
}

void GetIndexInfo(t_index_info* aIndexInfo, const t_pack_header* aHeader, FILE* aFile)
{
	fseek(aFile,aHeader->index_offset,SEEK_SET);
	fread(aIndexInfo, sizeof(t_index_info), aHeader->count, aFile);
}

extern "C" 
UPDATECOMPPACK_API HRESULT PackComb(const char* stdFilePath,const char* addFilePath)
{//�ϲ��ļ�
	t_pack_header stdHeader;
	t_pack_header addHeader;
	unsigned long offset;
	t_index_info* stdIndex;
	t_index_info* addIndex;
	//����ԴԴ�ļ�
	FILE* stdFile =NULL;
	stdFile = fopen(stdFilePath, "r+b");
	if(stdFile == NULL)
	{
		return E_INVALIDARG;
	}
	GetHeader(&stdHeader, stdFile);

	//����Դ�������ļ�
	FILE* addFile =NULL;
	addFile = fopen(addFilePath, "rb");
	if(addFile == NULL)
	{
		return E_INVALIDARG;
	}
	GetHeader(&addHeader, addFile);

	stdIndex =  new t_index_info[stdHeader.count + addHeader.count];
	memset(stdIndex,0,sizeof(t_index_info) * (stdHeader.count + addHeader.count));
	GetIndexInfo(stdIndex, &stdHeader, stdFile);

	addIndex =  new t_index_info[addHeader.count];
	memset(addIndex,0,sizeof(t_index_info) * addHeader.count);
	//���addFile����û����Ҫ���ӵ����ݾ�ֱ�����
	if (addHeader.count == 0)
	{//�ر��ļ�
		fclose(stdFile);
		fclose(addFile);
		return S_OK;
	}
	GetIndexInfo(addIndex, &addHeader, addFile);
	
	//copy���ӵ����ݵ�Դ�ļ���
	int DataSize = addHeader.index_offset - addHeader.data_offset;
	char* DataBuffer = new char[DataSize];
	fseek(addFile,addHeader.data_offset,SEEK_SET);
	fread(DataBuffer, 1, DataSize, addFile);

	fseek(stdFile,0,SEEK_END);
	offset = ftell(stdFile);
	char *ptr = DataBuffer;
	while(DataSize > 0) {
		int result;
		if(DataSize > 0x100000) result = fwrite(ptr ,1,0x100000,stdFile);
		else result = fwrite(ptr ,1, DataSize,stdFile);
		DataSize -= result;
		ptr += result;
	}
	//���½���index
	int i,j;
	for(i = 0; i < addHeader.count; i++)
	{
		t_index_info* index = &addIndex[i];
		for(j = 0; j < stdHeader.count; j++) {
			if(index->id < stdIndex[j].id)
			{	//��������
				memcpy(temp_buffer, (char *)&stdIndex[j], (stdHeader.count - j) * sizeof(t_index_info));
				memcpy((char *)&stdIndex[j + 1], temp_buffer, (stdHeader.count - j) * sizeof(t_index_info));
				//���������
				memcpy(&stdIndex[j], index, sizeof(t_index_info));
				stdIndex[j].offset = offset + index->offset - sizeof(t_pack_header);
				++stdHeader.count;
				break;
			}
			else if(index->id == stdIndex[j].id)
			{	//�������滻������
				stdIndex[j].offset = offset + index->offset - sizeof(t_pack_header);
				stdIndex[j].size = index->size;
				stdIndex[j].compress_size = index->compress_size;
				break;
			}
		}
		if(j == stdHeader.count) {
			memcpy(&stdIndex[j], index, sizeof(t_index_info));
			stdIndex[j].offset = offset + index->offset - sizeof(t_pack_header);//Add By Fellow 09.04
			stdHeader.count++;
		}
	}

	//д���µ�index
	fseek(stdFile,0,SEEK_END);
	offset = ftell(stdFile);
	fwrite(stdIndex,sizeof(t_index_info),stdHeader.count,stdFile);
	//д��header
	stdHeader.index_offset = offset;
	fseek(stdFile,0,SEEK_SET);
	fwrite(&stdHeader,sizeof(t_pack_header),1,stdFile);

	//�ر��ļ�
	delete [] DataBuffer;
	delete [] stdIndex;
	delete [] addIndex;
	fclose(stdFile);
	fclose(addFile);
	return S_OK;
}
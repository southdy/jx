//�����Ǵ���SPR�ļ��Ĵ���

//SPRͷ�ļ��Ķ��壬Ϊ��ʵ�ֿ���ֲ��Ŀ��
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <IOSTREAM>
#include <conio.h>
#include "ZPackFile.h"
#include "SavePack.h"

using namespace std;
#define MAX_FILE					2004800			//���20����ļ�

index_info index_list[MAX_FILE];
char temp_buffer[MAX_FILE * sizeof(index_info)];

void IndexNT2Normal(index_info_nt* IndexNT, index_info* aIndex)
{
	aIndex->id = IndexNT->id;
	aIndex->offset = IndexNT->offset;
	aIndex->size = IndexNT->size;
	aIndex->compress_size = IndexNT->compress_size;
	return;
}
bool PackExp(char* baseFile, char* addFile, char* nowFile, char* saveFile)
{
	//���ĸ��ļ�
	ZCache baseCache(65536 * 1024);
	ZPackFileNT basePack(baseFile, &baseCache);
	cout<<"Open Base Pack "<<baseFile<<" Successfully.";
	cout<<"Base Pack File Count:"<<basePack.getFileCount()<<endl;

	ZCache addCache(65536 * 1024);
	ZPackFileNT addPack(addFile, &addCache);
	if(addFile)
	{
		cout<<"Open Add Pack "<<addFile<<" Successfully.";
		cout<<"Add Pack File Count:"<<addPack.getFileCount()<<endl;
	}
	else
	{
		cout<<"No Add Pack File"<<endl;
	}
	
	ZCache nowCache(65536 * 1024);
	ZPackFileNT nowPack(nowFile, &nowCache);
	cout<<"Open Latest Pack "<<nowFile<<" Successfully.";
	cout<<"Latest Pack File Count:"<<nowPack.getFileCount()<<endl;

	CSavePack SavePack;
	if(SavePack.open(saveFile, index_list))
		cout<<"Open Save Pack "<<saveFile<<" Successfully."<<endl;
	else
	{
		cout<<"Error Save Pack "<<saveFile<<" ."<<endl;
		getch();
		exit(1);
	}
	//cout<<"Press any key to continue..."<<endl;
	//getch();
	
	int i;
	for(i=0;i<nowPack.getFileCount();++i)
	{
		index_info_nt* IndexInfoNow = nowPack.IndexData(i);
		
		if(addFile)	//���addFile����������ļ�
		{									//�͸�addFile������ļ��Ƚ�ʱ��
			int addPackIndex = addPack.getNodeIndex(IndexInfoNow->id);
			if(addPackIndex != -1)
			{
				index_info_nt* IndexInfoAdd=addPack.IndexData(i);
				if(CompareFileTime(&IndexInfoNow->UpdateTime,&IndexInfoAdd->UpdateTime) != 1)
				{//���ʱ����ԭ�����ļ�һ���Ļ�������
					cout<<"ID:"<<IndexInfoNow->id<<" Skip."<<endl;
					continue;
				}
			}
		}
		index_info aIndex;
		IndexNT2Normal(IndexInfoNow,&aIndex);
		char* aData = nowPack.getOrigData(IndexInfoNow->id);
		SavePack.AddData(aData, &aIndex, temp_buffer);
		int fileSize = IndexInfoNow->compress_size & 0x00FFFFFF;
		cout<<"Add Data From File:["<<nowFile<<"] ID:"<<IndexInfoNow->id
				<<" Size:"<<fileSize<<endl;
	}
	if(SavePack.Close())
		cout<<"File "<<saveFile<< " Saved."<<endl;
	else
		cout<<"Error Save File "<<saveFile<< " ."<<endl;

	return true;
}


int main(int argc, char **argv)
{

	if(argc < 5) {
		printf("usage: PackExp [base pack file] [previous version pack file] [present version pack file] [output pack file].\n");
		return 0;
	}

	if (ucl_init() != UCL_E_OK) return 0;

	//argv[1] ----- f:\\temp\\PackBase.pac
	//argv[2] ----- f:\\temp\\PackUpdate1.pac
	//argv[3] ----- f:\\temp\\PackUpdate2.pac
	//argv[4] ----- f:\\temp\\Update1-2.pak

	//���ɴӰ汾a���汾b��������
	char* basePackFile = argv[1];		//�汾0���ļ���ԭʼ�汾��
	char* addPackFile = argv[2];		//�汾a���ļ�
	char* nowPackFile = argv[3];		//�汾b���ļ�
	char* outputPackFile = argv[4];		//�������ļ�
	if (strcmp(addPackFile,"none") == 0) addPackFile =NULL;
	PackExp(basePackFile,addPackFile,nowPackFile,outputPackFile);

	if(argc == 5)return 0;
	
	cout<<"Press any key to begin test..."<<endl;
	getch();
	ZCache aaCache(65536 * 1024);
	ZPackFile aaPack(outputPackFile, &aaCache);
	int i;
	const z_pack_header* header = aaPack.GetHeader();
	cout<<"Pack File Count:"<<aaPack.getFileCount()<<endl; 

	getch();
	
	for(i=0;i<header->count;++i)
	{
		index_info* IndexInfo=aaPack.IndexData(i);
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
// DBDumpLoad.cpp: implementation of the CDBDump class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "DBDumpLoad.h"

////////CDBDump////////////////////////////////////////////////////////////////////////////
CDBDump::CDBDump()
{
	memset(m_FilePath,0,MAX_PATH);
	m_output = NULL;
	m_IsOpened = false;
}
bool CDBDump::Open(char* aFilePath)
{//�򿪰��ļ�
	if(m_IsOpened)return false;

	_finddata_t FindData;
	long aFileFound =_findfirst(aFilePath, &FindData);
	if(aFileFound != -1)return false;	//������ļ����ھͷ��ز��ɹ�

	m_output = fopen(aFilePath, "wb");
	if(!m_output)return false;			//����򿪸��ļ�ʧ�ܾͷ��ز��ɹ�

	strcpy(m_FilePath,aFilePath);
	m_IsOpened = true;
	return true;
}

bool CDBDump::Close()
{//������ļ�
	if(!m_IsOpened) return false;

	if(fclose(m_output)) return false;
	
	memset(m_FilePath,0,MAX_PATH);
	m_output = NULL;
	m_IsOpened = false;
	return true;
}

bool CDBDump::AddData(char* key, const size_t keysize, char *aData, const size_t size)
{//����ļ�
	if(!m_IsOpened) return false;
	unsigned long offset = ftell(m_output);

	if(fwrite(&keysize, sizeof(size_t), 1, m_output) != 1)
	{
		fseek(m_output, offset, SEEK_SET);
		return false;
	}
	if(fwrite(key, 1, keysize, m_output) != keysize)
	{
		fseek(m_output, offset, SEEK_SET);
		return false;
	}

	if(fwrite(&size, sizeof(size_t), 1, m_output) != 1)
	{
		fseek(m_output, offset, SEEK_SET);
		return false;
	}

	if(fwrite(aData, 1, size, m_output) != size)
	{
		fseek(m_output, offset, SEEK_SET);
		return false;
	}

	return true;
}

////////CDBLoad////////////////////////////////////////////////////////////////////////////
CDBLoad::CDBLoad()
{
	memset(m_FilePath,0,MAX_PATH);
	m_output = NULL;
	m_IsOpened = false;
}
bool CDBLoad::Open(char* aFilePath)
{//�򿪰��ļ�
	if(m_IsOpened)return false;
	_finddata_t FindData;
	long aFileFound =_findfirst(aFilePath, &FindData);
	if(aFileFound == -1)return false;	//������ļ������ھͷ��ز��ɹ�

	m_output = fopen(aFilePath, "rb");
	if(!m_output)return false;			//����򿪸��ļ�ʧ�ܾͷ��ز��ɹ�

	strcpy(m_FilePath,aFilePath);
	m_IsOpened = true;
	return true;
}

bool CDBLoad::Close()
{//������ļ�
	if(!m_IsOpened) return false;

	if(fclose(m_output)) return false;
	
	memset(m_FilePath,0,MAX_PATH);
	m_output = NULL;
	m_IsOpened = false;
	return true;
}

bool CDBLoad::ReadData(char* key, size_t& keysize, char *aData, size_t& size)
{//����ļ�
	if(!m_IsOpened) return false;

	if(fread(&keysize, sizeof(size_t), 1, m_output) != 1)
		return false;
	
	if(fread(key, 1, keysize, m_output) != keysize)
		return false;

	if(fread(&size, sizeof(size_t), 1, m_output) != 1)
		return false;

	if(fread(aData, 1, size, m_output) != size)
		return false;

	return true;
}

void CDBLoad::GotoHead()
{//���ļ�ָ���ƶ����ļ�ͷ
	if(!m_IsOpened) return;
	if(m_output)
		fseek(m_output, 0, SEEK_SET);
}

bool CDBLoad::SearchData(char* key, char *aData, size_t& size)
{//��������
	GotoHead();		//���ļ�ָ���ƶ����ļ�ͷ

	char aBuffer[64 * 1024] = {0};
	char aKeyBuffer[32] = {0};
	size_t asize,akeysize;

	while(ReadData(aKeyBuffer,akeysize,aBuffer,asize))
	{
		if(strcmp(key, aKeyBuffer) == 0)
		{
			memcpy(aData, aBuffer, size);
			size = asize;
			return true;
		}
	}
	return false;
}
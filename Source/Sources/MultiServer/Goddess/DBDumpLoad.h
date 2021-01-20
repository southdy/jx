// DBDumpLoad.h: interface for the CDBDump class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBDUMPLOAD_H__956A6AC3_33CD_44D2_B4B2_771DE290B32F__INCLUDED_)
#define AFX_DBDUMPLOAD_H__956A6AC3_33CD_44D2_B4B2_771DE290B32F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBDump  
{
private:
	char m_FilePath[MAX_PATH];							//�ļ���
	FILE* m_output;
	bool m_IsOpened;
	//unsigned long offset;
public:
	CDBDump();
	virtual ~CDBDump(){};
	bool Open(char* aFilePath);	//�򿪰��ļ�
	bool Close();								//������ļ�
	bool AddData(char* key, const size_t keysize, char *aData, const size_t size);		//�������

};

class CDBLoad
{
private:
	char m_FilePath[MAX_PATH];							//�ļ���
	FILE* m_output;
	bool m_IsOpened;
	//unsigned long offset;
public:
	CDBLoad();
	virtual ~CDBLoad(){};
	bool Open(char* aFilePath);	//�򿪰��ļ�
	bool Close();								//�رհ��ļ�
	bool ReadData(char* key, size_t& keysize, char *aData, size_t& size);		//��ȡ����
	void GotoHead();					//���ļ�ָ���ƶ����ļ�ͷ
	bool SearchData(char* key, char *aData, size_t& size);		//��������
};
#endif // !defined(AFX_DBDUMPLOAD_H__956A6AC3_33CD_44D2_B4B2_771DE290B32F__INCLUDED_)

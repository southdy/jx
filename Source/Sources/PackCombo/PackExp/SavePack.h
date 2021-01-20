// SavePack.h: interface for the CSavePack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEPACK_H__2776CBC0_DFB3_4814_A570_D2FC339E0D05__INCLUDED_)
#define AFX_SAVEPACK_H__2776CBC0_DFB3_4814_A570_D2FC339E0D05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZPackFile.h"

#define MAX_FILE					2004800			//���20����ļ�

class CSavePack  
{
private:
	char FileName[256];							//�ļ���
	index_info* index_list;						//�ļ�����
	z_pack_header header;						//���ļ�ͷ��
	FILE* output;
	unsigned long offset;
public:
	CSavePack();
	virtual ~CSavePack(){};
	bool open(char* FileName, index_info* index);	//�򿪰��ļ�
	bool Close();										//������ļ�
	bool AddData(char* data, index_info* index, char *temp_buffer);		//����ļ�
};

#endif // !defined(AFX_SAVEPACK_H__2776CBC0_DFB3_4814_A570_D2FC339E0D05__INCLUDED_)

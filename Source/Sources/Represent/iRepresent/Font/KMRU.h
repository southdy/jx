/*******************************************************************************
// FileName			:	KMRU.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-12 17:42:49
// FileDescription	:	���ʹ�ñ���
// Revision Count	:
*******************************************************************************/
#pragma once

//MRU�����
#pragma pack(1)
typedef struct _KMruItem
{
	unsigned short	Id;			//��id
	unsigned short	Data;		//������
	unsigned char	MRURecord;	//ʹ�ü�¼
	unsigned char	Reserved;	//������δ�õ�
    unsigned short  Reserved1;
}KMruItem;
#pragma pack()

class KMRU  
{
public:
	//�ύ�ַ������õ�����ȫ���ַ�����ͼ�е�λ��
	void Commit(unsigned short* pString, int nCount, unsigned short* pPositions, unsigned short* pNewItems, int& nNumNewItem);
	void Terminate();				//�������������
	bool Init(int nMaxItemNum);		//��ʼ��
	
	KMRU();
	virtual ~KMRU();

private:
	//�����¼������ڱ��е�λ�ã�ʹ���е������Id����
	void AdjustCharPos(unsigned int nIndex,  unsigned int nDueIndex);
	void Update();								//ִ��һ�ֽ���ʱMRU�����Ϣ����
	unsigned int GetMostLongestUnusedItem();	//�ڱ��������û�б�ʹ����
	unsigned short GetItemData(int nIndex);		//��ñ���ָ�������������
	//�ڸ���Χ�ڶ�λһ���ַ���û�ҵ���������������Ӧ���õ�λ�ã�
	bool GetCharPos(unsigned short Id, int nFrom, int nCount, unsigned int& nPosition);
private:
	KMruItem*	m_pItemTable;		//���
	int			m_nMaxItemNum;		//�����������ɵ������Ŀ
	int			m_nItemCount;		//��ǰ���а����������Ŀ
	int			m_nCountInTurn;		//���������õĲ�ͬ�����Ŀ
};

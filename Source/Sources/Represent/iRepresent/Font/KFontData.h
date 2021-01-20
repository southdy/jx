/*******************************************************************************
// FileName			:	KFontData.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2002-9-4
// FileDescription	:	����ͼ��������ͷ�ļ�
// Revision Count	:	
*******************************************************************************/
#pragma once

//#define KFR_MAX_NUM_CHAR	23940		// For GBK (0xFE-0x80)*(0xFE-0x3F-1)=23940
/*#ifdef CHARASET_GB2132
	#define KFR_MAX_NUM_CHAR	94 * 94		// for GB2132 = 35 KB
#endif
#ifdef CHARASET_BIG5
	#define KFR_MAX_NUM_CHAR	94 * 190	// for BIG5 = 70 KB
#endif*/

//�ֿ��ļ��ļ�ͷ�ṹ
typedef struct _KFontHead
{
	char			Id[4];		// ��ʶ
	unsigned int	Size;		// ��С
	unsigned int	Count;		// ����
	unsigned short	Width;		// ���
	unsigned short	Height;		// �߶�
}KFontHead;

//����ͼ����Դ��
class KFontData
{
public:
	//�����ֿ��ļ�
	bool			Load(const char* pszFontFile);
	//�������������
	void			Terminate();
	//�õ�����Ŀ�ȸ߶�
	void			GetInfo(int& nWidth, int& nHeight) const;
	//ȡ�õ����ַ���������
	unsigned char*	GetCharacterData(unsigned char cFirst, unsigned char cNext) const;
	
	KFontData();
	~KFontData();

protected:
	unsigned int*	m_pdwOffs;					//ƫ�Ʊ�
	unsigned char*	m_pFontData;				//�ַ�������Ϣ����
	unsigned int	m_dwDataSize;				//�ַ�������Ϣ���ݴ洢�Ĵ�С
	unsigned int	m_uCharCount;				//�����ַ�����
	int				m_nFontW;					//�ַ���
	int				m_nFontH;					//�ַ���
};


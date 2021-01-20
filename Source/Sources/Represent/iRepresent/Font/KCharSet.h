/*******************************************************************************
// FileName			:	KCharSet.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-13 10:05:57
// FileDescription	:	�ַ�����
// Revision Count	:	2002-9-4��д����Ϊ����d3d��Wooy
*******************************************************************************/
#pragma once

#include "KFontRes.h"
#include "KMru.h"

//�������ֵ������ζ���ṹ
struct	KFontVertex
{
	float			x, y, z, w;
	unsigned    	color;
    float		 	u, v;
	//unsigned        uReserve;   // for Align
};

//һ�ο����ύ��KCharSet���ַ��������Ŀ
#define	KCS_CHAR_NUM_LIMIT	64

class KCharSet
{
public:
	//��ʼ��
	bool	Init(const char* pszFontFile, LPDIRECT3DDEVICE9 pd3dDevice);
	//�������������
	void	Terminate();
	//�ύ�ַ��������㴮�и��ַ����õ�������ͼ���꣬���������ڡ�
	void	CommitText(unsigned short* pString, int nNumChars, KFontVertex* pCharPolys);
	//��ȡ������Ϣ
	void	GetInfo(int& nFontW, int& nFontH);
	//�õ���ͼ���
	LPDIRECT3DTEXTURE9 GetTexture() const;
	
	KCharSet();
	virtual ~KCharSet();
	
private:
	int			m_nFontW;			//�������λ�����أ�
	int			m_nFontH;			//�����
	int			m_nNumCharH;		//������ͼ��һ���ַ�����Ŀ
	float		m_fFontW;			//��������ͼ�п���λ����ͼ�������꣩
	float		m_fFontH;			//��������ͼ�и�
	float		m_fCharHInterval;	//�ַ�����ͼ��ˮƽ�ࣨ�ַ�������м���������ַ�����ͼ��ˮƽ�������ܲ�ͬ��
	float		m_fCharVInterval;	//�ַ�����ͼ�д�ֱ��
	KFontRes	m_Resource;			//�ֿ���Դ
	KMRU		m_MruTable;			//�����ַ�MRU��
};

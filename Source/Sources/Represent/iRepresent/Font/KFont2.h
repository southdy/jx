/*****************************************************************************
					����Direct3D������ʾϵͳ֮����ӿ�

Ӧ�ã�
	����ϵͳ����ӿڣ�iFont��
	�ⲿ�ͻ�ֱ�����ô�ϵͳҪ����ͷ�ļ�iFont.h��

�ӿڹ��ܣ�
	������ʾ�ַ�����

ʹ�ã�
	�ӿ�ʹ��ǰ�ĳ�ʼ����
		ʹ�ýӿ�ǰ���ȵ�����ӿڷ���Init����ʼ��ʱ�ṩ��ͼ�豸�ӿڵ�
		ʵ����ָ�롣

����������Ϲ�ϵ��
	*����KEngine��KCanvas*

���ػ���
	�ı�KFR_MAX_NUM_CHARֵ�Ķ��壬����ӦĿ�������ַ������ַ���Ŀ��

    ���Ҫͬʱ���ڶ���ַ��������Կ��ǰ�KFR_MAX_NUM_CHAR�ĳ�KFontRes�����ݳ�Ա��
	�������ֿ��ļ�ʱ��̬ȷ����ͬʱ��̬����KFontRes::m_dwOffs�Ŀռ䣬������һЩ����Ķ���

	���Ŀ���ַ���Ҫ������ַ���Ŀ���٣����Ե�СDESIRE_TEXTURE_SIDE_WIDTHֵ�Ķ��塣

	��ϵͳ��Ҫ�����ṩ�˻�����ƣ������ַ���Ŀ�ܴ���ַ�����ֻ����ͼ��ά��һЩ
	�����ַ�����ͼ�����Ŀ�������ַ������ַ���Ŀ���٣�����Ҫ������ƣ���������
	��ϵͳ�����ص�Ļ������Կ��Ǹ���KCharSet����KMRU���������

	��ֻ֧�����ģ�˫�ֽڱ�������ֶ�֧�֡�
	���ֽڱ��������Ҳ���Թ̶��Ѹ��ֽ����㣬��չ��˫�ֽ�ʹ�ô�ϵͳ��
	Ҳ���Ը����֧�ֵ��ֽڻ������ֽڱ�������֣��ı����ַ��������Լ��ַ�����
	������Ŀ��صĽṹ���ݡ��ӿڲ����������Լ������ǵ���ز�����

	KFont2::TextOut�С��ַ����ж��봦���Լ�KFontRes�Ķ���ӿڷ���Update�����
	������صģ�Ҫ���ݱ������ֵı�������Ӧ���޸ġ�

�ļ���
	��ϵͳ��ǰ��������ͷ�ļ���
		KFont2.h, KFontRes.h
	��������ʵ�ִ����ļ���
		KFont2.cpp, KFontRes.cpp

*******************************************************************************/

/*******************************************************************************
// FileName			:	KFont2.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2002-9-5
// FileDescription	:	������
// Revision Count	:	
*******************************************************************************/
#pragma once
#include "KFontData.h"
#include "FontDef.h"
#include "iFont.h"

class KFont2 : public iFont
{
public:
	//--------iKFont�ӿڷ���---------
	//��ʼ��
	bool	Init(void* pDrawDevice);
	//�����ֿ�
	bool	Load(const char* pszFontFile);
	//�������������
	void	Terminate();
	//���û���ʱ�ַ���Ե����ɫ����alphaΪ0��ʾ�ַ���Ե����������
	void	SetBorderColor(unsigned int uColor);
	//��ȡ�����С��Ϣ
	void	GetFontSize(int* pWidth, int* pHeight);
	//�����ַ����ƴ�С
	void	SetOutputSize(int nOutputWith, int nOutputHeight);
	//��ʾ�ַ���
	void	OutputText(const char* pszText, int nCount = KRF_ZERO_END,
					int nX = KRF_FOLLOW, int nY = KRF_FOLLOW,
					unsigned int nColor = 0xff000000, int nLineWidth = 0);
	//��¡һ���ӿڶ���ָ��
	iFont* Clone();
	//�ͷŽӿڶ���
	void Release();
	//���캯��
	KFont2();
private:
	~KFont2();
	//�����ַ�
	void	DrawCharacter(int x, int y, unsigned char cFirst, unsigned char cNext, int nColor) const;

private:
	bool				m_bLoaded;			//�Ƿ��Ѿ������ֿ�
	unsigned short		m_nRefCount;		//���ü���
	KFontData			m_Resources;		//������Դ
	int					m_nFontWidth;		//����Ŀ�
	int					m_nFontHeight;		//����ĸ�
	int					m_nFontHalfWidth[2];//����İ��
	int					m_nOutputWidth;		//������
	int					m_nOutputHeight;	//����߶�

	int					m_nLastPosH;		//�ϴ����ֻ��ƺ�ĺ�������ˮƽֵ
	int					m_nLastPosV;		//�ϴ����ֻ��ƺ�ĺ������괹ֱֵ

	unsigned short		m_nBorderColor;
	short				m_nDrawBorderWithDeffColor;

	void*				m_pDevice;			//�����豸ָ��
};

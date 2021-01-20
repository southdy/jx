/*****************************************************************************
					����Direct3D������ʾϵͳ֮����ӿ�

Ӧ�ã�
	����ϵͳ����ӿڣ�iFont��
	�ⲿ�ͻ�ֱ�����ô�ϵͳҪ����ͷ�ļ�iFont.h��

�ӿڹ��ܣ�
	������ʾ�ַ�����

ʹ�ã�
	�ӿ�ʹ��ǰ�ĳ�ʼ����
		ʹ�ýӿ�ǰ���ȵ�����ӿڷ���Init����ʼ��ʱ�ṩDirect3D Device�ӿڵ�
		ʵ����ָ�롣����ʱ���������Direct3D Device�ѳ�ʼ��֮��
		��ʼ����ʹ�Ѿ������ֿ����ݶ�ʧ��

����������Ϲ�ϵ��
	*����M$ DirectX 8.0*

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

	KFont3::TextOut�С��ַ����ж��봦���Լ�KFontRes�Ķ���ӿڷ���Update�����
	������صģ�Ҫ���ݱ������ֵı�������Ӧ���޸ġ�

�ļ���
	��ϵͳ��ǰ��������ͷ�ļ���
		KFont3.h, KFontRes.h, KMRU.h, KCharSet.h
	��������ʵ�ִ����ļ���
		KFont3.cpp, KFontRes.cpp, KMRU.cpp, KCharSet.cpp

*******************************************************************************/


/*******************************************************************************
// FileName			:	KFont.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-13 10:18:42
// FileDescription	:	������
// Revision Count	:	2002-9-4��д����Ϊ����d3d��ȥ���ı����Ʊ�ǵ��жϡ�wooy
*******************************************************************************/
#pragma once
#include "KCharSet.h"
#include "FontDef.h"
#include "iFont.h"

typedef void (*fnRenderText)(KFontVertex* pPolyVertices, unsigned int nNumPolys);

class KFont3 : public iFont
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
	//ʹDevice״̬�л�����Ӱ��Ļ�ͼ�豸�������ʧЧ
	void	InvalidateDeviceObjects();
	//��D3d Device��lost state�ָ�Ϊoperational state��KFont��d3d Device������ݵĻָ�������
	bool	RestoreDeviceObjects();
	//���캯��
	KFont3();
private:
	~KFont3();
	//�����ַ���
	static fnRenderText	ms_RenderText;	//ָ��ǰ������Ϊ������ָ��
	static void	RenderTextDirect(KFontVertex* pPolyVertices, unsigned int nNumPolys);
	static void	RenderTextCmp(KFontVertex* pPolyVertices, unsigned int nNumPolys);
	static void	RenderTextCmpReverse(KFontVertex* pPolyVertices, unsigned int nNumPolys);

	static void	CheckAlphaCmpCaps();//����ͼ���豸��֧�ֵ�alpha��ⷽ��

private:
	bool				m_bLoaded;			//�Ƿ��Ѿ������ֿ�
	unsigned short		m_nRefCount;		//���ü���
	KCharSet			m_Resources;		//������Դ
	float				m_fFontWidth;		//����Ŀ�
	float				m_fFontHeight;		//����ĸ�
	int					m_nFontHalfWidth[2];//����İ��
	int					m_nOutputWidth;		//������
	int					m_nOutputHeight;	//����߶�

	static unsigned int	ms_uBorderColor;

	LPDIRECT3DTEXTURE9	m_pTexture;			//������ͼ���
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockSaved;	//���ֻ���ǰD3d device��״̬��¼
    LPDIRECT3DSTATEBLOCK9 m_pStateBlockDrawText;//���ֻ���ʱD3d device��״̬��¼
	int					m_nLastPosH;		//�ϴ����ֻ��ƺ�ĺ�������ˮƽֵ
	int					m_nLastPosV;		//�ϴ����ֻ��ƺ�ĺ������괹ֱֵ

private:
	static LPDIRECT3DDEVICE9	ms_pd3dDevice;		//Direct 3d device�ӿڵ�ʵ����ָ��
	static unsigned int	ms_AlphaCmpMethod;	//������ͼ���õ�alpha�жϷ���
	static unsigned int	ms_AlphaRef1, ms_AlphaRef2;	//alpha�ж�ʱ���õ���alphaֵ
};
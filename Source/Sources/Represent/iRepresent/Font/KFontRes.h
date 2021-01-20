/*******************************************************************************
// FileName			:	KFontRes.h
// FileAuthor		:	Wooy
// FileCreateDate	:	2001-9-12 9:51:04
// FileDescription	:	3D���Ʒ�ʽ����ͼ����Դ��ͷ�ļ�
// Revision Count	:	2002-9-4��д����Ϊ����d3d��Wooy
*******************************************************************************/
#pragma once
#include <D3d9.h>
#include "KFontData.h"

//3D���Ʒ�ʽ����ͼ����Դ��
class KFontRes
{
public:
	//����/�������ֱ�ԵЧ��
	static void EnableTextBorder(bool bEnable);
	//��ʼ��
	bool Init(const char* pszFontFile, LPDIRECT3DDEVICE9 pd3dDevice);
	//�������������
	void Terminate();
	//�õ�����Ŀ�ȸ߶ȣ���ͼ���Ի�����ַ��ĸ���
	void GetInfo(int& nWidth, int& nHeight, int& nHCount, int& nCount) const;
	//�õ��ַ�����ͼ��Ŀ�ȸ߶ȣ�ˮƽ�������ͼ���Ի�����ַ��ĸ���
	void GetInfo(float& fWidth, float& fHeight, float& fHInterval, float& fVInterval) const;
	//���»����ַ�
	void Update(unsigned short* pUpdates, int nCount);
	
	//!�õ���ͼ���
	LPDIRECT3DTEXTURE9 GetTexture()	const {	return m_pCharTexture;	}

	KFontRes();
	virtual ~KFontRes();

private:
	//���µ����ַ��ĵ�������
	void Update(unsigned char* pCharImage, unsigned char* pTexData, int nTexPitch);
		
private:
	KFontData		m_FontData;					//�ֿ���Դ
	int				m_nFontW;					//�ַ���
	int				m_nFontH;					//�ַ���
	int				m_nNumCountH;				//��ͼ��һ�д洢�ַ��ĸ���
	int				m_nMaxCount;				//��ͼ����������ɵ��ַ�����	

	unsigned int		m_nTextureSideWidth;	//������ͼ�ı߳�
	LPDIRECT3DDEVICE9	m_pd3dDevice;			//Direct 3d device�ӿڵ�ʵ����ָ��
	LPDIRECT3DTEXTURE9	m_pCharTexture;			//��ͼ���	

	static	int		m_bEnableTextBorder;
};

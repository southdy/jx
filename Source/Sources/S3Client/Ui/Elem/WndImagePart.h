/*****************************************************************************************
//	���洰����ϵ�ṹ--��ʾͼ�ξֲ��Ĵ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
------------------------------------------------------------------------------------------
	��ʾ(spr)ͼ�εĴ��ڡ�
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "UiImage.h"

class KWndImagePart : public KWndWindow
{
public:
	KUiImagePartRef	m_Image;
protected:
	void			Clone(KWndImagePart* pCopy);
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	virtual void	PaintWindow();								//�������
	void			SetPart(int nPartValue, int nFullValue);	//���ݲ���ֵ��ȫֵ�ı�����ϵ���û���ͼ���Ĳ�������
	KWndImagePart();
};
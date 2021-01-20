/*****************************************************************************************
//	���洰����ϵ�ṹ--ͼ�δ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
------------------------------------------------------------------------------------------
	��ʾ(spr)ͼ�εĴ��ڡ�
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "UiImage.h"

#define	WNDIMG_ES_EXCLUDE_TRANS		0x0001		//��ͼ�ε���״����Ϊ���ڵ���״

class KWndImage : public KWndWindow
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual int		PtInWindow(int x, int y);				//�ж�һ�����Ƿ��ڴ��ڷ�Χ��,������Ǿ�������
	virtual void	PaintWindow();							//�������
	void			Clone(KWndImage* pCopy);
	
	void	SetFrame(int nFrame);							//����ͼ��֡
	int		NextFrame();									//ͼ�λ�֡
	void	SetImage(short nType, const char* pszImgName, bool bAdjustWndSize = false);
	void	UpdateTimer();
	KWndImage();
protected:
	KUiImageRef		m_Image;
};
/*****************************************************************************************
//	���洰����ϵ�ṹ--λͼ����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
------------------------------------------------------------------------------------------
	��ʾ(Jpg)λͼ�Ĵ��ڡ�
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "KBitmap16.h"

class KWndBitmap : public KWndWindow
{
private:
	KBitmap16		m_Bitmap;			//λͼ��Դ
	BOOL			m_bBitmapLoaded;	//�Ƿ���λͼ������
public:
	KWndBitmap();
	virtual			~KWndBitmap();
	virtual BOOL	Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual void	PaintWindow();							//���ƴ���	
};
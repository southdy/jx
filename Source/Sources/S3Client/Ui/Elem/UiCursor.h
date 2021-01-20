/*****************************************************************************************
//	���ָ�봰��
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-17
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "UiImage.h"

#define KUI_USE_HARDWARE_MOUSE  1
//#undef  KUI_USE_HARDWARE_MOUSE

#define	MAX_CURSOR_IMAGE	16

class KUiCursor
{
public:
	void	Show(int bShow);								//�������ָ�����ʾ״̬
	void	SetPosition(int h, int v);						//�������ָ��λ��
	void	GetPosition(int& h, int& v);					//��ȡ���ָ��λ��
	void	SetImage(int nIndex, const char* pImgFile);		//�������ָ��ͼ��
	int		SwitchImage(int nIndex);						//�л���ǰ���ָ��ͼ��
	void	RestoreCursor();	        					//���¼������
	void	Paint();										//�������ָ��
	void	Cleanup();										//���ȫ����Դ
	KUiCursor();
	~KUiCursor();
private:
	int			m_bShow;
	int			m_nCurImage;
	int			m_nPositionH;
	int			m_nPositionV;

    #ifdef KUI_USE_HARDWARE_MOUSE
	    HCURSOR     m_CursorImages[MAX_CURSOR_IMAGE];
    #else // KUI_USE_HARDWARE_MOUSE
		KUiImageRef	m_CursorImages[MAX_CURSOR_IMAGE];
		int			m_nHotspotH;
		int			m_nHotspotV;
    #endif
};

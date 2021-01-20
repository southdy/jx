/*****************************************************************************************
//	���洰����ϵ�ṹ--����������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-22
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "WndButton.h"

#define WNDSCROLL_ES_VERTICAL		0x0001		//��ֱ�͵Ĺ�����
#define	WNDSCROLL_F_DRAGGING_SLIDE	0x0100		//�������Ļ������ڱ��϶�

class KWndScrollBar : public KWndImage
{
private:
	unsigned int	m_Flag;
	int			m_nMinValue;	//��Сֵ
	int			m_nMaxValue;	//���ֵ
	int			m_nCurValue;	//��ǰֵ
//	int			m_nLineSize;	//һ�д�С
	int			m_nPageSize;	//һҳ��С
	int			m_nMinPosition;	//�������ʼλ��
	int			m_nMaxPosition;	//�������ֹλ��
	KWndButton	m_SlideBtn;		//������ť
	int			m_nImgRange;	//�ظ���ͼʱ����ͼ�Ŀ�Ȼ�߶�

public:
	KWndScrollBar();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);	//��ʼ��
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual void	PaintWindow();							//�������
	void			SetSize(int nWidth, int nHeight);		//���ô��ڴ�С

	int				ScrollLine(bool bPre);
	int				ScrollPage(bool bPre);
	void			SetScrollPos(int nPosition);				//����λ��
	int				GetScrollPos() { return m_nCurValue; }		//��ȡλ��
	void			SetValueRange(int nMinValue, int nMaxValue);	//����ȡֵ��Χ
	int				GetMaxValue() { return m_nMaxValue;}	//ȡֵ��Χ
	int				GetMinValue() { return m_nMinValue;}	//ȡֵ��Χ
	void			Clone(KWndScrollBar* pCopy);
	int				GetMinHeight();
private:
	void			OnLButtonDown(int x,int y);					//��Ӧ����������
	void			OnSlideBtnPressed();						//��Ӧ������ť������
	void			OnDraggingSlide(int x, int y);				//�����϶�������ť
	void			SetSlideBtnPos();							//���û����鴰��λ��
};

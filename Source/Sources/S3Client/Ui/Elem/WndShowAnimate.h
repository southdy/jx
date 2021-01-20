/*****************************************************************************************
//	���洰����ϵ�ṹ--�л���ʾ״̬ʱ���ƶ����Ƶ�ͼ�εĴ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-17
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndMovingImage.h"

class KWndShowAnimate : public KWndMovingImage
{
public:
	virtual int	Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual void Show();
	virtual void Hide();
    virtual void ShowCompleted();
	static void	SetMoveSpeed(int nMoveSpeed, int nFullRange);
	static void SetInterval(unsigned int uInterval);
	KWndShowAnimate();
	void	Clone(KWndShowAnimate* pCopy);
protected:
	void	PaintWindow();
private:
	static	int	ms_nMoveSpeed;
	static	int	ms_nFullRange;
	int		m_nCurrentSpeed;
	SIZE	m_AppearRange;
	SIZE	m_DisappearRange;
	static	unsigned int ms_uMoveTimeInterval;
	unsigned int	m_uMoveLastTime;		//��ǰ֡�л�������ʱ��
};

// -------------------------------------------------------------------------
//	�ļ���		��	UiReconnect.h
//	������		��	Freeway Chen
//	����ʱ��	��	2003-7-18
//	��������	��	�Զ����������̿���	
//
// -------------------------------------------------------------------------
#ifndef __UIRECONNECT_H__
#define __UIRECONNECT_H__

#include "..\Elem\WndWindow.h"

class KReconnectWnd : private KWndWindow
{
public:
	static bool LaunchReconnect(int nMaxTimes = 12);
	static bool	IsReconnecttingGoingOn();
	static void Exit(bool bQuitGame);

private:
	KReconnectWnd()  {}
	~KReconnectWnd() {}

	virtual int	    WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	virtual void	Breathe();													//���ڵĳ�����Ϊ
	void			FirstReconnect();
	bool			StartReconnect();

private:
	static KReconnectWnd*	m_pSelf;
	unsigned int			m_uWaitStartTime;
	unsigned int			m_uToWaitTime;
	short					m_nReconnectTimes;
	short					m_nMaxReconnectTimes;
	int						m_nHideTimes;
	bool					m_bWaitToReconnect;
	bool					m_bStop;
};

#endif // __UIRECONNECT_H__
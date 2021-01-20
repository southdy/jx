// -------------------------------------------------------------------------
//	�ļ���		��	UiGetMoney.h
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-1-7
//	��������	��	ȡǮ����
// -------------------------------------------------------------------------
#ifndef __UiGetMoney_H__
#define __UiGetMoney_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"


class KUiGetMoney : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiGetMoney*	OpenWindow(int nMoney, int nMaxMoney, KWndWindow* pRequester,
							 unsigned int uParam, KWndWindow* pMoneyWnd);	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiGetMoney*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//������淽��
	static void			CloseWindow(bool bDestroy);		//�رմ���
private:
	KUiGetMoney();
	~KUiGetMoney() {}
	int		Initialize();								//��ʼ��
	void	Show(KWndWindow* pMoneyWnd);
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
	void	OnCheckInput();
private:
	static KUiGetMoney*	m_pSelf;
	KWndEdit32			m_Money;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	int					m_nMaxMoney;
	KWndWindow*			m_pRequester;
	unsigned int		m_uRequesterParam;
};


#endif // __UiGetMoney_H__
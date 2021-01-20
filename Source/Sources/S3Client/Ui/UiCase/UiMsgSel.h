// -------------------------------------------------------------------------
//	�ļ���		��	UiMsgSel.h
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-1-6
//	��������	��	������������Ϣѡ��
// -------------------------------------------------------------------------
#ifndef __UiMsgSel_H__
#define __UiMsgSel_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;

class KUiMsgSel : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiMsgSel*	OpenWindow(KUiQuestionAndAnswer* pContent);		//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiMsgSel*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//������淽��
	static void			CloseWindow(bool bDestroy);		//�رմ���
private:
	KUiMsgSel() {}
	~KUiMsgSel() {}
	void	Show(KUiQuestionAndAnswer* pContent);
	int		Initialize();								//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);			//��Ӧ�����Ϣ
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();

private:
	static KUiMsgSel*	m_pSelf;
	KScrollMessageListBox	m_MsgScrollList;	//��ѡ���ֺ͹�����
	KWndText512			m_InfoText;	//˵������

	//Ϊ�Զ������ӵı���
	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;
};


#endif // __UiMsgSel_H__
// -------------------------------------------------------------------------
//	�ļ���		��	UiSelDlg.h
//	������		��	����
//	����ʱ��	��	2002-9-10 11:25:36
//	��������	��	ѡ��Ի���	
//
// -------------------------------------------------------------------------
#ifndef __UiSelDlg_H__
#define __UiSelDlg_H__

#include "../Elem/WndLabeledButton.h"
#include "../../../core/src/GameDataDef.h"

#define KUISEL_MAX_OPTION	4
class KUiSelDlg : protected KWndImage
{
public:
	int		Initialize();						//��ʼ��
	void	Show(KUiQuestionAndAnswer* pContent);//��ʾ����
	void	LoadScheme(const char* pScheme);	//������淽��

private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickButton(KWndWindow* pWnd);	//��Ӧ�����ť

private:
	KWndRichLabeledButton m_OptionBtn[KUISEL_MAX_OPTION];//ѡ�ť
	KWndText256	m_IlluminateTxt;						//˵������
};

extern KUiSelDlg	g_UiSelDlg;

#endif // __UiSelDlg_H__
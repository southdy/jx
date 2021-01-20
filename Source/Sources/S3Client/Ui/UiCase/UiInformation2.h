/*****************************************************************************************
//	����--��Ϣ����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-14
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndText.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndShowAnimate.h"

struct KUiInformationParam;

class KUiInformation2 : protected KWndShowAnimate
{
public:
	void	Initialize();							//��ʼ��
	void	LoadScheme(const char* pScheme);		//������淽��
	void	Show(const char* pInformation, int nInforLen, const char* pBtnLabel,
				KWndWindow* pCallerWnd = 0, unsigned int uParam = 0);//��ʾ����

	void	SpeakWords(KUiInformationParam* pWordDataList, int nCount);	//��ʾ�Ի�����
	KWndWindow* TopChildFromPoint(int x, int y) {
		return this;
	}
	void	Hide();									//���ش���
	void	Close();
	KUiInformation2();
	~KUiInformation2();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
private:
	KWndText256			m_Information;
	KWndPureTextBtn		m_OKBtn;
	KWndWindow*			m_pCallerWnd;
	unsigned int		m_uCallerParam;

	KUiInformationParam*	m_pWordDataList;
	int						m_nNumWordData;
	int						m_nCurrentWord;
};

void UIMessageBox2(const char* pMsg, int nMsgLen = -1, const char* pBtnLabel = 0,
				   KWndWindow* pCaller = 0, unsigned int uParam = 0);

extern KUiInformation2	g_UiInformation2;
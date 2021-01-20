/*******************************************************************************
File        : UiPopupPasswordQuery.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-30-2003(mm-dd-yyyy)
Description : ѯ�����봰��
********************************************************************************/

#if !defined(AFX_UIPOPUPPASSWORDQUERY_H__136E1BB1_F7B4_425B_927A_E7F805EE7E71__INCLUDED_)
#define AFX_UIPOPUPPASSWORDQUERY_H__136E1BB1_F7B4_425B_927A_E7F805EE7E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
                    .-.
          .-._    _.../   `,    _.-.
          |   `'-'    \     \_'`   |
          \            '.__,/ `\_.--,
           /                '._/     |
          /                    '.    /
         ;   _                  _'--;
      '--|- (_)       __       (_) -|--'
      .--|-          (__)          -|--.
       .-\-                        -/-.
      '   '.                      .'   `
            '-._              _.-'
                `""--....--""`

*/

#include "../elem/wndbutton.h"
#include "../elem/wndimage.h"
#include "../elem/wndedit.h"

class KUiPopupPasswordQuery : KWndImage
{
public:
	KUiPopupPasswordQuery();
	virtual ~KUiPopupPasswordQuery();

	static        KUiPopupPasswordQuery* OpenWindow(    //�򿪴���
                                    KWndWindow *pCaller, unsigned int uParam);

	static        KUiPopupPasswordQuery* GetIfVisible();//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = FALSE);   //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);      //������淽��

private:
	static KUiPopupPasswordQuery* ms_pSelf;

private:
	void          Initialize();                         //��ʼ��

	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	void          OnConfirm();

private:
	KWndEdit32             m_Password;                  //��������ı༭�ؼ�
	KWndButton             m_Confirm;                   //ȷ�ϰ�ť
	KWndWindow            *m_pCaller;                   //����������ڵĴ��ڵ�ָ��

private:
	unsigned int           m_uParam;                    //��Ĵ��ڵ����������ʱ����
	                                                    //�Ĳ�����ͨ������ʶ�𷵻ؽ�
	                                                    //��ʱ�Ĵ�����
};

#endif // !defined(AFX_UIPOPUPPASSWORDQUERY_H__136E1BB1_F7B4_425B_927A_E7F805EE7E71__INCLUDED_)

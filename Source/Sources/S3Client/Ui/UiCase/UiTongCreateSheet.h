/*******************************************************************************
File        : UiTongCreateSheet.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : �������ı�
********************************************************************************/


#if !defined(AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
#define AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_

/*---------------------------*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*------------------------------------------*/
#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"

/*---------------------------------------------------------*/
class KUiTongCreateSheet : KWndImage
{
public:
	KUiTongCreateSheet();
	virtual ~KUiTongCreateSheet();

	static        KUiTongCreateSheet* OpenWindow();  //�򿪴���
	static        KUiTongCreateSheet* GetIfVisible();//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE); //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);   //������淽��

public:

private:
	static        KUiTongCreateSheet *ms_pSelf;

private:
	void          Initialize();               //��ʼ��
	                      /*------------------------------------------------*/
	                                          //���ں���
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	void          AlignmentButtonCheck(       //����аCheckBox�Ĺ�����
		                               NPCCAMP eSide);

	void          OnDone();                   //��Ӧȷ����ť������

private:
	KWndText32    m_TextError;                //������ȷ����ť��ʱ����Щ�ط�û���

	KWndEdit32    m_EditTongName;             //��������
	KWndButton    m_BtnOrder, m_BtnNatural;   //����а(��DnD�Ļ����棬Order��������
	KWndButton    m_BtnChaos;                 //       Natural����������Chaos�������)

    KWndLabeledButton
		          m_BtnDone, m_BtnCancel;     //ȷ��/ȡ����ť

	int           m_nSelectSide;              //ѡ������һ����Ӫ
	char          m_szNameNullString[32];     //û�������ֵ���ʾ�ַ���
	char          m_szAlignmentNullString[32];//û��ѡ��Ӫ����ʾ�ַ���
};


#endif // !defined(AFX_KUITONGCREATESHEET_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)

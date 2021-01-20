/*****************************************************************************************
//	���洰����ϵ�ṹ--ҳ�洰��\���ҳ�漯�ϴ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-8
------------------------------------------------------------------------------------------
    ������M$ Win32����� PropertiePage
*****************************************************************************************/
#pragma once
#include "WndImage.h"
#include "WndShowAnimate.h"

class KWndPage : public KWndImage
{
public:
	virtual int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
};

//------------------------------------------------------------------------------------------
//	���ҳ�漯�ϴ���������M$ Win32����� PropertieSheet
//------------------------------------------------------------------------------------------

class KWndButton;

class KWndPageSet : public KWndShowAnimate
{
public:
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	bool			AddPage(KWndPage* pPage, KWndButton* pPageBtn);	//���ҳ��
	bool			ActivePage(int nPageIndex);						//����ָ����ҳ��
	int				GetActivePageIndex() { return m_nAcitvePage; }	//ȡ�õ�ǰ����ҳ�������
	KWndPage*		GetActivePage();								//����ָ����ҳ��
	KWndPageSet();
	~KWndPageSet();

private:
	void			OnPageBtnClick(KWndWindow* pBtn);		//��Ӧ�����л�ҳ�水ť�Ĳ���

private:
	struct KWndPageBtnPair
	{
		KWndPage*	pPage;						//ҳ�洰��ָ��
		KWndButton*	pPageBtn;					//ҳ�洰�ڹ�����ťָ��
	}*				m_pPageBtnPairList;			//ָ��洢ȫ��ҳ�漰�������ťָ�������
	int				m_nNumPage;					//ҳ�����Ŀ
	int				m_nAcitvePage;				//��ǰ�����ҳ��
};
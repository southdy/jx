/*******************************************************************************
File        : UiTrembleItem.h
Creator     : Fyt(Fan Zhanpeng)
create data : 09-08-2003(mm-dd-yyyy)
Description : ���������Ľ���
********************************************************************************/

#if !defined(AFX_UITREMBLEITEM_H__69079BE2_10C3_4AA0_A3C2_190285964E36__INCLUDED_)
#define AFX_UITREMBLEITEM_H__69079BE2_10C3_4AA0_A3C2_190285964E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndobjcontainer.h"
#include "../elem/wndbutton.h"
#include "../elem/wndimage.h"

class KCanGetNumImage : public KWndImage
{
public:
	int GetMaxFrame();
	int GetCurrentFrame();
};

class KUiTrembleItem : public KWndImage
{
public:
	KUiTrembleItem();
	virtual ~KUiTrembleItem();

	static        KUiTrembleItem* OpenWindow();      //�򿪴���
	static        KUiTrembleItem* GetIfVisible();    //�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE); //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);   //������淽��

private:
	static        KUiTrembleItem *ms_pSelf;

private:
	void          Initialize();
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void  Breathe();

private:
	void          ApplyAssemble();                   //��Core���ʹ������������
	void          ResultReturn();                    //����������
	int           PlayEffect();                      //������Ч
	void          UpdateResult();                    //�ѽ�����µ�������

private:
	enum THIS_INTERFACE_STATUS
	{
		STATUS_WAITING_MATERIALS,
		STATUS_BEGIN_TREMBLE,
		STATUS_TREMBLING,
		STATUS_CHANGING_ITEM,
		STATUS_FINISH,
	};

private:
	KWndObjectBox m_GemSpirit;                       //������ʯ���
	KWndObjectBox m_GemWater;                        //ˮ�峺��ʯ���
	KWndObjectBox m_GemEarth;                        //�����籦ʯ���
	KWndObjectBox m_GemLevel;                        //�ȼ���ʯ���
	KWndObjectBox m_GemFire;                         //��Ծ����ʯ���
	KWndObjectBox m_GemGold;                         //������ʯ���
	KWndObjectBox m_GemWood;                         //ľ��ʵ��ʯ���
	KWndObjectBox m_Item;                            //ϴ������Ʒ

	KWndButton    m_Confirm;                         //�ϳɰ�ť
	KWndButton    m_Cancel;                          //�رհ�ť

	KCanGetNumImage
		          m_TrembleEffect;                   //�ϳ��е���Ч

private:
	int           m_nStatus;                         //��ǰ����������״̬
};

#endif // !defined(AFX_UITREMBLEITEM_H__69079BE2_10C3_4AA0_A3C2_190285964E36__INCLUDED_)

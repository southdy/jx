// -------------------------------------------------------------------------
//	�ļ���		��	UiTrade.h
//	������		��	����, Wooy(Wu yue)
//	����ʱ��	��	2002-9-16 11:57:03
//	��������	��	��ҽ���ϵͳ����
// -------------------------------------------------------------------------
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../elem/wndbutton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../elem/wndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

struct KUiObjAtRegion;

class KUiTrade : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiTrade*	OpenWindow(KUiPlayerItem* pOther);//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiTrade*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow();					//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void			LoadScheme(const char* pScheme);//������淽��

	void				OnChangedTakewithItem(KUiObjAtRegion* pObj, int bAdd);	//�Է����������Ʒ
	void				OnOppositeChangedItem(KUiObjAtRegion* pObj, int bAdd);	//�Է����������Ʒ
	void				OnSelfChangedItem(KUiObjAtRegion* pObj, int bAdd);		//�Լ����������Ʒ
	void				UpdateOperData();

	void				UpdateTradeEnd(int bTraded);	//���׽���

private:
	KUiTrade();
	~KUiTrade();
	void	Initialize();						//��ʼ��
	void	LoadScheme(KIniFile* pIni);			//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���

//	void	Clear();						//�������
	void	Breathe();
	void	OnTrade();						//��Ӧ�����ʼ����
	void	OnOk(int bChecked);				//��Ӧ����н�������
	void	OnCancelTrade();
	void	OnAdjustMoney(bool bAdd);
	void	OnAdjustedMoney();
	void	OnModififyMoney();
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	void	UpdateTakewithItems();

private:
	static KUiTrade*	m_pSelf;
private:
	KWndText32			m_TakewithMoney;	//����Ľ�Ǯ
	KWndObjectMatrix	m_TakewithItemsBox;	//�������Ʒ

	KWndObjectMatrix	m_SelfItemsBox;		//��Ʒ��
	KWndEdit32			m_SelfMoney;		//Ǯ��
	KWndButton			m_Ok;				//ȷ��
	KWndButton			m_Cancel;			//ȡ��
	KWndButton			m_Trade;			//ִ�н���
	KWndButton			m_AddMoney;			//����Ǯ
	KWndButton			m_ReduceMoney;		//����Ǯ
	int					m_nSelfTradeMoney;

	int					m_bAdjustingMoney;
	int					m_nTakewithMoney;

	KWndText32			m_OtherName;		//�Է�������
	KWndObjectMatrix	m_OtherItemsBox;	//��Ʒ��
	KWndText32			m_OtherMoney;		//Ǯ��
	KWndText32			m_Info;				//�Է�״̬ѶϢ
	KUiPlayerItem		m_OtherData;

	unsigned int		m_uWaitTradeMsgColor;
	unsigned int		m_uOtherLockMsgColor;
	unsigned int		m_uOtherNotLockMsgColor;
};

/*****************************************************************************************
//	����--״̬����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
	����11��װ���ڷ�λ�ã�����2��4���·�2��3��ͷ��2��2������2��1��
	����1��1��Ь��2��2����ָ1��1����׹1��2������1��1����2��3��
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShowAnimate.h"
#include "../elem/WndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

enum UI_PLAYER_ATTRIBUTE;
struct KUiObjAtRegion;

#define _ITEM_COUNT 11

class KUiParadeItem : protected KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiParadeItem*	OpenWindow(KUiPlayerItem* pDest);					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiParadeItem*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void			LoadScheme(const char* pScheme);//������淽��

	void	UpdateData(KUiPlayerItem* pDest);

private:
	KUiParadeItem() {}
	~KUiParadeItem() {}
	void	Initialize();								//��ʼ��	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	LoadScheme(class KIniFile* pIni);			//������淽��
	void	UpdateBaseData(KUiPlayerItem* pDest);							//���»������ݣ������Ȳ��ױ����ݣ�
	void	UpdateAllEquips(KUiPlayerItem* pDest);
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);	//װ���仯����
private:
	static KUiParadeItem*	m_pSelf;

private:
	KWndWindow	m_Face;
	KWndText32	m_Name;
	KWndText32	m_Title;	

	KWndButton	m_Close;

	//װ����Ʒ
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];

	KUiPlayerItem m_Dest;
};

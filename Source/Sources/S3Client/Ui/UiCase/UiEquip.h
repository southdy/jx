/***************************************************************************
//	����--װ������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue), ����
//	CreateTime:	2002-9-2
----------------------------------------------------------------------------
	װ������
		ѡ���װ��һ��װ����
		�ܹ�20��װ����λ,ÿ��ѡ�����10��
		��ť��һ���رհ�ť
			ͨ��ѡ��趨����װ������Ϸ�п���ͨ����ݼ������л�����װ������
		��10��װ���ڷ�λ�ã�����2��4���·�2��3��ͷ��2��2������2��1������1��1��
		Ь��2��2����ָ1��1����׹1��2������1��1��
*****************************************************************************/
#pragma once

#include "../elem/wndbutton.h"
#include "../elem/wndpage.h"
#include "../Elem/WndText.h"
#include "../elem/WndObjContainer.h"

#define _ITEM_COUNT 10

struct KUiObjAtRegion;

// -------------------------------------------------------------------------
// ---> װ������ҳ
class KUiEquipPage : public KWndPage
{
public:
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);	//װ���仯����
	void	UpdateData();									//��������
	void	Clear();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//��Ӧ�����������װ���ĸı�

private:
	// ����Ԫ��
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];
};

// ---> ��װ������ҳ
class KUiHorsePage : public KWndPage
{
public:
	void	Initialize();						//��ʼ��
	void	LoadScheme(const char* pScheme);	//������淽��
	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);	//װ���仯����
	void	Clear();
	void	UpdateRideStatus(int bRiding);	//��������״̬
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	int		OnClickItem(unsigned int uMsg, unsigned int uParam, int nParam);//��Ӧ�����ť
	void	OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//��Ӧ�����������װ���ĸı�
private:
	KWndObjectBox	m_HorseBox;
	KWndButton		m_UpDownHorseBtn;
	KWndText256		m_HorseDesc;
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ---> װ������
class KUiEquip : public KWndPageSet
{
public:
	static KUiEquip* OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiEquip* GetIfVisible();				//�������������ʾ���򷵻�ʵ��ָ��
	static void		 CloseWindow(bool bDestroy);	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void		LoadScheme(const char* pScheme);//������淽��
	static void		UpdateRideStatus(int bRiding);	//��������״̬
	void			UpdateEquip(KUiObjAtRegion* pEquipt, int bAdd);//װ���仯����
private:
	KUiEquip() {}
	~KUiEquip() {}
	void	Initialize();							//��ʼ��	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
private:
	static KUiEquip*	m_pSelf;
private:
	KUiEquipPage	m_EquipPage;	// װ��1����ҳ
	KUiHorsePage	m_HorsePage;	// װ��2����ҳ
	KWndButton		m_Equip1Btn;	// �л���װ��1
	KWndButton		m_Equip2Btn;	// �л���װ��2
	KWndButton		m_Close;		// �رհ�ť
};
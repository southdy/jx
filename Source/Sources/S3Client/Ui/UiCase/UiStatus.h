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

enum UI_PLAYER_ATTRIBUTE;
struct KUiPlayerRuntimeInfo;
struct KUiObjAtRegion;
struct KUiPlayerAttribute;

#define _ITEM_COUNT 11

class KUiStatus : public KWndShowAnimate
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiStatus*	OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiStatus*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//	static void			LoadConfig(KIniFile* pIni);		//�����Զ�������
	static void			LoadScheme(const char* pScheme);//������淽��

	void	UpdateBaseData();							//���»������ݣ������Ȳ��ױ����ݣ�
	void	UpdateData();
	void	UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo);
	void	UpdateAllEquips();
	void	UpdateRuntimeAttribute(KUiPlayerAttribute* pInfo);

	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);	//װ���仯����

private:
	KUiStatus() {}
	~KUiStatus() {}
	void	Initialize();								//��ʼ��	
	void	UseRemainPoint(UI_PLAYER_ATTRIBUTE type);	//����ĳ������
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//���ں���
	void	LoadScheme(class KIniFile* pIni);			//������淽��
	void	OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);//��Ӧ�����������װ���ĸı�
private:
	static KUiStatus*	m_pSelf;

private:
	KWndWindow	m_Face;
	KWndText32	m_Agname;
	KWndText32	m_Name;
	KWndText32	m_Title;
	
	KWndText32	m_Money;
	
	KWndText32	m_Life;
	KWndText32	m_Mana;
	KWndText32	m_Experience;
	KWndText32	m_Angry;
	KWndText32	m_Stamina;

	int			m_nRemainPoint;
	KWndText32	m_RemainPoint;

	KWndText32	m_Strength, m_Dexterity, m_Vitality, m_Energy;
	KWndButton	m_AddStrength, m_AddDexterity, m_AddVitality, m_AddEnergy;

	KWndText32	m_LeftDamage, m_RightDamage, m_Attack, m_Defence, m_MoveSpeed, m_AttackSpeed;
	KWndText32	m_PhyDef, m_CoolDef, m_LightDef, m_FireDef, m_PoisonDef;
	KWndText32	m_Level, m_StatusDesc;

	KWndButton	m_OpenItemPad;
	KWndButton	m_Close;

	//װ����Ʒ
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];

};

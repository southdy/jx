/*****************************************************************************************
//	����--���ܴ���
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-28
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/wndpage.h"
#include "../Elem/WndImagePart.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/WndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

#define LIVE_SKILL_COUNT			10
#define	FIGHT_SKILL_COUNT			50
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define	FIGHT_SKILL_SUB_PAGE_COUNT	FIGHT_SKILL_COUNT / FIGHT_SKILL_COUNT_PER_PAGE

class KUiFightSkillSubPage : public KWndPage
{
public:
	KUiFightSkillSubPage();
	void	Initialize(/*int nSubPageIndex*/);				//��ʼ��
	void	LoadScheme(const char* pScheme);			//������淽��
	void	UpdateRemainPoint(int nPoint);				//������������
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	//���µ�������
	void	UpdateData(KUiSkillData* pSkills);			//��������
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	PaintWindow();								//���ƴ���
private:
//	int			m_nSubPagIndex;
	int			m_nRemainSkillPoint;
	KWndObjectBox	m_FightSkills[FIGHT_SKILL_COUNT_PER_PAGE];
//	KWndButton	m_FightSkills[FIGHT_SKILL_COUNT_PER_PAGE];
//	KUiSkillData	m_Skills[FIGHT_SKILL_COUNT_PER_PAGE];
	struct
	{
		int		nFont;
		SIZE	Offset;
		unsigned int Color;
	}			m_SkillTextParam;
};

class KUiFightSkillSub : public KWndPageSet
{
public:	
	void	Initialize();								//��ʼ��
	void	LoadScheme(const char* pScheme);			//������淽��
	void	UpdateRemainPoint(int nPoint);				//������������
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	//���¼���
	void	UpdateData();								//��������
private:
	KUiFightSkillSubPage	m_SubPages[FIGHT_SKILL_SUB_PAGE_COUNT];
	KWndButton				m_SubPageBtn[FIGHT_SKILL_SUB_PAGE_COUNT];
};

class KUiFightSkill : public KWndPage
{
public:
	void	Initialize();								//��ʼ��
	void	LoadScheme(const char* pScheme);			//������淽��
	void	UpdateRemainPoint(int nPoint);				//������������
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	//���¼���
	void	UpdateData();								//��������	
private:
	KWndText32			m_RemainSkillPoint;
	KUiFightSkillSub	m_InternalPad;
};

class KUiLiveSkill : public KWndPage
{
public:
	KUiLiveSkill();
	void	Initialize();								//��ʼ��
	void	LoadScheme(const char* pScheme);			//������淽��
	void	UpdateBaseData();							//���¼��ܹ�������
	void	UpdateSkill(KUiSkillData* pSkill, int nIndex);	//���¼���
	void	UpdateData();								//��������
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	PaintWindow();								//���ƴ���
private:
	KWndText32		m_RemainSkillPoint;
	int				m_nRemainSkillPoint;

	KWndObjectBox	m_LiveSkill[LIVE_SKILL_COUNT];
//	KUiSkillData	m_Skills[LIVE_SKILL_COUNT];
	struct
	{
		int		nFont;
		SIZE	Offset;
		unsigned int Color;
	}			m_SkillTextParam;
};

class KUiSkills : public KWndPageSet
{
public:
	static KUiSkills* OpenWindow();							//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiSkills* GetIfVisible();						//�������������ʾ���򷵻�ʵ��ָ��
	static void		 CloseWindow(bool bDestroy);			//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void		 LoadScheme(const char* pScheme);		//������淽��
	static void		 UpdateSkill(KUiSkillData* pSkill, int nIndex);	//���¼���

	void	UpdateFightRemainPoint(int nPoint);				//����ս��������������
	void	UpdateLiveBaseData();							//��������ܹ�������

private:
	KUiSkills(){}
	~KUiSkills() {}
	void	Initialize();							//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
private:
	static KUiSkills* m_pSelf;
private:
	KUiFightSkill	m_FightSkillPad;
	KUiLiveSkill	m_LiveSkillPad;
	KWndButton		m_FightSkillPadBtn;
	KWndButton		m_LiveSkillPadBtn;
	KWndButton		m_Close;
};

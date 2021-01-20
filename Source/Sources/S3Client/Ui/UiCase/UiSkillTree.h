/*****************************************************************************************
//	����--����������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../../../core/src/gamedatadef.h"

#define	SKILLTREE_MAX_SKILL_COUNT		65
#define	SKILLTREE_SHORTCUT_SKILL_COUNT	9
enum SHORTCUT_KEY_INDEX;

class KUiSkillTree : protected KWndWindow
{
public:
	static KUiSkillTree*	OpenWindow(int bLeft);			//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiSkillTree*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void				CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				LoadScheme(const char* pScheme);//������淽��
	static void				HandleShortcutKey(int nIndex);
	static void				LoadConfig(KIniFile* pIni);
	static void				SaveConfig(KIniFile* pIni);		//�����Զ�������

	static void				DirectHandleShortcutKey(int nIndex);
private:
	KUiSkillTree();
	~KUiSkillTree() {}
	void	Initialize();							//��ʼ��
	void	PaintWindow();							//�������
	int		PtInWindow(int x, int y);
	void	UpdateData();							//��������
	void	UpdateWndRect();
	void	Hide();									//���ش���
	int		GetSkillAtPos(int x, int y);			//�õ�ָ��λ�õļ��ܵ�����
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
private:
	static KUiSkillTree*	m_pSelf;
private:
	int				m_nNumSkills;
	KUiSkillData	m_Skills[SKILLTREE_MAX_SKILL_COUNT];
					//KUiSkillData::nLevel�ڴ˵ĺ����ʾλ�˼�����ʾ�ڵڼ���

	int				m_nFont;
	unsigned int	m_uColor;

	unsigned int	m_bLeft;
	int				m_nLeftLeft, m_nLeftTop;
	int				m_nRightLeft, m_nRightTop;
	int				m_nWidthPerSkill, m_nHeightPerSkill;
	static KUiSkillData	ms_ShortcutSkills[SKILLTREE_SHORTCUT_SKILL_COUNT];
					//KUiSkillData::nLevel�ڴ˵ĺ���0��ʾ���ּ��ܣ�1��ʾ���ּ���

};
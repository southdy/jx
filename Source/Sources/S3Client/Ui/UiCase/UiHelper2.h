/* 
 * File:     KUiHelper2.h
 * Desc:     ��ϸ��������
 * Author:   flying
 * Creation: 2003/7/16
 */
//////////////////////////////////////////////////////////////////////////
//                     ���������ļ����ֵ���ʷ����
// Ϊʲô��ϸ�����������Helper2������ϸ�������漤���ͼƬ�����������
// Helper�أ�������ԭ��ġ�ʲôԭ���أ������ʱ��˵��Ҫһ������Diablo
// ��H�����Ĺ��ڰ�ť��ͼƬ��������Ҳ����������İ���ϵͳ����������ʱ
// ʵ�֣�������Ȼ����Helper�ˡ�֮����Ҫһ���������͵İ��������Ұ�ԭ����
// Ҫ��İ���ϵͳת�Ƶ�����ȥ��������ڿ���ʱ������ͼƬ����֮�������
// �����ͽ���Helper2�ˡ��ں�  �ں�
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
#pragma once

#if !defined _UIHELPER2
#define _UIHELPER2

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndList.h"
#include "../Elem/WndMessageListBox.h"

#define		MAX_INDEX_NUM		512

class KUiHelper2 : protected KWndShowAnimate
{
public:
	//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiHelper2*	OpenWindow(bool bShow);
	//�������������ʾ���򷵻�ʵ��ָ��
	static KUiHelper2*  GetIfVisible();
	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void			 CloseWindow(bool bDestory);
	//������淽��
	static void			 LoadScheme(const char* pScheme);
private:
	KUiHelper2();
	~KUiHelper2();
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadIndexList(const char* pScheme);
	void	Clear();
	void	UpdateData(int nIndex);
private:
	static KUiHelper2*	m_pSelf;
	KWndButton				m_BtnClose;
	KWndButton				m_BtnPic;
	KWndButton              m_BtnKeyboard;
	KWndButton              m_BtnWuxing;
	KWndList				m_IndexList;
	KWndScrollBar			m_IndexScroll;
	KWndMessageListBox		m_MessageList;
	KWndScrollBar			m_MessageScroll;
	int						m_nIndexCount;
	
	typedef struct _IndexData
	{
		char szName[128];
		char szIni[128];
		char szIniSection[128];
	} IndexData;
	IndexData m_IndexData[MAX_INDEX_NUM];
};

#endif